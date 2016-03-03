/*
 * Copyright (c) 2013-2014 The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * This file was originally distributed by Qualcomm Atheros, Inc.
 * under proprietary terms before Copyright ownership was assigned
 * to the Linux Foundation.
 */



#include <osdep.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/if_arp.h>
#include "if_pci.h"
#include "hif_msg_based.h"
#include "hif_pci.h"
#include "copy_engine_api.h"
#include "copy_engine_internal.h"
#include "bmi_msg.h" /* TARGET_TYPE_ */
#include "regtable.h"
#include "ol_fw.h"
#include <osapi_linux.h>
#include "vos_api.h"
#include "vos_sched.h"
#include "wma_api.h"
#include "adf_os_atomic.h"
#if defined(QCA_WIFI_2_0) && !defined(QCA_WIFI_ISOC)
#include "wlan_hdd_power.h"
#endif
#include "wlan_hdd_main.h"
#ifdef CONFIG_CNSS
#include <net/cnss.h>
#endif
#include "epping_main.h"

#ifdef WLAN_BTAMP_FEATURE
#include "wlan_btc_svc.h"
#include "wlan_nlink_common.h"
#endif

#ifndef REMOVE_PKT_LOG
#include "ol_txrx_types.h"
#include "pktlog_ac_api.h"
#include "pktlog_ac.h"
#endif

#define AR9888_DEVICE_ID (0x003c)
#define AR6320_DEVICE_ID (0x003e)
#define AR6320_FW_1_1  (0x11)
#define AR6320_FW_1_3  (0x13)
#define AR6320_FW_2_0  (0x20)
#define AR6320_FW_3_0  (0x30)

#ifdef CONFIG_SLUB_DEBUG_ON
#define MAX_NUM_OF_RECEIVES 400 /* Maximum number of Rx buf to process before*
                                   break out in SLUB debug builds */
#else
#define MAX_NUM_OF_RECEIVES 1000 /* Maximum number of Rx buf to process before break out */
#endif

#define PCIE_WAKE_TIMEOUT 1000 /* Maximum ms timeout for host to wake up target */
#define RAMDUMP_EVENT_TIMEOUT 2500

unsigned int msienable = 0;
module_param(msienable, int, 0644);

int hif_pci_configure(struct hif_pci_softc *sc, hif_handle_t *hif_hdl);
void hif_nointrs(struct hif_pci_softc *sc);

static struct pci_device_id hif_pci_id_table[] = {
	{ 0x168c, 0x003c, PCI_ANY_ID, PCI_ANY_ID },
	{ 0x168c, 0x003e, PCI_ANY_ID, PCI_ANY_ID },
	{ 0 }
};

#ifndef REMOVE_PKT_LOG
struct ol_pl_os_dep_funcs *g_ol_pl_os_dep_funcs = NULL;
#endif

/* Setting SOC_GLOBAL_RESET during driver unload causes intermittent PCIe data bus error
 * As workaround for this issue - changing the reset sequence to use TargetCPU warm reset
 * instead of SOC_GLOBAL_RESET
 */
#define CPU_WARM_RESET_WAR

/*
 * Top-level interrupt handler for all PCI interrupts from a Target.
 * When a block of MSI interrupts is allocated, this top-level handler
 * is not used; instead, we directly call the correct sub-handler.
 */
static irqreturn_t
hif_pci_interrupt_handler(int irq, void *arg)
{
    struct hif_pci_softc *sc = (struct hif_pci_softc *) arg;
    struct HIF_CE_state *hif_state = (struct HIF_CE_state *)sc->hif_device;
    volatile int tmp;
    A_UINT16 val;
    A_UINT32 bar0;

    if (sc->hif_init_done == TRUE) {
        adf_os_spin_lock_irqsave(&hif_state->suspend_lock);
    }

    if (LEGACY_INTERRUPTS(sc)) {

        if (sc->hif_init_done == TRUE) {
            if (Q_TARGET_ACCESS_BEGIN(hif_state->targid) < 0) {
                adf_os_spin_unlock_irqrestore(&hif_state->suspend_lock);
                return IRQ_HANDLED;
            }
        }

        /* Clear Legacy PCI line interrupts */
        /* IMPORTANT: INTR_CLR regiser has to be set after INTR_ENABLE is set to 0, */
        /*            otherwise interrupt can not be really cleared */
        A_PCI_WRITE32(sc->mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS), 0);
        A_PCI_WRITE32(sc->mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_CLR_ADDRESS), PCIE_INTR_FIRMWARE_MASK | PCIE_INTR_CE_MASK_ALL);
        /* IMPORTANT: this extra read transaction is required to flush the posted write buffer */
        tmp = A_PCI_READ32(sc->mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS));

        if (tmp == 0xdeadbeef) {
            printk(KERN_ERR "BUG(%s): SoC returns 0xdeadbeef!!\n", __func__);

            pci_read_config_word(sc->pdev, PCI_VENDOR_ID, &val);
            printk(KERN_ERR "%s: PCI Vendor ID = 0x%04x\n", __func__, val);

            pci_read_config_word(sc->pdev, PCI_DEVICE_ID, &val);
            printk(KERN_ERR "%s: PCI Device ID = 0x%04x\n", __func__, val);

            pci_read_config_word(sc->pdev, PCI_COMMAND, &val);
            printk(KERN_ERR "%s: PCI Command = 0x%04x\n", __func__, val);

            pci_read_config_word(sc->pdev, PCI_STATUS, &val);
            printk(KERN_ERR "%s: PCI Status = 0x%04x\n", __func__, val);

            pci_read_config_dword(sc->pdev, PCI_BASE_ADDRESS_0, &bar0);
            printk(KERN_ERR "%s: PCI BAR0 = 0x%08x\n", __func__, bar0);

            printk(KERN_ERR "%s: RTC_STATE_ADDRESS = 0x%08x, "
                "PCIE_SOC_WAKE_ADDRESS = 0x%08x\n", __func__,
                A_PCI_READ32(sc->mem + PCIE_LOCAL_BASE_ADDRESS
                    + RTC_STATE_ADDRESS),
                A_PCI_READ32(sc->mem + PCIE_LOCAL_BASE_ADDRESS
                    + PCIE_SOC_WAKE_ADDRESS));

            VOS_BUG(0);
        }

        if (sc->hif_init_done == TRUE) {
            if (Q_TARGET_ACCESS_END(hif_state->targid) < 0) {
                adf_os_spin_unlock_irqrestore(&hif_state->suspend_lock);
                return IRQ_HANDLED;
            }
        }
    }
    /* TBDXXX: Add support for WMAC */

    sc->irq_event = irq;
    adf_os_atomic_set(&sc->tasklet_from_intr, 1);
    tasklet_schedule(&sc->intr_tq);

    if (sc->hif_init_done == TRUE) {
        adf_os_spin_unlock_irqrestore(&hif_state->suspend_lock);
    }
    return IRQ_HANDLED;
}

static irqreturn_t
hif_pci_msi_fw_handler(int irq, void *arg)
{
    struct hif_pci_softc *sc = (struct hif_pci_softc *) arg;

    (irqreturn_t)HIF_fw_interrupt_handler(sc->irq_event, sc);

    return IRQ_HANDLED;
}

bool
hif_pci_targ_is_awake(struct hif_pci_softc *sc, void *__iomem *mem)
{
    A_UINT32 val;
    val = A_PCI_READ32(mem + PCIE_LOCAL_BASE_ADDRESS + RTC_STATE_ADDRESS);
    return (RTC_STATE_V_GET(val) == RTC_STATE_V_ON);
}

bool hif_pci_targ_is_present(A_target_id_t targetid, void *__iomem *mem)
{
    return 1; /* FIX THIS */
}

bool hif_max_num_receives_reached(unsigned int count)
{
    if (WLAN_IS_EPPING_ENABLED(vos_get_conparam()))
        return (count > 120);
    else
        return (count > MAX_NUM_OF_RECEIVES);
}

void hif_init_adf_ctx(adf_os_device_t adf_dev, void *ol_sc)
{
	struct ol_softc *sc = (struct ol_softc *)ol_sc;
	struct hif_pci_softc *hif_sc = sc->hif_sc;
	adf_dev->drv = &hif_sc->aps_osdev;
	adf_dev->drv_hdl = hif_sc->aps_osdev.bdev;
	adf_dev->dev = hif_sc->aps_osdev.device;
	sc->adf_dev = adf_dev;
}

void hif_deinit_adf_ctx(void *ol_sc)
{
	struct ol_softc *sc = (struct ol_softc *)ol_sc;
	sc->adf_dev = NULL;
}

#define A_PCIE_LOCAL_REG_READ(mem, addr) \
        A_PCI_READ32((char *)(mem) + PCIE_LOCAL_BASE_ADDRESS + (A_UINT32)(addr))

#define A_PCIE_LOCAL_REG_WRITE(mem, addr, val) \
        A_PCI_WRITE32(((char *)(mem) + PCIE_LOCAL_BASE_ADDRESS + (A_UINT32)(addr)), (val))

#define ATH_PCI_RESET_WAIT_MAX 10 /* Ms */
static void
hif_pci_device_reset(struct hif_pci_softc *sc)
{
    void __iomem *mem = sc->mem;
    int i;
    u_int32_t val;

    /* NB: Don't check resetok here.  This form of reset is integral to correct operation. */

    if (!SOC_GLOBAL_RESET_ADDRESS) {
        return;
    }

    if (!mem) {
        return;
    }

    printk("Reset Device \n");

    /*
     * NB: If we try to write SOC_GLOBAL_RESET_ADDRESS without first
     * writing WAKE_V, the Target may scribble over Host memory!
     */
    A_PCIE_LOCAL_REG_WRITE(mem, PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_V_MASK);
    for (i=0; i<ATH_PCI_RESET_WAIT_MAX; i++) {
        if (hif_pci_targ_is_awake(sc, mem)) {
            break;
        }

        A_MDELAY(1);
    }

    /* Put Target, including PCIe, into RESET. */
    val = A_PCIE_LOCAL_REG_READ(mem, SOC_GLOBAL_RESET_ADDRESS);
    val |= 1;
    A_PCIE_LOCAL_REG_WRITE(mem, SOC_GLOBAL_RESET_ADDRESS, val);
    for (i=0; i<ATH_PCI_RESET_WAIT_MAX; i++) {
        if (A_PCIE_LOCAL_REG_READ(mem, RTC_STATE_ADDRESS) & RTC_STATE_COLD_RESET_MASK) {
            break;
        }

        A_MDELAY(1);
    }

    /* Pull Target, including PCIe, out of RESET. */
    val &= ~1;
    A_PCIE_LOCAL_REG_WRITE(mem, SOC_GLOBAL_RESET_ADDRESS, val);
    for (i=0; i<ATH_PCI_RESET_WAIT_MAX; i++) {
        if (!(A_PCIE_LOCAL_REG_READ(mem, RTC_STATE_ADDRESS) & RTC_STATE_COLD_RESET_MASK)) {
            break;
        }

        A_MDELAY(1);
    }

    A_PCIE_LOCAL_REG_WRITE(mem, PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_RESET);
}


