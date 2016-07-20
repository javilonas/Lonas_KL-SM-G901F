/*
 * Copyright (c) 2011-2014 The Linux Foundation. All rights reserved.
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
 * Copyright (c) 2011-2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */


#if !defined( __SMEINTERNAL_H )
#define __SMEINTERNAL_H


/**=========================================================================

  \file  smeInternal.h

  \brief prototype for SME internal structures and APIs used for SME and MAC

  ========================================================================*/

/* $Header$ */

/*--------------------------------------------------------------------------
  Include Files
  ------------------------------------------------------------------------*/
#include "vos_status.h"
#include "vos_lock.h"
#include "vos_trace.h"
#include "vos_memory.h"
#include "vos_types.h"
#include "vos_diag_core_event.h"
#include "csrLinkList.h"

/*--------------------------------------------------------------------------
  Type declarations
  ------------------------------------------------------------------------*/

// Mask can be only have one bit set
typedef enum eSmeCommandType
{
    eSmeNoCommand = 0,
    eSmeDropCommand,
    //CSR
    eSmeCsrCommandMask = 0x10000,   //this is not a command, it is to identify this is a CSR command
    eSmeCommandScan,
    eSmeCommandRoam,
    eSmeCommandWmStatusChange,
    eSmeCommandSetKey,
    eSmeCommandRemoveKey,
    eSmeCommandAddStaSession,
    eSmeCommandDelStaSession,
#ifdef FEATURE_WLAN_TDLS
    //eSmeTdlsCommandMask = 0x80000,  //To identify TDLS commands <TODO>
    //These can be considered as csr commands.
    eSmeCommandTdlsSendMgmt,
    eSmeCommandTdlsAddPeer,
    eSmeCommandTdlsDelPeer,
    eSmeCommandTdlsLinkEstablish,
#ifdef FEATURE_WLAN_TDLS_INTERNAL
    eSmeCommandTdlsDiscovery,
    eSmeCommandTdlsLinkSetup,
    eSmeCommandTdlsLinkTear,
    eSmeCommandTdlsEnterUapsd,
    eSmeCommandTdlsExitUapsd,
#endif
#endif
    //PMC
    eSmePmcCommandMask = 0x20000, //To identify PMC commands
    eSmeCommandEnterImps,
    eSmeCommandExitImps,
    eSmeCommandEnterBmps,
    eSmeCommandExitBmps,
    eSmeCommandEnterUapsd,
    eSmeCommandExitUapsd,
    eSmeCommandEnterWowl,
    eSmeCommandExitWowl,
    eSmeCommandEnterStandby,
    //QOS
    eSmeQosCommandMask = 0x40000,  //To identify Qos commands
    eSmeCommandAddTs,
    eSmeCommandDelTs,
#ifdef FEATURE_OEM_DATA_SUPPORT
    eSmeCommandOemDataReq = 0x80000, //To identify the oem data commands
#endif
    eSmeCommandRemainOnChannel,
    eSmeCommandNoAUpdate,
} eSmeCommandType;


typedef enum eSmeState
{
    SME_STATE_STOP,
    SME_STATE_START,
    SME_STATE_READY,
} eSmeState;

#define SME_IS_START(pMac)  (SME_STATE_STOP != (pMac)->sme.state)
#define SME_IS_READY(pMac)  (SME_STATE_READY == (pMac)->sme.state)

typedef struct sStatsExtEvent {
    tANI_U32 event_data_len;
    tANI_U8 event_data[];
} tStatsExtEvent, *tpStatsExtEvent;

#define MAX_ACTIVE_CMD_STATS    16

typedef struct sActiveCmdStats {
    eSmeCommandType command;
    tANI_U32 reason;
    tANI_U32 sessionId;
    v_U64_t timestamp;
} tActiveCmdStats;

typedef struct sSelfRecoveryStats {
    tActiveCmdStats activeCmdStats[MAX_ACTIVE_CMD_STATS];
    tANI_U8 cmdStatsIndx;
} tSelfRecoveryStats;

typedef struct tagSmeStruct
{
    eSmeState state;
    vos_lock_t lkSmeGlobalLock;
    tANI_U32 totalSmeCmd;
    void *pSmeCmdBufAddr;
    tDblLinkList smeCmdActiveList;
    tDblLinkList smeCmdPendingList;
    tDblLinkList smeCmdFreeList;   //preallocated roam cmd list
    void (*pTxPerHitCallback) (void *pCallbackContext); /* callback for Tx PER hit to HDD */
    void *pTxPerHitCbContext;
    tVOS_CON_MODE currDeviceMode;
#ifdef FEATURE_WLAN_LPHB
    void (*pLphbIndCb) (void *pAdapter, void *indParam);
#endif /* FEATURE_WLAN_LPHB */
    //pending scan command list
    tDblLinkList smeScanCmdPendingList;
    //active scan command list
    tDblLinkList smeScanCmdActiveList;
#ifdef FEATURE_WLAN_DIAG_SUPPORT_CSR
    vos_event_wlan_status_payload_type eventPayload;
#endif
#ifdef FEATURE_WLAN_CH_AVOID
    void (*pChAvoidNotificationCb) (void *hdd_context, void *indi_param);
#endif /* FEATURE_WLAN_CH_AVOID */
    /* Maximum interfaces allowed by the host */
    tANI_U8 max_intf_count;
    void (* StatsExtCallback) (void *, tStatsExtEvent *);
    /* linkspeed callback */
    void (*pLinkSpeedIndCb) (tSirLinkSpeedInfo *indParam, void *pDevContext);
    void *pLinkSpeedCbContext;
    v_BOOL_t enableSelfRecovery;
} tSmeStruct, *tpSmeStruct;


#endif //#if !defined( __SMEINTERNAL_H )