/* CPU warm reset function
 * Steps:
 *	1. Disable all pending interrupts - so no pending interrupts on WARM reset
 *	2. Clear the FW_INDICATOR_ADDRESS -so Traget CPU intializes FW correctly on WARM reset
 *      3. Clear TARGET CPU LF timer interrupt
 *      4. Reset all CEs to clear any pending CE tarnsactions
 *      5. Warm reset CPU
 */
void
hif_pci_device_warm_reset(struct hif_pci_softc *sc)
{
    void __iomem *mem = sc->mem;
    int i;
    u_int32_t val;
    u_int32_t fw_indicator;

    /* NB: Don't check resetok here.  This form of reset is integral to correct operation. */

    if (!mem) {
        return;
    }

    printk("Target Warm Reset\n");

    /*
     * NB: If we try to write SOC_GLOBAL_RESET_ADDRESS without first
     * writing WAKE_V, the Target may scribble over Host memory!
     */
    A_PCIE_LOCAL_REG_WRITE(mem, PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_V_MASK);
    for (i=0; i<ATH_PCI_RESET_WAIT_MAX; i++) {
        if (hif_pci_targ_is_awake(sc, mem)) {
            break;
        }
        A_MDELAY(1);
    }

    /*
     * Disable Pending interrupts
     */
    val = A_PCI_READ32(mem + (SOC_CORE_BASE_ADDRESS | PCIE_INTR_CAUSE_ADDRESS));
    printk("Host Intr Cause reg 0x%x : value : 0x%x \n", (SOC_CORE_BASE_ADDRESS | PCIE_INTR_CAUSE_ADDRESS), val);
    /* Target CPU Intr Cause */
    val = A_PCI_READ32(mem + (SOC_CORE_BASE_ADDRESS | CPU_INTR_ADDRESS));
    printk("Target CPU Intr Cause 0x%x \n", val);

    val = A_PCI_READ32(mem + (SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS));
    A_PCI_WRITE32((mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS)), 0);
    A_PCI_WRITE32((mem+(SOC_CORE_BASE_ADDRESS+PCIE_INTR_CLR_ADDRESS)), 0xffffffff);

    A_MDELAY(100);

    /* Clear FW_INDICATOR_ADDRESS */
    fw_indicator = A_PCI_READ32(mem + FW_INDICATOR_ADDRESS);
    A_PCI_WRITE32(mem+FW_INDICATOR_ADDRESS, 0);

    /* Clear Target LF Timer interrupts */
    val = A_PCI_READ32(mem + (RTC_SOC_BASE_ADDRESS + SOC_LF_TIMER_CONTROL0_ADDRESS));
    printk("addr 0x%x :  0x%x \n", (RTC_SOC_BASE_ADDRESS + SOC_LF_TIMER_CONTROL0_ADDRESS), val);
    val &= ~SOC_LF_TIMER_CONTROL0_ENABLE_MASK;
    A_PCI_WRITE32(mem+(RTC_SOC_BASE_ADDRESS + SOC_LF_TIMER_CONTROL0_ADDRESS), val);

    /* Reset CE */
    val = A_PCI_READ32(mem + (RTC_SOC_BASE_ADDRESS | SOC_RESET_CONTROL_ADDRESS));
    val |= SOC_RESET_CONTROL_CE_RST_MASK;
    A_PCI_WRITE32((mem+(RTC_SOC_BASE_ADDRESS | SOC_RESET_CONTROL_ADDRESS)), val);
    val = A_PCI_READ32(mem + (RTC_SOC_BASE_ADDRESS | SOC_RESET_CONTROL_ADDRESS));
    A_MDELAY(10);

    /* CE unreset */
    val &= ~SOC_RESET_CONTROL_CE_RST_MASK;
    A_PCI_WRITE32(mem+(RTC_SOC_BASE_ADDRESS | SOC_RESET_CONTROL_ADDRESS), val);
    val = A_PCI_READ32(mem + (RTC_SOC_BASE_ADDRESS | SOC_RESET_CONTROL_ADDRESS));
    A_MDELAY(10);

    /* Read Target CPU Intr Cause */
    val = A_PCI_READ32(mem + (SOC_CORE_BASE_ADDRESS | CPU_INTR_ADDRESS));
    printk("Target CPU Intr Cause after CE reset 0x%x \n", val);

    /* CPU warm RESET */
    val = A_PCI_READ32(mem + (RTC_SOC_BASE_ADDRESS | SOC_RESET_CONTROL_ADDRESS));
    val |= SOC_RESET_CONTROL_CPU_WARM_RST_MASK;
    A_PCI_WRITE32(mem+(RTC_SOC_BASE_ADDRESS | SOC_RESET_CONTROL_ADDRESS), val);
    val = A_PCI_READ32(mem + (RTC_SOC_BASE_ADDRESS | SOC_RESET_CONTROL_ADDRESS));
    printk("RESET_CONTROL after cpu warm reset 0x%x \n", val);

    A_MDELAY(100);
    printk("Target Warm reset complete\n");

}


int hif_pci_check_fw_reg(struct hif_pci_softc *sc)
{
    struct HIF_CE_state *hif_state = (struct HIF_CE_state *)sc->hif_device;
    A_target_id_t targid = hif_state->targid;
    void __iomem *mem = sc->mem;
    u_int32_t val;

    A_TARGET_ACCESS_BEGIN_RET(targid);
    val = A_PCI_READ32(mem + FW_INDICATOR_ADDRESS);
    A_TARGET_ACCESS_END_RET(targid);

    printk("%s: FW_INDICATOR register is 0x%x\n", __func__, val);

    if (val & FW_IND_HELPER)
        return 0;

    return 1;
}

int hif_pci_check_soc_status(struct hif_pci_softc *sc)
{
    u_int16_t device_id;
    u_int32_t val;
    u_int16_t timeout_count = 0;

    /* Check device ID from PCIe configuration space for link status */
    pci_read_config_word(sc->pdev, PCI_DEVICE_ID, &device_id);
    if(device_id != sc->devid) {
        printk(KERN_ERR "PCIe link is down!\n");
        return -EACCES;
    }

    /* Check PCIe local register for bar/memory access */
    val = A_PCI_READ32(sc->mem + PCIE_LOCAL_BASE_ADDRESS +
                       RTC_STATE_ADDRESS);
    printk("RTC_STATE_ADDRESS is %08x\n", val);

    /* Try to wake up taget if it sleeps */
    A_PCI_WRITE32(sc->mem + PCIE_LOCAL_BASE_ADDRESS +
                  PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_V_MASK);
    printk("PCIE_SOC_WAKE_ADDRESS is %08x\n",
           A_PCI_READ32(sc->mem + PCIE_LOCAL_BASE_ADDRESS +
                        PCIE_SOC_WAKE_ADDRESS));

    /* Check if taget can be woken up */
    while(!hif_pci_targ_is_awake(sc, sc->mem)) {
        if(timeout_count >= PCIE_WAKE_TIMEOUT) {
            printk(KERN_ERR "Target cannot be woken up! "
                "RTC_STATE_ADDRESS is %08x, PCIE_SOC_WAKE_ADDRESS is %08x\n",
                A_PCI_READ32(sc->mem + PCIE_LOCAL_BASE_ADDRESS +
                RTC_STATE_ADDRESS), A_PCI_READ32(sc->mem +
                PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS));
            return -EACCES;
        }

        A_PCI_WRITE32(sc->mem + PCIE_LOCAL_BASE_ADDRESS +
                      PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_V_MASK);

        A_MDELAY(100);
        timeout_count += 100;
    }

    /* Check Power register for SoC internal bus issues */
    val = A_PCI_READ32(sc->mem + RTC_SOC_BASE_ADDRESS + SOC_POWER_REG_OFFSET);
    printk("Power register is %08x\n", val);

    return EOK;
}

void dump_CE_debug_register(struct hif_pci_softc *sc)
{
    struct HIF_CE_state *hif_state = (struct HIF_CE_state *)sc->hif_device;
    A_target_id_t targid = hif_state->targid;
    void __iomem *mem = sc->mem;
    u_int32_t val, i, j;
    u_int32_t wrapper_idx[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    u_int32_t ce_base;

    A_TARGET_ACCESS_BEGIN(targid);

    /* DEBUG_INPUT_SEL_SRC = 0x6 */
    val = A_PCI_READ32(mem + GPIO_BASE_ADDRESS + WLAN_DEBUG_INPUT_SEL_OFFSET);
    val &= ~WLAN_DEBUG_INPUT_SEL_SRC_MASK;
    val |= WLAN_DEBUG_INPUT_SEL_SRC_SET(0x6);
    A_PCI_WRITE32(mem + GPIO_BASE_ADDRESS + WLAN_DEBUG_INPUT_SEL_OFFSET, val);

    /* DEBUG_CONTROL_ENABLE = 0x1 */
    val = A_PCI_READ32(mem + GPIO_BASE_ADDRESS + WLAN_DEBUG_CONTROL_OFFSET);
    val &= ~WLAN_DEBUG_CONTROL_ENABLE_MASK;
    val |= WLAN_DEBUG_CONTROL_ENABLE_SET(0x1);
    A_PCI_WRITE32(mem + GPIO_BASE_ADDRESS + WLAN_DEBUG_CONTROL_OFFSET, val);

    printk("Debug: inputsel: %x dbgctrl: %x\n",
        A_PCI_READ32(mem + GPIO_BASE_ADDRESS + WLAN_DEBUG_INPUT_SEL_OFFSET),
        A_PCI_READ32(mem + GPIO_BASE_ADDRESS + WLAN_DEBUG_CONTROL_OFFSET));

    printk("Debug CE: \n");
    /* Loop CE debug output */
    /* AMBA_DEBUG_BUS_SEL = 0xc */
    val = A_PCI_READ32(mem + GPIO_BASE_ADDRESS + AMBA_DEBUG_BUS_OFFSET);
    val &= ~AMBA_DEBUG_BUS_SEL_MASK;
    val |= AMBA_DEBUG_BUS_SEL_SET(0xc);
    A_PCI_WRITE32(mem + GPIO_BASE_ADDRESS + AMBA_DEBUG_BUS_OFFSET, val);

    for (i = 0; i < sizeof(wrapper_idx)/sizeof(A_UINT32); i++) {
        /* For (i=1,2,3,4,8,9) write CE_WRAPPER_DEBUG_SEL = i */
        val = A_PCI_READ32(mem + CE_WRAPPER_BASE_ADDRESS +
                           CE_WRAPPER_DEBUG_OFFSET);
        val &= ~CE_WRAPPER_DEBUG_SEL_MASK;
        val |= CE_WRAPPER_DEBUG_SEL_SET(wrapper_idx[i]);
        A_PCI_WRITE32(mem + CE_WRAPPER_BASE_ADDRESS +
                      CE_WRAPPER_DEBUG_OFFSET, val);

        printk("ce wrapper: %d amdbg: %x cewdbg: %x\n", wrapper_idx[i],
            A_PCI_READ32(mem + GPIO_BASE_ADDRESS + AMBA_DEBUG_BUS_OFFSET),
            A_PCI_READ32(mem + CE_WRAPPER_BASE_ADDRESS +
                         CE_WRAPPER_DEBUG_OFFSET));

        if (wrapper_idx[i] <= 7) {
            for (j = 0; j <= 5; j++) {
                ce_base = CE_BASE_ADDRESS(wrapper_idx[i]);
                /* For (j=0~5) write CE_DEBUG_SEL = j */
                val = A_PCI_READ32(mem + ce_base + CE_DEBUG_OFFSET);
                val &= ~CE_DEBUG_SEL_MASK;
                val |= CE_DEBUG_SEL_SET(j);
                A_PCI_WRITE32(mem + ce_base + CE_DEBUG_OFFSET, val);

                /* read (@gpio_athr_wlan_reg) WLAN_DEBUG_OUT_DATA */
                val = A_PCI_READ32(mem + GPIO_BASE_ADDRESS +
                                   WLAN_DEBUG_OUT_OFFSET);
                val = WLAN_DEBUG_OUT_DATA_GET(val);

                printk("    module%d: cedbg: %x out: %x\n", j,
                    A_PCI_READ32(mem + ce_base + CE_DEBUG_OFFSET), val);
            }
        } else {
            /* read (@gpio_athr_wlan_reg) WLAN_DEBUG_OUT_DATA */
            val = A_PCI_READ32(mem + GPIO_BASE_ADDRESS + WLAN_DEBUG_OUT_OFFSET);
            val = WLAN_DEBUG_OUT_DATA_GET(val);

            printk("    out: %x\n", val);
        }
    }

    printk("Debug PCIe: \n");
    /* Loop PCIe debug output */
    /* Write AMBA_DEBUG_BUS_SEL = 0x1c */
    val = A_PCI_READ32(mem + GPIO_BASE_ADDRESS + AMBA_DEBUG_BUS_OFFSET);
    val &= ~AMBA_DEBUG_BUS_SEL_MASK;
    val |= AMBA_DEBUG_BUS_SEL_SET(0x1c);
    A_PCI_WRITE32(mem + GPIO_BASE_ADDRESS + AMBA_DEBUG_BUS_OFFSET, val);

    for (i = 0; i <= 8; i++) {
        /* For (i=1~8) write AMBA_DEBUG_BUS_PCIE_DEBUG_SEL = i */
        val = A_PCI_READ32(mem + GPIO_BASE_ADDRESS + AMBA_DEBUG_BUS_OFFSET);
        val &= ~AMBA_DEBUG_BUS_PCIE_DEBUG_SEL_MASK;
        val |= AMBA_DEBUG_BUS_PCIE_DEBUG_SEL_SET(i);
        A_PCI_WRITE32(mem + GPIO_BASE_ADDRESS + AMBA_DEBUG_BUS_OFFSET, val);

        /* read (@gpio_athr_wlan_reg) WLAN_DEBUG_OUT_DATA */
        val = A_PCI_READ32(mem + GPIO_BASE_ADDRESS + WLAN_DEBUG_OUT_OFFSET);
        val = WLAN_DEBUG_OUT_DATA_GET(val);

        printk("amdbg: %x out: %x %x\n",
            A_PCI_READ32(mem + GPIO_BASE_ADDRESS + WLAN_DEBUG_OUT_OFFSET), val,
            A_PCI_READ32(mem + GPIO_BASE_ADDRESS + WLAN_DEBUG_OUT_OFFSET));
    }

    A_TARGET_ACCESS_END(targid);
}

/*
 * Handler for a per-engine interrupt on a PARTICULAR CE.
 * This is used in cases where each CE has a private
 * MSI interrupt.
 */
static irqreturn_t
CE_per_engine_handler(int irq, void *arg)
{
    struct hif_pci_softc *sc = (struct hif_pci_softc *) arg;
    int CE_id = irq - MSI_ASSIGN_CE_INITIAL;

    /*
     * NOTE: We are able to derive CE_id from irq because we
     * use a one-to-one mapping for CE's 0..5.
     * CE's 6 & 7 do not use interrupts at all.
     *
     * This mapping must be kept in sync with the mapping
     * used by firmware.
     */

    CE_per_engine_service(sc, CE_id);

    return IRQ_HANDLED;
}

#ifdef CONFIG_SLUB_DEBUG_ON

/* worker thread to schedule wlan_tasklet in SLUB debug build */
static void reschedule_tasklet_work_handler(struct work_struct *recovery)
{
  void *vos_context = vos_get_global_context(VOS_MODULE_ID_HIF, NULL);
  struct ol_softc *scn =  vos_get_context(VOS_MODULE_ID_HIF, vos_context);
  struct hif_pci_softc *sc;

  if (NULL == scn){
         printk(KERN_ERR "%s: tasklet scn is null\n", __func__);
         return;
   }

   sc = scn->hif_sc;

   if (sc->hif_init_done == FALSE) {
         printk(KERN_ERR "%s: wlan driver is unloaded\n", __func__);
         return;
   }

   tasklet_schedule(&sc->intr_tq);
   return;
}

static DECLARE_WORK(reschedule_tasklet_work, reschedule_tasklet_work_handler);

#endif


static void
wlan_tasklet(unsigned long data)
{
    struct hif_pci_softc *sc = (struct hif_pci_softc *) data;
    struct HIF_CE_state *hif_state = (struct HIF_CE_state *)sc->hif_device;
    volatile int tmp;

    if (sc->hif_init_done == FALSE) {
         goto irq_handled;
    }

    if (adf_os_atomic_read(&sc->wow_done))
         goto irq_handled;

    adf_os_atomic_set(&sc->ce_suspend, 0);

    (irqreturn_t)HIF_fw_interrupt_handler(sc->irq_event, sc);
    if (sc->ol_sc->target_status == OL_TRGET_STATUS_RESET)
         goto irq_handled;

    CE_per_engine_service_any(sc->irq_event, sc);
    adf_os_atomic_set(&sc->tasklet_from_intr, 0);
    if (CE_get_rx_pending(sc)) {
        /*
         * There are frames pending, schedule tasklet to process them.
         * Enable the interrupt only when there is no pending frames in
         * any of the Copy Engine pipes.
         */
        adf_os_atomic_set(&sc->ce_suspend, 1);
#ifdef CONFIG_SLUB_DEBUG_ON
        schedule_work(&reschedule_tasklet_work);
#else
        tasklet_schedule(&sc->intr_tq);
#endif
        return;
    }
irq_handled:
    if (LEGACY_INTERRUPTS(sc) && (sc->ol_sc->target_status !=
                                  OL_TRGET_STATUS_RESET) &&
           (!adf_os_atomic_read(&sc->pci_link_suspended))) {

        if (sc->hif_init_done == TRUE)
            A_TARGET_ACCESS_BEGIN(hif_state->targid);

        /* Enable Legacy PCI line interrupts */
        A_PCI_WRITE32(sc->mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS),
		    PCIE_INTR_FIRMWARE_MASK | PCIE_INTR_CE_MASK_ALL);
        /* IMPORTANT: this extra read transaction is required to flush the posted write buffer */
        tmp = A_PCI_READ32(sc->mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS));

        if (sc->hif_init_done == TRUE)
           A_TARGET_ACCESS_END(hif_state->targid);
    }
    adf_os_atomic_set(&sc->ce_suspend, 1);
}

#define ATH_PCI_PROBE_RETRY_MAX 3

int
hif_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    void __iomem *mem;
    int ret = 0;
    u_int32_t hif_type, target_type;
    struct hif_pci_softc *sc;
    struct ol_softc *ol_sc;
    int probe_again = 0;
    u_int16_t device_id;
    u_int16_t revision_id;
    u_int32_t lcr_val;

    printk(KERN_INFO "%s:, con_mode= 0x%x\n", __func__, vos_get_conparam());

again:
    ret = 0;

#define BAR_NUM 0
    /*
     * Without any knowledge of the Host, the Target
     * may have been reset or power cycled and its
     * Config Space may no longer reflect the PCI
     * address space that was assigned earlier
     * by the PCI infrastructure.  Refresh it now.
     */
     /*WAR for EV#117307, if PCI link is down, return from probe() */
     pci_read_config_word(pdev,PCI_DEVICE_ID,&device_id);
     printk("PCI device id is %04x :%04x\n",device_id,id->device);
     if(device_id != id->device)  {
	printk(KERN_ERR "ath: PCI link is down.\n");
	/* pci link is down, so returing with error code */
	return -EIO;
     }

    /* FIXME: temp. commenting out assign_resource
     * call for dev_attach to work on 2.6.38 kernel
     */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0) && !defined(__LINUX_ARM_ARCH__)
    if (pci_assign_resource(pdev, BAR_NUM)) {
        printk(KERN_ERR "ath: cannot assign PCI space\n");
        return -EIO;
    }
#endif

    if (pci_enable_device(pdev)) {
        printk(KERN_ERR "ath: cannot enable PCI device\n");
        return -EIO;
    }

#define BAR_NUM 0
    /* Request MMIO resources */
    ret = pci_request_region(pdev, BAR_NUM, "ath");
    if (ret) {
        dev_err(&pdev->dev, "ath: PCI MMIO reservation error\n");
        ret = -EIO;
        goto err_region;
    }
#ifdef CONFIG_ARM_LPAE
    /* if CONFIG_ARM_LPAE is enabled, we have to set 64 bits mask
     * for 32 bits device also. */
    ret =  pci_set_dma_mask(pdev, DMA_BIT_MASK(64));
    if (ret) {
        printk(KERN_ERR "ath: Cannot enable 64-bit pci DMA\n");
        goto err_dma;
    }
    ret = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(64));
    if (ret) {
        printk(KERN_ERR "ath: Cannot enable 64-bit consistent DMA\n");
        goto err_dma;
    }
#else
    ret = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));
    if (ret) {
        printk(KERN_ERR "ath: Cannot enable 32-bit pci DMA\n");
        goto err_dma;
    }
    ret = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(32));
    if (ret) {
        printk(KERN_ERR "%s: Cannot enable 32-bit consistent DMA!\n",
               __func__);
        goto err_dma;
    }
#endif

#ifdef DISABLE_L1SS_STATES
    pci_read_config_dword(pdev, 0x188, &lcr_val);
    pci_write_config_dword(pdev, 0x188, (lcr_val & ~0x0000000f));
#endif

    /* Set bus master bit in PCI_COMMAND to enable DMA */
    pci_set_master(pdev);

    /* Temporary FIX: disable ASPM on peregrine. Will be removed after the OTP is programmed */
    pci_read_config_dword(pdev, 0x80, &lcr_val);
    pci_write_config_dword(pdev, 0x80, (lcr_val & 0xffffff00));

    /* Arrange for access to Target SoC registers. */
    mem = pci_iomap(pdev, BAR_NUM, 0);
    if (!mem) {
        printk(KERN_ERR "ath: PCI iomap error\n") ;
        ret = -EIO;
        goto err_iomap;
    }

    /* Disable asynchronous suspend */
    device_disable_async_suspend(&pdev->dev);

    sc = A_MALLOC(sizeof(*sc));
    if (!sc) {
        ret = -ENOMEM;
        goto err_alloc;
    }

    OS_MEMZERO(sc, sizeof(*sc));
    sc->mem = mem;
    sc->pdev = pdev;
    sc->dev = &pdev->dev;

    sc->aps_osdev.bdev = pdev;
    sc->aps_osdev.device = &pdev->dev;
    sc->aps_osdev.bc.bc_handle = (void *)mem;
    sc->aps_osdev.bc.bc_bustype = HAL_BUS_TYPE_PCI;
    sc->devid = id->device;

    adf_os_spinlock_init(&sc->target_lock);

    sc->cacheline_sz = dma_get_cache_alignment();

    pci_read_config_word(pdev, 0x08, &revision_id);

    switch (id->device) {
    case AR9888_DEVICE_ID:
        hif_type = HIF_TYPE_AR9888;
        target_type = TARGET_TYPE_AR9888;
        break;

    case AR6320_DEVICE_ID:
        switch(revision_id) {
        case AR6320_FW_1_1:
        case AR6320_FW_1_3:
            hif_type = HIF_TYPE_AR6320;
            target_type = TARGET_TYPE_AR6320;
            break;

        case AR6320_FW_2_0:
        case AR6320_FW_3_0:
            hif_type = HIF_TYPE_AR6320V2;
            target_type = TARGET_TYPE_AR6320V2;
            break;

        default:
            printk(KERN_ERR "unsupported revision id\n");
            ret = -ENODEV;
            goto err_tgtstate;
        }
        break;

    default:
        printk(KERN_ERR "unsupported device id\n");
        ret = -ENODEV;
        goto err_tgtstate;
    }
    /*
     * Attach Target register table.  This is needed early on --
     * even before BMI -- since PCI and HIF initialization (and BMI init)
     * directly access Target registers (e.g. CE registers).
     */

    hif_register_tbl_attach(sc, hif_type);
    target_register_tbl_attach(sc, target_type);
    {
        A_UINT32 fw_indicator;
#if PCIE_BAR0_READY_CHECKING
        int wait_limit = 200;
#endif
        int targ_awake_limit = 500;

        /*
         * Verify that the Target was started cleanly.
         *
         * The case where this is most likely is with an AUX-powered
         * Target and a Host in WoW mode. If the Host crashes,
         * loses power, or is restarted (without unloading the driver)
         * then the Target is left (aux) powered and running.  On a
         * subsequent driver load, the Target is in an unexpected state.
         * We try to catch that here in order to reset the Target and
         * retry the probe.
         */
        A_PCI_WRITE32(mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_V_MASK);
        while (!hif_pci_targ_is_awake(sc, mem)) {
            if (0 == targ_awake_limit) {
                printk(KERN_ERR "%s: target awake timeout\n", __func__);
                ret = -EAGAIN;
                goto err_tgtstate;
            }
            A_MDELAY(1);
            targ_awake_limit--;
        }

#if PCIE_BAR0_READY_CHECKING
        /* Synchronization point: wait the BAR0 is configured */
        while (wait_limit-- &&
            !(A_PCI_READ32(mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_RDY_STATUS_ADDRESS) \
             & PCIE_SOC_RDY_STATUS_BAR_MASK)) {
            A_MDELAY(10);
        }
        if (wait_limit < 0) {
            /* AR6320v1 doesn't support checking of BAR0 configuration,
               takes one sec to wait BAR0 ready */
            printk(KERN_INFO "AR6320v1 waits two sec for BAR0 ready.\n");
        }
#endif

        fw_indicator = A_PCI_READ32(mem + FW_INDICATOR_ADDRESS);
        A_PCI_WRITE32(mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_RESET);

        if (fw_indicator & FW_IND_INITIALIZED) {
            probe_again++;
            printk(KERN_ERR "ath: Target is in an unknown state. Resetting (attempt %d).\n", probe_again);
            /* hif_pci_device_reset, below, will reset the target */
            ret = -EIO;
            goto err_tgtstate;
        }
    }

    ol_sc = A_MALLOC(sizeof(*ol_sc));
    if (!ol_sc)
	    goto err_attach;
    OS_MEMZERO(ol_sc, sizeof(*ol_sc));
    ol_sc->sc_osdev = &sc->aps_osdev;
    ol_sc->hif_sc = (void *)sc;
    sc->ol_sc = ol_sc;
    ol_sc->target_type = target_type;
    if (hif_pci_configure(sc, &ol_sc->hif_hdl))
	    goto err_config;

    ol_sc->enableuartprint = 0;
    ol_sc->enablefwlog = 0;
#ifdef QCA_SINGLE_BINARY_SUPPORT
    ol_sc->enablesinglebinary = TRUE;
#else
    ol_sc->enablesinglebinary = FALSE;
#endif
    ol_sc->max_no_of_peers = 1;

#ifdef CONFIG_CNSS
    /* Get RAM dump memory address and size */
    if (!cnss_get_ramdump_mem(&ol_sc->ramdump_address, &ol_sc->ramdump_size)) {
        ol_sc->ramdump_base = ioremap(ol_sc->ramdump_address,
            ol_sc->ramdump_size);
        if (!ol_sc->ramdump_base) {
            pr_err("%s: Cannot map ramdump_address 0x%lx!\n",
                __func__, ol_sc->ramdump_address);
        }
    } else {
        pr_info("%s: Failed to get RAM dump memory address or size!\n",
            __func__);
    }
#endif

    adf_os_atomic_init(&sc->tasklet_from_intr);
    adf_os_atomic_init(&sc->wow_done);
    adf_os_atomic_init(&sc->ce_suspend);
    adf_os_atomic_init(&sc->pci_link_suspended);
    init_waitqueue_head(&ol_sc->sc_osdev->event_queue);

    ret = hdd_wlan_startup(&pdev->dev, ol_sc);

    if (ret) {
        hif_nointrs(sc);
        HIFShutDownDevice(ol_sc->hif_hdl);
        goto err_config;
    }

    /* Re-enable ASPM after firmware/OTP download is complete */
    pci_write_config_dword(pdev, 0x80, lcr_val);

#ifndef REMOVE_PKT_LOG
    if (vos_get_conparam() != VOS_FTM_MODE &&
        !WLAN_IS_EPPING_ENABLED(vos_get_conparam())) {
        /*
         * pktlog initialization
         */
        ol_pl_sethandle(&ol_sc->pdev_txrx_handle->pl_dev, ol_sc);

        if (pktlogmod_init(ol_sc))
            printk(KERN_ERR "%s: pktlogmod_init failed\n", __func__);
    }
#endif

#ifdef WLAN_BTAMP_FEATURE
    /* Send WLAN UP indication to Nlink Service */
    send_btc_nlink_msg(WLAN_MODULE_UP_IND, 0);
#endif

    return 0;

err_config:
    A_FREE(ol_sc);
err_attach:
    ret = -EIO;
err_tgtstate:
    pci_set_drvdata(pdev, NULL);
    hif_pci_device_reset(sc);
    A_FREE(sc);
err_alloc:
    /* call HIF PCI free here */
    printk("%s: HIF PCI Free needs to happen here \n", __func__);
    pci_iounmap(pdev, mem);
err_iomap:
    pci_clear_master(pdev);
err_dma:
    pci_release_region(pdev, BAR_NUM);
err_region:
    pci_disable_device(pdev);

    if (probe_again && (probe_again <= ATH_PCI_PROBE_RETRY_MAX)) {
        int delay_time;

        /*
         * We can get here after a Host crash or power fail when
         * the Target has aux power.  We just did a device_reset,
         * so we need to delay a short while before we try to
         * reinitialize.  Typically only one retry with the smallest
         * delay is needed.  Target should never need more than a 100Ms
         * delay; that would not conform to the PCIe std.
         */

        printk(KERN_INFO "pci reprobe.\n");
        delay_time = max(100, 10 * (probe_again * probe_again)); /* 10, 40, 90, 100, 100, ... */
        A_MDELAY(delay_time);
        goto again;
    }
    return ret;
}

/* This function will be called when SSR frame work wants to
 * power up WLAN host driver when SSR happens. Most of this
 * function is duplicated from hif_pci_probe().
 */
#if defined(QCA_WIFI_2_0) && !defined(QCA_WIFI_ISOC)
int hif_pci_reinit(struct pci_dev *pdev, const struct pci_device_id *id)
{
    void __iomem *mem;
    struct hif_pci_softc *sc;
    struct ol_softc *ol_sc;
    int probe_again = 0;
    int ret = 0;
    u_int16_t device_id;
    u_int32_t hif_type;
    u_int32_t target_type;
    u_int32_t lcr_val;
    u_int16_t revision_id;

again:
    ret = 0;

    if (vos_is_load_unload_in_progress(VOS_MODULE_ID_HIF, NULL)) {
        printk("Load/unload in progress, ignore SSR reinit\n");
        return 0;
    }

#define BAR_NUM 0
    /*
     * Without any knowledge of the Host, the Target
     * may have been reset or power cycled and its
     * Config Space may no longer reflect the PCI
     * address space that was assigned earlier
     * by the PCI infrastructure.  Refresh it now.
     */
    /* If PCI link is down, return from reinit() */
    pci_read_config_word(pdev,PCI_DEVICE_ID,&device_id);
    printk("PCI device id is %04x :%04x\n", device_id, id->device);
    if (device_id != id->device)  {
        printk(KERN_ERR "%s: PCI link is down!\n", __func__);
        /* PCI link is down, so return with error code. */
        return -EIO;
    }

    /* FIXME: Commenting out assign_resource
     * call for dev_attach to work on 2.6.38 kernel
     */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0) && !defined(__LINUX_ARM_ARCH__)
    if (pci_assign_resource(pdev, BAR_NUM)) {
        printk(KERN_ERR "%s: Cannot assign PCI space!\n", __func__);
        return -EIO;
    }
#endif

    if (pci_enable_device(pdev)) {
        printk(KERN_ERR "%s: Cannot enable PCI device!\n", __func__);
        return -EIO;
    }

    /* Request MMIO resources */
    ret = pci_request_region(pdev, BAR_NUM, "ath");
    if (ret) {
        dev_err(&pdev->dev, "%s: PCI MMIO reservation error!\n", __func__);
        ret = -EIO;
        goto err_region;
    }

#ifdef CONFIG_ARM_LPAE
    /* if CONFIG_ARM_LPAE is enabled, we have to set 64 bits mask
     * for 32 bits device also. */
    ret =  pci_set_dma_mask(pdev, DMA_BIT_MASK(64));
    if (ret) {
        printk(KERN_ERR "ath: Cannot enable 64-bit pci DMA\n");
        goto err_dma;
    }
    ret = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(64));
    if (ret) {
        printk(KERN_ERR "ath: Cannot enable 64-bit consistent DMA\n");
        goto err_dma;
    }
#else
    ret = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));
    if (ret) {
        printk(KERN_ERR "ath: Cannot enable 32-bit pci DMA\n");
        goto err_dma;
    }
    ret = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(32));
    if (ret) {
        printk(KERN_ERR "%s: Cannot enable 32-bit consistent DMA!\n",
               __func__);
        goto err_dma;
    }
#endif

#ifdef DISABLE_L1SS_STATES
    pci_read_config_dword(pdev, 0x188, &lcr_val);
    pci_write_config_dword(pdev, 0x188, (lcr_val & ~0x0000000f));
#endif

    /* Set bus master bit in PCI_COMMAND to enable DMA */
    pci_set_master(pdev);

    /* Temporary FIX: disable ASPM. Will be removed after
       the OTP is programmed. */
    pci_read_config_dword(pdev, 0x80, &lcr_val);
    pci_write_config_dword(pdev, 0x80, (lcr_val & 0xffffff00));

    /* Arrange for access to Target SoC registers */
    mem = pci_iomap(pdev, BAR_NUM, 0);
    if (!mem) {
        printk(KERN_ERR "%s: PCI iomap error!\n", __func__) ;
        ret = -EIO;
        goto err_iomap;
    }

    /* Disable asynchronous suspend */
    device_disable_async_suspend(&pdev->dev);

    sc = A_MALLOC(sizeof(*sc));
    if (!sc) {
        ret = -ENOMEM;
        goto err_alloc;
    }

    OS_MEMZERO(sc, sizeof(*sc));
    sc->mem = mem;
    sc->pdev = pdev;
    sc->dev = &pdev->dev;
    sc->aps_osdev.bdev = pdev;
    sc->aps_osdev.device = &pdev->dev;
    sc->aps_osdev.bc.bc_handle = (void *)mem;
    sc->aps_osdev.bc.bc_bustype = HAL_BUS_TYPE_PCI;
    sc->devid = id->device;

    adf_os_spinlock_init(&sc->target_lock);

    sc->cacheline_sz = dma_get_cache_alignment();
    pci_read_config_word(pdev, 0x08, &revision_id);

    switch (id->device) {
    case AR9888_DEVICE_ID:
        hif_type = HIF_TYPE_AR9888;
        target_type = TARGET_TYPE_AR9888;
        break;

    case AR6320_DEVICE_ID:
        switch(revision_id) {
        case AR6320_FW_1_1:
        case AR6320_FW_1_3:
            hif_type = HIF_TYPE_AR6320;
            target_type = TARGET_TYPE_AR6320;
            break;

        case AR6320_FW_2_0:
        case AR6320_FW_3_0:
            hif_type = HIF_TYPE_AR6320V2;
            target_type = TARGET_TYPE_AR6320V2;
            break;

        default:
            printk(KERN_ERR "unsupported revision id\n");
            ret = -ENODEV;
            goto err_tgtstate;
        }
        break;

    default:
        printk(KERN_ERR "%s: Unsupported device ID!\n", __func__);
        ret = -ENODEV;
        goto err_tgtstate;
    }

    /*
     * Attach Target register table. This is needed early on --
     * even before BMI -- since PCI and HIF initialization (and BMI init)
     * directly access Target registers (e.g. CE registers).
     */

    hif_register_tbl_attach(sc, hif_type);
    target_register_tbl_attach(sc, target_type);
    {
        A_UINT32 fw_indicator;
#if PCIE_BAR0_READY_CHECKING
        int wait_limit = 200;
#endif

        /*
         * Verify that the Target was started cleanly.
         *
         * The case where this is most likely is with an AUX-powered
         * Target and a Host in WoW mode. If the Host crashes,
         * loses power, or is restarted (without unloading the driver)
         * then the Target is left (aux) powered and running. On a
         * subsequent driver load, the Target is in an unexpected state.
         * We try to catch that here in order to reset the Target and
         * retry the probe.
         */
        A_PCI_WRITE32(mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS,
                      PCIE_SOC_WAKE_V_MASK);
        while (!hif_pci_targ_is_awake(sc, mem)) {
            ;
        }

#if PCIE_BAR0_READY_CHECKING
        /* Synchronization point: wait the BAR0 is configured. */
        while (wait_limit-- &&
            !(A_PCI_READ32(mem + PCIE_LOCAL_BASE_ADDRESS +
            PCIE_SOC_RDY_STATUS_ADDRESS) & PCIE_SOC_RDY_STATUS_BAR_MASK)) {
            A_MDELAY(10);
        }
        if (wait_limit < 0) {
            /* AR6320v1 doesn't support checking of BAR0 configuration,
               takes one sec to wait BAR0 ready. */
            printk(KERN_INFO "AR6320v1 waits two sec for BAR0 ready.\n");
        }
#endif

        fw_indicator = A_PCI_READ32(mem + FW_INDICATOR_ADDRESS);
        A_PCI_WRITE32(mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS,
                      PCIE_SOC_WAKE_RESET);

        if (fw_indicator & FW_IND_INITIALIZED) {
            probe_again++;
            printk(KERN_ERR "%s: Target is in an unknown state. "
                   "Resetting (attempt %d).\n", __func__, probe_again);
            /* hif_pci_device_reset, below will reset the target. */
            ret = -EIO;
            goto err_tgtstate;
        }
    }

    ol_sc = A_MALLOC(sizeof(*ol_sc));
    if (!ol_sc)
        goto err_attach;

    OS_MEMZERO(ol_sc, sizeof(*ol_sc));
    ol_sc->sc_osdev = &sc->aps_osdev;
    ol_sc->hif_sc = (void *)sc;
    sc->ol_sc = ol_sc;
    ol_sc->target_type = target_type;

    if (hif_pci_configure(sc, &ol_sc->hif_hdl))
        goto err_config;

    ol_sc->enableuartprint = 0;
    ol_sc->enablefwlog = 0;
#ifdef QCA_SINGLE_BINARY_SUPPORT
    ol_sc->enablesinglebinary = TRUE;
#else
    ol_sc->enablesinglebinary = FALSE;
#endif
    ol_sc->max_no_of_peers = 1;

#ifdef CONFIG_CNSS
    /* Get RAM dump memory address and size */
    if (!cnss_get_ramdump_mem(&ol_sc->ramdump_address, &ol_sc->ramdump_size)) {
        ol_sc->ramdump_base = ioremap(ol_sc->ramdump_address,
            ol_sc->ramdump_size);
        if (!ol_sc->ramdump_base) {
            pr_err("%s: Cannot map ramdump_address 0x%lx!\n",
                __func__, ol_sc->ramdump_address);
        }
    } else {
        pr_info("%s: Failed to get RAM dump memory address or size!\n",
            __func__);
    }
#endif

    adf_os_atomic_init(&sc->tasklet_from_intr);
    adf_os_atomic_init(&sc->wow_done);
    adf_os_atomic_init(&sc->ce_suspend);
    adf_os_atomic_init(&sc->pci_link_suspended);

    init_waitqueue_head(&ol_sc->sc_osdev->event_queue);

    if (VOS_STATUS_SUCCESS == hdd_wlan_re_init(ol_sc)) {
        ret = 0;
    }

    /* Re-enable ASPM after firmware/OTP download is complete */
    pci_write_config_dword(pdev, 0x80, lcr_val);

    if (ret) {
        hif_nointrs(sc);
        goto err_config;
    }

#ifndef REMOVE_PKT_LOG
    if (vos_get_conparam() != VOS_FTM_MODE &&
        !WLAN_IS_EPPING_ENABLED(vos_get_conparam())) {
        /*
         * pktlog initialization
         */
        ol_pl_sethandle(&ol_sc->pdev_txrx_handle->pl_dev, ol_sc);

        if (pktlogmod_init(ol_sc))
            printk(KERN_ERR "%s: pktlogmod_init failed!\n", __func__);
    }
#endif

#ifdef WLAN_BTAMP_FEATURE
    /* Send WLAN UP indication to Nlink Service */
    send_btc_nlink_msg(WLAN_MODULE_UP_IND, 0);
#endif

    printk("%s: WLAN host driver reinitiation completed!\n", __func__);
    return 0;

err_config:
    A_FREE(ol_sc);
err_attach:
    ret = -EIO;
err_tgtstate:
    pci_set_drvdata(pdev, NULL);
    hif_pci_device_reset(sc);
    A_FREE(sc);
err_alloc:
    /* Call HIF PCI free here */
    printk("%s: HIF PCI free needs to happen here.\n", __func__);
    pci_iounmap(pdev, mem);
err_iomap:
    pci_clear_master(pdev);
err_dma:
    pci_release_region(pdev, BAR_NUM);
err_region:
    pci_disable_device(pdev);

    if (probe_again && (probe_again <= ATH_PCI_PROBE_RETRY_MAX)) {
        int delay_time;

        /*
         * We can get here after a Host crash or power fail when
         * the Target has aux power. We just did a device_reset,
         * so we need to delay a short while before we try to
         * reinitialize. Typically only one retry with the smallest
         * delay is needed. Target should never need more than a 100Ms
         * delay; that would not conform to the PCIe std.
         */

        printk(KERN_INFO "%s: PCI rereinit\n", __func__);
        /* 10, 40, 90, 100, 100, ... */
        delay_time = max(100, 10 * (probe_again * probe_again));
        A_MDELAY(delay_time);
        goto again;
    }

    return ret;
}
#endif

void hif_pci_notify_handler(struct pci_dev *pdev, int state)
{
   if (!WLAN_IS_EPPING_ENABLED(vos_get_conparam())) {
       int ret = 0;
       ret = hdd_wlan_notify_modem_power_state(state);
       if (ret < 0)
          printk(KERN_ERR "%s: Fail to send notify\n", __func__);
   }
}

void
hif_nointrs(struct hif_pci_softc *sc)
{
    int i;

    if (sc->num_msi_intrs > 0) {
        /* MSI interrupt(s) */
        for (i = 0; i < sc->num_msi_intrs; i++) {
            free_irq(sc->pdev->irq + i, sc);
        }
        sc->num_msi_intrs = 0;
    } else {
        /* Legacy PCI line interrupt */
        free_irq(sc->pdev->irq, sc);
    }
}

int
hif_pci_configure(struct hif_pci_softc *sc, hif_handle_t *hif_hdl)
{
    int ret = 0;
    int num_msi_desired;
    u_int32_t val;

    BUG_ON(pci_get_drvdata(sc->pdev) != NULL);
    pci_set_drvdata(sc->pdev, sc);

    tasklet_init(&sc->intr_tq, wlan_tasklet, (unsigned long)sc);

	/*
	 * Interrupt Management is divided into these scenarios :
	 * A) We wish to use MSI and Multiple MSI is supported and we
	 *    are able to obtain the number of MSI interrupts desired
	 *    (best performance)
	 * B) We wish to use MSI and Single MSI is supported and we are
	 *    able to obtain a single MSI interrupt
	 * C) We don't want to use MSI or MSI is not supported and we
	 *    are able to obtain a legacy interrupt
	 * D) Failure
	 */
#if defined(FORCE_LEGACY_PCI_INTERRUPTS)
    num_msi_desired = 0; /* Use legacy PCI line interrupts rather than MSI */
#else
    num_msi_desired = MSI_NUM_REQUEST; /* Multiple MSI */
    if (!msienable) {
        num_msi_desired = 0;
    }
#endif

    printk("\n %s : num_desired MSI set to %d\n", __func__, num_msi_desired);

    if (num_msi_desired > 1) {
        int i;
        int rv;

        rv = pci_enable_msi_block(sc->pdev, MSI_NUM_REQUEST);

	if (rv == 0) { /* successfully allocated all MSI interrupts */
		/*
		 * TBDXXX: This path not yet tested,
		 * since Linux x86 does not currently
		 * support "Multiple MSIs".
		 */
		sc->num_msi_intrs = MSI_NUM_REQUEST;
		ret = request_irq(sc->pdev->irq+MSI_ASSIGN_FW, hif_pci_msi_fw_handler,
				  IRQF_SHARED, "wlan_pci", sc);
		if(ret) {
			dev_err(&sc->pdev->dev, "request_irq failed\n");
			goto err_intr;
		}
		for (i=MSI_ASSIGN_CE_INITIAL; i<=MSI_ASSIGN_CE_MAX; i++) {
			ret = request_irq(sc->pdev->irq+i, CE_per_engine_handler, IRQF_SHARED,
					  "wlan_pci", sc);
			if(ret) {
				dev_err(&sc->pdev->dev, "request_irq failed\n");
				goto err_intr;
			}
		}
	} else {
            if (rv < 0) {
                /* Can't get any MSI -- try for legacy line interrupts */
                num_msi_desired = 0;
            } else {
                /* Can't get enough MSI interrupts -- try for just 1 */
                printk("\n %s : Can't allocate requested number of MSI, just use 1\n", __func__);
                num_msi_desired = 1;
            }
        }
    }

    if (num_msi_desired == 1) {
        /*
         * We are here because either the platform only supports
         * single MSI OR because we couldn't get all the MSI interrupts
         * that we wanted so we fall back to a single MSI.
         */
        if (pci_enable_msi(sc->pdev) < 0) {
            printk(KERN_ERR "ath: single MSI interrupt allocation failed\n");
            /* Try for legacy PCI line interrupts */
            num_msi_desired = 0;
        } else {
            /* Use a single Host-side MSI interrupt handler for all interrupts */
            num_msi_desired = 1;
        }
    }

    if ( num_msi_desired <= 1) {
	    /* We are here because we want to multiplex a single host interrupt among all
	     * Target interrupt sources
	     */
	    ret = request_irq(sc->pdev->irq, hif_pci_interrupt_handler, IRQF_SHARED,
			      "wlan_pci", sc);
	    if(ret) {
		    dev_err(&sc->pdev->dev, "request_irq failed\n");
		    goto err_intr;
	    }

    }
#if CONFIG_PCIE_64BIT_MSI
    {
        struct ol_ath_softc_net80211 *scn = sc->scn;
        u_int8_t MSI_flag;
        u_int32_t reg;

#define OL_ATH_PCI_MSI_POS        0x50
#define MSI_MAGIC_RDY_MASK  0x00000001
#define MSI_MAGIC_EN_MASK   0x80000000

        pci_read_config_byte(sc->pdev, OL_ATH_PCI_MSI_POS + PCI_MSI_FLAGS, &MSI_flag);
        if (MSI_flag & PCI_MSI_FLAGS_ENABLE) {
            A_PCI_WRITE32(sc->mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_V_MASK);
            while (!ath_pci_targ_is_awake(sc->mem)) {
                ;
            }
            scn->MSI_magic = OS_MALLOC_CONSISTENT(scn->sc_osdev, 4, &scn->MSI_magic_dma, \
                             OS_GET_DMA_MEM_CONTEXT(scn, MSI_dmacontext), 0);
            A_PCI_WRITE32(sc->mem + SOC_PCIE_BASE_ADDRESS + MSI_MAGIC_ADR_ADDRESS,
                          scn->MSI_magic_dma);
            reg = A_PCI_READ32(sc->mem + SOC_PCIE_BASE_ADDRESS + MSI_MAGIC_ADDRESS);
            A_PCI_WRITE32(sc->mem + SOC_PCIE_BASE_ADDRESS + MSI_MAGIC_ADDRESS, reg | MSI_MAGIC_RDY_MASK);

            A_PCI_WRITE32(sc->mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_RESET);
        }
    }
#endif

    if(num_msi_desired == 0) {
        printk("\n Using PCI Legacy Interrupt\n");

        /* Make sure to wake the Target before enabling Legacy Interrupt */
        A_PCI_WRITE32(sc->mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS,
                      PCIE_SOC_WAKE_V_MASK);
        while (!hif_pci_targ_is_awake(sc, sc->mem)) {
                ;
        }
        /* Use Legacy PCI Interrupts */
        /*
         * A potential race occurs here: The CORE_BASE write depends on
         * target correctly decoding AXI address but host won't know
         * when target writes BAR to CORE_CTRL. This write might get lost
         * if target has NOT written BAR. For now, fix the race by repeating
         * the write in below synchronization checking.
         */
        A_PCI_WRITE32(sc->mem+(SOC_CORE_BASE_ADDRESS |
                      PCIE_INTR_ENABLE_ADDRESS),
                      PCIE_INTR_FIRMWARE_MASK | PCIE_INTR_CE_MASK_ALL);
        A_PCI_WRITE32(sc->mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS,
                      PCIE_SOC_WAKE_RESET);
    }

    sc->num_msi_intrs = num_msi_desired;
    sc->ce_count = CE_COUNT;

    { /* Synchronization point: Wait for Target to finish initialization before we proceed. */
        int wait_limit = 1000; /* 10 sec */

        /* Make sure to wake Target before accessing Target memory */
        A_PCI_WRITE32(sc->mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_V_MASK);
        while (!hif_pci_targ_is_awake(sc, sc->mem)) {
                ;
        }
        while (wait_limit-- && !(A_PCI_READ32(sc->mem + FW_INDICATOR_ADDRESS) & FW_IND_INITIALIZED)) {
            if (num_msi_desired == 0) {
                /* Fix potential race by repeating CORE_BASE writes */
                A_PCI_WRITE32(sc->mem + (SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS),
                      PCIE_INTR_FIRMWARE_MASK | PCIE_INTR_CE_MASK_ALL);
            }
            A_MDELAY(10);
        }

        if (wait_limit < 0) {
            printk(KERN_ERR "ath: %s: TARGET STALLED: .\n", __FUNCTION__);
            A_PCI_WRITE32(sc->mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_RESET);
            ret = -EIO;
            goto err_stalled;
        }
        A_PCI_WRITE32(sc->mem + PCIE_LOCAL_BASE_ADDRESS + PCIE_SOC_WAKE_ADDRESS, PCIE_SOC_WAKE_RESET);
    }

    if (HIF_PCIDeviceProbed(sc)) {
            printk(KERN_ERR "ath: %s: Target probe failed.\n", __FUNCTION__);
            ret = -EIO;
            goto err_stalled;
    }

    *hif_hdl = sc->hif_device;

    /*
     * Flag to avoid potential unallocated memory access from MSI
     * interrupt handler which could get scheduled as soon as MSI
     * is enabled, i.e to take care of the race due to the order
     * in where MSI is enabled before the memory, that will be
     * in interrupt handlers, is allocated.
     */
    sc->hif_init_done = TRUE;
    return 0;

err_stalled:
    /* Read Target CPU Intr Cause for debug */
    val = A_PCI_READ32(sc->mem + (SOC_CORE_BASE_ADDRESS | CPU_INTR_ADDRESS));
    printk("ERROR: Target Stalled : Target CPU Intr Cause 0x%x \n", val);
    hif_nointrs(sc);
err_intr:
    if (num_msi_desired) {
        pci_disable_msi(sc->pdev);
    }
    pci_set_drvdata(sc->pdev, NULL);

    return ret;
}

void
hif_pci_remove(struct pci_dev *pdev)
{
    struct hif_pci_softc *sc = pci_get_drvdata(pdev);
    struct ol_softc *scn;
    void __iomem *mem;

    /* Attach did not succeed, all resources have been
     * freed in error handler
     */
    if (!sc)
        return;

    scn = sc->ol_sc;

#ifndef REMOVE_PKT_LOG
    if (vos_get_conparam() != VOS_FTM_MODE &&
        !WLAN_IS_EPPING_ENABLED(vos_get_conparam()))
        pktlogmod_exit(scn);
#endif

    __hdd_wlan_exit();

    mem = (void __iomem *)sc->mem;

    pci_disable_msi(pdev);

#ifdef CONFIG_CNSS
    if (scn->ramdump_base)
        iounmap(scn->ramdump_base);
#endif

    A_FREE(scn);
    A_FREE(sc->hif_device);
    A_FREE(sc);
    pci_set_drvdata(pdev, NULL);
    pci_iounmap(pdev, mem);
    pci_release_region(pdev, BAR_NUM);
    pci_clear_master(pdev);
    pci_disable_device(pdev);
    printk(KERN_INFO "pci_remove\n");
}

/* This function will be called when SSR framework wants to
 * shutdown WLAN host driver when SSR happens. Most of this
 * function is duplicated from hif_pci_remove().
 */
#if defined(QCA_WIFI_2_0) && !defined(QCA_WIFI_ISOC)
void hif_pci_shutdown(struct pci_dev *pdev)
{
    void __iomem *mem;
    struct hif_pci_softc *sc;
    struct ol_softc *scn;

    sc = pci_get_drvdata(pdev);
    /* Attach did not succeed, all resources have been
     * freed in error handler.
     */
    if (!sc)
        return;

    if (vos_is_load_unload_in_progress(VOS_MODULE_ID_HIF, NULL)) {
        printk("Load/unload in progress, ignore SSR shutdown\n");
        return;
    }
    /* this is for cases, where shutdown invoked from CNSS */
    vos_set_logp_in_progress(VOS_MODULE_ID_HIF, TRUE);

    scn = sc->ol_sc;

#ifndef REMOVE_PKT_LOG
    if (vos_get_conparam() != VOS_FTM_MODE &&
        !WLAN_IS_EPPING_ENABLED(vos_get_conparam()))
        pktlogmod_exit(scn);
#endif

    if (!vos_is_ssr_ready(__func__))
        printk("Host driver is not ready for SSR, attempting anyway\n");

    if (!WLAN_IS_EPPING_ENABLED(vos_get_conparam()))
        hdd_wlan_shutdown();

    mem = (void __iomem *)sc->mem;

    pci_disable_msi(pdev);

#ifdef CONFIG_CNSS
    if (scn->ramdump_base)
        iounmap(scn->ramdump_base);
#endif

    A_FREE(scn);
    A_FREE(sc);
    pci_set_drvdata(pdev, NULL);
    pci_iounmap(pdev, mem);
    pci_release_region(pdev, BAR_NUM);
    pci_clear_master(pdev);
    pci_disable_device(pdev);

    printk("%s: WLAN host driver shutting down completed!\n", __func__);
}

void hif_pci_crash_shutdown(struct pci_dev *pdev)
{
#ifdef TARGET_RAMDUMP_AFTER_KERNEL_PANIC
    struct hif_pci_softc *sc;
    struct ol_softc *scn;
    struct HIF_CE_state *hif_state;

    sc = pci_get_drvdata(pdev);
    if (!sc)
        return;

    hif_state = (struct HIF_CE_state *)sc->hif_device;
    if (!hif_state)
        return;

    scn = sc->ol_sc;
    if (!scn)
        return;

    if (OL_TRGET_STATUS_RESET == scn->target_status) {
        printk("%s: Target is already asserted, ignore!\n", __func__);
        return;
    }

    adf_os_spin_lock_irqsave(&hif_state->suspend_lock);

#ifdef DEBUG
    if (hif_pci_check_soc_status(scn->hif_sc)
        || dump_CE_register(scn)) {
        goto out;
    }

    dump_CE_debug_register(scn->hif_sc);
#endif

    if (ol_copy_ramdump(scn)) {
        goto out;
    }

    printk("%s: RAM dump collecting completed!\n", __func__);

out:
    adf_os_spin_unlock_irqrestore(&hif_state->suspend_lock);
    return;
#else
    printk("%s: Collecting target RAM dump after kernel panic is disabled!\n",
           __func__);
    return;
#endif
}
#endif

#define OL_ATH_PCI_PM_CONTROL 0x44

static int
__hif_pci_suspend(struct pci_dev *pdev, pm_message_t state)
{
    struct hif_pci_softc *sc = pci_get_drvdata(pdev);
    void *vos = vos_get_global_context(VOS_MODULE_ID_HIF, NULL);
    ol_txrx_pdev_handle txrx_pdev = vos_get_context(VOS_MODULE_ID_TXRX, vos);
    struct HIF_CE_state *hif_state = (struct HIF_CE_state *)sc->hif_device;
    A_target_id_t targid = hif_state->targid;
    u32 tx_drain_wait_cnt = 0;
    u32 val;
    u32 ce_drain_wait_cnt = 0;
    v_VOID_t * temp_module;
    u32 tmp;
    int ret = -1;

    if (vos_is_logp_in_progress(VOS_MODULE_ID_HIF, NULL))
        return ret;

    A_TARGET_ACCESS_BEGIN_RET(targid);
    A_PCI_WRITE32(sc->mem + FW_INDICATOR_ADDRESS, (state.event << 16));
    A_TARGET_ACCESS_END_RET(targid);

    if (!txrx_pdev) {
        printk("%s: txrx_pdev is NULL\n", __func__);
        goto out;
    }
    /* Wait for pending tx completion */
    while (ol_txrx_get_tx_pending(txrx_pdev)) {
        msleep(OL_ATH_TX_DRAIN_WAIT_DELAY);
        if (++tx_drain_wait_cnt > OL_ATH_TX_DRAIN_WAIT_CNT) {
            printk("%s: tx frames are pending\n", __func__);
            goto out;
        }
    }

    /* No need to send WMI_PDEV_SUSPEND_CMDID to FW if WOW is enabled */
    temp_module = vos_get_context(VOS_MODULE_ID_WDA, vos);
    if (!temp_module) {
        printk("%s: WDA module is NULL\n", __func__);
        goto out;
    }

    if (wma_check_scan_in_progress(temp_module)) {
        printk("%s: Scan in progress. Aborting suspend\n", __func__);
        goto out;
    }

    printk("%s: wow mode %d event %d\n", __func__,
       wma_is_wow_mode_selected(temp_module), state.event);

    if (wma_is_wow_mode_selected(temp_module)) {
          if(wma_enable_wow_in_fw(temp_module))
            goto out;
    } else if (state.event == PM_EVENT_FREEZE || state.event == PM_EVENT_SUSPEND) {
          if (wma_suspend_target(temp_module, 0))
            goto out;
    }

    while (!adf_os_atomic_read(&sc->ce_suspend)) {
        if (++ce_drain_wait_cnt > HIF_CE_DRAIN_WAIT_CNT) {
            printk("%s: CE still not done with access: \n", __func__);
            adf_os_atomic_set(&sc->wow_done, 0);

            A_TARGET_ACCESS_BEGIN_RET(targid);
            val = A_PCI_READ32(sc->mem + FW_INDICATOR_ADDRESS) >> 16;
            A_TARGET_ACCESS_END_RET(targid);

            if (!wma_is_wow_mode_selected(temp_module) &&
               (val == PM_EVENT_HIBERNATE || val == PM_EVENT_SUSPEND)) {
                  wma_resume_target(temp_module);
                goto out;
            }
            else {
               wma_disable_wow_in_fw(temp_module);
               goto out;
            }
        }
        printk("%s: Waiting for CE to finish access: \n", __func__);
        msleep(10);
    }

    adf_os_spin_lock_irqsave(&hif_state->suspend_lock);

    /*Disable PCIe interrupts*/
    if (Q_TARGET_ACCESS_BEGIN(targid) < 0) {
        adf_os_spin_unlock_irqrestore( &hif_state->suspend_lock);
        return -1;
    }

    A_PCI_WRITE32(sc->mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS), 0);
    A_PCI_WRITE32(sc->mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_CLR_ADDRESS),
                  PCIE_INTR_FIRMWARE_MASK | PCIE_INTR_CE_MASK_ALL);
    /* IMPORTANT: this extra read transaction is required to flush the posted write buffer */
    tmp = A_PCI_READ32(sc->mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS));
    if (tmp == 0xffffffff) {
         printk(KERN_ERR "%s: PCIe pcie link is down\n", __func__);
         VOS_ASSERT(0);
    }

    if (Q_TARGET_ACCESS_END(targid) < 0) {
        adf_os_spin_unlock_irqrestore( &hif_state->suspend_lock);
        return -1;
    }

    /* Stop the HIF Sleep Timer */
    HIFCancelDeferredTargetSleep(sc->hif_device);

    adf_os_atomic_set(&sc->pci_link_suspended, 1);

    adf_os_spin_unlock_irqrestore( &hif_state->suspend_lock);

#ifdef CONFIG_CNSS
    /* Keep PCIe bus driver's shadow memory intact */
    cnss_pcie_shadow_control(pdev, FALSE);
#endif

    pci_read_config_dword(pdev, OL_ATH_PCI_PM_CONTROL, &val);
    if ((val & 0x000000ff) != 0x3) {
        pci_save_state(pdev);
        pci_disable_device(pdev);
        pci_write_config_dword(pdev, OL_ATH_PCI_PM_CONTROL, (val & 0xffffff00) | 0x03);
    }

    printk("%s: Suspend completes\n", __func__);
    ret = 0;

out:
    return ret;
}

static int hif_pci_suspend(struct pci_dev *pdev, pm_message_t state)
{
    int ret;

    vos_ssr_protect(__func__);

    ret = __hif_pci_suspend(pdev, state);

    vos_ssr_unprotect(__func__);

    return ret;
}

static int
__hif_pci_resume(struct pci_dev *pdev)
{
    struct hif_pci_softc *sc = pci_get_drvdata(pdev);
    void *vos_context = vos_get_global_context(VOS_MODULE_ID_HIF, NULL);
    struct HIF_CE_state *hif_state = (struct HIF_CE_state *)sc->hif_device;
    A_target_id_t targid = hif_state->targid;
    u32 val;
    int err = 0;
    v_VOID_t * temp_module;
    u32 tmp;

    if (vos_is_logp_in_progress(VOS_MODULE_ID_HIF, NULL))
        return err;

    adf_os_atomic_set(&sc->pci_link_suspended, 0);

    /* Enable Legacy PCI line interrupts */
    A_TARGET_ACCESS_BEGIN_RET(targid);
    A_PCI_WRITE32(sc->mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS),
                              PCIE_INTR_FIRMWARE_MASK | PCIE_INTR_CE_MASK_ALL);
    /* IMPORTANT: this extra read transaction is required to flush the posted write buffer */
    tmp = A_PCI_READ32(sc->mem+(SOC_CORE_BASE_ADDRESS | PCIE_INTR_ENABLE_ADDRESS));
    if (tmp == 0xffffffff) {
        printk(KERN_ERR "%s: PCIe link is down\n", __func__);
        VOS_ASSERT(0);
    }
    A_TARGET_ACCESS_END_RET(targid);


    err = pci_enable_device(pdev);
    if (err)
    {
        printk("\n%s %d : pci_enable_device returned failure %d\n",
           __func__, __LINE__, err);
        goto out;
    }

    pci_read_config_dword(pdev, OL_ATH_PCI_PM_CONTROL, &val);
    if ((val & 0x000000ff) != 0) {
        pci_restore_state(pdev);
        pci_write_config_dword(pdev, OL_ATH_PCI_PM_CONTROL, val & 0xffffff00);

        /*
         * Suspend/Resume resets the PCI configuration space, so we have to
         * re-disable the RETRY_TIMEOUT register (0x41) to keep
         * PCI Tx retries from interfering with C3 CPU state
         *
         */
        pci_read_config_dword(pdev, 0x40, &val);

        if ((val & 0x0000ff00) != 0)
            pci_write_config_dword(pdev, 0x40, val & 0xffff00ff);
    }

    printk("\n%s: Rome PS: %d", __func__, val);

#ifdef CONFIG_CNSS
    /* Keep PCIe bus driver's shadow memory intact */
    cnss_pcie_shadow_control(pdev, TRUE);
#endif

#ifdef DISABLE_L1SS_STATES
    pci_read_config_dword(pdev, 0x188, &val);
    pci_write_config_dword(pdev, 0x188, (val & ~0x0000000f));
#endif

    A_TARGET_ACCESS_BEGIN_RET(targid);
    val = A_PCI_READ32(sc->mem + FW_INDICATOR_ADDRESS) >> 16;
    A_TARGET_ACCESS_END_RET(targid);

    /* No need to send WMI_PDEV_RESUME_CMDID to FW if WOW is enabled */
    temp_module = vos_get_context(VOS_MODULE_ID_WDA, vos_context);
    if (!temp_module) {
        printk("%s: WDA module is NULL\n", __func__);
        goto out;
    }

    printk("%s: wow mode %d val %d\n", __func__,
       wma_is_wow_mode_selected(temp_module), val);

    adf_os_atomic_set(&sc->wow_done, 0);

    if (!wma_is_wow_mode_selected(temp_module) &&
        (val == PM_EVENT_HIBERNATE || val == PM_EVENT_SUSPEND))
        err = wma_resume_target(temp_module);
    else
        err = wma_disable_wow_in_fw(temp_module);


out:
    printk("%s: Resume completes %d\n", __func__, err);

    if (err)
        return (-1);

    return (0);
}

static int
hif_pci_resume(struct pci_dev *pdev)
{
    int ret;

    vos_ssr_protect(__func__);

    ret = __hif_pci_resume(pdev);

    vos_ssr_unprotect(__func__);

    return ret;
}

/* routine to modify the initial buffer count to be allocated on an os
 * platform basis. Platform owner will need to modify this as needed
 */
adf_os_size_t initBufferCount(adf_os_size_t maxSize)
{
    return maxSize;
}

#ifdef CONFIG_CNSS
struct cnss_wlan_driver cnss_wlan_drv_id = {
    .name       = "hif_pci",
    .id_table   = hif_pci_id_table,
    .probe      = hif_pci_probe,
    .remove     = hif_pci_remove,
    .reinit     = hif_pci_reinit,
    .shutdown   = hif_pci_shutdown,
    .crash_shutdown = hif_pci_crash_shutdown,
    .modem_status   = hif_pci_notify_handler,
#ifdef ATH_BUS_PM
    .suspend    = hif_pci_suspend,
    .resume     = hif_pci_resume,
#endif
};
#else
MODULE_DEVICE_TABLE(pci, hif_pci_id_table);
struct pci_driver hif_pci_drv_id = {
    .name       = "hif_pci",
    .id_table   = hif_pci_id_table,
    .probe      = hif_pci_probe,
    .remove     = hif_pci_remove,
#ifdef ATH_BUS_PM
    .suspend    = hif_pci_suspend,
    .resume     = hif_pci_resume,
#endif
};
#endif

int hif_register_driver(void)
{
#ifdef CONFIG_CNSS
    return cnss_wlan_register_driver(&cnss_wlan_drv_id);
#else
    return pci_register_driver(&hif_pci_drv_id);
#endif
}

void hif_unregister_driver(void)
{
#ifdef CONFIG_CNSS
	cnss_wlan_unregister_driver(&cnss_wlan_drv_id);
#else
	pci_unregister_driver(&hif_pci_drv_id);
#endif
}

void hif_init_pdev_txrx_handle(void *ol_sc, void *txrx_handle)
{
	struct ol_softc *sc = (struct ol_softc *)ol_sc;
	sc->pdev_txrx_handle = txrx_handle;
}

void hif_disable_isr(void *ol_sc)
{
	struct ol_softc *sc = (struct ol_softc *)ol_sc;
	struct hif_pci_softc *hif_sc = sc->hif_sc;
	struct ol_softc *scn;

	scn = hif_sc->ol_sc;
	hif_nointrs(hif_sc);
#if CONFIG_PCIE_64BIT_MSI
	OS_FREE_CONSISTENT(scn->sc_osdev, 4, scn->MSI_magic, scn->MSI_magic_dma,
                       OS_GET_DMA_MEM_CONTEXT(scn, MSI_dmacontext));
	scn->MSI_magic = NULL;
	scn->MSI_magic_dma = 0;
#endif
	/* Cancel the pending tasklet */
	tasklet_kill(&hif_sc->intr_tq);
}

void hif_reset_soc(void *ol_sc)
{
	struct ol_softc *scn = (struct ol_softc *)ol_sc;
	struct hif_pci_softc *sc = scn->hif_sc;

#if defined(CPU_WARM_RESET_WAR)
	/* Currently CPU warm reset sequence is tested only for AR9888_REV2
	* Need to enable for AR9888_REV1 once CPU warm reset sequence is
	* verified for AR9888_REV1
	*/
	if (scn->target_version == AR9888_REV2_VERSION) {
		hif_pci_device_warm_reset(sc);
	}
	else {
		hif_pci_device_reset(sc);
	}
#else
	hif_pci_device_reset(sc);
#endif
}

void hif_disable_aspm(void)
{
	u_int32_t lcr_val = 0;
	void *vos_context = vos_get_global_context(VOS_MODULE_ID_HIF, NULL);
	struct ol_softc *scn =  vos_get_context(VOS_MODULE_ID_HIF, vos_context);
	struct hif_pci_softc *sc;

	if (NULL == scn)
	{
		printk(KERN_ERR "%s: Could not disable ASPM scn is null\n", __func__);
		return;
	}

	sc = scn->hif_sc;

	/* Disable ASPM when pkt log is enabled */
	pci_read_config_dword(sc->pdev, 0x80, &lcr_val);
	pci_write_config_dword(sc->pdev, 0x80, (lcr_val & 0xffffff00));
}

void hif_pci_save_htc_htt_config_endpoint(int htc_endpoint)
{
    void *vos_context = vos_get_global_context(VOS_MODULE_ID_HIF, NULL);
    struct ol_softc *scn =  vos_get_context(VOS_MODULE_ID_HIF, vos_context);

    if (!scn || !scn->hif_sc) {
        printk(KERN_ERR "%s: error: scn or scn->hif_sc is NULL!\n", __func__);
        return;
    }

    scn->hif_sc->htc_endpoint = htc_endpoint;
}

void hif_get_hw_info(void *ol_sc, u32 *version, u32 *revision)
{
    *version = ((struct ol_softc *)ol_sc)->target_version;
    *revision = ((struct ol_softc *)ol_sc)->target_revision;
}
