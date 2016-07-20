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

/**
 * @addtogroup WMIAPI
 *@{
 */

/** @file
 * This file specifies the WMI interface for the  Software Architecture.
 *
 * It includes definitions of all the commands and events. Commands are messages
 * from the host to the target. Events and Replies are messages from the target
 * to the host.
 *
 * Ownership of correctness in regards to WMI commands
 * belongs to the host driver and the target is not required to validate
 * parameters for value, proper range, or any other checking.
 *
 * Guidelines for extending this interface are below.
 *
 * 1. Add new WMI commands ONLY within the specified range - 0x9000 - 0x9fff
 * 2. Use ONLY A_UINT32 type for defining member variables within WMI command/event
 *    structures. Do not use A_UINT8, A_UINT16, A_BOOL or enum types within these structures.
 * 3. DO NOT define bit fields within structures. Implement bit fields using masks
 *    if necessary. Do not use the programming language's bit field definition.
 * 4. Define macros for encode/decode of A_UINT8, A_UINT16 fields within the A_UINT32
 *    variables. Use these macros for set/get of these fields. Try to use this to
 *    optimize the structure without bloating it with A_UINT32 variables for every lower
 *    sized field.
 * 5. Do not use PACK/UNPACK attributes for the structures as each member variable is
 *    already 4-byte aligned by virtue of being a A_UINT32 type.
 * 6. Comment each parameter part of the WMI command/event structure by using the
 *    2 stars at the begining of C comment instead of one star to enable HTML document
 *    generation using Doxygen.
 *
 */

#ifndef _WMI_UNIFIED_H_
#define _WMI_UNIFIED_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <wlan_defs.h>
#include <wmi_services.h>
#include <dbglog.h>

#define ATH_MAC_LEN             6               /**< length of MAC in bytes */
#define WMI_EVENT_STATUS_SUCCESS 0 /* Success return status to host */
#define WMI_EVENT_STATUS_FAILURE 1 /* Failure return status to host */

#define MAX_TX_RATE_VALUES      10 /*Max Tx Rates*/
#define MAX_RSSI_VALUES         10 /*Max Rssi values*/

//The maximum value of access category
#define WLAN_MAX_AC  4

/*
 * These don't necessarily belong here; but as the MS/SM macros require
 * ar6000_internal.h to be included, it may not be defined as yet.
 */
#define WMI_F_MS(_v, _f)                                            \
            ( ((_v) & (_f)) >> (_f##_S) )

/*
 * This breaks the "good macro practice" of only referencing each
 * macro field once (to avoid things like field++ from causing issues.)
 */
#define WMI_F_RMW(_var, _v, _f)                                     \
            do {                                                    \
                (_var) &= ~(_f);                                    \
                (_var) |= ( ((_v) << (_f##_S)) & (_f));             \
            } while (0)

/** 2 word representation of MAC addr */
typedef struct {
    /** upper 4 bytes of  MAC address */
    A_UINT32 mac_addr31to0;
    /** lower 2 bytes of  MAC address */
    A_UINT32 mac_addr47to32;
} wmi_mac_addr;

/** macro to convert MAC address from WMI word format to char array */
#define WMI_MAC_ADDR_TO_CHAR_ARRAY(pwmi_mac_addr,c_macaddr) do {               \
     (c_macaddr)[0] =    ((pwmi_mac_addr)->mac_addr31to0) & 0xff;     \
     (c_macaddr)[1] =  ( ((pwmi_mac_addr)->mac_addr31to0) >> 8) & 0xff; \
     (c_macaddr)[2] =  ( ((pwmi_mac_addr)->mac_addr31to0) >> 16) & 0xff; \
     (c_macaddr)[3] =  ( ((pwmi_mac_addr)->mac_addr31to0) >> 24) & 0xff;  \
     (c_macaddr)[4] =    ((pwmi_mac_addr)->mac_addr47to32) & 0xff;        \
     (c_macaddr)[5] =  ( ((pwmi_mac_addr)->mac_addr47to32) >> 8) & 0xff; \
   } while(0)

/** macro to convert MAC address from char array to WMI word format */
#define WMI_CHAR_ARRAY_TO_MAC_ADDR(c_macaddr,pwmi_mac_addr)  do { \
    (pwmi_mac_addr)->mac_addr31to0  =                                   \
        ( (c_macaddr)[0] | ((c_macaddr)[1] << 8)                           \
               | ((c_macaddr)[2] << 16) | ((c_macaddr)[3] << 24) );         \
    (pwmi_mac_addr)->mac_addr47to32  =                                  \
                  ( (c_macaddr)[4] | ((c_macaddr)[5] << 8));             \
   } while(0)


/*
 * wmi command groups.
 */
typedef enum {
    /* 0 to 2 are reserved */
    WMI_GRP_START=0x3,
    WMI_GRP_SCAN=WMI_GRP_START,
    WMI_GRP_PDEV,
    WMI_GRP_VDEV,
    WMI_GRP_PEER,
    WMI_GRP_MGMT,
    WMI_GRP_BA_NEG,
    WMI_GRP_STA_PS,
    WMI_GRP_DFS,
    WMI_GRP_ROAM,
    WMI_GRP_OFL_SCAN,
    WMI_GRP_P2P,
    WMI_GRP_AP_PS,
    WMI_GRP_RATE_CTRL,
    WMI_GRP_PROFILE,
    WMI_GRP_SUSPEND,
    WMI_GRP_BCN_FILTER,
    WMI_GRP_WOW,
    WMI_GRP_RTT,
    WMI_GRP_SPECTRAL,
    WMI_GRP_STATS,
    WMI_GRP_ARP_NS_OFL,
    WMI_GRP_NLO_OFL,
    WMI_GRP_GTK_OFL,
    WMI_GRP_CSA_OFL,
    WMI_GRP_CHATTER,
    WMI_GRP_TID_ADDBA,
    WMI_GRP_MISC,
    WMI_GRP_GPIO,
    WMI_GRP_FWTEST,
    WMI_GRP_TDLS,
    WMI_GRP_RESMGR,
    WMI_GRP_STA_SMPS,
    WMI_GRP_WLAN_HB,
    WMI_GRP_RMC,
    WMI_GRP_MHF_OFL,
    WMI_GRP_LOCATION_SCAN,
    WMI_GRP_OEM,
    WMI_GRP_NAN,
    WMI_GRP_COEX,
    WMI_GRP_OBSS_OFL,
    WMI_GRP_LPI,
    WMI_GRP_EXTSCAN,
} WMI_GRP_ID;

#define WMI_CMD_GRP_START_ID(grp_id) (((grp_id) << 12) | 0x1)
#define WMI_EVT_GRP_START_ID(grp_id) (((grp_id) << 12) | 0x1)

/**
 * Command IDs and commange events
 */
typedef enum {
    /** initialize the wlan sub system */
    WMI_INIT_CMDID=0x1,

    /* Scan specific commands */

    /** start scan request to FW  */
    WMI_START_SCAN_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_SCAN) ,
    /** stop scan request to FW  */
    WMI_STOP_SCAN_CMDID,
    /** full list of channels as defined by the regulatory that will be used by scanner   */
    WMI_SCAN_CHAN_LIST_CMDID,
    /** overwrite default priority table in scan scheduler   */
    WMI_SCAN_SCH_PRIO_TBL_CMDID,
    /** This command to adjust the priority and min.max_rest_time
     * of an on ongoing scan request.
     */
    WMI_SCAN_UPDATE_REQUEST_CMDID,

    /* PDEV(physical device) specific commands */
    /** set regulatorty ctl id used by FW to determine the exact ctl power limits */
    WMI_PDEV_SET_REGDOMAIN_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_PDEV),
    /** set channel. mainly used for supporting monitor mode */
    WMI_PDEV_SET_CHANNEL_CMDID,
    /** set pdev specific parameters */
    WMI_PDEV_SET_PARAM_CMDID,
    /** enable packet log */
    WMI_PDEV_PKTLOG_ENABLE_CMDID,
    /** disable packet log*/
    WMI_PDEV_PKTLOG_DISABLE_CMDID,
    /** set wmm parameters */
    WMI_PDEV_SET_WMM_PARAMS_CMDID,
    /** set HT cap ie that needs to be carried probe requests HT/VHT channels */
    WMI_PDEV_SET_HT_CAP_IE_CMDID,
    /** set VHT cap ie that needs to be carried on probe requests on VHT channels */
    WMI_PDEV_SET_VHT_CAP_IE_CMDID,

    /** Command to send the DSCP-to-TID map to the target */
    WMI_PDEV_SET_DSCP_TID_MAP_CMDID,
    /** set quiet ie parameters. primarily used in AP mode */
    WMI_PDEV_SET_QUIET_MODE_CMDID,
    /** Enable/Disable Green AP Power Save  */
    WMI_PDEV_GREEN_AP_PS_ENABLE_CMDID,
    /** get TPC config for the current operating channel */
    WMI_PDEV_GET_TPC_CONFIG_CMDID,

    /** set the base MAC address for the physical device before a VDEV is created.
     *  For firmware that doesn�t support this feature and this command, the pdev
     *  MAC address will not be changed. */
    WMI_PDEV_SET_BASE_MACADDR_CMDID,

    /* eeprom content dump , the same to bdboard data */
    WMI_PDEV_DUMP_CMDID,
     /* set LED configuration  */
    WMI_PDEV_SET_LED_CONFIG_CMDID,

    /* VDEV(virtual device) specific commands */
    /** vdev create */
    WMI_VDEV_CREATE_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_VDEV),
    /** vdev delete */
    WMI_VDEV_DELETE_CMDID,
    /** vdev start request */
    WMI_VDEV_START_REQUEST_CMDID,
    /** vdev restart request (RX only, NO TX, used for CAC period)*/
    WMI_VDEV_RESTART_REQUEST_CMDID,
    /** vdev up request */
    WMI_VDEV_UP_CMDID,
    /** vdev stop request */
    WMI_VDEV_STOP_CMDID,
    /** vdev down request */
    WMI_VDEV_DOWN_CMDID,
    /* set a vdev param */
    WMI_VDEV_SET_PARAM_CMDID,
    /* set a key (used for setting per peer unicast and per vdev multicast) */
    WMI_VDEV_INSTALL_KEY_CMDID,

    /* wnm sleep mode command */
    WMI_VDEV_WNM_SLEEPMODE_CMDID,
    WMI_VDEV_WMM_ADDTS_CMDID,
    WMI_VDEV_WMM_DELTS_CMDID,
    WMI_VDEV_SET_WMM_PARAMS_CMDID,
    WMI_VDEV_SET_GTX_PARAMS_CMDID,
    WMI_VDEV_IPSEC_NATKEEPALIVE_FILTER_CMDID,

    WMI_VDEV_PLMREQ_START_CMDID,
    WMI_VDEV_PLMREQ_STOP_CMDID,

    /* peer specific commands */

    /** create a peer */
    WMI_PEER_CREATE_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_PEER),
    /** delete a peer */
    WMI_PEER_DELETE_CMDID,
    /** flush specific  tid queues of a peer */
    WMI_PEER_FLUSH_TIDS_CMDID,
    /** set a parameter of a peer */
    WMI_PEER_SET_PARAM_CMDID,
    /** set peer to associated state. will cary all parameters determined during assocication time */
    WMI_PEER_ASSOC_CMDID,
    /**add a wds  (4 address ) entry. used only for testing WDS feature on AP products */
    WMI_PEER_ADD_WDS_ENTRY_CMDID,
    /**remove wds  (4 address ) entry. used only for testing WDS feature on AP products */
    WMI_PEER_REMOVE_WDS_ENTRY_CMDID,
    /** set up mcast group infor for multicast to unicast conversion */
    WMI_PEER_MCAST_GROUP_CMDID,
    /** request peer info from FW. FW shall respond with PEER_INFO_EVENTID */
    WMI_PEER_INFO_REQ_CMDID,

    /** request the estimated link speed for the peer. FW shall respond with
     *  WMI_PEER_ESTIMATED_LINKSPEED_EVENTID.
     */
    WMI_PEER_GET_ESTIMATED_LINKSPEED_CMDID,

    /* beacon/management specific commands */

    /** transmit beacon by reference . used for transmitting beacon on low latency interface like pcie */
    WMI_BCN_TX_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_MGMT),
    /** transmit beacon by value */
    WMI_PDEV_SEND_BCN_CMDID,
    /** set the beacon template. used in beacon offload mode to setup the
     *  the common beacon template with the FW to be used by FW to generate beacons */
    WMI_BCN_TMPL_CMDID,
    /** set beacon filter with FW */
    WMI_BCN_FILTER_RX_CMDID,
    /* enable/disable filtering of probe requests in the firmware */
    WMI_PRB_REQ_FILTER_RX_CMDID,
    /** transmit management frame by value. will be deprecated */
    WMI_MGMT_TX_CMDID,
    /** set the probe response template. used in beacon offload mode to setup the
     *  the common probe response template with the FW to be used by FW to generate
     *  probe responses */
    WMI_PRB_TMPL_CMDID,

    /** commands to directly control ba negotiation directly from host. only used in test mode */

    /** turn off FW Auto addba mode and let host control addba */
    WMI_ADDBA_CLEAR_RESP_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_BA_NEG),
    /** send add ba request */
    WMI_ADDBA_SEND_CMDID,
    WMI_ADDBA_STATUS_CMDID,
    /** send del ba */
    WMI_DELBA_SEND_CMDID,
    /** set add ba response will be used by FW to generate addba response*/
    WMI_ADDBA_SET_RESP_CMDID,
    /** send single VHT MPDU with AMSDU */
    WMI_SEND_SINGLEAMSDU_CMDID,

    /** Station power save specific config */
    /** enable/disable station powersave */
    WMI_STA_POWERSAVE_MODE_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_STA_PS),
    /** set station power save specific parameter */
    WMI_STA_POWERSAVE_PARAM_CMDID,
    /** set station mimo powersave mode */
    WMI_STA_MIMO_PS_MODE_CMDID,


    /** DFS-specific commands */
    /** enable DFS (radar detection)*/
    WMI_PDEV_DFS_ENABLE_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_DFS),
    /** disable DFS (radar detection)*/
    WMI_PDEV_DFS_DISABLE_CMDID,
    /** enable DFS phyerr/parse filter offload */
    WMI_DFS_PHYERR_FILTER_ENA_CMDID,
    /** enable DFS phyerr/parse filter offload */
    WMI_DFS_PHYERR_FILTER_DIS_CMDID,

    /* Roaming specific  commands */
    /** set roam scan mode */
    WMI_ROAM_SCAN_MODE=WMI_CMD_GRP_START_ID(WMI_GRP_ROAM),
    /** set roam scan rssi threshold below which roam scan is enabled  */
    WMI_ROAM_SCAN_RSSI_THRESHOLD,
    /** set roam scan period for periodic roam scan mode  */
    WMI_ROAM_SCAN_PERIOD,
    /** set roam scan trigger rssi change threshold   */
    WMI_ROAM_SCAN_RSSI_CHANGE_THRESHOLD,
    /** set roam AP profile   */
    WMI_ROAM_AP_PROFILE,
    /** set channel list for roam scans */
    WMI_ROAM_CHAN_LIST,
    /** Stop scan command */
    WMI_ROAM_SCAN_CMD,

    /** offload scan specific commands */
    /** set offload scan AP profile   */
    WMI_OFL_SCAN_ADD_AP_PROFILE=WMI_CMD_GRP_START_ID(WMI_GRP_OFL_SCAN),
    /** remove offload scan AP profile   */
    WMI_OFL_SCAN_REMOVE_AP_PROFILE,
    /** set offload scan period   */
    WMI_OFL_SCAN_PERIOD,

    /* P2P specific commands */
    /**set P2P device info. FW will used by FW to create P2P IE to be carried in probe response
     * generated during p2p listen and for p2p discoverability  */
    WMI_P2P_DEV_SET_DEVICE_INFO=WMI_CMD_GRP_START_ID(WMI_GRP_P2P),
    /** enable/disable p2p discoverability on STA/AP VDEVs  */
    WMI_P2P_DEV_SET_DISCOVERABILITY,
    /** set p2p ie to be carried in beacons generated by FW for GO  */
    WMI_P2P_GO_SET_BEACON_IE,
    /** set p2p ie to be carried in probe response frames generated by FW for GO  */
    WMI_P2P_GO_SET_PROBE_RESP_IE,
    /** set the vendor specific p2p ie data. FW will use this to parse the P2P NoA
     *  attribute in the beacons/probe responses received.
     */
    WMI_P2P_SET_VENDOR_IE_DATA_CMDID,
    /** set the configure of p2p find offload */
    WMI_P2P_DISC_OFFLOAD_CONFIG_CMDID,
    /** set the vendor specific p2p ie data for p2p find offload using */
    WMI_P2P_DISC_OFFLOAD_APPIE_CMDID,
    /** set the BSSID/device name pattern of p2p find offload */
    WMI_P2P_DISC_OFFLOAD_PATTERN_CMDID,
	/** set OppPS related parameters **/
	WMI_P2P_SET_OPPPS_PARAM_CMDID,

    /** AP power save specific config */
    /** set AP power save specific param */
    WMI_AP_PS_PEER_PARAM_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_AP_PS),
    /** set AP UAPSD coex pecific param */
    WMI_AP_PS_PEER_UAPSD_COEX_CMDID,


    /** Rate-control specific commands */
    WMI_PEER_RATE_RETRY_SCHED_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_RATE_CTRL),

    /** WLAN Profiling commands. */
    WMI_WLAN_PROFILE_TRIGGER_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_PROFILE),
    WMI_WLAN_PROFILE_SET_HIST_INTVL_CMDID,
    WMI_WLAN_PROFILE_GET_PROFILE_DATA_CMDID,
    WMI_WLAN_PROFILE_ENABLE_PROFILE_ID_CMDID,
    WMI_WLAN_PROFILE_LIST_PROFILE_ID_CMDID,

    /** Suspend resume command Ids */
    WMI_PDEV_SUSPEND_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_SUSPEND),
    WMI_PDEV_RESUME_CMDID,

    /* Beacon filter commands */
    /** add a beacon filter */
    WMI_ADD_BCN_FILTER_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_BCN_FILTER),
    /** remove a  beacon filter */
    WMI_RMV_BCN_FILTER_CMDID,

    /* WOW Specific WMI commands*/
    /** add pattern for awake */
    WMI_WOW_ADD_WAKE_PATTERN_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_WOW),
    /** deleta a wake pattern */
    WMI_WOW_DEL_WAKE_PATTERN_CMDID,
    /** enable/deisable wake event  */
    WMI_WOW_ENABLE_DISABLE_WAKE_EVENT_CMDID,
    /** enable WOW  */
    WMI_WOW_ENABLE_CMDID,
    /** host woke up from sleep event to FW. Generated in response to WOW Hardware event */
    WMI_WOW_HOSTWAKEUP_FROM_SLEEP_CMDID,
    /* Acer IOAC add keep alive cmd. */
    WMI_WOW_ACER_IOAC_ADD_KEEPALIVE_CMDID,
    /* Acer IOAC del keep alive cmd. */
    WMI_WOW_ACER_IOAC_DEL_KEEPALIVE_CMDID,
    /* Acer IOAC add pattern for awake */
    WMI_WOW_ACER_IOAC_ADD_WAKE_PATTERN_CMDID,
    /* Acer IOAC deleta a wake pattern */
    WMI_WOW_ACER_IOAC_DEL_WAKE_PATTERN_CMDID,

    /* RTT measurement related cmd */
    /** reques to make an RTT measurement */
    WMI_RTT_MEASREQ_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_RTT),
    /** reques to report a tsf measurement */
    WMI_RTT_TSF_CMDID,

    /** spectral scan command */
    /** configure spectral scan */
    WMI_VDEV_SPECTRAL_SCAN_CONFIGURE_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_SPECTRAL),
    /** enable/disable spectral scan and trigger */
    WMI_VDEV_SPECTRAL_SCAN_ENABLE_CMDID,

    /* F/W stats */
    /** one time request for stats */
    WMI_REQUEST_STATS_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_STATS),
    /** Push MCC Adaptive Scheduler Stats to Firmware */
    WMI_MCC_SCHED_TRAFFIC_STATS_CMDID,
    /** one time request for txrx stats */
    WMI_REQUEST_STATS_EXT_CMDID,

    /* Link Layer stats */
    /** Request for link layer stats */
    WMI_REQUEST_LINK_STATS_CMDID,
    /** Request for setting params to link layer stats */
    WMI_START_LINK_STATS_CMDID,
    /** Request to clear stats*/
    WMI_CLEAR_LINK_STATS_CMDID,

    /** ARP OFFLOAD REQUEST*/
    WMI_SET_ARP_NS_OFFLOAD_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_ARP_NS_OFL),

    /** Proactive ARP Response Add Pattern Command*/
    WMI_ADD_PROACTIVE_ARP_RSP_PATTERN_CMDID,

    /** Proactive ARP Response Del Pattern Command*/
    WMI_DEL_PROACTIVE_ARP_RSP_PATTERN_CMDID,

    /** NS offload confid*/
    WMI_NETWORK_LIST_OFFLOAD_CONFIG_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_NLO_OFL),

	/* GTK offload Specific WMI commands*/
	WMI_GTK_OFFLOAD_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_GTK_OFL),

	/* CSA offload Specific WMI commands*/
    /** csa offload enable */
    WMI_CSA_OFFLOAD_ENABLE_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_CSA_OFL),
    /** chan switch command */
    WMI_CSA_OFFLOAD_CHANSWITCH_CMDID,

    /* Chatter commands*/
    /* Change chatter mode of operation */
    WMI_CHATTER_SET_MODE_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_CHATTER),
    /** chatter add coalescing filter command */
    WMI_CHATTER_ADD_COALESCING_FILTER_CMDID,
    /** chatter delete coalescing filter command */
    WMI_CHATTER_DELETE_COALESCING_FILTER_CMDID,
    /** chatter coalecing query command */
    WMI_CHATTER_COALESCING_QUERY_CMDID,

    /**addba specific commands */
    /** start the aggregation on this TID */
    WMI_PEER_TID_ADDBA_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_TID_ADDBA),
    /** stop the aggregation on this TID */
    WMI_PEER_TID_DELBA_CMDID,

    /** set station mimo powersave method */
    WMI_STA_DTIM_PS_METHOD_CMDID,
    /** Configure the Station UAPSD AC Auto Trigger Parameters */
    WMI_STA_UAPSD_AUTO_TRIG_CMDID,
    /** Configure the Keep Alive Parameters */
    WMI_STA_KEEPALIVE_CMDID,

    /* Request ssn from target for a sta/tid pair */
    WMI_BA_REQ_SSN_CMDID,
    /* misc command group */
    /** echo command mainly used for testing */
    WMI_ECHO_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_MISC),

	/* !!IMPORTANT!!
	  * If you need to add a new WMI command to the WMI_GRP_MISC sub-group,
	  * please make sure you add it BEHIND WMI_PDEV_UTF_CMDID,
	  * as we MUST have a fixed value here to maintain compatibility between
	  * UTF and the ART2 driver
	*/
	/** UTF WMI commands */
	WMI_PDEV_UTF_CMDID,

    /** set debug log config */
    WMI_DBGLOG_CFG_CMDID,
    /* QVIT specific command id */
    WMI_PDEV_QVIT_CMDID,
    /* Factory Testing Mode request command
     * used for integrated chipsets */
    WMI_PDEV_FTM_INTG_CMDID,
    /* set and get keepalive parameters command */
    WMI_VDEV_SET_KEEPALIVE_CMDID,
    WMI_VDEV_GET_KEEPALIVE_CMDID,
    /* For fw recovery test command */
    WMI_FORCE_FW_HANG_CMDID,
    /* Set Mcast/Bdcast filter */
    WMI_SET_MCASTBCAST_FILTER_CMDID,
    /** set thermal management params **/
    WMI_THERMAL_MGMT_CMDID,
    /** set host auto shutdown params **/
    WMI_HOST_AUTO_SHUTDOWN_CFG_CMDID,

    /* GPIO Configuration */
    WMI_GPIO_CONFIG_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_GPIO),
    WMI_GPIO_OUTPUT_CMDID,

    /* Txbf configuration command */
    WMI_TXBF_CMDID,

    /* FWTEST Commands */
    WMI_FWTEST_VDEV_MCC_SET_TBTT_MODE_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_FWTEST),
	/** set NoA descs **/
	WMI_FWTEST_P2P_SET_NOA_PARAM_CMDID,

    /** TDLS Configuration */
    /** enable/disable TDLS */
    WMI_TDLS_SET_STATE_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_TDLS),
    /** set tdls peer state */
    WMI_TDLS_PEER_UPDATE_CMDID,

    /** Resmgr Configuration */
    /** Adaptive OCS is enabled by default in the FW. This command is used to
     * disable FW based adaptive OCS.
     */
    WMI_RESMGR_ADAPTIVE_OCS_ENABLE_DISABLE_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_RESMGR),
    /** set the requested channel time quota for the home channels */
    WMI_RESMGR_SET_CHAN_TIME_QUOTA_CMDID,
    /** set the requested latency for the home channels */
    WMI_RESMGR_SET_CHAN_LATENCY_CMDID,

    /** STA SMPS Configuration */
    /** force SMPS mode */
    WMI_STA_SMPS_FORCE_MODE_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_STA_SMPS),
    /** set SMPS parameters */
    WMI_STA_SMPS_PARAM_CMDID,

    /* Wlan HB commands*/
    /* enalbe/disable wlan HB */
    WMI_HB_SET_ENABLE_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_WLAN_HB),
    /* set tcp parameters for wlan HB */
    WMI_HB_SET_TCP_PARAMS_CMDID,
    /* set tcp pkt filter for wlan HB */
    WMI_HB_SET_TCP_PKT_FILTER_CMDID,
    /* set udp parameters for wlan HB */
    WMI_HB_SET_UDP_PARAMS_CMDID,
    /* set udp pkt filter for wlan HB */
    WMI_HB_SET_UDP_PKT_FILTER_CMDID,

    /** Wlan RMC commands*/
    /** enable/disable RMC */
    WMI_RMC_SET_MODE_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_RMC),
    /** configure action frame period */
    WMI_RMC_SET_ACTION_PERIOD_CMDID,
    /** For debug/future enhancement purposes only,
     *  configures/finetunes RMC algorithms */
    WMI_RMC_CONFIG_CMDID,

    /** WLAN multihop forwarding (MHF) offload commands */
    /** enable/disable multihop forwarding offload */
    WMI_MHF_OFFLOAD_SET_MODE_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_MHF_OFL),
    /** Plumb routing table for multihop forwarding offload */
    WMI_MHF_OFFLOAD_PLUMB_ROUTING_TBL_CMDID,

    /*location scan commands*/
    /*start batch scan*/
    WMI_BATCH_SCAN_ENABLE_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_LOCATION_SCAN),
    /*stop batch scan*/
    WMI_BATCH_SCAN_DISABLE_CMDID,
    /*get batch scan result*/
    WMI_BATCH_SCAN_TRIGGER_RESULT_CMDID,
    /* OEM related cmd */
    WMI_OEM_REQ_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_OEM),

    /** Nan Request */
    WMI_NAN_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_NAN),

    /** Modem power state command */
    WMI_MODEM_POWER_STATE_CMDID=WMI_CMD_GRP_START_ID(WMI_GRP_COEX),
    WMI_CHAN_AVOID_UPDATE_CMDID,

    /**
     *  OBSS scan offload enable/disable commands
     *  OBSS scan enable CMD will send to FW after VDEV UP, if these conditions are true:
     *  1.  WMI_SERVICE_OBSS_SCAN is reported by FW in service ready,
     *  2.  STA connect to a 2.4Ghz ht20/ht40 AP,
     *  3.  AP enable 20/40 coexistence (OBSS_IE-74 can be found in beacon or association response)
     *  If OBSS parameters from beacon changed, also use enable CMD to update parameters.
     *  OBSS scan disable CMD will send to FW if have enabled when tearing down connection.
     */
    WMI_OBSS_SCAN_ENABLE_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_OBSS_OFL),
    WMI_OBSS_SCAN_DISABLE_CMDID,

    /**LPI commands*/
    /**LPI mgmt snooping config command*/
    WMI_LPI_MGMT_SNOOPING_CONFIG_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_LPI),
    /**LPI scan start command*/
    WMI_LPI_START_SCAN_CMDID,
    /**LPI scan stop command*/
    WMI_LPI_STOP_SCAN_CMDID,

     /** ExtScan commands */
    WMI_EXTSCAN_START_CMDID = WMI_CMD_GRP_START_ID(WMI_GRP_EXTSCAN),
    WMI_EXTSCAN_STOP_CMDID,
    WMI_EXTSCAN_CONFIGURE_WLAN_CHANGE_MONITOR_CMDID,
    WMI_EXTSCAN_CONFIGURE_HOTLIST_MONITOR_CMDID,
    WMI_EXTSCAN_GET_CACHED_RESULTS_CMDID,
    WMI_EXTSCAN_GET_WLAN_CHANGE_RESULTS_CMDID,
    WMI_EXTSCAN_SET_CAPABILITIES_CMDID,
    WMI_EXTSCAN_GET_CAPABILITIES_CMDID,
} WMI_CMD_ID;

typedef enum {
    /** WMI service is ready; after this event WMI messages can be sent/received  */
    WMI_SERVICE_READY_EVENTID=0x1,
    /** WMI is ready; after this event the wlan subsystem is initialized and can process commands. */
    WMI_READY_EVENTID,

    /** Scan specific events */
    WMI_SCAN_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_SCAN) ,

    /* PDEV specific events */
    /** TPC config for the current operating channel */
    WMI_PDEV_TPC_CONFIG_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_PDEV) ,
    /** Channel stats event    */
    WMI_CHAN_INFO_EVENTID,

    /** PHY Error specific WMI event */
    WMI_PHYERR_EVENTID,

    /** eeprom dump event  */
    WMI_PDEV_DUMP_EVENTID,

    /** traffic pause event */
    WMI_TX_PAUSE_EVENTID,

    /** DFS radar event  */
    WMI_DFS_RADAR_EVENTID,

    /** track L1SS entry and residency event */
    WMI_PDEV_L1SS_TRACK_EVENTID,

    /* VDEV specific events */
    /** VDEV started event in response to VDEV_START request */
    WMI_VDEV_START_RESP_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_VDEV),
    /** vdev stopped event , generated in response to VDEV_STOP request */
    WMI_VDEV_STOPPED_EVENTID,
    /* Indicate the set key (used for setting per
     * peer unicast and per vdev multicast)
     * operation has completed */
    WMI_VDEV_INSTALL_KEY_COMPLETE_EVENTID,
    /* NOTE: WMI_VDEV_MCC_BCN_INTERVAL_CHANGE_REQ_EVENTID would be deprecated. Please
     don't use this for any new implementations */
    /* Firmware requests dynamic change to a specific beacon interval for a specific vdev ID in MCC scenario.
     This request is valid only for vdevs operating in soft AP or P2P GO mode */
    WMI_VDEV_MCC_BCN_INTERVAL_CHANGE_REQ_EVENTID,

    /* peer  specific events */
    /** FW reauet to kick out the station for reasons like inactivity,lack of response ..etc */
    WMI_PEER_STA_KICKOUT_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_PEER),

    /** Peer Info Event with data_rate, rssi, tx_fail_cnt etc */
    WMI_PEER_INFO_EVENTID,

    /** Event indicating that TX fail count reaching threshold */
    WMI_PEER_TX_FAIL_CNT_THR_EVENTID,
    /** Return the estimate link speed for the Peer specified in the
     * WMI_PEER_GET_ESTIMATED_LINKSPEED_CMDID command.
     */
    WMI_PEER_ESTIMATED_LINKSPEED_EVENTID,
    /* Return the peer state
     * WMI_PEER_SET_PARAM_CMDID, WMI_PEER_AUTHORIZE
     */
    WMI_PEER_STATE_EVENTID,

    /* beacon/mgmt specific events */
    /** RX management frame. the entire frame is carried along with the event.  */
    WMI_MGMT_RX_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_MGMT),
    /** software beacon alert event to Host requesting host to Queue a beacon for transmission
        use only in host beacon mode */
    WMI_HOST_SWBA_EVENTID,
    /** beacon tbtt offset event indicating the tsf offset of the tbtt from the theritical value.
        tbtt offset is normally 0 and will be non zero if there are multiple VDEVs operating in
        staggered beacon transmission mode */
    WMI_TBTTOFFSET_UPDATE_EVENTID,

    /** event after the first beacon is transmitted following
             a change in the template.*/
    WMI_OFFLOAD_BCN_TX_STATUS_EVENTID,
    /** event after the first probe response is transmitted following
             a change in the template.*/
    WMI_OFFLOAD_PROB_RESP_TX_STATUS_EVENTID,

    /*ADDBA Related WMI Events*/
    /** Indication the completion of the prior
       WMI_PEER_TID_DELBA_CMDID(initiator) */
    WMI_TX_DELBA_COMPLETE_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_BA_NEG),
    /** Indication the completion of the prior
       *WMI_PEER_TID_ADDBA_CMDID(initiator) */
    WMI_TX_ADDBA_COMPLETE_EVENTID,

    /* Seq num returned from hw for a sta/tid pair */
    WMI_BA_RSP_SSN_EVENTID,

    /* Aggregation state requested by BTC */
    WMI_AGGR_STATE_TRIG_EVENTID,

    /** Roam event to trigger roaming on host */
    WMI_ROAM_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_ROAM),

    /** matching AP found from list of profiles */
    WMI_PROFILE_MATCH,

    /** P2P disc found */
    WMI_P2P_DISC_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_P2P),

    /*send noa info to host when noa is changed for beacon tx offload enable*/
    WMI_P2P_NOA_EVENTID,

    /** WOW wake up host event.generated in response to WMI_WOW_HOSTWAKEUP_FROM_SLEEP_CMDID.
        will cary wake reason */
    WMI_WOW_WAKEUP_HOST_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_WOW),

    /*RTT related event ID*/
    /** RTT measurement report */
    WMI_RTT_MEASUREMENT_REPORT_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_RTT),
    /** TSF measurement report */
    WMI_TSF_MEASUREMENT_REPORT_EVENTID,
    /** RTT error report */
    WMI_RTT_ERROR_REPORT_EVENTID,
    /*STATS specific events*/
    /** txrx stats event requested by host */
    WMI_STATS_EXT_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_STATS),
    /** FW iface link stats Event  */
    WMI_IFACE_LINK_STATS_EVENTID,
    /** FW iface peer link stats Event  */
    WMI_PEER_LINK_STATS_EVENTID,
    /** FW Update radio stats Event  */
    WMI_RADIO_LINK_STATS_EVENTID,

    /*NLO specific events*/
    /** NLO match event after the first match */
    WMI_NLO_MATCH_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_NLO_OFL),

    /** NLO scan complete event */
    WMI_NLO_SCAN_COMPLETE_EVENTID,


    /** GTK offload stautus event requested by host */
    WMI_GTK_OFFLOAD_STATUS_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_GTK_OFL),

	/** GTK offload failed to rekey event */
	WMI_GTK_REKEY_FAIL_EVENTID,
    /* CSA IE received event */
    WMI_CSA_HANDLING_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_CSA_OFL),

    /*chatter query reply event*/
    WMI_CHATTER_PC_QUERY_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_CHATTER),

    /** echo event in response to echo command */
    WMI_ECHO_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_MISC),

	/* !!IMPORTANT!!
	  * If you need to add a new WMI event ID to the WMI_GRP_MISC sub-group,
	  * please make sure you add it BEHIND WMI_PDEV_UTF_EVENTID,
	  * as we MUST have a fixed value here to maintain compatibility between
	  * UTF and the ART2 driver
	*/
	/** UTF specific WMI event */
	WMI_PDEV_UTF_EVENTID,

    /** event carries buffered debug messages  */
    WMI_DEBUG_MESG_EVENTID,
    /** FW stats(periodic or on shot)  */
    WMI_UPDATE_STATS_EVENTID,
    /** debug print message used for tracing FW code while debugging  */
    WMI_DEBUG_PRINT_EVENTID,
    /** DCS wlan or non-wlan interference event
     */
    WMI_DCS_INTERFERENCE_EVENTID,
    /** VI spoecific event  */
    WMI_PDEV_QVIT_EVENTID,
    /** FW code profile data in response to profile request  */
    WMI_WLAN_PROFILE_DATA_EVENTID,
    /* Factory Testing Mode request event
     * used for integrated chipsets */
    WMI_PDEV_FTM_INTG_EVENTID,
    /* avoid list of frequencies .
     */
    WMI_WLAN_FREQ_AVOID_EVENTID,
    /* Indicate the keepalive parameters */
    WMI_VDEV_GET_KEEPALIVE_EVENTID,
    /* Thermal Management event */
    WMI_THERMAL_MGMT_EVENTID,

    /* Container for QXDM/DIAG events */
    WMI_DIAG_DATA_CONTAINER_EVENTID,

     /* host auto shutdown event */
    WMI_HOST_AUTO_SHUTDOWN_EVENTID,

    /*update mib counters together with WMI_UPDATE_STATS_EVENTID*/
    WMI_UPDATE_WHAL_MIB_STATS_EVENTID,

    /* GPIO Event */
    WMI_GPIO_INPUT_EVENTID=WMI_EVT_GRP_START_ID(WMI_GRP_GPIO),
    /** upload H_CV info WMI event
     * to indicate uploaded H_CV info to host
     */
    WMI_UPLOADH_EVENTID,

    /** capture H info WMI event
     * to indicate captured H info to host
     */
    WMI_CAPTUREH_EVENTID,
    /* hw RFkill */
    WMI_RFKILL_STATE_CHANGE_EVENTID,

    /* TDLS Event */
    WMI_TDLS_PEER_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_TDLS),

    /*location scan event*/
    /*report the firmware's capability of batch scan*/
    WMI_BATCH_SCAN_ENABLED_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_LOCATION_SCAN),
    /*batch scan result*/
    WMI_BATCH_SCAN_RESULT_EVENTID,
    /* OEM Event */
    WMI_OEM_CAPABILITY_EVENTID=WMI_EVT_GRP_START_ID(WMI_GRP_OEM),
    WMI_OEM_MEASUREMENT_REPORT_EVENTID,
    WMI_OEM_ERROR_REPORT_EVENTID,

    /* NAN Event */
    WMI_NAN_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_NAN),

    /* LPI Event */
    WMI_LPI_RESULT_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_LPI),

     /* ExtScan events */
    WMI_EXTSCAN_START_STOP_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_EXTSCAN),
    WMI_EXTSCAN_OPERATION_EVENTID,
    WMI_EXTSCAN_TABLE_USAGE_EVENTID,
    WMI_EXTSCAN_CACHED_RESULTS_EVENTID,
    WMI_EXTSCAN_WLAN_CHANGE_RESULTS_EVENTID,
    WMI_EXTSCAN_HOTLIST_MATCH_EVENTID,
    WMI_EXTSCAN_CAPABILITIES_EVENTID,
} WMI_EVT_ID;

/* defines for OEM message sub-types */
#define WMI_OEM_CAPABILITY_REQ     0x01
#define WMI_OEM_CAPABILITY_RSP     0x02
#define WMI_OEM_MEASUREMENT_REQ    0x03
#define WMI_OEM_MEASUREMENT_RSP    0x04
#define WMI_OEM_ERROR_REPORT_RSP   0x05
#define WMI_OEM_NAN_MEAS_REQ       0x06
#define WMI_OEM_NAN_MEAS_RSP       0x07
#define WMI_OEM_NAN_PEER_INFO      0x08


/* below message subtype is internal to CLD. Target should
 * never use internal response type
 */
#define WMI_OEM_INTERNAL_RSP       0xdeadbeef

#define WMI_CHAN_LIST_TAG 0x1
#define WMI_SSID_LIST_TAG 0x2
#define WMI_BSSID_LIST_TAG 0x3
#define WMI_IE_TAG 0x4

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_channel */
    /** primary 20 MHz channel frequency in mhz */
    A_UINT32 mhz;
    /** Center frequency 1 in MHz*/
    A_UINT32 band_center_freq1;
    /** Center frequency 2 in MHz - valid only for 11acvht 80plus80 mode*/
    A_UINT32 band_center_freq2;
    /** channel info described below */
    A_UINT32 info;
    /** contains min power, max power, reg power and reg class id.  */
    A_UINT32 reg_info_1;
    /** contains antennamax */
    A_UINT32 reg_info_2;
} wmi_channel;

typedef enum{
WMI_CHANNEL_CHANGE_CAUSE_NONE = 0,
WMI_CHANNEL_CHANGE_CAUSE_CSA,
}wmi_channel_change_cause;

/** channel info consists of 6 bits of channel mode */

#define WMI_SET_CHANNEL_MODE(pwmi_channel,val) do { \
     (pwmi_channel)->info &= 0xffffffc0;            \
     (pwmi_channel)->info |= (val);                 \
     } while(0)

#define WMI_GET_CHANNEL_MODE(pwmi_channel) ((pwmi_channel)->info & 0x0000003f )

#define WMI_CHAN_FLAG_HT40_PLUS   6
#define WMI_CHAN_FLAG_PASSIVE     7
#define WMI_CHAN_ADHOC_ALLOWED    8
#define WMI_CHAN_AP_DISABLED      9
#define WMI_CHAN_FLAG_DFS         10
#define WMI_CHAN_FLAG_ALLOW_HT    11  /* HT is allowed on this channel */
#define WMI_CHAN_FLAG_ALLOW_VHT   12  /* VHT is allowed on this channel */
#define WMI_CHANNEL_CHANGE_CAUSE_CSA 13 /*Indicate reason for channel switch */

#define WMI_SET_CHANNEL_FLAG(pwmi_channel,flag) do { \
        (pwmi_channel)->info |=  (1 << flag);      \
     } while(0)

#define WMI_GET_CHANNEL_FLAG(pwmi_channel,flag)   \
        (((pwmi_channel)->info & (1 << flag)) >> flag)

#define WMI_SET_CHANNEL_MIN_POWER(pwmi_channel,val) do { \
     (pwmi_channel)->reg_info_1 &= 0xffffff00;           \
     (pwmi_channel)->reg_info_1 |= (val&0xff);           \
     } while(0)
#define WMI_GET_CHANNEL_MIN_POWER(pwmi_channel) ((pwmi_channel)->reg_info_1 & 0xff )

#define WMI_SET_CHANNEL_MAX_POWER(pwmi_channel,val) do { \
     (pwmi_channel)->reg_info_1 &= 0xffff00ff;           \
     (pwmi_channel)->reg_info_1 |= ((val&0xff) << 8);    \
     } while(0)
#define WMI_GET_CHANNEL_MAX_POWER(pwmi_channel) ( (((pwmi_channel)->reg_info_1) >> 8) & 0xff )

#define WMI_SET_CHANNEL_REG_POWER(pwmi_channel,val) do { \
     (pwmi_channel)->reg_info_1 &= 0xff00ffff;           \
     (pwmi_channel)->reg_info_1 |= ((val&0xff) << 16);   \
     } while(0)
#define WMI_GET_CHANNEL_REG_POWER(pwmi_channel) ( (((pwmi_channel)->reg_info_1) >> 16) & 0xff )
#define WMI_SET_CHANNEL_REG_CLASSID(pwmi_channel,val) do { \
     (pwmi_channel)->reg_info_1 &= 0x00ffffff;             \
     (pwmi_channel)->reg_info_1 |= ((val&0xff) << 24);     \
     } while(0)
#define WMI_GET_CHANNEL_REG_CLASSID(pwmi_channel) ( (((pwmi_channel)->reg_info_1) >> 24) & 0xff )

#define WMI_SET_CHANNEL_ANTENNA_MAX(pwmi_channel,val) do { \
     (pwmi_channel)->reg_info_2 &= 0xffffff00;             \
     (pwmi_channel)->reg_info_2 |= (val&0xff);             \
     } while(0)
#define WMI_GET_CHANNEL_ANTENNA_MAX(pwmi_channel) ((pwmi_channel)->reg_info_2 & 0xff )

#define WMI_SET_CHANNEL_MAX_TX_POWER(pwmi_channel,val) do { \
     (pwmi_channel)->reg_info_2 &= 0xffff00ff;              \
     (pwmi_channel)->reg_info_2 |= ((val&0xff)<<8);         \
     } while(0)
#define WMI_GET_CHANNEL_MAX_TX_POWER(pwmi_channel) ( (((pwmi_channel)->reg_info_2)>>8) & 0xff )


/** HT Capabilities*/
#define WMI_HT_CAP_ENABLED                0x0001   /* HT Enabled/ disabled */
#define WMI_HT_CAP_HT20_SGI               0x0002   /* Short Guard Interval with HT20 */
#define WMI_HT_CAP_DYNAMIC_SMPS           0x0004   /* Dynamic MIMO powersave */
#define WMI_HT_CAP_TX_STBC                0x0008   /* B3 TX STBC */
#define WMI_HT_CAP_TX_STBC_MASK_SHIFT     3
#define WMI_HT_CAP_RX_STBC                0x0030   /* B4-B5 RX STBC */
#define WMI_HT_CAP_RX_STBC_MASK_SHIFT     4
#define WMI_HT_CAP_LDPC                   0x0040   /* LDPC supported */
#define WMI_HT_CAP_L_SIG_TXOP_PROT        0x0080   /* L-SIG TXOP Protection */
#define WMI_HT_CAP_MPDU_DENSITY           0x0700   /* MPDU Density */
#define WMI_HT_CAP_MPDU_DENSITY_MASK_SHIFT 8
#define WMI_HT_CAP_HT40_SGI               0x0800

/* These macros should be used when we wish to advertise STBC support for
 * only 1SS or 2SS or 3SS. */
#define WMI_HT_CAP_RX_STBC_1SS            0x0010   /* B4-B5 RX STBC */
#define WMI_HT_CAP_RX_STBC_2SS            0x0020   /* B4-B5 RX STBC */
#define WMI_HT_CAP_RX_STBC_3SS            0x0030   /* B4-B5 RX STBC */


#define WMI_HT_CAP_DEFAULT_ALL (WMI_HT_CAP_ENABLED       | \
                                WMI_HT_CAP_HT20_SGI      | \
                                WMI_HT_CAP_HT40_SGI      | \
                                WMI_HT_CAP_TX_STBC       | \
                                WMI_HT_CAP_RX_STBC       | \
                                WMI_HT_CAP_LDPC)

/* WMI_VHT_CAP_* these maps to ieee 802.11ac vht capability information
   field. The fields not defined here are not supported, or reserved.
   Do not change these masks and if you have to add new one follow the
   bitmask as specified by 802.11ac draft.
*/

#define WMI_VHT_CAP_MAX_MPDU_LEN_MASK            0x00000003
#define WMI_VHT_CAP_RX_LDPC                      0x00000010
#define WMI_VHT_CAP_SGI_80MHZ                    0x00000020
#define WMI_VHT_CAP_TX_STBC                      0x00000080
#define WMI_VHT_CAP_RX_STBC_MASK                 0x00000300
#define WMI_VHT_CAP_RX_STBC_MASK_SHIFT           8
#define WMI_VHT_CAP_MAX_AMPDU_LEN_EXP            0x03800000
#define WMI_VHT_CAP_MAX_AMPDU_LEN_EXP_SHIT       23
#define WMI_VHT_CAP_RX_FIXED_ANT                 0x10000000
#define WMI_VHT_CAP_TX_FIXED_ANT                 0x20000000

#define WMI_VHT_CAP_MAX_MPDU_LEN_11454           0x00000002

/* These macros should be used to advertise VHT CAP support */
#define WMI_VHT_CAP_SU_BFORMER            0x00000800
#define WMI_VHT_CAP_SU_BFORMEE            0x00001000
#define WMI_VHT_CAP_MU_BFORMER            0x00080000
#define WMI_VHT_CAP_MU_BFORMEE            0x00100000

/* These macros should be used when we wish to advertise STBC support for
 * only 1SS or 2SS or 3SS. */
#define WMI_VHT_CAP_RX_STBC_1SS            0x00000100
#define WMI_VHT_CAP_RX_STBC_2SS            0x00000200
#define WMI_vHT_CAP_RX_STBC_3SS            0x00000300

#define WMI_VHT_CAP_DEFAULT_ALL (WMI_VHT_CAP_MAX_MPDU_LEN_11454  |      \
                                 WMI_VHT_CAP_SGI_80MHZ           |      \
                                 WMI_VHT_CAP_TX_STBC             |      \
                                 WMI_VHT_CAP_RX_STBC_MASK        |      \
                                 WMI_VHT_CAP_RX_LDPC             |      \
                                 WMI_VHT_CAP_MAX_AMPDU_LEN_EXP   |      \
                                 WMI_VHT_CAP_RX_FIXED_ANT        |      \
                                 WMI_VHT_CAP_TX_FIXED_ANT)

/* Interested readers refer to Rx/Tx MCS Map definition as defined in
   802.11ac
*/
#define WMI_VHT_MAX_MCS_4_SS_MASK(r,ss)      ((3 & (r)) << (((ss) - 1) << 1))
#define WMI_VHT_MAX_SUPP_RATE_MASK           0x1fff0000
#define WMI_VHT_MAX_SUPP_RATE_MASK_SHIFT     16

/* WMI_SYS_CAPS_* refer to the capabilities that system support
*/
#define WMI_SYS_CAP_ENABLE                       0x00000001
#define WMI_SYS_CAP_TXPOWER                      0x00000002

/** NOTE: This structure cannot be extended in the future without breaking WMI compatibility */
typedef struct _wmi_abi_version {
    A_UINT32    abi_version_0;   /** WMI Major and Minor versions */
    A_UINT32    abi_version_1;   /** WMI change revision */
    A_UINT32    abi_version_ns_0;  /** ABI version namespace first four dwords */
    A_UINT32    abi_version_ns_1;  /** ABI version namespace second four dwords */
    A_UINT32    abi_version_ns_2;  /** ABI version namespace third four dwords */
    A_UINT32    abi_version_ns_3;  /** ABI version namespace fourth four dwords */
} wmi_abi_version;

/*
* maximum number of memroy requests allowed from FW.
*/
#define WMI_MAX_MEM_REQS 16

/* !!NOTE!!:
 * This HW_BD_INFO_SIZE cannot be changed without breaking compatibility.
 * Please don't change it.
 */
#define HW_BD_INFO_SIZE       5

/**
 * The following struct holds optional payload for
 * wmi_service_ready_event_fixed_param,e.g., 11ac pass some of the
 * device capability to the host.
*/
typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_SERVICE_READY_EVENT */
    A_UINT32    fw_build_vers; /* firmware build number */
    wmi_abi_version fw_abi_vers;
    A_UINT32    phy_capability;  /* WMI_PHY_CAPABILITY */
	A_UINT32    max_frag_entry;  /* Maximum number of frag table entries that SW will populate less 1 */
    A_UINT32    num_rf_chains;
    /* The following field is only valid for service type WMI_SERVICE_11AC */
    A_UINT32    ht_cap_info; /* WMI HT Capability */
    A_UINT32    vht_cap_info; /* VHT capability info field of 802.11ac */
    A_UINT32    vht_supp_mcs; /* VHT Supported MCS Set field Rx/Tx same */
    A_UINT32    hw_min_tx_power;
    A_UINT32    hw_max_tx_power;
    A_UINT32    sys_cap_info;
    A_UINT32    min_pkt_size_enable; /* Enterprise mode short pkt enable */
    /** Max beacon and Probe Response IE offload size (includes
     *  optional P2P IEs) */
    A_UINT32    max_bcn_ie_size;
    /*
     * request to host to allocate a chuck of memory and pss it down to FW via WM_INIT.
     * FW uses this as FW extesnsion memory for saving its data structures. Only valid
     * for low latency interfaces like PCIE where FW can access this memory directly (or)
     * by DMA.
     */
    A_UINT32    num_mem_reqs;
    /* Max No. scan channels target can support
     * If FW is too old and doesn't indicate this number, host side value will default to
     * 0, and host will take the original compatible value (62) for future scan channel
     * setup.
     */
    A_UINT32 max_num_scan_channels;

    /* Hardware board specific ID. Values defined in enum WMI_HWBOARD_ID.
     * Default 0 means tha hw_bd_info[] is invalid(legacy board).
     */
    A_UINT32 hw_bd_id;
    A_UINT32 hw_bd_info[HW_BD_INFO_SIZE]; /* Board specific information. Invalid if hw_hd_id is zero. */

    /* The TLVs for hal_reg_capabilities, wmi_service_bitmap and mem_reqs[] will follow this TLV.
         *     HAL_REG_CAPABILITIES   hal_reg_capabilities;
         *     A_UINT32 wmi_service_bitmap[WMI_SERVICE_BM_SIZE];
         *     wlan_host_mem_req mem_reqs[];
         */
} wmi_service_ready_event_fixed_param;

typedef enum {
    WMI_HWBD_NONE       = 0,            /* No hw board information is given */
    WMI_HWBD_QCA6174    = 1,            /* Rome(AR6320) */
    WMI_HWBD_QCA2582    = 2,            /* Killer 1525*/
} WMI_HWBD_ID;

#define ATH_BD_DATA_REV_MASK            0x000000FF
#define ATH_BD_DATA_REV_SHIFT           0

#define ATH_BD_DATA_PROJ_ID_MASK        0x0000FF00
#define ATH_BD_DATA_PROJ_ID_SHIFT       8

#define ATH_BD_DATA_CUST_ID_MASK        0x00FF0000
#define ATH_BD_DATA_CUST_ID_SHIFT       16

#define ATH_BD_DATA_REF_DESIGN_ID_MASK  0xFF000000
#define ATH_BD_DATA_REF_DESIGN_ID_SHIFT 24

#define SET_BD_DATA_REV(bd_data_ver, value)     \
    ((bd_data_ver) &= ~ATH_BD_DATA_REV_MASK, (bd_data_ver) |= ((value) << ATH_BD_DATA_REV_SHIFT))

#define GET_BD_DATA_REV(bd_data_ver)            \
    (((bd_data_ver) & ATH_BD_DATA_REV_MASK) >> ATH_BD_DATA_REV_SHIFT)

#define SET_BD_DATA_PROJ_ID(bd_data_ver, value) \
    ((bd_data_ver) &= ~ATH_BD_DATA_PROJ_ID_MASK, (bd_data_ver) |= ((value) << ATH_BD_DATA_PROJ_ID_SHIFT))

#define GET_BD_DATA_PROJ_ID(bd_data_ver)        \
    (((bd_data_ver) & ATH_BD_DATA_PROJ_ID_MASK) >> ATH_BD_DATA_PROJ_ID_SHIFT)

#define SET_BD_DATA_CUST_ID(bd_data_ver, value) \
    ((bd_data_ver) &= ~ATH_BD_DATA_CUST_ID_MASK, (bd_data_ver) |= ((value) << ATH_BD_DATA_CUST_ID_SHIFT))

#define GET_BD_DATA_CUST_ID(bd_data_ver)        \
    (((bd_data_ver) & ATH_BD_DATA_CUST_ID_MASK) >> ATH_BD_DATA_CUST_ID_SHIFT)

#define SET_BD_DATA_REF_DESIGN_ID(bd_data_ver, value)   \
    ((bd_data_ver) &= ~ATH_BD_DATA_REF_DESIGN_ID_MASK, (bd_data_ver) |= ((value) << ATH_BD_DATA_REF_DESIGN_ID_SHIFT))

#define GET_BD_DATA_REF_DESIGN_ID(bd_data_ver)          \
    (((bd_data_ver) & ATH_BD_DATA_REF_DESIGN_ID_MASK) >> ATH_BD_DATA_REF_DESIGN_ID_SHIFT)

#ifdef ROME_LTE_COEX_FREQ_AVOID
typedef struct {
    A_UINT32 start_freq; //start frequency, not channel center freq
    A_UINT32 end_freq;//end frequency
}avoid_freq_range_desc;

typedef struct {
    //bad channel range count, multi range is allowed, 0 means all channel clear
    A_UINT32 num_freq_ranges;
    //multi range with num_freq_ranges, LTE advance multi carrier, CDMA,etc
    avoid_freq_range_desc avd_freq_range[0];
}wmi_wlan_avoid_freq_ranges_event;
#endif

/** status consists of  upper 16 bits fo A_STATUS status and lower 16 bits of module ID that retuned status */
#define WLAN_INIT_STATUS_SUCCESS   0x0
#define WLAN_INIT_STATUS_GEN_FAILED   0x1
#define WLAN_GET_INIT_STATUS_REASON(status)    ((status) & 0xffff)
#define WLAN_GET_INIT_STATUS_MODULE_ID(status) (((status) >> 16) & 0xffff)

typedef A_UINT32 WLAN_INIT_STATUS;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_ready_event_fixed_param */
    wmi_abi_version fw_abi_vers;
    wmi_mac_addr mac_addr;
    A_UINT32    status;
} wmi_ready_event_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_resource_config */
/**
 * @brief num_vdev - number of virtual devices (VAPs) to support
 */
    A_UINT32 num_vdevs;
/**
 * @brief num_peers - number of peer nodes to support
 */
    A_UINT32 num_peers;
/*
 * @brief In offload mode target supports features like WOW, chatter and other
 * protocol offloads. In order to support them some functionalities like
 * reorder buffering, PN checking need to be done in target. This determines
 * maximum number of peers suported by target in offload mode
 */
    A_UINT32 num_offload_peers;
/* @brief Number of reorder buffers available for doing target based reorder
 * Rx reorder buffering
 */
    A_UINT32 num_offload_reorder_buffs;
/**
 * @brief num_peer_keys - number of keys per peer
 */
    A_UINT32 num_peer_keys;
/**
 * @brief num_peer_tids - number of TIDs to provide storage for per peer.
 */
    A_UINT32 num_tids;
/**
 * @brief ast_skid_limit - max skid for resolving hash collisions
 * @details
 *     The address search table is sparse, so that if two MAC addresses
 *     result in the same hash value, the second of these conflicting
 *     entries can slide to the next index in the address search table,
 *     and use it, if it is unoccupied.  This ast_skid_limit parameter
 *     specifies the upper bound on how many subsequent indices to search
 *     over to find an unoccupied space.
 */
    A_UINT32 ast_skid_limit;
/**
 * @brief tx_chain_mask - the nominal chain mask for transmit
 * @details
 *     The chain mask may be modified dynamically, e.g. to operate AP tx with
 *     a reduced number of chains if no clients are associated.
 *     This configuration parameter specifies the nominal chain-mask that
 *     should be used when not operating with a reduced set of tx chains.
 */
    A_UINT32 tx_chain_mask;
/**
 * @brief rx_chain_mask - the nominal chain mask for receive
 * @details
 *     The chain mask may be modified dynamically, e.g. for a client to use
 *     a reduced number of chains for receive if the traffic to the client
 *     is low enough that it doesn't require downlink MIMO or antenna
 *     diversity.
 *     This configuration parameter specifies the nominal chain-mask that
 *     should be used when not operating with a reduced set of rx chains.
 */
    A_UINT32 rx_chain_mask;
/**
 * @brief rx_timeout_pri - what rx reorder timeout (ms) to use for the AC
 * @details
 *     Each WMM access class (voice, video, best-effort, background) will
 *     have its own timeout value to dictate how long to wait for missing
 *     rx MPDUs to arrive before flushing subsequent MPDUs that have already
 *     been received.
 *     This parameter specifies the timeout in milliseconds for each class .
 *     NOTE: the number of class (defined as 4) cannot be
 *     changed in the future without breaking WMI compatibility.
 */
    A_UINT32 rx_timeout_pri[4];
/**
 * @brief rx_decap mode - what mode the rx should decap packets to
 * @details
 *     MAC can decap to RAW (no decap), native wifi or Ethernet types
 *     THis setting also determines the default TX behavior, however TX
 *     behavior can be modified on a per VAP basis during VAP init
 */
    A_UINT32 rx_decap_mode;
 /**
  * @brief  scan_max_pending_req - what is the maximum scan requests than can be queued
  */
    A_UINT32 scan_max_pending_req;

    /**
     * @brief maximum VDEV that could use BMISS offload
     */
    A_UINT32 bmiss_offload_max_vdev;

    /**
     * @brief maximum VDEV that could use offload roaming
     */
    A_UINT32 roam_offload_max_vdev;

    /**
     * @brief maximum AP profiles that would push to offload roaming
     */
    A_UINT32 roam_offload_max_ap_profiles;

/**
 * @brief num_mcast_groups - how many groups to use for mcast->ucast conversion
 * @details
 *     The target's WAL maintains a table to hold information regarding which
 *     peers belong to a given multicast group, so that if multicast->unicast
 *     conversion is enabled, the target can convert multicast tx frames to a
 *     series of unicast tx frames, to each peer within the multicast group.
 *     This num_mcast_groups configuration parameter tells the target how
 *     many multicast groups to provide storage for within its multicast
 *     group membership table.
 */
    A_UINT32 num_mcast_groups;

/**
 * @brief num_mcast_table_elems - size to alloc for the mcast membership table
 * @details
 *     This num_mcast_table_elems configuration parameter tells the target
 *     how many peer elements it needs to provide storage for in its
 *     multicast group membership table.
 *     These multicast group membership table elements are shared by the
 *     multicast groups stored within the table.
 */
    A_UINT32 num_mcast_table_elems;

/**
 * @brief mcast2ucast_mode - whether/how to do multicast->unicast conversion
 * @details
 *     This configuration parameter specifies whether the target should
 *     perform multicast --> unicast conversion on transmit, and if so,
 *     what to do if it finds no entries in its multicast group membership
 *     table for the multicast IP address in the tx frame.
 *     Configuration value:
 *     0 -> Do not perform multicast to unicast conversion.
 *     1 -> Convert multicast frames to unicast, if the IP multicast address
 *          from the tx frame is found in the multicast group membership
 *          table.  If the IP multicast address is not found, drop the frame.
 *     2 -> Convert multicast frames to unicast, if the IP multicast address
 *          from the tx frame is found in the multicast group membership
 *          table.  If the IP multicast address is not found, transmit the
 *          frame as multicast.
 */
    A_UINT32 mcast2ucast_mode;


 /**
  * @brief tx_dbg_log_size - how much memory to allocate for a tx PPDU dbg log
  * @details
  *     This parameter controls how much memory the target will allocate to
  *     store a log of tx PPDU meta-information (how large the PPDU was,
  *     when it was sent, whether it was successful, etc.)
  */
    A_UINT32 tx_dbg_log_size;

 /**
  * @brief num_wds_entries - how many AST entries to be allocated for WDS
  */
    A_UINT32 num_wds_entries;

 /**
  * @brief dma_burst_size - MAC DMA burst size, e.g., on Peregrine on PCI
  * this limit can be 0 -default, 1 256B
  */
    A_UINT32 dma_burst_size;

  /**
   * @brief mac_aggr_delim - Fixed delimiters to be inserted after every MPDU
   * to account for interface latency to avoid underrun.
   */
    A_UINT32 mac_aggr_delim;
    /**
     * @brief rx_skip_defrag_timeout_dup_detection_check
     * @details
     *  determine whether target is responsible for detecting duplicate
     *  non-aggregate MPDU and timing out stale fragments.
     *
     *  A-MPDU reordering is always performed on the target.
     *
     *  0: target responsible for frag timeout and dup checking
     *  1: host responsible for frag timeout and dup checking
     */
    A_UINT32 rx_skip_defrag_timeout_dup_detection_check;

    /**
     * @brief vow_config - Configuration for VoW : No of Video Nodes to be supported
     * and Max no of descriptors for each Video link (node).
     */
    A_UINT32 vow_config;

    /**
     * @brief maximum VDEV that could use GTK offload
     */
	A_UINT32 gtk_offload_max_vdev;

    /**
     * @brief num_msdu_desc - Number of msdu descriptors target should use
     */
    A_UINT32 num_msdu_desc; /* Number of msdu desc */
 /**
  * @brief max_frag_entry - Max. number of Tx fragments per MSDU
  * @details
  *     This parameter controls the max number of Tx fragments per MSDU.
  *     This is sent by the target as part of the WMI_SERVICE_READY event
  *     and is overriden by the OS shim as required.
  */
    A_UINT32 max_frag_entries;

    /**
     * @brief num_tdls_vdevs - Max. number of vdevs that can support TDLS
     * @brief num_msdu_desc - Number of vdev that can support beacon offload
     */

    A_UINT32 num_tdls_vdevs; /* number of vdevs allowed to do tdls */

    /**
     * @brief num_tdls_conn_table_entries - Number of peers tracked by tdls vdev
     * @details
     *      Each TDLS enabled vdev can track outgoing transmits/rssi/rates to/of
     *      peers in a connection tracking table for possible TDLS link creation
     *      or deletion. This controls the number of tracked peers per vdev.
     */
    A_UINT32  num_tdls_conn_table_entries; /* number of peers to track per TDLS vdev */
     A_UINT32 beacon_tx_offload_max_vdev;
     A_UINT32 num_multicast_filter_entries;
     A_UINT32 num_wow_filters; /*host can configure the number of wow filters*/

    /**
     * @brief num_keep_alive_pattern - Num of keep alive patterns configured
     * from host.
     */
    A_UINT32 num_keep_alive_pattern;
    /**
     * @brief keep_alive_pattern_size - keep alive pattern size.
     */
    A_UINT32 keep_alive_pattern_size;

    /**
     * @brief max_tdls_concurrent_sleep_sta - Number of tdls sleep sta supported
     * @details
     *      Each TDLS STA can become a sleep STA independently. This parameter
     *      mentions how many such sleep STAs can be supported concurrently.
     */
    A_UINT32 max_tdls_concurrent_sleep_sta;

    /**
     * @brief max_tdls_concurrent_buffer_sta - Number of tdls buffer sta supported
     * @details
     *      Each TDLS STA can become a buffer STA independently. This parameter
     *      mentions how many such buffer STAs can be supported concurrently.
     */
    A_UINT32 max_tdls_concurrent_buffer_sta;
} wmi_resource_config;


typedef struct {
    A_UINT32   tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_init_cmd_fixed_param */

    /** The following indicate the WMI versions to be supported by
     *  the host driver. Note that the host driver decide to
     *  "downgrade" its WMI version support and this may not be the
     *  native version of the host driver. */
    wmi_abi_version  host_abi_vers;

    A_UINT32   num_host_mem_chunks; /** size of array host_mem_chunks[] */
    /* The TLVs for resource_config and host_mem_chunks[] will follow.
     *     wmi_resource_config   resource_config;
     *     wlan_host_memory_chunk host_mem_chunks[];
     */

} wmi_init_cmd_fixed_param;

/**
 * TLV for channel list
 */
typedef struct {
    /** WMI_CHAN_LIST_TAG */
    A_UINT32     tag;
    /** # if channels to scan */
    A_UINT32 num_chan;
    /** channels in Mhz */
    A_UINT32 channel_list[1];
} wmi_chan_list;


/**
 * TLV for bssid list
 */
typedef struct {
    /** WMI_BSSID_LIST_TAG */
    A_UINT32     tag;
    /** number of bssids   */
    A_UINT32 num_bssid;
    /** bssid list         */
    wmi_mac_addr bssid_list[1];
} wmi_bssid_list;

/**
 * TLV for  ie data.
 */
typedef struct {
    /** WMI_IE_TAG */
    A_UINT32     tag;
    /** number of bytes in ie data   */
    A_UINT32 ie_len;
    /** ie data array  (ie_len adjusted to  number of words  (ie_len + 4)/4 )  */
    A_UINT32 ie_data[1];
} wmi_ie_data;


typedef struct {
    /** Len of the SSID */
    A_UINT32     ssid_len;
    /** SSID */
    A_UINT32     ssid[8];
} wmi_ssid;

typedef struct {
    /** WMI_SSID_LIST_TAG */
    A_UINT32     tag;
    A_UINT32     num_ssids;
    wmi_ssid ssids[1];
} wmi_ssid_list;

/* prefix used by scan requestor ids on the host */
#define WMI_HOST_SCAN_REQUESTOR_ID_PREFIX 0xA000
/* prefix used by scan request ids generated on the host */
/* host cycles through the lower 12 bits to generate ids */
#define WMI_HOST_SCAN_REQ_ID_PREFIX 0xA000

#define WLAN_SCAN_PARAMS_MAX_SSID    16
#define WLAN_SCAN_PARAMS_MAX_BSSID   4
#define WLAN_SCAN_PARAMS_MAX_IE_LEN  256

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_start_scan_cmd_fixed_param */
    /** Scan ID */
    A_UINT32 scan_id;
    /** Scan requestor ID */
    A_UINT32 scan_req_id;
    /** VDEV id(interface) that is requesting scan */
    A_UINT32 vdev_id;
    /** Scan Priority, input to scan scheduler */
    A_UINT32 scan_priority;
    /** Scan events subscription */
    A_UINT32 notify_scan_events;
    /** dwell time in msec on active channels */
    A_UINT32 dwell_time_active;
    /** dwell time in msec on passive channels */
    A_UINT32 dwell_time_passive;
    /** min time in msec on the BSS channel,only valid if atleast one VDEV is active*/
    A_UINT32 min_rest_time;
    /** max rest time in msec on the BSS channel,only valid if at least one VDEV is active*/
    /** the scanner will rest on the bss channel at least min_rest_time. after min_rest_time the scanner
     *  will start checking for tx/rx activity on all VDEVs. if there is no activity the scanner will
     *  switch to off channel. if there is activity the scanner will let the radio on the bss channel
     *  until max_rest_time expires.at max_rest_time scanner will switch to off channel
     *  irrespective of activity. activity is determined by the idle_time parameter.
     */
    A_UINT32  max_rest_time;
    /** time before sending next set of probe requests.
     *   The scanner keeps repeating probe requests transmission with period specified by repeat_probe_time.
     *   The number of probe requests specified depends on the ssid_list and bssid_list
     */
    A_UINT32  repeat_probe_time;
    /** time in msec between 2 consequetive probe requests with in a set. */
    A_UINT32  probe_spacing_time;
    /** data inactivity time in msec on bss channel that will be used by scanner for measuring the inactivity  */
    A_UINT32 idle_time;
    /** maximum time in msec allowed for scan  */
    A_UINT32  max_scan_time;
    /** delay in msec before sending first probe request after switching to a channel */
    A_UINT32  probe_delay;
    /** Scan control flags */
    A_UINT32 scan_ctrl_flags;
    /** Burst duration time in msec*/
    A_UINT32 burst_duration;

    /** # if channels to scan. In the TLV channel_list[] */
    A_UINT32 num_chan;
    /** number of bssids. In the TLV bssid_list[] */
    A_UINT32 num_bssid;
    /** number of ssid. In the TLV ssid_list[] */
    A_UINT32     num_ssids;
    /** number of bytes in ie data. In the TLV ie_data[] */
    A_UINT32 ie_len;
    /** Max number of probes to be sent */
    A_UINT32 n_probes;


    /**
         * TLV (tag length value ) parameters follow the scan_cmd
         * structure. The TLV's are:
         *     A_UINT32 channel_list[];
         *     wmi_ssid ssid_list[];
         *     wmi_mac_addr bssid_list[];
         *     A_UINT8 ie_data[];
         */
} wmi_start_scan_cmd_fixed_param;

/**
 * scan control flags.
 */

/** passively scan all channels including active channels */
#define WMI_SCAN_FLAG_PASSIVE        0x1
/** add wild card ssid probe request even though ssid_list is specified. */
#define WMI_SCAN_ADD_BCAST_PROBE_REQ 0x2
/** add cck rates to rates/xrate ie for the generated probe request */
#define WMI_SCAN_ADD_CCK_RATES 0x4
/** add ofdm rates to rates/xrate ie for the generated probe request */
#define WMI_SCAN_ADD_OFDM_RATES 0x8
/** To enable indication of Chan load and Noise floor to host */
#define WMI_SCAN_CHAN_STAT_EVENT 0x10
/** Filter Probe request frames  */
#define WMI_SCAN_FILTER_PROBE_REQ 0x20
/**When set, not to scan DFS channels*/
#define WMI_SCAN_BYPASS_DFS_CHN 0x40
/**When set, certain errors are ignored and scan continues.
* Different FW scan engine may use its own logic to decide what errors to ignore*/
#define WMI_SCAN_CONTINUE_ON_ERROR 0x80
/* Enable promiscous mode for CCXv4 */
#define WMI_SCAN_FILTER_PROMISCOUS 0x100
/** allow to send probe req on DFS channel */
#define WMI_SCAN_FLAG_FORCE_ACTIVE_ON_DFS 0x200

/** WMI_SCAN_CLASS_MASK must be the same value as IEEE80211_SCAN_CLASS_MASK */
#define WMI_SCAN_CLASS_MASK 0xFF000000

/*
* Masks identifying types/ID of scans
* Scan_Stop macros should be the same value as below defined in UMAC
* #define IEEE80211_SPECIFIC_SCAN       0x00000000
* #define IEEE80211_VAP_SCAN            0x01000000
* #define IEEE80211_ALL_SCANS           0x04000000
*/
#define WMI_SCAN_STOP_ONE       0x00000000
#define WMI_SCN_STOP_VAP_ALL    0x01000000
#define WMI_SCAN_STOP_ALL       0x04000000

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_stop_scan_cmd_fixed_param */
    /** requestor requesting cancel  */
    A_UINT32 requestor;
    /** Scan ID */
    A_UINT32 scan_id;
    /**
    * Req Type
    * req_type should be WMI_SCAN_STOP_ONE, WMI_SCN_STOP_VAP_ALL or WMI_SCAN_STOP_ALL
    * WMI_SCAN_STOP_ONE indicates to stop a specific scan with scan_id
    * WMI_SCN_STOP_VAP_ALL indicates to stop all scan requests on a specific vDev with vdev_id
    * WMI_SCAN_STOP_ALL indicates to stop all scan requests in both Scheduler's queue and Scan Engine
    */
    A_UINT32 req_type;
    /**
    * vDev ID
    * used when req_type equals to WMI_SCN_STOP_VAP_ALL, it indexed the vDev on which to stop the scan
    */
    A_UINT32 vdev_id;
} wmi_stop_scan_cmd_fixed_param;

#define MAX_NUM_CHAN_PER_WMI_CMD     58    // each WMI cmd can hold 58 channel entries at most
#define APPEND_TO_EXISTING_CHAN_LIST 1

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_scan_chan_list_cmd_fixed_param */
    A_UINT32 num_scan_chans;  /** no of elements in chan_info[] */
    A_UINT32 flags; /* Flags used to control the behavior of channel list update on target side */
    /** Followed by the variable length TLV chan_info:
     *  wmi_channel chan_info[] */
} wmi_scan_chan_list_cmd_fixed_param;

/*
 * Priority numbers must be sequential, starting with 0.
 */
 /* NOTE: WLAN SCAN_PRIORITY_COUNT can't be changed without breaking the compatibility */
typedef enum {
    WMI_SCAN_PRIORITY_VERY_LOW    = 0,
    WMI_SCAN_PRIORITY_LOW,
    WMI_SCAN_PRIORITY_MEDIUM,
    WMI_SCAN_PRIORITY_HIGH,
    WMI_SCAN_PRIORITY_VERY_HIGH,

    WMI_SCAN_PRIORITY_COUNT   /* number of priorities supported */
} wmi_scan_priority;

/* Five Levels for Requested Priority */
/* VERY_LOW LOW  MEDIUM   HIGH  VERY_HIGH */
typedef A_UINT32 WLAN_PRIORITY_MAPPING[WMI_SCAN_PRIORITY_COUNT];

/**
* to keep align with UMAC implementation, we pass only vdev_type but not vdev_subtype when we overwrite an entry for a specific vdev_subtype
* ex. if we need overwrite P2P Client prority entry, we will overwrite the whole table for WLAN_M_STA
* we will generate the new WLAN_M_STA table with modified P2P Client Entry but keep STA entry intact
*/
typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_scan_sch_priority_table_cmd_fixed_param */
    /**
    * used as an index to find the proper table for a specific vdev type in default_scan_priority_mapping_table
    * vdev_type should be one of enum in WLAN_OPMODE which inculdes WLAN_M_IBSS, WLAN_M_STA, WLAN_M_AP and WLAN_M_MONITOR currently
    */
    A_UINT32      vdev_type;
    /**
    * number of rows in mapping_table for a specific vdev
    * for WLAN_M_STA type, there are 3 entries in the table (refer to default_scan_priority_mapping_table definition)
    */
    A_UINT32                    number_rows;
    /**  mapping_table for a specific vdev follows this TLV
      *   WLAN_PRIORITY_MAPPING mapping_table[]; */
}wmi_scan_sch_priority_table_cmd_fixed_param;

/** update flags */
#define WMI_SCAN_UPDATE_SCAN_PRIORITY           0x1
#define WMI_SCAN_UPDATE_SCAN_MIN_REST_TIME      0x2
#define WMI_SCAN_UPDATE_SCAN_MAX_REST_TIME      0x4

typedef struct {
    A_UINT32 tlv_header;
    /** requestor requesting update scan request  */
    A_UINT32 requestor;
    /** Scan ID of the scan request that need to be update */
    A_UINT32 scan_id;
    /** update flags, indicating which of the following fields are valid and need to be updated*/
    A_UINT32 scan_update_flags;
    /** scan priority. Only valid if WMI_SCAN_UPDATE_SCAN_PRIORITY flag is set in scan_update_flag */
    A_UINT32 scan_priority;
    /** min rest time. Only valid if WMI_SCAN_UPDATE_MIN_REST_TIME flag is set in scan_update_flag */
    A_UINT32 min_rest_time;
    /** min rest time. Only valid if WMI_SCAN_UPDATE_MAX_REST_TIME flag is set in scan_update_flag */
    A_UINT32 max_rest_time;
} wmi_scan_update_request_cmd_fixed_param;

enum wmi_scan_event_type {
    WMI_SCAN_EVENT_STARTED=0x1,
    WMI_SCAN_EVENT_COMPLETED=0x2,
    WMI_SCAN_EVENT_BSS_CHANNEL=0x4,
    WMI_SCAN_EVENT_FOREIGN_CHANNEL = 0x8,
    WMI_SCAN_EVENT_DEQUEUED=0x10,       /* scan request got dequeued */
    WMI_SCAN_EVENT_PREEMPTED=0x20,		/* preempted by other high priority scan */
    WMI_SCAN_EVENT_START_FAILED=0x40,   /* scan start failed */
    WMI_SCAN_EVENT_RESTARTED=0x80,      /*scan restarted*/
    WMI_SCAN_EVENT_MAX=0x8000
};

enum wmi_scan_completion_reason {
    /** scan related events */
    WMI_SCAN_REASON_NONE                = 0xFF,
    WMI_SCAN_REASON_COMPLETED           = 0,
    WMI_SCAN_REASON_CANCELLED           = 1,
    WMI_SCAN_REASON_PREEMPTED           = 2,
    WMI_SCAN_REASON_TIMEDOUT            = 3,
    WMI_SCAN_REASON_INTERNAL_FAILURE    = 4, /* This reason indication failures when performaing scan */
    WMI_SCAN_REASON_MAX,
};

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_scan_event_fixed_param */
    /** scan event (wmi_scan_event_type) */
    A_UINT32 event;
    /** status of the scan completion event */
    A_UINT32 reason;
    /** channel freq , only valid for FOREIGN channel event*/
    A_UINT32 channel_freq;
    /**id of the requestor whose scan is in progress */
    A_UINT32 requestor;
    /**id of the scan that is in progress */
    A_UINT32 scan_id;
    /**id of VDEV that requested the scan */
    A_UINT32 vdev_id;
} wmi_scan_event_fixed_param;


/*
* If FW has multiple active channels due to MCC(multi channel concurrency),
* then these stats are combined stats for all the active channels.
*/
typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_update_whal_mib_stats_event_fixed_param */
    /** ack count, it is an incremental number, not accumulated number */
    A_UINT32 ackRcvBad;
    /** bad rts count, it is an incremental number, not accumulated number */
    A_UINT32 rtsBad;
    /** good rts, it is an incremental number, not accumulated number */
    A_UINT32 rtsGood;
    /** fcs count, it is an incremental number, not accumulated number */
    A_UINT32 fcsBad;
    /** beacon count, it is an incremental number, not accumulated number */
    A_UINT32 noBeacons;
} wmi_update_whal_mib_stats_event_fixed_param;

/*
 * This defines how much headroom is kept in the
 * receive frame between the descriptor and the
 * payload, in order for the WMI PHY error and
 * management handler to insert header contents.
 *
 * This is in bytes.
 */
#define WMI_MGMT_RX_HDR_HEADROOM    sizeof(wmi_comb_phyerr_rx_hdr) + WMI_TLV_HDR_SIZE + sizeof(wmi_single_phyerr_rx_hdr)

/** This event will be used for sending scan results
 * as well as rx mgmt frames to the host. The rx buffer
 * will be sent as part of this WMI event. It would be a
 * good idea to pass all the fields in the RX status
 * descriptor up to the host.
 */
 /* ATH_MAX_ANTENNA value (4) can't be changed without breaking the compatibility */
#define ATH_MAX_ANTENNA 4 /* To support beelinear, which is up to 4 chains */
typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_mgmt_rx_hdr */
    /** channel on which this frame is received. */
    A_UINT32 channel;
    /** snr information used to cal rssi */
    A_UINT32 snr;
    /** Rate kbps */
    A_UINT32 rate;
    /** rx phy mode WLAN_PHY_MODE */
    A_UINT32 phy_mode;
    /** length of the frame */
    A_UINT32 buf_len;
    /** rx status */
    A_UINT32 status;
    /** RSSI of PRI 20MHz for each chain. */
    A_UINT32 rssi_ctl[ATH_MAX_ANTENNA];
    /* This TLV is followed by array of bytes:
         * // management frame buffer
         *   A_UINT8 bufp[];
         */
} wmi_mgmt_rx_hdr;

/* WMI PHY Error RX */

typedef struct {
    /** TSF timestamp */
    A_UINT32 tsf_timestamp;

    /**
     * Current freq1, freq2
     *
     * [7:0]:    freq1[lo]
     * [15:8] :   freq1[hi]
     * [23:16]:   freq2[lo]
     * [31:24]:   freq2[hi]
     */
    A_UINT32 freq_info_1;

    /**
     * Combined RSSI over all chains and channel width for this PHY error
     *
     * [7:0]: RSSI combined
     * [15:8]: Channel width (MHz)
     * [23:16]: PHY error code
     * [24:16]: reserved (future use)
     */
    A_UINT32 freq_info_2;

    /**
     * RSSI on chain 0 through 3
     *
     * This is formatted the same as the PPDU_START RX descriptor
     * field:
     *
     * [7:0]:   pri20
     * [15:8]:  sec20
     * [23:16]: sec40
     * [31:24]: sec80
     */
    A_UINT32 rssi_chain0;
    A_UINT32 rssi_chain1;
    A_UINT32 rssi_chain2;
    A_UINT32 rssi_chain3;

   /**
     * Last calibrated NF value for chain 0 through 3
     *
     * nf_list_1:
     *
     * + [15:0] - chain 0
     * + [31:16] - chain 1
     *
     * nf_list_2:
     *
     * + [15:0] - chain 2
     * + [31:16] - chain 3
     */
    A_UINT32 nf_list_1;
    A_UINT32 nf_list_2;

    /** Length of the frame */
    A_UINT32 buf_len;
} wmi_single_phyerr_rx_hdr;

#define WMI_UNIFIED_FREQINFO_1_LO   0x000000ff
#define WMI_UNIFIED_FREQINFO_1_LO_S 0
#define WMI_UNIFIED_FREQINFO_1_HI   0x0000ff00
#define WMI_UNIFIED_FREQINFO_1_HI_S 8
#define WMI_UNIFIED_FREQINFO_2_LO   0x00ff0000
#define WMI_UNIFIED_FREQINFO_2_LO_S 16
#define WMI_UNIFIED_FREQINFO_2_HI   0xff000000
#define WMI_UNIFIED_FREQINFO_2_HI_S 24

/*
 * Please keep in mind that these _SET macros break macro side effect
 * assumptions; don't be clever with them.
 */
#define WMI_UNIFIED_FREQ_INFO_GET(hdr, f)                                   \
            ( WMI_F_MS( (hdr)->freq_info_1,                                 \
              WMI_UNIFIED_FREQINFO_##f##_LO )                               \
              | (WMI_F_MS( (hdr)->freq_info_1,                              \
                 WMI_UNIFIED_FREQINFO_##f##_HI ) << 8) )

#define WMI_UNIFIED_FREQ_INFO_SET(hdr, f, v)                                \
        do {                                                                \
            WMI_F_RMW((hdr)->freq_info_1, (v) & 0xff,                       \
              WMI_UNIFIED_FREQINFO_##f##_LO);                               \
            WMI_F_RMW((hdr)->freq_info_1, ((v) >> 8) & 0xff,                \
                WMI_UNIFIED_FREQINFO_##f##_HI);                             \
        } while (0)

#define WMI_UNIFIED_FREQINFO_2_RSSI_COMB    0x000000ff
#define WMI_UNIFIED_FREQINFO_2_RSSI_COMB_S  0
#define WMI_UNIFIED_FREQINFO_2_CHWIDTH      0x0000ff00
#define WMI_UNIFIED_FREQINFO_2_CHWIDTH_S    8
#define WMI_UNIFIED_FREQINFO_2_PHYERRCODE   0x00ff0000
#define WMI_UNIFIED_FREQINFO_2_PHYERRCODE_S 16

#define WMI_UNIFIED_RSSI_COMB_GET(hdr)                                      \
            ( (int8_t) (WMI_F_MS((hdr)->freq_info_2,                        \
                WMI_UNIFIED_FREQINFO_2_RSSI_COMB)))

#define WMI_UNIFIED_RSSI_COMB_SET(hdr, v)                                   \
            WMI_F_RMW((hdr)->freq_info_2, (v) & 0xff,                       \
              WMI_UNIFIED_FREQINFO_2_RSSI_COMB);

#define WMI_UNIFIED_CHWIDTH_GET(hdr)                                        \
            WMI_F_MS((hdr)->freq_info_2, WMI_UNIFIED_FREQINFO_2_CHWIDTH)

#define WMI_UNIFIED_CHWIDTH_SET(hdr, v)                                     \
            WMI_F_RMW((hdr)->freq_info_2, (v) & 0xff,                       \
              WMI_UNIFIED_FREQINFO_2_CHWIDTH);

#define WMI_UNIFIED_PHYERRCODE_GET(hdr)                                     \
            WMI_F_MS((hdr)->freq_info_2, WMI_UNIFIED_FREQINFO_2_PHYERRCODE)

#define WMI_UNIFIED_PHYERRCODE_SET(hdr, v)                                  \
            WMI_F_RMW((hdr)->freq_info_2, (v) & 0xff,                       \
              WMI_UNIFIED_FREQINFO_2_PHYERRCODE);

#define WMI_UNIFIED_CHAIN_0     0x0000ffff
#define WMI_UNIFIED_CHAIN_0_S   0
#define WMI_UNIFIED_CHAIN_1     0xffff0000
#define WMI_UNIFIED_CHAIN_1_S   16
#define WMI_UNIFIED_CHAIN_2     0x0000ffff
#define WMI_UNIFIED_CHAIN_2_S   0
#define WMI_UNIFIED_CHAIN_3     0xffff0000
#define WMI_UNIFIED_CHAIN_3_S   16

#define WMI_UNIFIED_CHAIN_0_FIELD   nf_list_1
#define WMI_UNIFIED_CHAIN_1_FIELD   nf_list_1
#define WMI_UNIFIED_CHAIN_2_FIELD   nf_list_2
#define WMI_UNIFIED_CHAIN_3_FIELD   nf_list_2

#define WMI_UNIFIED_NF_CHAIN_GET(hdr, c)                                    \
            ((int16_t) (WMI_F_MS((hdr)->WMI_UNIFIED_CHAIN_##c##_FIELD,      \
              WMI_UNIFIED_CHAIN_##c)))

#define WMI_UNIFIED_NF_CHAIN_SET(hdr, c, nf)                                \
            WMI_F_RMW((hdr)->WMI_UNIFIED_CHAIN_##c##_FIELD, (nf) & 0xffff,  \
              WMI_UNIFIED_CHAIN_##c);

/*
 * For now, this matches what the underlying hardware is doing.
 * Update ar6000ProcRxDesc() to use these macros when populating
 * the rx descriptor and then we can just copy the field over
 * to the WMI PHY notification without worrying about breaking
 * things.
 */
#define WMI_UNIFIED_RSSI_CHAN_PRI20     0x000000ff
#define WMI_UNIFIED_RSSI_CHAN_PRI20_S   0
#define WMI_UNIFIED_RSSI_CHAN_SEC20     0x0000ff00
#define WMI_UNIFIED_RSSI_CHAN_SEC20_S   8
#define WMI_UNIFIED_RSSI_CHAN_SEC40     0x00ff0000
#define WMI_UNIFIED_RSSI_CHAN_SEC40_S   16
#define WMI_UNIFIED_RSSI_CHAN_SEC80     0xff000000
#define WMI_UNIFIED_RSSI_CHAN_SEC80_S   24

#define WMI_UNIFIED_RSSI_CHAN_SET(hdr, c, ch, rssi)                         \
            WMI_F_RMW((hdr)->rssi_chain##c, (rssi) & 0xff,                  \
              WMI_UNIFIED_RSSI_CHAN_##ch);

#define WMI_UNIFIED_RSSI_CHAN_GET(hdr, c, ch)                               \
            ((int8_t) (WMI_F_MS((hdr)->rssi_chain##c,                       \
              WMI_UNIFIED_RSSI_CHAN_##ch)))

typedef struct {
    /** Phy error event header */
    wmi_single_phyerr_rx_hdr hdr;
    /** frame buffer */
    A_UINT8 bufp[1];
}wmi_single_phyerr_rx_event;

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_comb_phyerr_rx_hdr */
    /** Phy error phy error count */
    A_UINT32 num_phyerr_events;
    A_UINT32 tsf_l32;
    A_UINT32 tsf_u32;
    A_UINT32 buf_len;
    /* This TLV is followed by array of bytes:
         * // frame buffer - contains multiple payloads in the order:
         * // header - payload, header - payload...
         *  (The header is of type: wmi_single_phyerr_rx_hdr)
         *   A_UINT8 bufp[];
         */
} wmi_comb_phyerr_rx_hdr;

/* WMI MGMT TX  */
typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_mgmt_tx_hdr */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** xmit rate */
    A_UINT32 tx_rate;
    /** xmit power */
    A_UINT32 tx_power;
    /** Buffer length in bytes */
    A_UINT32 buf_len;
    /* This TLV is followed by array of bytes:
         * // management frame buffer
         *   A_UINT8 bufp[];
         */
} wmi_mgmt_tx_hdr;

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_echo_event_fixed_param */
    A_UINT32 value;
} wmi_echo_event_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_echo_cmd_fixed_param */
    A_UINT32 value;
}wmi_echo_cmd_fixed_param;


typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_set_regdomain_cmd_fixed_param */

    A_UINT32 reserved0;      /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
    /** reg domain code */
    A_UINT32 reg_domain;
    A_UINT32 reg_domain_2G;
    A_UINT32 reg_domain_5G;
    A_UINT32 conformance_test_limit_2G;
    A_UINT32 conformance_test_limit_5G;
} wmi_pdev_set_regdomain_cmd_fixed_param;

typedef struct {
    /** TRUE for scan start and flase for scan end */
    A_UINT32 scan_start;
} wmi_pdev_scan_cmd;

/*Command to set/unset chip in quiet mode*/
typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_set_quiet_cmd_fixed_param */
    A_UINT32 reserved0;      /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
	A_UINT32 period;		/*period in TUs*/
	A_UINT32 duration;		/*duration in TUs*/
	A_UINT32 next_start;	/*offset in TUs*/
        A_UINT32 enabled;		/*enable/disable*/
} wmi_pdev_set_quiet_cmd_fixed_param;

/*
 * Command to enable/disable Green AP Power Save.
 * This helps conserve power during AP operation. When the AP has no
 * stations associated with it, the host can enable Green AP Power Save
 * to request the firmware to shut down all but one transmit and receive
 * chains.
 */
typedef struct {
    A_UINT32 tlv_header;         /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_green_ap_ps_enable_cmd_fixed_param */
    A_UINT32 reserved0;          /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
    A_UINT32 enable;              /*1:enable, 0:disable*/
} wmi_pdev_green_ap_ps_enable_cmd_fixed_param;


#define MAX_HT_IE_LEN 32
typedef struct {
    A_UINT32 tlv_header;   /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_set_ht_ie_cmd_fixed_param */
    A_UINT32 reserved0;    /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
    A_UINT32 ie_len;       /*length of the ht ie in the TLV ie_data[] */
    /** The TLV for the HT IE follows:
     *       A_UINT32 ie_data[];
     */
} wmi_pdev_set_ht_ie_cmd_fixed_param;

#define MAX_VHT_IE_LEN 32
typedef struct {
    A_UINT32 tlv_header;   /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_set_vht_ie_cmd_fixed_param */
    A_UINT32 reserved0;    /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
    A_UINT32 ie_len;          /*length of the vht ie in the TLV ie_data[] */
    /** The TLV for the VHT IE follows:
     *       A_UINT32 ie_data[];
     */
} wmi_pdev_set_vht_ie_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;   /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_set_base_macaddr_cmd_fixed_param */
    A_UINT32 reserved0;    /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
    wmi_mac_addr base_macaddr;
} wmi_pdev_set_base_macaddr_cmd_fixed_param;

/*
 * For now, the spectral configuration is global rather than
 * per-vdev.  The vdev is a placeholder and will be ignored
 * by the firmware.
 */
typedef struct {
        A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_spectral_configure_cmd_fixed_param */
        A_UINT32    vdev_id;
        A_UINT32    spectral_scan_count;
        A_UINT32    spectral_scan_period;
        A_UINT32    spectral_scan_priority;
        A_UINT32    spectral_scan_fft_size;
        A_UINT32    spectral_scan_gc_ena;
        A_UINT32    spectral_scan_restart_ena;
        A_UINT32    spectral_scan_noise_floor_ref;
        A_UINT32    spectral_scan_init_delay;
        A_UINT32    spectral_scan_nb_tone_thr;
        A_UINT32    spectral_scan_str_bin_thr;
        A_UINT32    spectral_scan_wb_rpt_mode;
        A_UINT32    spectral_scan_rssi_rpt_mode;
        A_UINT32    spectral_scan_rssi_thr;
        A_UINT32    spectral_scan_pwr_format;
        A_UINT32    spectral_scan_rpt_mode;
        A_UINT32    spectral_scan_bin_scale;
        A_UINT32    spectral_scan_dBm_adj;
        A_UINT32    spectral_scan_chn_mask;
} wmi_vdev_spectral_configure_cmd_fixed_param;

/*
 * Enabling, disabling and triggering the spectral scan
 * is a per-vdev operation.  That is, it will set channel
 * flags per vdev rather than globally; so concurrent scan/run
 * and multiple STA (eg p2p, tdls, multi-band STA) is possible.
 */
typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_spectral_enable_cmd_fixed_param */
    A_UINT32    vdev_id;
    /* 0 - ignore; 1 - trigger, 2 - clear trigger */
    A_UINT32    trigger_cmd;
    /* 0 - ignore; 1 - enable, 2 - disable */
    A_UINT32    enable_cmd;
} wmi_vdev_spectral_enable_cmd_fixed_param;

typedef enum {
WMI_CSA_IE_PRESENT = 0x00000001,
WMI_XCSA_IE_PRESENT = 0x00000002,
WMI_WBW_IE_PRESENT = 0x00000004,
WMI_CSWARP_IE_PRESENT = 0x00000008,
}WMI_CSA_EVENT_IES_PRESENT_FLAG;

/* wmi CSA receive event from beacon frame */
typedef struct{
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_csa_event_fixed_param */
    A_UINT32 i_fc_dur;
//  Bit 0-15: FC
//  Bit 16-31: DUR
    wmi_mac_addr i_addr1;
    wmi_mac_addr i_addr2;
    /* NOTE: size of array of csa_ie[], xcsa_ie[], and wb_ie[] cannot be
         * changed in the future without breaking WMI compatibility */
    A_UINT32 csa_ie[2];
    A_UINT32 xcsa_ie[2];
    A_UINT32 wb_ie[2];
    A_UINT32 cswarp_ie;
    A_UINT32 ies_present_flag; //WMI_CSA_EVENT_IES_PRESENT_FLAG
}wmi_csa_event_fixed_param;

typedef enum {
    /** TX chian mask */
    WMI_PDEV_PARAM_TX_CHAIN_MASK = 0x1,
    /** RX chian mask */
    WMI_PDEV_PARAM_RX_CHAIN_MASK,
    /** TX power limit for 2G Radio */
    WMI_PDEV_PARAM_TXPOWER_LIMIT2G,
    /** TX power limit for 5G Radio */
    WMI_PDEV_PARAM_TXPOWER_LIMIT5G,
    /** TX power scale */
    WMI_PDEV_PARAM_TXPOWER_SCALE,
    /** Beacon generation mode . 0: host, 1: target   */
    WMI_PDEV_PARAM_BEACON_GEN_MODE,
    /** Beacon generation mode . 0: staggered 1: bursted   */
    WMI_PDEV_PARAM_BEACON_TX_MODE,
    /** Resource manager off chan mode .
     * 0: turn off off chan mode. 1: turn on offchan mode
     */
    WMI_PDEV_PARAM_RESMGR_OFFCHAN_MODE,
    /** Protection mode  0: no protection 1:use CTS-to-self 2: use RTS/CTS */
    WMI_PDEV_PARAM_PROTECTION_MODE,
    /** Dynamic bandwidth 0: disable 1: enable */
    WMI_PDEV_PARAM_DYNAMIC_BW,
    /** Non aggregrate/ 11g sw retry threshold.0-disable */
    WMI_PDEV_PARAM_NON_AGG_SW_RETRY_TH,
    /** aggregrate sw retry threshold. 0-disable*/
    WMI_PDEV_PARAM_AGG_SW_RETRY_TH,
    /** Station kickout threshold (non of consecutive failures).0-disable */
    WMI_PDEV_PARAM_STA_KICKOUT_TH,
    /** Aggerate size scaling configuration per AC */
    WMI_PDEV_PARAM_AC_AGGRSIZE_SCALING,
    /** LTR enable */
    WMI_PDEV_PARAM_LTR_ENABLE,
    /** LTR latency for BE, in us */
    WMI_PDEV_PARAM_LTR_AC_LATENCY_BE,
    /** LTR latency for BK, in us */
    WMI_PDEV_PARAM_LTR_AC_LATENCY_BK,
    /** LTR latency for VI, in us */
    WMI_PDEV_PARAM_LTR_AC_LATENCY_VI,
    /** LTR latency for VO, in us  */
    WMI_PDEV_PARAM_LTR_AC_LATENCY_VO,
    /** LTR AC latency timeout, in ms */
    WMI_PDEV_PARAM_LTR_AC_LATENCY_TIMEOUT,
    /** LTR platform latency override, in us */
    WMI_PDEV_PARAM_LTR_SLEEP_OVERRIDE,
    /** LTR-M override, in us */
    WMI_PDEV_PARAM_LTR_RX_OVERRIDE,
    /** Tx activity timeout for LTR, in us */
    WMI_PDEV_PARAM_LTR_TX_ACTIVITY_TIMEOUT,
    /** L1SS state machine enable */
    WMI_PDEV_PARAM_L1SS_ENABLE,
    /** Deep sleep state machine enable */
    WMI_PDEV_PARAM_DSLEEP_ENABLE,
    /** RX buffering flush enable */
    WMI_PDEV_PARAM_PCIELP_TXBUF_FLUSH,
    /** RX buffering matermark */
    WMI_PDEV_PARAM_PCIELP_TXBUF_WATERMARK,
    /** RX buffering timeout enable */
    WMI_PDEV_PARAM_PCIELP_TXBUF_TMO_EN,
    /** RX buffering timeout value */
    WMI_PDEV_PARAM_PCIELP_TXBUF_TMO_VALUE,
    /** pdev level stats update period in ms */
    WMI_PDEV_PARAM_PDEV_STATS_UPDATE_PERIOD,
    /** vdev level stats update period in ms */
    WMI_PDEV_PARAM_VDEV_STATS_UPDATE_PERIOD,
    /** peer level stats update period in ms */
    WMI_PDEV_PARAM_PEER_STATS_UPDATE_PERIOD,
    /** beacon filter status update period */
    WMI_PDEV_PARAM_BCNFLT_STATS_UPDATE_PERIOD,
    /** QOS Mgmt frame protection MFP/PMF 0: disable, 1: enable */
    WMI_PDEV_PARAM_PMF_QOS,
    /** Access category on which ARP frames are sent */
    WMI_PDEV_PARAM_ARP_AC_OVERRIDE,
    /** DCS configuration */
    WMI_PDEV_PARAM_DCS,
    /** Enable/Disable ANI on target */
    WMI_PDEV_PARAM_ANI_ENABLE,
    /** configure the ANI polling period */
    WMI_PDEV_PARAM_ANI_POLL_PERIOD,
    /** configure the ANI listening period */
    WMI_PDEV_PARAM_ANI_LISTEN_PERIOD,
    /** configure OFDM immunity level */
    WMI_PDEV_PARAM_ANI_OFDM_LEVEL,
    /** configure CCK immunity level */
    WMI_PDEV_PARAM_ANI_CCK_LEVEL,
    /** Enable/Disable CDD for 1x1 STAs in rate control module */
    WMI_PDEV_PARAM_DYNTXCHAIN,
    /** Enable/Disable proxy STA */
    WMI_PDEV_PARAM_PROXY_STA,
    /** Enable/Disable low power state when all VDEVs are inactive/idle. */
    WMI_PDEV_PARAM_IDLE_PS_CONFIG,
    /** Enable/Disable power gating sleep */
    WMI_PDEV_PARAM_POWER_GATING_SLEEP,
    /** Enable/Disable Rfkill */
    WMI_PDEV_PARAM_RFKILL_ENABLE,
    /** Set Bursting DUR */
    WMI_PDEV_PARAM_BURST_DUR,
    /** Set Bursting ENABLE */
    WMI_PDEV_PARAM_BURST_ENABLE,
    /** HW rfkill config */
    WMI_PDEV_PARAM_HW_RFKILL_CONFIG,
    /** Enable radio low power features */
    WMI_PDEV_PARAM_LOW_POWER_RF_ENABLE,
    /** L1SS entry and residency time track */
    WMI_PDEV_PARAM_L1SS_TRACK,
    /** set hyst at runtime, requirement from SS */
    WMI_PDEV_PARAM_HYST_EN,
    /** Enable/ Disable POWER COLLAPSE */
    WMI_PDEV_PARAM_POWER_COLLAPSE_ENABLE,
    /** configure LED system state */
    WMI_PDEV_PARAM_LED_SYS_STATE,
   /** Enable/Disable LED */
    WMI_PDEV_PARAM_LED_ENABLE,
} WMI_PDEV_PARAM;

typedef enum {
    /** Set the loglevel */
    WMI_DBGLOG_LOG_LEVEL = 0x1,
    /** Enable VAP level debug */
    WMI_DBGLOG_VAP_ENABLE,
    /** Disable VAP level debug */
    WMI_DBGLOG_VAP_DISABLE,
    /** Enable MODULE level debug */
    WMI_DBGLOG_MODULE_ENABLE,
    /** Disable MODULE level debug */
    WMI_DBGLOG_MODULE_DISABLE,
    /** Enable MODULE level debug */
    WMI_DBGLOG_MOD_LOG_LEVEL,
    /** set type of the debug output */
    WMI_DBGLOG_TYPE,
    /** Enable Disable debug */
    WMI_DBGLOG_REPORT_ENABLE
} WMI_DBG_PARAM;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_set_param_cmd_fixed_param */
    A_UINT32 reserved0;      /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
    /** parameter id   */
    A_UINT32 param_id;
    /** parametr value */
    A_UINT32 param_value;
} wmi_pdev_set_param_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_get_tpc_config_cmd_fixed_param */
    A_UINT32 reserved0;      /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
    /** parameter   */
    A_UINT32 param;
} wmi_pdev_get_tpc_config_cmd_fixed_param;


typedef struct {
    /** parameter   */
    A_UINT32 param;
} wmi_pdev_dump_cmd;

typedef enum {
    PAUSE_TYPE_CHOP =           0x1, /** for MCC (switch channel), only vdev_map is valid */
    PAUSE_TYPE_PS =             0x2, /** for peer station sleep in sap mode, only peer_id is valid */
    PAUSE_TYPE_UAPSD =          0x3, /** for uapsd, only peer_id and tid_map are valid. */
    PAUSE_TYPE_P2P_CLIENT_NOA = 0x4, /** only vdev_map is valid, actually only one vdev id is set at one time */
    PAUSE_TYPE_P2P_GO_PS =      0x5, /** only vdev_map is valid, actually only one vdev id is set at one time */
    PAUSE_TYPE_STA_ADD_BA =     0x6, /** only peer_id and tid_map are valid, actually only one tid is set at one time */
    PAUSE_TYPE_AP_PS =          0x7, /** for pausing AP vdev when all the connected clients are in PS. only vdev_map is valid */
    PAUSE_TYPE_IBSS_PS =        0x8, /** for pausing IBSS vdev when all the peers are in PS. only vdev_map is valid */
    PAUSE_TYPE_HOST =           0x15,/** host is requesting vdev pause */
} wmi_tx_pause_type;

typedef enum {
    ACTION_PAUSE =     0x0,
    ACTION_UNPAUSE =   0x1,
} wmi_tx_pause_action;

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 pause_type;
    A_UINT32 action;
    A_UINT32 vdev_map;
    A_UINT32 peer_id;
    A_UINT32 tid_map;
} wmi_tx_pause_event_fixed_param;

#define WMI_TPC_RATE_MAX            160
/* WMI_TPC_TX_NUM_CHAIN macro can't be changed without breaking the WMI compatibility */
#define WMI_TPC_TX_NUM_CHAIN        4

typedef enum {
    WMI_TPC_CONFIG_EVENT_FLAG_TABLE_CDD     = 0x1,
    WMI_TPC_CONFIG_EVENT_FLAG_TABLE_STBC    = 0x2,
    WMI_TPC_CONFIG_EVENT_FLAG_TABLE_TXBF    = 0x4,
} WMI_TPC_CONFIG_EVENT_FLAG;

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_tpc_config_event_fixed_param  */
    A_UINT32 regDomain;
    A_UINT32 chanFreq;
    A_UINT32 phyMode;
    A_UINT32 twiceAntennaReduction;
    A_UINT32 twiceMaxRDPower;
    A_INT32  twiceAntennaGain;
    A_UINT32 powerLimit;
    A_UINT32 rateMax;
    A_UINT32 numTxChain;
    A_UINT32 ctl;
    A_UINT32 flags;
    /* WMI_TPC_TX_NUM_CHAIN macro can't be changed without breaking the WMI compatibility */
    A_INT8  maxRegAllowedPower[WMI_TPC_TX_NUM_CHAIN];
    A_INT8  maxRegAllowedPowerAGCDD[WMI_TPC_TX_NUM_CHAIN][WMI_TPC_TX_NUM_CHAIN];
    A_INT8  maxRegAllowedPowerAGSTBC[WMI_TPC_TX_NUM_CHAIN][WMI_TPC_TX_NUM_CHAIN];
    A_INT8  maxRegAllowedPowerAGTXBF[WMI_TPC_TX_NUM_CHAIN][WMI_TPC_TX_NUM_CHAIN];
    /* This TLV is followed by a byte array:
         *      A_UINT8 ratesArray[];
         */
} wmi_pdev_tpc_config_event_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_l1ss_track_event_fixed_param  */
    A_UINT32 periodCnt;
    A_UINT32 L1Cnt;
    A_UINT32 L11Cnt;
    A_UINT32 L12Cnt;
    A_UINT32 L1Entry;
    A_UINT32 L11Entry;
    A_UINT32 L12Entry;
} wmi_pdev_l1ss_track_event_fixed_param;

typedef struct {
    A_UINT32 len;
    A_UINT32 msgref;
    A_UINT32 segmentInfo;
} wmi_pdev_seg_hdr_info;


/*
 * Transmit power scale factor.
 *
 */
typedef enum {
    WMI_TP_SCALE_MAX    = 0,        /* no scaling (default) */
    WMI_TP_SCALE_50     = 1,        /* 50% of max (-3 dBm) */
    WMI_TP_SCALE_25     = 2,        /* 25% of max (-6 dBm) */
    WMI_TP_SCALE_12     = 3,        /* 12% of max (-9 dBm) */
    WMI_TP_SCALE_MIN    = 4,        /* min, but still on   */
    WMI_TP_SCALE_SIZE   = 5,        /* max num of enum     */
} WMI_TP_SCALE;

#define WMI_MAX_DEBUG_MESG (sizeof(A_UINT32) * 32)

typedef struct {
   /** message buffer, NULL terminated */
   char bufp[WMI_MAX_DEBUG_MESG];
} wmi_debug_mesg_event;

enum {
    /** IBSS station */
    VDEV_TYPE_IBSS  = 0,
    /** infra STA */
    VDEV_TYPE_STA   = 1,
    /** infra AP */
    VDEV_TYPE_AP    = 2,
    /** Monitor */
    VDEV_TYPE_MONITOR =3,
};

enum {
    /** P2P device */
    VDEV_SUBTYPE_P2PDEV=0,
    /** P2P client */
    VDEV_SUBTYPE_P2PCLI,
    /** P2P GO */
    VDEV_SUBTYPE_P2PGO,
    /** BT3.0 HS */
    VDEV_SUBTYPE_BT,
};

typedef struct {
    /** idnore power , only use flags , mode and freq */
   wmi_channel  chan;
}    wmi_pdev_set_channel_cmd;

typedef enum {
    WMI_PKTLOG_EVENT_RX  = 0x1,
    WMI_PKTLOG_EVENT_TX  = 0x2,
    WMI_PKTLOG_EVENT_RCF = 0x4, /* Rate Control Find */
    WMI_PKTLOG_EVENT_RCU = 0x8, /* Rate Control Update */
} WMI_PKTLOG_EVENT;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_pktlog_enable_cmd_fixed_param */
    A_UINT32 reserved0;      /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
    WMI_PKTLOG_EVENT evlist;
} wmi_pdev_pktlog_enable_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_pktlog_disable_cmd_fixed_param */
    A_UINT32 reserved0;
} wmi_pdev_pktlog_disable_cmd_fixed_param;

/** Customize the DSCP (bit) to TID (0-7) mapping for QOS.
 *  NOTE: This constant cannot be changed without breaking
 *  WMI Compatibility. */

#define WMI_DSCP_MAP_MAX    (64)
    /*
     * @brief dscp_tid_map_cmdid - command to send the dscp to tid map to the target
     * @details
     * Create an API for sending the custom DSCP-to-TID map to the target
     * If this is a request from the user space or from above the UMAC
     * then the best place to implement this is in the umac_if_offload of the OL path.
     * Provide a place holder for this API in the ieee80211com (ic).
     *
     * This API will be a function pointer in the ieee80211com (ic). Any user space calls for manually setting the DSCP-to-TID mapping
     * in the target should be directed to the function pointer in the ic.
     *
     * Implementation details of the API to send the map to the target are as described-
     *
     * 1. The function will have 2 arguments- struct ieee80211com, DSCP-to-TID map.
     *    DSCP-to-TID map is a one dimensional u_int32_t array of length 64 to accomodate
     *    64 TID values for 2^6 (64) DSCP ids.
     *    Example:
     *      A_UINT32 dscp_tid_map[WMI_DSCP_MAP_MAX] = {
	 *									0, 0, 0, 0, 0, 0, 0, 0,
	 *									1, 1, 1, 1, 1, 1, 1, 1,
	 *									2, 2, 2, 2, 2, 2, 2, 2,
	 *									3, 3, 3, 3, 3, 3, 3, 3,
	 *									4, 4, 4, 4, 4, 4, 4, 4,
	 *									5, 5, 5, 5, 5, 5, 5, 5,
	 *									6, 6, 6, 6, 6, 6, 6, 6,
	 *									7, 7, 7, 7, 7, 7, 7, 7,
	 *								  };
     *
     * 2. Request for the WMI buffer of size equal to the size of the DSCP-to-TID map.
     *
     * 3. Copy the DSCP-to-TID map into the WMI buffer.
     *
     * 4. Invoke the wmi_unified_cmd_send to send the cmd buffer to the target with the
     *    WMI_PDEV_SET_DSCP_TID_MAP_CMDID. Arguments to the wmi send cmd API
     *    (wmi_unified_send_cmd) are wmi handle, cmd buffer, length of the cmd buffer and
     *    the WMI_PDEV_SET_DSCP_TID_MAP_CMDID id.
     *
     */
typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_set_dscp_tid_map_cmd_fixed_param */
    A_UINT32 reserved0;      /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
 /* map indicating DSCP to TID conversion */
    A_UINT32 dscp_to_tid_map[WMI_DSCP_MAP_MAX];
} wmi_pdev_set_dscp_tid_map_cmd_fixed_param;

/** Fixed rate (rate-code) for broadcast/ multicast data frames */
/* @brief bcast_mcast_data_rate - set the rates for the bcast/ mcast frames
 * @details
 * Create an API for setting the custom rate for the MCAST and BCAST frames
 * in the target. If this is a request from the user space or from above the UMAC
 * then the best place to implement this is in the umac_if_offload of the OL path.
 * Provide a place holder for this API in the ieee80211com (ic).
 *
 * Implementation details of the API to set custom rates for MCAST and BCAST in
 * the target are as described-
 *
 * 1. The function will have 3 arguments-
 *    vap structure,
 *    MCAST/ BCAST identifier code,
 *    8 bit rate code
 *
 * The rate-code is a 1-byte field in which:for given rate, nss and preamble
 * b'7-b-6 indicate the preamble (0 OFDM, 1 CCK, 2, HT, 3 VHT)
 * b'5-b'4 indicate the NSS (0 - 1x1, 1 - 2x2, 2 - 3x3)
 * b'3-b'0 indicate the rate, which is indicated as follows:
 *          OFDM :     0: OFDM 48 Mbps
 *                     1: OFDM 24 Mbps
 *                     2: OFDM 12 Mbps
 *                     3: OFDM 6 Mbps
 *                     4: OFDM 54 Mbps
 *                     5: OFDM 36 Mbps
 *                     6: OFDM 18 Mbps
 *                     7: OFDM 9 Mbps
 *         CCK (pream == 1)
 *                     0: CCK 11 Mbps Long
 *                     1: CCK 5.5 Mbps Long
 *                     2: CCK 2 Mbps Long
 *                     3: CCK 1 Mbps Long
 *                     4: CCK 11 Mbps Short
 *                     5: CCK 5.5 Mbps Short
 *                     6: CCK 2 Mbps Short
 *         HT/VHT (pream == 2/3)
 *                     0..7: MCS0..MCS7 (HT)
 *                     0..9: MCS0..MCS9 (VHT)
 *
 * 2. Invoke the wmi_unified_vdev_set_param_send to send the rate value
 *    to the target.
 *    Arguments to the API are-
 *    wmi handle,
 *    VAP interface id (av_if_id) defined in ol_ath_vap_net80211,
 *    WMI_VDEV_PARAM_BCAST_DATA_RATE/ WMI_VDEV_PARAM_MCAST_DATA_RATE,
 *    rate value.
 */
typedef enum {
    WMI_SET_MCAST_RATE,
    WMI_SET_BCAST_RATE
} MCAST_BCAST_RATE_ID;

typedef struct {
    MCAST_BCAST_RATE_ID rate_id;
    A_UINT32 rate;
} mcast_bcast_rate;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_wmm_params */
    A_UINT32 cwmin;
    A_UINT32 cwmax;
    A_UINT32 aifs;
    A_UINT32 txoplimit;
    A_UINT32 acm;
    A_UINT32 no_ack;
} wmi_wmm_params;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_set_wmm_params_cmd_fixed_param */
    A_UINT32 reserved0;      /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
    A_UINT32 dg_type;

    /* The TLVs for the 4 AC follows:
     *     wmi_wmm_params wmm_params_ac_be;
     *     wmi_wmm_params wmm_params_ac_bk;
     *     wmi_wmm_params wmm_params_ac_vi;
     *     wmi_wmm_params wmm_params_ac_vo;
     */
} wmi_pdev_set_wmm_params_cmd_fixed_param;

typedef enum {
    WMI_REQUEST_PEER_STAT = 0x01,
    WMI_REQUEST_AP_STAT = 0x02,
    WMI_REQUEST_PDEV_STAT = 0x04,
    WMI_REQUEST_VDEV_STAT = 0x08,
    WMI_REQUEST_BCNFLT_STAT = 0x10,
} wmi_stats_id;

typedef struct {
    A_UINT32   tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_request_stats_cmd_fixed_param */
    wmi_stats_id stats_id;
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
} wmi_request_stats_cmd_fixed_param;

/* stats type bitmap  */
#define WMI_LINK_STATS_RADIO         0x00000001
#define WMI_LINK_STATS_IFACE         0x00000002
#define WMI_LINK_STATS_ALL_PEER      0x00000004
#define WMI_LINK_STATS_PER_PEER      0x00000008


/* wifi clear statistics bitmap  */
#define WIFI_STATS_RADIO              0x00000001 /** all radio statistics */
#define WIFI_STATS_RADIO_CCA          0x00000002 /** cca_busy_time (within radio statistics) */
#define WIFI_STATS_RADIO_CHANNELS     0x00000004 /** all channel statistics (within radio statistics) */
#define WIFI_STATS_RADIO_SCAN         0x00000008 /** all scan statistics (within radio statistics) */
#define WIFI_STATS_IFACE              0x00000010 /** all interface statistics */
#define WIFI_STATS_IFACE_TXRATE       0x00000020 /** all tx rate statistics (within interface statistics) */
#define WIFI_STATS_IFACE_AC           0x00000040 /** all ac statistics (within interface statistics) */
#define WIFI_STATS_IFACE_CONTENTION   0x00000080 /** all contention (min, max, avg) statistics (within ac statisctics) */
#define WMI_STATS_IFACE_ALL_PEER      0x00000100 /** All peer stats on this interface */
#define WMI_STATS_IFACE_PER_PEER      0x00000200 /** Clear particular peer stats depending on the peer_mac */

/** Default value for stats if the stats collection has not started */
#define WMI_STATS_VALUE_INVALID       0xffffffff

typedef enum {
    WMI_ROAMING_IDLE = 0,
    WMI_ROAMING_ACTIVE = 1,
} wmi_roam_state;

/* access categories */
typedef enum {
   WMI_AC_VO  = 0,
   WMI_AC_VI  = 1,
   WMI_AC_BE  = 2,
   WMI_AC_BK  = 3,
   WMI_AC_MAX = 4,
} wmi_traffic_ac;

typedef enum {
    WMI_STA_STATS = 0,
    WMI_SOFTAP_STATS = 1,
    WMI_IBSS_STATS = 2,
    WMI_P2P_CLIENT_STATS = 3,
    WMI_P2P_GO_STATS = 4,
    WMI_NAN_STATS = 5,
    WMI_MESH_STATS = 6,
} wmi_link_iface_type;

/* channel operating width */
typedef enum {
    WMI_CHAN_WIDTH_20    = 0,
    WMI_CHAN_WIDTH_40    = 1,
    WMI_CHAN_WIDTH_80    = 2,
    WMI_CHAN_WIDTH_160   = 3,
    WMI_CHAN_WIDTH_80P80 = 4,
    WMI_CHAN_WIDTH_5     = 5,
    WMI_CHAN_WIDTH_10    = 6,
} wmi_channel_width;

/* wifi peer type */
typedef enum {
   WMI_PEER_STA,
   WMI_PEER_AP,
   WMI_PEER_P2P_GO,
   WMI_PEER_P2P_CLIENT,
   WMI_PEER_NAN,
   WMI_PEER_TDLS,
   WMI_PEER_INVALID,
} wmi_peer_type;

/*Clear stats*/
typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_clear_link_stats_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** stop_stats_collection_req = 1 will imply stop the statistics collection */
    A_UINT32 stop_stats_collection_req;
    /** identifies what stats to be cleared */
    A_UINT32 stats_clear_req_mask;
    /** identifies which peer stats to be cleared. Valid only while clearing PER_REER */
    wmi_mac_addr peer_macaddr;
} wmi_clear_link_stats_cmd_fixed_param;

/* Link Stats configuration params. Trigger the link layer statistics collection*/
typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_start_link_stats_cmd_fixed_param */
    /** threshold to classify the pkts as short or long */
    A_UINT32 mpdu_size_threshold;
    /** set for field debug mode. Driver should collect all statistics regardless of performance impact.*/
    A_UINT32 aggressive_statistics_gathering;
} wmi_start_link_stats_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_request_link_stats_cmd_fixed_param */
    /** Type of stats required. This is a bitmask WMI_LINK_STATS_RADIO, WMI_LINK_STATS_IFACE */
    A_UINT32 stats_type;
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** unique id identifying the request, generated by the caller */
    A_UINT32 request_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
} wmi_request_link_stats_cmd_fixed_param;

/* channel statistics */
typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_channel_stats */
    /** Channel width (20, 40, 80, 80+80, 160) enum wmi_channel_width*/
    A_UINT32 channel_width;
    /** Primary 20 MHz channel */
    A_UINT32 center_freq;
    /** center frequency (MHz) first segment */
    A_UINT32 center_freq0;
    /** center frequency (MHz) second segment */
    A_UINT32 center_freq1;
    /** msecs the radio is awake (32 bits number accruing over time) */
    A_UINT32 radio_awake_time;
    /** msecs the CCA register is busy (32 bits number accruing over time) */
    A_UINT32 cca_busy_time;
} wmi_channel_stats;

/* radio statistics */
typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_radio_link_stats */
    /** Wifi radio (if multiple radio supported) */
    A_UINT32 radio_id;
    /** msecs the radio is awake (32 bits number accruing over time) */
    A_UINT32 on_time;
    /** msecs the radio is transmitting (32 bits number accruing over time) */
    A_UINT32 tx_time;
    /** msecs the radio is in active receive (32 bits number accruing over time) */
    A_UINT32 rx_time;
    /** msecs the radio is awake due to all scan (32 bits number accruing over time) */
    A_UINT32 on_time_scan;
    /** msecs the radio is awake due to NAN (32 bits number accruing over time) */
    A_UINT32 on_time_nbd;
    /** msecs the radio is awake due to G?scan (32 bits number accruing over time) */
    A_UINT32 on_time_gscan;
    /** msecs the radio is awake due to roam?scan (32 bits number accruing over time) */
    A_UINT32 on_time_roam_scan;
    /** msecs the radio is awake due to PNO scan (32 bits number accruing over time) */
    A_UINT32 on_time_pno_scan;
    /** msecs the radio is awake due to HS2.0 scans and GAS exchange (32 bits number accruing over time) */
    A_UINT32 on_time_hs20;
    /** number of channels */
    A_UINT32 num_channels;
} wmi_radio_link_stats;

/** Radio statistics (once started) do not stop or get reset unless wifi_clear_link_stats is invoked */
typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_stats_event_fixed_param */
    /** unique id identifying the request, given in the request stats command */
    A_UINT32 request_id;
    /** Number of radios*/
    A_UINT32 num_radio;
    /** more_data will be set depending on the number of radios */
    A_UINT32 more_radio_events;
/*
 * This TLV is followed by another TLV of array of bytes
 *   size of(struct wmi_radio_link_stats);
 *
 * This TLV is followed by another TLV of array of bytes
 *   num_channels * size of(struct wmi_channel_stats)
 */

} wmi_radio_link_stats_event_fixed_param;

/* per rate statistics */
typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_rate_stats */
    /** rate information
     * The rate-code is a 1-byte field in which:for given rate, nss and preamble
     * b'7-b-6 indicate the preamble (0 OFDM, 1 CCK, 2, HT, 3 VHT)
     * b'5-b'4 indicate the NSS (0 - 1x1, 1 - 2x2, 2 - 3x3)
     * b'3-b'0 indicate the rate, which is indicated as follows:
     *          OFDM :     0: OFDM 48 Mbps
     *                     1: OFDM 24 Mbps
     *                     2: OFDM 12 Mbps
     *                     3: OFDM 6 Mbps
     *                     4: OFDM 54 Mbps
     *                     5: OFDM 36 Mbps
     *                     6: OFDM 18 Mbps
     *                     7: OFDM 9 Mbps
     *         CCK (pream == 1)
     *                     0: CCK 11 Mbps Long
     *                     1: CCK 5.5 Mbps Long
     *                     2: CCK 2 Mbps Long
     *                     3: CCK 1 Mbps Long
     *                     4: CCK 11 Mbps Short
     *                     5: CCK 5.5 Mbps Short
     *                     6: CCK 2 Mbps Short
     *         HT/VHT (pream == 2/3)
     *                     0..7: MCS0..MCS7 (HT)
     *                     0..9: MCS0..MCS9 (VHT)
     */
    A_UINT32 rate;
    /** units of 100 Kbps */
    A_UINT32 bitrate;
    /** number of successfully transmitted data pkts (ACK rcvd) */
    A_UINT32 tx_mpdu;
    /** number of received data pkts */
    A_UINT32 rx_mpdu;
    /** number of data packet losses (no ACK) */
    A_UINT32 mpdu_lost;
    /** total number of data pkt retries */
    A_UINT32 retries;
    /** number of short data pkt retries */
    A_UINT32 retries_short;
    /** number of long data pkt retries */
    A_UINT32 retries_long;
} wmi_rate_stats;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_link_stats */
    /** peer type (AP, TDLS, GO etc.) enum wmi_peer_type*/
    A_UINT32 peer_type;
    /** mac address */
    wmi_mac_addr peer_mac_address;
    /** peer wmi_CAPABILITY_XXX */
    A_UINT32 capabilities;
    /** number of rates */
    A_UINT32 num_rates;
} wmi_peer_link_stats;

/** PEER statistics (once started) reset and start afresh after each connection */
typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_stats_event_fixed_param */
    /** unique id identifying the request, given in the request stats command */
    A_UINT32 request_id;
    /** number of peers accomidated in this particular event  */
    A_UINT32 num_peers;
    /** Indicates the fragment number  */
    A_UINT32 peer_event_number;
    /** Indicates if there are more peers which will be sent as seperate peer_stats event */
    A_UINT32 more_data;

/**
 * This TLV is followed by another TLV
 * num_peers * size of(struct wmi_peer_stats)
 * num_rates * size of(struct wmi_rate_stats). num_rates is the sum of the rates of all the peers.
 */
} wmi_peer_stats_event_fixed_param;

/* per access category statistics */
typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_wmm_ac_stats */
    /** access category (VI, VO, BE, BK) enum wmi_traffic_ac*/
    A_UINT32 ac_type;
    /** number of successfully transmitted unicast data pkts (ACK rcvd) */
    A_UINT32 tx_mpdu;
    /** number of received unicast mpdus */
    A_UINT32 rx_mpdu;
    /** number of succesfully transmitted multicast data packets */
    /** STA case: implies ACK received from AP for the unicast packet in which mcast pkt was sent */
    A_UINT32 tx_mcast;
    /** number of received multicast data packets */
    A_UINT32 rx_mcast;
    /** number of received unicast a-mpdus */
    A_UINT32 rx_ampdu;
    /** number of transmitted unicast a-mpdus */
    A_UINT32 tx_ampdu;
    /** number of data pkt losses (no ACK) */
    A_UINT32 mpdu_lost;
    /** total number of data pkt retries */
    A_UINT32 retries;
    /** number of short data pkt retries */
    A_UINT32 retries_short;
    /** number of long data pkt retries */
    A_UINT32 retries_long;
    /** data pkt min contention time (usecs) */
    A_UINT32 contention_time_min;
    /** data pkt max contention time (usecs) */
    A_UINT32 contention_time_max;
    /** data pkt avg contention time (usecs) */
    A_UINT32 contention_time_avg;
    /** num of data pkts used for contention statistics */
    A_UINT32 contention_num_samples;
} wmi_wmm_ac_stats;

/* interface statistics */
typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_iface_link_stats */
    /** access point beacon received count from connected AP */
    A_UINT32 beacon_rx;
    /** access point mgmt frames received count from connected AP (including Beacon) */
    A_UINT32 mgmt_rx;
    /** action frames received count */
    A_UINT32 mgmt_action_rx;
    /** action frames transmit count */
    A_UINT32 mgmt_action_tx;
    /** access Point Beacon and Management frames RSSI (averaged) */
    A_UINT32 rssi_mgmt;
    /** access Point Data Frames RSSI (averaged) from connected AP */
    A_UINT32 rssi_data;
    /** access Point ACK RSSI (averaged) from connected AP */
    A_UINT32 rssi_ack;
    /** number of peers */
    A_UINT32 num_peers;
    /** Indicates how many peer_stats events will be sent depending on the num_peers. */
    A_UINT32 num_peer_events;
    /** number of ac */
    A_UINT32 num_ac;
    /** Roaming Stat */
    A_UINT32 roam_state;
} wmi_iface_link_stats;

/** Interface statistics (once started) reset and start afresh after each connection */
typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_iface_link_stats_event_fixed_param */
    /** unique id identifying the request, given in the request stats command */
    A_UINT32 request_id;
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
/*
 * This TLV is followed by another TLV
 *   wmi_iface_link_stats iface_link_stats;
 *   num_ac * size of(struct wmi_wmm_ac_stats)
 */
} wmi_iface_link_stats_event_fixed_param;

/** Suspend option */
enum {
    WMI_PDEV_SUSPEND, /* suspend */
    WMI_PDEV_SUSPEND_AND_DISABLE_INTR, /* suspend and disable all interrupts */
};

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_suspend_cmd_fixed_param  */
    /* suspend option sent to target */
    A_UINT32    reserved0;                           /** placeholder for pdev_id of future multiple MAC products. Init. to 0. */
    A_UINT32 suspend_opt;
} wmi_pdev_suspend_cmd_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_resume_cmd_fixed_param  */
    /** Reserved for future use */
    A_UINT32    reserved0;
} wmi_pdev_resume_cmd_fixed_param;

typedef struct {
    A_UINT32   tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_stats_event_fixed_param */
    wmi_stats_id stats_id;
    /** number of pdev stats event structures (wmi_pdev_stats) 0 or 1 */
    A_UINT32 num_pdev_stats;
    /** number of vdev stats event structures  (wmi_vdev_stats) 0 or max vdevs */
    A_UINT32 num_vdev_stats;
    /** number of peer stats event structures  (wmi_peer_stats) 0 or max peers */
    A_UINT32 num_peer_stats;
    A_UINT32 num_bcnflt_stats;
    /** number of chan stats event structures  (wmi_chan_stats) 0 to MAX MCC CHANS */
    A_UINT32 num_chan_stats;
    /* This TLV is followed by another TLV of array of bytes
         *   A_UINT8 data[];
         *  This data array contains
         *   num_pdev_stats * size of(struct wmi_pdev_stats)
         *   num_vdev_stats * size of(struct wmi_vdev_stats)
         *   num_peer_stats * size of(struct wmi_peer_stats)
         *   num_bcnflt_stats * size_of()
 *   num_chan_stats * size of(struct wmi_chan_stats)
         *
         */
} wmi_stats_event_fixed_param;

/**
 *  PDEV statistics
 *  @todo
 *  add all PDEV stats here
 */
typedef struct {
    /** Channel noise floor */
    A_INT32 chan_nf;
    /** TX frame count */
    A_UINT32 tx_frame_count;
    /** RX frame count */
    A_UINT32 rx_frame_count;
    /** rx clear count */
    A_UINT32 rx_clear_count;
    /** cycle count */
    A_UINT32 cycle_count;
    /** Phy error count */
    A_UINT32 phy_err_count;
    /** Channel Tx Power */
    A_UINT32 chan_tx_pwr;
    /** WAL dbg stats */
    struct wlan_dbg_stats pdev_stats;

} wmi_pdev_stats;

/**
 *  VDEV statistics
 *  @todo
 *  add all VDEV stats here
 */

typedef struct {
    A_INT32 bcn_snr;
    A_INT32 dat_snr;
} wmi_snr_info;

typedef struct {
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32         vdev_id;
    wmi_snr_info     vdev_snr;
    A_UINT32         tx_frm_cnt[WLAN_MAX_AC];/* Total number of packets(per AC) that were successfully transmitted(with and without retries, including multi-cast, broadcast) */
    A_UINT32         rx_frm_cnt;/* Total number of packets that were successfully received (after appropriate filter rules including multi-cast, broadcast)*/
    A_UINT32         multiple_retry_cnt[WLAN_MAX_AC];/*The number of MSDU packets and MMPDU frames per AC
                                                       that the 802.11 station successfully transmitted after more than one retransmission attempt*/
    A_UINT32         fail_cnt[WLAN_MAX_AC]; /*Total number packets(per AC) failed to transmit */
    A_UINT32         rts_fail_cnt;/*Total number of RTS/CTS sequence failures for transmission of a packet*/
    A_UINT32         rts_succ_cnt;/*Total number of RTS/CTS sequence success for transmission of a packet*/
    A_UINT32         rx_err_cnt;/*The receive error count. HAL will provide the RxP FCS error global */
    A_UINT32         rx_discard_cnt;/* The sum of the receive error count and dropped-receive-buffer error count. (FCS error)*/
    A_UINT32         ack_fail_cnt;/*Total number packets failed transmit because of no ACK from the remote entity*/
   A_UINT32          tx_rate_history[MAX_TX_RATE_VALUES];/*History of last ten transmit rate, in units of 500 kbit/sec*/
    A_UINT32         bcn_rssi_history[MAX_RSSI_VALUES];/*History of last ten Beacon rssi of the connected Bss*/
} wmi_vdev_stats;

/**
 *  peer statistics.
 *
 * @todo
 * add more stats
 *
 */
typedef struct {
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** rssi */
    A_UINT32  peer_rssi;
    /** last tx data rate used for peer */
    A_UINT32  peer_tx_rate;
    /** last rx data rate used for peer */
    A_UINT32  peer_rx_rate;
} wmi_peer_stats;

typedef struct {
    /** Primary channel freq of the channel for which stats are sent */
    A_UINT32 chan_mhz;
    /** Time spent on the channel */
    A_UINT32 sampling_period_us;
    /** Aggregate duration over a sampling period for which channel activity was observed */
    A_UINT32 rx_clear_count;
    /** Accumalation of the TX PPDU duration over a sampling period */
    A_UINT32 tx_duration_us;
    /** Accumalation of the RX PPDU duration over a sampling period */
    A_UINT32 rx_duration_us;
} wmi_chan_stats;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_create_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** VDEV type (AP,STA,IBSS,MONITOR) */
    A_UINT32 vdev_type;
    /** VDEV subtype (P2PDEV, P2PCLI, P2PGO, BT3.0)*/
    A_UINT32 vdev_subtype;
    /** VDEV MAC address */
    wmi_mac_addr vdev_macaddr;
} wmi_vdev_create_cmd_fixed_param;

/* wmi_p2p_noa_descriptor structure can't be modified without breaking the compatibility for WMI_HOST_SWBA_EVENTID */
typedef struct {
    A_UINT32   tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_p2p_noa_descriptor */
    A_UINT32   type_count; /** 255: continuous schedule, 0: reserved */
    A_UINT32   duration ;  /** Absent period duration in micro seconds */
    A_UINT32   interval;   /** Absent period interval in micro seconds */
    A_UINT32   start_time; /** 32 bit tsf time when in starts */
} wmi_p2p_noa_descriptor;

/** values for vdev_type */
#define WMI_VDEV_TYPE_AP         0x1
#define WMI_VDEV_TYPE_STA        0x2
#define WMI_VDEV_TYPE_IBSS       0x3
#define WMI_VDEV_TYPE_MONITOR    0x4

/** VDEV type is for social wifi interface.This VDEV is Currently mainly needed
* by FW to execute the NAN specific WMI commands and also implement NAN specific
* operations like Network discovery, service provisioning and service
* subscription  ..etc. If FW needs NAN VDEV then Host should issue VDEV create
* WMI command to create this VDEV once during initialization and host is not
* expected to use any VDEV specific WMI commands on this VDEV.
**/
#define WMI_VDEV_TYPE_NAN        0x5

/** values for vdev_subtype */
#define WMI_UNIFIED_VDEV_SUBTYPE_P2P_DEVICE 0x1
#define WMI_UNIFIED_VDEV_SUBTYPE_P2P_CLIENT 0x2
#define WMI_UNIFIED_VDEV_SUBTYPE_P2P_GO     0x3

/** values for vdev_start_request flags */
/** Indicates that AP VDEV uses hidden ssid. only valid for
 *  AP/GO */
#define WMI_UNIFIED_VDEV_START_HIDDEN_SSID  (1<<0)
/** Indicates if robust management frame/management frame
 *  protection is enabled. For GO/AP vdevs, it indicates that
 *  it may support station/client associations with RMF enabled.
 *  For STA/client vdevs, it indicates that sta will
 *  associate with AP with RMF enabled. */
#define WMI_UNIFIED_VDEV_START_PMF_ENABLED  (1<<1)

/*
 * Host is sending bcn_tx_rate to override the beacon tx rates.
 */
#define WMI_UNIFIED_VDEV_START_BCN_TX_RATE_PRESENT (1<<2)

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_start_request_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** requestor id identifying the caller module */
    A_UINT32 requestor_id;
    /** beacon interval from received beacon */
    A_UINT32 beacon_interval;
    /** DTIM Period from the received beacon */
    A_UINT32 dtim_period;
    /** Flags */
    A_UINT32 flags;
    /** ssid field. Only valid for AP/GO/IBSS/BTAmp VDEV type. */
    wmi_ssid ssid;
    /** beacon/probe reponse xmit rate. Applicable for SoftAP. */
    /** This field will be invalid and ignored unless the */
    /** flags field has the WMI_UNIFIED_VDEV_START_BCN_TX_RATE_PRESENT bit. */
    /** When valid, this field contains the fixed tx rate for the beacon */
    /** and probe response frames send by the GO or SoftAP */
    A_UINT32 bcn_tx_rate;
    /** beacon/probe reponse xmit power. Applicable for SoftAP. */
    A_UINT32 bcn_txPower;
    /** number of p2p NOA descriptor(s) from scan entry */
    A_UINT32  num_noa_descriptors;
    /** Disable H/W ack. This used by WMI_VDEV_RESTART_REQUEST_CMDID.
          During CAC, Our HW shouldn't ack ditected frames */
    A_UINT32  disable_hw_ack;
    /* The TLVs follows this structure:
         *     wmi_channel chan;   //WMI channel
         *     wmi_p2p_noa_descriptor  noa_descriptors[]; //actual p2p NOA descriptor from scan entry
         */
} wmi_vdev_start_request_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_delete_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
} wmi_vdev_delete_cmd_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_up_cmdid_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** aid (assoc id) received in association response for STA VDEV  */
    A_UINT32 vdev_assoc_id;
    /** bssid of the BSS the VDEV is joining  */
    wmi_mac_addr vdev_bssid;
} wmi_vdev_up_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_stop_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
} wmi_vdev_stop_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_down_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
} wmi_vdev_down_cmd_fixed_param;

typedef struct {
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
} wmi_vdev_standby_response_cmd;

typedef struct {
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
} wmi_vdev_resume_response_cmd;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_set_param_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** parameter id   */
    A_UINT32 param_id;
    /** parameter value */
    A_UINT32 param_value;
} wmi_vdev_set_param_cmd_fixed_param;

typedef struct {
    A_UINT32 key_seq_counter_l;
    A_UINT32 key_seq_counter_h;
} wmi_key_seq_counter;

#define  WMI_CIPHER_NONE     0x0  /* clear key */
#define  WMI_CIPHER_WEP      0x1
#define  WMI_CIPHER_TKIP     0x2
#define  WMI_CIPHER_AES_OCB  0x3
#define  WMI_CIPHER_AES_CCM  0x4
#define  WMI_CIPHER_WAPI     0x5
#define  WMI_CIPHER_CKIP     0x6
#define  WMI_CIPHER_AES_CMAC 0x7
#define  WMI_CIPHER_ANY      0x8

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_install_key_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** MAC address used for installing   */
    wmi_mac_addr peer_macaddr;
    /** key index */
    A_UINT32 key_ix;
    /** key flags */
    A_UINT32 key_flags;
    /** key cipher, defined above */
    A_UINT32 key_cipher;
    /** key rsc counter */
    wmi_key_seq_counter key_rsc_counter;
    /** global key rsc counter */
    wmi_key_seq_counter key_global_rsc_counter;
    /** global key tsc counter */
    wmi_key_seq_counter key_tsc_counter;
    /** WAPI key rsc counter */
    A_UINT8 wpi_key_rsc_counter[16];
    /** WAPI key tsc counter */
    A_UINT8 wpi_key_tsc_counter[16];
    /** key length */
    A_UINT32 key_len;
    /** key tx mic length */
    A_UINT32 key_txmic_len;
    /** key rx mic length */
    A_UINT32 key_rxmic_len;
        /*
         * Following this struct are this TLV.
         *     // actual key data
         *     A_UINT8  key_data[]; // contains key followed by tx mic followed by rx mic
         */
} wmi_vdev_install_key_cmd_fixed_param;

/** Preamble types to be used with VDEV fixed rate configuration */
typedef enum {
    WMI_RATE_PREAMBLE_OFDM,
    WMI_RATE_PREAMBLE_CCK,
    WMI_RATE_PREAMBLE_HT,
    WMI_RATE_PREAMBLE_VHT,
} WMI_RATE_PREAMBLE;

/** Value to disable fixed rate setting */
#define WMI_FIXED_RATE_NONE    (0xff)

/** the definition of different VDEV parameters */
typedef enum {
    /** RTS Threshold */
    WMI_VDEV_PARAM_RTS_THRESHOLD = 0x1,
    /** Fragmentation threshold */
    WMI_VDEV_PARAM_FRAGMENTATION_THRESHOLD,
    /** beacon interval in TUs */
    WMI_VDEV_PARAM_BEACON_INTERVAL,
    /** Listen interval in TUs */
    WMI_VDEV_PARAM_LISTEN_INTERVAL,
    /** muticast rate in Mbps */
    WMI_VDEV_PARAM_MULTICAST_RATE,
    /** management frame rate in Mbps */
    WMI_VDEV_PARAM_MGMT_TX_RATE,
    /** slot time (long vs short) */
    WMI_VDEV_PARAM_SLOT_TIME,
    /** preamble (long vs short) */
    WMI_VDEV_PARAM_PREAMBLE,
    /** SWBA time (time before tbtt in msec) */
    WMI_VDEV_PARAM_SWBA_TIME,
    /** time period for updating VDEV stats */
    WMI_VDEV_STATS_UPDATE_PERIOD,
    /** age out time in msec for frames queued for station in power save*/
    WMI_VDEV_PWRSAVE_AGEOUT_TIME,
    /** Host SWBA interval (time in msec before tbtt for SWBA event generation) */
    WMI_VDEV_HOST_SWBA_INTERVAL,
    /** DTIM period (specified in units of num beacon intervals) */
    WMI_VDEV_PARAM_DTIM_PERIOD,
    /** scheduler air time limit for this VDEV. used by off chan scheduler  */
    WMI_VDEV_OC_SCHEDULER_AIR_TIME_LIMIT,
    /** enable/dsiable WDS for this VDEV  */
    WMI_VDEV_PARAM_WDS,
    /** ATIM Window */
    WMI_VDEV_PARAM_ATIM_WINDOW,
    /** BMISS max */
    WMI_VDEV_PARAM_BMISS_COUNT_MAX,
    /** BMISS first time */
    WMI_VDEV_PARAM_BMISS_FIRST_BCNT,
    /** BMISS final time */
    WMI_VDEV_PARAM_BMISS_FINAL_BCNT,
    /** WMM enables/disabled */
    WMI_VDEV_PARAM_FEATURE_WMM,
    /** Channel width */
    WMI_VDEV_PARAM_CHWIDTH,
    /** Channel Offset */
    WMI_VDEV_PARAM_CHEXTOFFSET,
    /** Disable HT Protection */
    WMI_VDEV_PARAM_DISABLE_HTPROTECTION,
    /** Quick STA Kickout */
    WMI_VDEV_PARAM_STA_QUICKKICKOUT,
    /** Rate to be used with Management frames */
    WMI_VDEV_PARAM_MGMT_RATE,
    /** Protection Mode */
    WMI_VDEV_PARAM_PROTECTION_MODE,
    /** Fixed rate setting */
    WMI_VDEV_PARAM_FIXED_RATE,
    /** Short GI Enable/Disable */
    WMI_VDEV_PARAM_SGI,
    /** Enable LDPC */
    WMI_VDEV_PARAM_LDPC,
    /** Enable Tx STBC */
    WMI_VDEV_PARAM_TX_STBC,
    /** Enable Rx STBC */
    WMI_VDEV_PARAM_RX_STBC,
    /** Intra BSS forwarding  */
    WMI_VDEV_PARAM_INTRA_BSS_FWD,
    /** Setting Default xmit key for Vdev */
    WMI_VDEV_PARAM_DEF_KEYID,
    /** NSS width */
    WMI_VDEV_PARAM_NSS,
    /** Set the custom rate for the broadcast data frames */
    WMI_VDEV_PARAM_BCAST_DATA_RATE,
    /** Set the custom rate (rate-code) for multicast data frames */
    WMI_VDEV_PARAM_MCAST_DATA_RATE,
    /** Tx multicast packet indicate Enable/Disable */
    WMI_VDEV_PARAM_MCAST_INDICATE,
    /** Tx DHCP packet indicate Enable/Disable */
    WMI_VDEV_PARAM_DHCP_INDICATE,
    /** Enable host inspection of Tx unicast packet to unknown destination */
    WMI_VDEV_PARAM_UNKNOWN_DEST_INDICATE,

    /* The minimum amount of time AP begins to consider STA inactive */
    WMI_VDEV_PARAM_AP_KEEPALIVE_MIN_IDLE_INACTIVE_TIME_SECS,

    /* An associated STA is considered inactive when there is no recent TX/RX
     * activity and no downlink frames are buffered for it. Once a STA exceeds
     * the maximum idle inactive time, the AP will send an 802.11 data-null as
     * a keep alive to verify the STA is still associated. If the STA does ACK
     * the data-null, or if the data-null is buffered and the STA does not
     * retrieve it, the STA will be considered unresponsive (see
     * WMI_VDEV_AP_KEEPALIVE_MAX_UNRESPONSIVE_TIME_SECS). */
    WMI_VDEV_PARAM_AP_KEEPALIVE_MAX_IDLE_INACTIVE_TIME_SECS,

    /* An associated STA is considered unresponsive if there is no recent
     * TX/RX activity and downlink frames are buffered for it. Once a STA
     * exceeds the maximum unresponsive time, the AP will send a
     * WMI_STA_KICKOUT event to the host so the STA can be deleted. */
    WMI_VDEV_PARAM_AP_KEEPALIVE_MAX_UNRESPONSIVE_TIME_SECS,

    /* Enable NAWDS : MCAST INSPECT Enable, NAWDS Flag set */
    WMI_VDEV_PARAM_AP_ENABLE_NAWDS,
    /** Enable/Disable RTS-CTS */
    WMI_VDEV_PARAM_ENABLE_RTSCTS,
    /* Enable TXBFee/er */
    WMI_VDEV_PARAM_TXBF,

    /**Set packet power save */
    WMI_VDEV_PARAM_PACKET_POWERSAVE,

    /**Drops un-encrypted packets if any received in an encryted connection
     * otherwise forwards to host
     */
    WMI_VDEV_PARAM_DROP_UNENCRY,

    /*
     * Set TX encap type.
     *
     * enum wmi_pkt_type is to be used as the parameter
     * specifying the encap type.
     */
    WMI_VDEV_PARAM_TX_ENCAP_TYPE,

    /*
     * Try to detect stations that woke-up and exited power save but did not
     * successfully transmit data-null with PM=0 to AP. When this happens,
     * STA and AP power save state are out-of-sync. Use buffered but
     * undelivered MSDU to the STA as a hint that the STA is really awake
     * and expecting normal ASAP delivery, rather than retrieving BU with
     * PS-Poll, U-APSD trigger, etc.
     *
     * 0 disables out-of-sync detection. Maximum time is 255 seconds.
     */
    WMI_VDEV_PARAM_AP_DETECT_OUT_OF_SYNC_SLEEPING_STA_TIME_SECS,

    /* Enable/Disable early rx dynamic adjust feature.
      * Early-rx dynamic adjust is a advance power save feature.
      * Early-rx is a wakeup duration before exact TBTT,which is deemed necessary to provide a cushion for various
      * timing discrepancies in the system.
      * In current code branch, the duration is set to a very conservative fix value to make sure the drift impact is minimum.
      * The fix early-tx will result in the unnessary power consume, so a dynamic early-rx adjust algorithm can be designed
      * properly to minimum the power consume.*/
    WMI_VDEV_PARAM_EARLY_RX_ADJUST_ENABLE,

    /* set target bmiss number per sample cycle if bmiss adjust was chosen.
     * In this adjust policy,early-rx is adjusted by comparing the current bmiss rate to target bmiss rate
     * which can be set by user through WMI command.
     */
    WMI_VDEV_PARAM_EARLY_RX_TGT_BMISS_NUM,

    /* set sample cycle(in the unit of beacon interval) if bmiss adjust was chosen */
    WMI_VDEV_PARAM_EARLY_RX_BMISS_SAMPLE_CYCLE,

    /* set slop_step */
    WMI_VDEV_PARAM_EARLY_RX_SLOP_STEP,

    /* set init slop */
    WMI_VDEV_PARAM_EARLY_RX_INIT_SLOP,

    /* pause adjust enable/disable */
    WMI_VDEV_PARAM_EARLY_RX_ADJUST_PAUSE,


    /* Set channel pwr limit value of the vdev the minimal value of all
     * vdevs operating on this channel will be set as channel tx power
     * limit, which is used to configure ratearray
     */
    WMI_VDEV_PARAM_TX_PWRLIMIT,

    /* set the count of snr value for calculation in snr monitor */
    WMI_VDEV_PARAM_SNR_NUM_FOR_CAL,

    /** Roaming offload */
    WMI_VDEV_PARAM_ROAM_FW_OFFLOAD,

    /** Enable Leader request RX functionality for RMC */
    WMI_VDEV_PARAM_ENABLE_RMC,

    /* IBSS does not have deauth/disassoc, vdev has to detect peer gone event
     * by himself. If the beacon lost time exceed this threshold, the peer is
     * thought to be gone. */
    WMI_VDEV_PARAM_IBSS_MAX_BCN_LOST_MS,

    /** max rate in kpbs, transmit rate can't go beyond it */
    WMI_VDEV_PARAM_MAX_RATE,

    /* enable/disable drift sample. 0: disable; 1: clk_drift; 2: ap_drift; 3 both clk and ap drift*/
    WMI_VDEV_PARAM_EARLY_RX_DRIFT_SAMPLE,
    /* set Tx failure count threshold for the vdev */
    WMI_VDEV_PARAM_SET_IBSS_TX_FAIL_CNT_THR,

    /* set ebt resync timeout value, in the unit of TU */
    WMI_VDEV_PARAM_EBT_RESYNC_TIMEOUT,

    /* Enable Aggregation State Trigger Event */
    WMI_VDEV_PARAM_AGGR_TRIG_EVENT_ENABLE,

    /* This parameter indicates whether IBSS station can enter into power save
    * mode by sending Null frame (with PM=1). When not allowed, IBSS station has to stay
    * awake all the time and should never set PM=1 in its transmitted frames.
    * This parameter is meaningful/valid only when WMI_VDEV_PARAM_ATIM_WINDOW_LENGTH
    * is non-zero. */
    WMI_VDEV_PARAM_IS_IBSS_POWER_SAVE_ALLOWED,

    /* This parameter indicates if this station can enter into power collapse
    * for the remaining beacon interval after the ATIM window.
    * This parameter is meaningful/valid only when WMI_VDEV_PARAM_IS_IBSS_POWER_SAVE_ALLOWED
    * is set to TRUE. */
    WMI_VDEV_PARAM_IS_POWER_COLLAPSE_ALLOWED,

    /* This parameter indicates whether IBSS station exit power save mode and
    * enter power active state (by sending Null frame with PM=0 in the immediate ATIM Window)
    * whenever there is a TX/RX activity. */
    WMI_VDEV_PARAM_IS_AWAKE_ON_TXRX_ENABLED,

    /* If Awake on TX/RX activity is enabled, this parameter indicates
    * the data inactivity time in number of beacon intervals after which
    * IBSS station reenters power save by sending Null frame with PM=1. */
    WMI_VDEV_PARAM_INACTIVITY_CNT,

    /* Inactivity time in msec after which TX Service Period (SP) is
    * terminated by sending a Qos Null frame with EOSP.
    * If value is 0, TX SP is terminated with the last buffered packet itself
    * instead of waiting for the inactivity timeout. */
    WMI_VDEV_PARAM_TXSP_END_INACTIVITY_TIME_MS,

    /** DTIM policy */
    WMI_VDEV_PARAM_DTIM_POLICY,

    /* When IBSS network is initialized, PS-supporting device
    * does not enter protocol sleep state during first
    * WMI_VDEV_PARAM_IBSS_PS_WARMUP_TIME_SECS seconds. */
    WMI_VDEV_PARAM_IBSS_PS_WARMUP_TIME_SECS,

} WMI_VDEV_PARAM;

/* Length of ATIM Window in TU */
#define WMI_VDEV_PARAM_ATIM_WINDOW_LENGTH WMI_VDEV_PARAM_ATIM_WINDOW

enum wmi_pkt_type {
    WMI_PKT_TYPE_RAW = 0,
    WMI_PKT_TYPE_NATIVE_WIFI = 1,
    WMI_PKT_TYPE_ETHERNET = 2,
};

typedef struct {
    A_UINT8     sutxbfee : 1,
                mutxbfee : 1,
                sutxbfer : 1,
                mutxbfer : 1,
                reserved : 4;
} wmi_vdev_txbf_en;

/** Upto 8 bits are available for Roaming module to be sent along with
WMI_VDEV_PARAM_ROAM_FW_OFFLOAD WMI_VDEV_PARAM **/
/* Enable Roaming FW offload LFR1.5/LFR2.0 implementation */
#define WMI_ROAM_FW_OFFLOAD_ENABLE_FLAG                          0x1
/* Enable Roaming module in FW to do scan based on Final BMISS */
#define WMI_ROAM_BMISS_FINAL_SCAN_ENABLE_FLAG                    0x2

        /** slot time long */
        #define WMI_VDEV_SLOT_TIME_LONG                                  0x1
        /** slot time short */
        #define WMI_VDEV_SLOT_TIME_SHORT                                 0x2
        /** preablbe long */
        #define WMI_VDEV_PREAMBLE_LONG                                   0x1
        /** preablbe short */
        #define WMI_VDEV_PREAMBLE_SHORT                                  0x2

/** the definition of different START/RESTART Event response  */
    typedef enum {
        /* Event respose of START CMD */
        WMI_VDEV_START_RESP_EVENT = 0,
        /* Event respose of RESTART CMD */
        WMI_VDEV_RESTART_RESP_EVENT,
    } WMI_START_EVENT_PARAM;

        typedef struct {
            A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_start_response_event_fixed_param  */
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
            /** requestor id that requested the VDEV start request */
            A_UINT32 requestor_id;
            /* Respose of Event type START/RESTART */
            WMI_START_EVENT_PARAM resp_type;
            /** status of the response */
            A_UINT32 status;
            /** Vdev chain mask */
            A_UINT32 chain_mask;
            /** Vdev mimo power save mode */
            A_UINT32 smps_mode;
        } wmi_vdev_start_response_event_fixed_param;

        typedef struct {
            A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_stopped_event_fixed_param  */
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
        } wmi_vdev_stopped_event_fixed_param;

        /** common structure used for simple events (stopped, resume_req, standby response) */
        typedef struct {
            A_UINT32    tlv_header;     /* TLV tag and len; tag would be equivalent to actual event  */
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
        } wmi_vdev_simple_event_fixed_param;


        /** VDEV start response status codes */
        #define WMI_VDEV_START_RESPONSE_STATUS_SUCCESS 0x0  /** VDEV succesfully started */
        #define WMI_VDEV_START_RESPONSE_INVALID_VDEVID  0x1  /** requested VDEV not found */
        #define WMI_VDEV_START_RESPONSE_NOT_SUPPORTED  0x2  /** unsupported VDEV combination */

        /** Beacon processing related command and event structures */
        typedef struct {
            A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_bcn_tx_hdr */
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
            /** xmit rate */
            A_UINT32 tx_rate;
            /** xmit power */
            A_UINT32 txPower;
            /** beacon buffer length in bytes */
            A_UINT32 buf_len;
            /* This TLV is followed by array of bytes:
                       * // beacon frame buffer
                       *   A_UINT8 bufp[];
                       */
        } wmi_bcn_tx_hdr;

        /* Beacon filter */
        #define WMI_BCN_FILTER_ALL   0 /* Filter all beacons */
        #define WMI_BCN_FILTER_NONE  1 /* Pass all beacons */
        #define WMI_BCN_FILTER_RSSI  2 /* Pass Beacons RSSI >= RSSI threshold */
        #define WMI_BCN_FILTER_BSSID 3 /* Pass Beacons with matching BSSID */
        #define WMI_BCN_FILTER_SSID  4 /* Pass Beacons with matching SSID */

        typedef struct {
            /** Filter ID */
            A_UINT32 bcn_filter_id;
            /** Filter type - wmi_bcn_filter */
            A_UINT32 bcn_filter;
            /** Buffer len */
            A_UINT32 bcn_filter_len;
            /** Filter info (threshold, BSSID, RSSI) */
            A_UINT8 *bcn_filter_buf;
        } wmi_bcn_filter_rx_cmd;

        /** Capabilities and IEs to be passed to firmware */
        typedef struct {
            A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_bcn_prb_info */
            /** Capabilities */
            A_UINT32 caps;
            /** ERP info */
            A_UINT32 erp;
            /** Advanced capabilities */
            /** HT capabilities */
            /** HT Info */
            /** ibss_dfs */
            /** wpa Info */
            /** rsn Info */
            /** rrm info */
            /** ath_ext */
            /** app IE */
        } wmi_bcn_prb_info;

         typedef struct {
             A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_bcn_tmpl_cmd_fixed_param */
             /** unique id identifying the VDEV, generated by the caller */
             A_UINT32 vdev_id;
             /** TIM IE offset from the beginning of the template. */
             A_UINT32 tim_ie_offset;
             /** beacon buffer length. data is in TLV data[] */
             A_UINT32 buf_len;
             /*
                         * The TLVs follows:
                         *    wmi_bcn_prb_info bcn_prb_info; //beacon probe capabilities and IEs
                         *    A_UINT8  data[]; //Variable length data
                         */
        } wmi_bcn_tmpl_cmd_fixed_param;


         typedef struct {
             A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_prb_tmpl_cmd_fixed_param */
             /** unique id identifying the VDEV, generated by the caller */
             A_UINT32 vdev_id;
             /** beacon buffer length. data is in TLV data[] */
             A_UINT32 buf_len;
             /*
                         * The TLVs follows:
                         *    wmi_bcn_prb_info bcn_prb_info; //beacon probe capabilities and IEs
                         *    A_UINT8  data[]; //Variable length data
                         */
         } wmi_prb_tmpl_cmd_fixed_param;

typedef struct {
  /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_offload_bcn_tx_status_event_fixed_param */
    A_UINT32 tlv_header;
  /** unique id identifying the VDEV */
    A_UINT32 vdev_id;
  /** bcn tx status, values defined in enum WMI_FRAME_TX_STATUS */
    A_UINT32 tx_status;
} wmi_offload_bcn_tx_status_event_fixed_param;

        enum wmi_sta_ps_mode {
            /** enable power save for the given STA VDEV */
            WMI_STA_PS_MODE_DISABLED = 0,
            /** disable power save  for a given STA VDEV */
            WMI_STA_PS_MODE_ENABLED = 1,
        };

        typedef struct {
            A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_sta_powersave_mode_cmd_fixed_param */
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;

            /** Power save mode
             *
             * (see enum wmi_sta_ps_mode)
             */
            A_UINT32 sta_ps_mode;
        } wmi_sta_powersave_mode_cmd_fixed_param;

       enum wmi_csa_offload_en{
           WMI_CSA_OFFLOAD_DISABLE = 0,
           WMI_CSA_OFFLOAD_ENABLE = 1,
       };

       typedef struct{
          A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_csa_offload_enable_cmd_fixed_param  */
          A_UINT32 vdev_id;
          A_UINT32 csa_offload_enable;
       } wmi_csa_offload_enable_cmd_fixed_param;

       typedef struct{
          A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_csa_offload_chanswitch_cmd_fixed_param  */
          A_UINT32 vdev_id;
          /*
           * The TLVs follows:
           *    wmi_channel chan;
           */
       } wmi_csa_offload_chanswitch_cmd_fixed_param;
        /**
         * This parameter controls the policy for retrieving frames from AP while the
         * STA is in sleep state.
         *
         * Only takes affect if the sta_ps_mode is enabled
         */
        enum wmi_sta_ps_param_rx_wake_policy {
            /* Wake up when ever there is an  RX activity on the VDEV. In this mode
             * the Power save SM(state machine) will come out of sleep by either
             * sending null frame (or) a data frame (with PS==0) in response to TIM
             * bit set in the received beacon frame from AP.
             */
            WMI_STA_PS_RX_WAKE_POLICY_WAKE = 0,

            /* Here the power save state machine will not wakeup in response to TIM
             * bit, instead it will send a PSPOLL (or) UASPD trigger based on UAPSD
             * configuration setup by WMISET_PS_SET_UAPSD  WMI command.  When all
             * access categories are delivery-enabled, the station will send a UAPSD
             * trigger frame, otherwise it will send a PS-Poll.
             */
            WMI_STA_PS_RX_WAKE_POLICY_POLL_UAPSD = 1,
        };

        /** Number of tx frames/beacon  that cause the power save SM to wake up.
         *
         * Value 1 causes the SM to wake up for every TX. Value 0 has a special
         * meaning, It will cause the SM to never wake up. This is useful if you want
         * to keep the system to sleep all the time for some kind of test mode . host
         * can change this parameter any time.  It will affect at the next tx frame.
         */
        enum wmi_sta_ps_param_tx_wake_threshold {
            WMI_STA_PS_TX_WAKE_THRESHOLD_NEVER = 0,
            WMI_STA_PS_TX_WAKE_THRESHOLD_ALWAYS = 1,

            /* Values greater than one indicate that many TX attempts per beacon
             * interval before the STA will wake up
             */
        };

        /**
         * The maximum number of PS-Poll frames the FW will send in response to
         * traffic advertised in TIM before waking up (by sending a null frame with PS
         * = 0). Value 0 has a special meaning: there is no maximum count and the FW
         * will send as many PS-Poll as are necessary to retrieve buffered BU. This
         * parameter is used when the RX wake policy is
         * WMI_STA_PS_RX_WAKE_POLICY_POLL_UAPSD and ignored when the RX wake
         * policy is WMI_STA_PS_RX_WAKE_POLICY_WAKE.
         */
        enum wmi_sta_ps_param_pspoll_count {
            WMI_STA_PS_PSPOLL_COUNT_NO_MAX = 0,
            /* Values greater than 0 indicate the maximum numer of PS-Poll frames FW
             * will send before waking up.
             */
        };

        /*
         * This will include the delivery and trigger enabled state for every AC.
         * This is the negotiated state with AP. The host MLME needs to set this based
         * on AP capability and the state Set in the association request by the
         * station MLME.Lower 8 bits of the value specify the UAPSD configuration.
         */
        #define WMI_UAPSD_AC_TYPE_DELI 0
        #define WMI_UAPSD_AC_TYPE_TRIG 1

        #define WMI_UAPSD_AC_BIT_MASK(ac,type) (type ==  WMI_UAPSD_AC_TYPE_DELI)?(1<<(ac<<1)):(1<<((ac<<1)+1))

        enum wmi_sta_ps_param_uapsd {
            WMI_STA_PS_UAPSD_AC0_DELIVERY_EN = (1 << 0),
            WMI_STA_PS_UAPSD_AC0_TRIGGER_EN  = (1 << 1),
            WMI_STA_PS_UAPSD_AC1_DELIVERY_EN = (1 << 2),
            WMI_STA_PS_UAPSD_AC1_TRIGGER_EN  = (1 << 3),
            WMI_STA_PS_UAPSD_AC2_DELIVERY_EN = (1 << 4),
            WMI_STA_PS_UAPSD_AC2_TRIGGER_EN  = (1 << 5),
            WMI_STA_PS_UAPSD_AC3_DELIVERY_EN = (1 << 6),
            WMI_STA_PS_UAPSD_AC3_TRIGGER_EN  = (1 << 7),
        };

        enum wmi_sta_powersave_param {
            /**
             * Controls how frames are retrievd from AP while STA is sleeping
             *
             * (see enum wmi_sta_ps_param_rx_wake_policy)
             */
            WMI_STA_PS_PARAM_RX_WAKE_POLICY = 0,

            /**
             * The STA will go active after this many TX
             *
             * (see enum wmi_sta_ps_param_tx_wake_threshold)
             */
            WMI_STA_PS_PARAM_TX_WAKE_THRESHOLD = 1,

            /**
             * Number of PS-Poll to send before STA wakes up
             *
             * (see enum wmi_sta_ps_param_pspoll_count)
             *
             */
            WMI_STA_PS_PARAM_PSPOLL_COUNT = 2,

            /**
             * TX/RX inactivity time in msec before going to sleep.
             *
             * The power save SM will monitor tx/rx activity on the VDEV, if no
             * activity for the specified msec of the parameter the Power save SM will
             * go to sleep.
             */
            WMI_STA_PS_PARAM_INACTIVITY_TIME = 3,

            /**
             * Set uapsd configuration.
             *
             * (see enum wmi_sta_ps_param_uapsd)
             */
            WMI_STA_PS_PARAM_UAPSD = 4,
    /**
     * Number of PS-Poll to send before STA wakes up in QPower Mode
     */
    WMI_STA_PS_PARAM_QPOWER_PSPOLL_COUNT = 5,

    /**
     * Enable QPower
     */
    WMI_STA_PS_ENABLE_QPOWER = 6,

            /**
             * Number of TX frames before the entering the Active state
             */
            WMI_STA_PS_PARAM_QPOWER_MAX_TX_BEFORE_WAKE = 7,

            /**
             * QPower SPEC PSPOLL interval
             */
            WMI_STA_PS_PARAM_QPOWER_SPEC_PSPOLL_WAKE_INTERVAL = 8,

            /**
             * Max SPEC PSPOLL to be sent when the PSPOLL response has
             * no-data bit set
             */
            WMI_STA_PS_PARAM_QPOWER_SPEC_MAX_SPEC_NODATA_PSPOLL = 9,
        };

        typedef struct {
            A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_sta_powersave_param_cmd_fixed_param */
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
            /** station power save parameter (see enum wmi_sta_powersave_param) */
            A_UINT32 param;
            A_UINT32 value;
        } wmi_sta_powersave_param_cmd_fixed_param;

         /** No MIMO power save */
        #define WMI_STA_MIMO_PS_MODE_DISABLE
         /** mimo powersave mode static*/
        #define WMI_STA_MIMO_PS_MODE_STATIC
         /** mimo powersave mode dynamic */
        #define WMI_STA_MIMO_PS_MODE_DYNAMI

        typedef struct {
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
            /** mimo powersave mode as defined above */
            A_UINT32 mimo_pwrsave_mode;
        } wmi_sta_mimo_ps_mode_cmd;


        /** U-APSD configuration of peer station from (re)assoc request and TSPECs */
        enum wmi_ap_ps_param_uapsd {
            WMI_AP_PS_UAPSD_AC0_DELIVERY_EN = (1 << 0),
            WMI_AP_PS_UAPSD_AC0_TRIGGER_EN  = (1 << 1),
            WMI_AP_PS_UAPSD_AC1_DELIVERY_EN = (1 << 2),
            WMI_AP_PS_UAPSD_AC1_TRIGGER_EN  = (1 << 3),
            WMI_AP_PS_UAPSD_AC2_DELIVERY_EN = (1 << 4),
            WMI_AP_PS_UAPSD_AC2_TRIGGER_EN  = (1 << 5),
            WMI_AP_PS_UAPSD_AC3_DELIVERY_EN = (1 << 6),
            WMI_AP_PS_UAPSD_AC3_TRIGGER_EN  = (1 << 7),
        };

        /** U-APSD maximum service period of peer station */
        enum wmi_ap_ps_peer_param_max_sp {
            WMI_AP_PS_PEER_PARAM_MAX_SP_UNLIMITED = 0,
            WMI_AP_PS_PEER_PARAM_MAX_SP_2 = 1,
            WMI_AP_PS_PEER_PARAM_MAX_SP_4 = 2,
            WMI_AP_PS_PEER_PARAM_MAX_SP_6 = 3,

            /* keep last! */
            MAX_WMI_AP_PS_PEER_PARAM_MAX_SP,
        };

        /**
         * AP power save parameter
         * Set a power save specific parameter for a peer station
         */
        enum wmi_ap_ps_peer_param {
            /** Set uapsd configuration for a given peer.
             *
             * This will include the delivery and trigger enabled state for every AC.
             * The host  MLME needs to set this based on AP capability and stations
             * request Set in the association request  received from the station.
             *
             * Lower 8 bits of the value specify the UAPSD configuration.
             *
             * (see enum wmi_ap_ps_param_uapsd)
             * The default value is 0.
             */
            WMI_AP_PS_PEER_PARAM_UAPSD = 0,

            /**
             * Set the service period for a UAPSD capable station
             *
             * The service period from wme ie in the (re)assoc request frame.
             *
             * (see enum wmi_ap_ps_peer_param_max_sp)
             */
            WMI_AP_PS_PEER_PARAM_MAX_SP = 1,

            /** Time in seconds for aging out buffered frames for STA in power save */
            WMI_AP_PS_PEER_PARAM_AGEOUT_TIME = 2,
        };

        typedef struct {
            A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_ap_ps_peer_cmd_fixed_param */
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
            /** peer MAC address */
            wmi_mac_addr peer_macaddr;
            /** AP powersave param (see enum wmi_ap_ps_peer_param) */
            A_UINT32 param;
            /** AP powersave param value */
            A_UINT32 value;
        } wmi_ap_ps_peer_cmd_fixed_param;

        /** Configure peer station 11v U-APSD coexistance
         *
         * Two parameters from uaspd coexistence ie info (as specified in 11v) are
         * sent down to FW along with this command.
         *
         * The semantics of these fields are described in the following text extracted
         * from 802.11v.
         *
         * ---  If the non-AP STA specified a non-zero TSF 0 Offset value in the
         *      U-APSD Coexistence element, the AP should not transmit frames to the
         *      non-AP STA outside of the U-APSD Coexistence Service Period, which
         *      begins when the AP receives the U-APSD trigger frame and ends after
         *      the transmission period specified by the result of the following
         *      calculation:
         *
         *          End of transmission period = T + (Interval . ((T . TSF 0 Offset) mod Interval))
         *
         *      Where T is the time the U-APSD trigger frame was received at the AP
         *      Interval is the UAPSD Coexistence element Duration/Interval field
         *      value (see 7.3.2.91) or upon the successful transmission of a frame
         *      with EOSP bit set to 1, whichever is earlier.
         *
         *
         * ---  If the non-AP STA specified a zero TSF 0 Offset value in the U-APSD
         *      Coexistence element, the AP should not transmit frames to the non-AP
         *      STA outside of the U-APSD Coexistence Service Period, which begins
         *      when the AP receives a U-APSD trigger frame and ends after the
         *      transmission period specified by the result of the following
         *      calculation: End of transmission period = T + Duration
         */
        typedef struct {
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
            /** peer MAC address */
            wmi_mac_addr peer_macaddr;
            /** Enable U-APSD coexistence support for this peer
             *
             * 0 -> disabled (default)
             * 1 -> enabled
             */
            A_UINT32 enabled;
            /** Duration/Interval as defined by 11v U-ASPD coexistance */
            A_UINT32 duration_interval;
            /** Upper 32 bits of 64-bit TSF offset */
            A_UINT32 tsf_offset_high;
            /** Lower 32 bits of 64-bit TSF offset */
            A_UINT32 tsf_offset_low;
        } wmi_ap_powersave_peer_uapsd_coex_cmd;

        /* 128 clients = 4 words */
        /* WMI_TIM_BITMAP_ARRAY_SIZE can't be modified without breaking the compatibility */
        #define WMI_TIM_BITMAP_ARRAY_SIZE 4

        typedef struct {
            A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_tim_info  */
            /** TIM bitmap len (in bytes)*/
            A_UINT32 tim_len;
            /** TIM Partial Virtual Bitmap */
            A_UINT32 tim_mcast;
            A_UINT32 tim_bitmap[WMI_TIM_BITMAP_ARRAY_SIZE];
            A_UINT32 tim_changed;
            A_UINT32 tim_num_ps_pending;
        } wmi_tim_info;

        typedef struct {
            /** Flag to enable quiet period IE support */
            A_UINT32   is_enabled;
            /** Quiet start */
            A_UINT32   tbttcount;
            /** Beacon intervals between quiets*/
            A_UINT32   period;
            /** TUs of each quiet*/
            A_UINT32   duration;
            /** TUs of from TBTT of quiet start*/
            A_UINT32   offset;
        } wmi_quiet_info;

/* WMI_P2P_MAX_NOA_DESCRIPTORS can't be modified without breaking the compatibility */
#define WMI_P2P_MAX_NOA_DESCRIPTORS 4	/* Maximum number of NOA Descriptors supported */

        typedef struct {
        A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_p2p_noa_info  */
        /** Bit  0:        Flag to indicate an update in NOA schedule
         *  Bits 7-1:    Reserved
         *  Bits 15-8:    Index (identifies the instance of NOA sub element)
         *  Bit  16:     Opp PS state of the AP
         *     Bits 23-17:    Ctwindow in TUs
         *     Bits 31-24:    Number of NOA descriptors
         */
        A_UINT32    noa_attributes;
        wmi_p2p_noa_descriptor    noa_descriptors[WMI_P2P_MAX_NOA_DESCRIPTORS];
        }wmi_p2p_noa_info;

#define	WMI_UNIFIED_NOA_ATTR_MODIFIED		0x1
#define	WMI_UNIFIED_NOA_ATTR_MODIFIED_S		0

#define WMI_UNIFIED_NOA_ATTR_IS_MODIFIED(hdr)                       \
			WMI_F_MS((hdr)->noa_attributes, WMI_UNIFIED_NOA_ATTR_MODIFIED)

#define WMI_UNIFIED_NOA_ATTR_MODIFIED_SET(hdr)                      \
            WMI_F_RMW((hdr)->noa_attributes, 0x1,                    \
              WMI_UNIFIED_NOA_ATTR_MODIFIED);

#define	WMI_UNIFIED_NOA_ATTR_INDEX			0xff00
#define	WMI_UNIFIED_NOA_ATTR_INDEX_S		8

#define WMI_UNIFIED_NOA_ATTR_INDEX_GET(hdr)                            \
            WMI_F_MS((hdr)->noa_attributes, WMI_UNIFIED_NOA_ATTR_INDEX)

#define WMI_UNIFIED_NOA_ATTR_INDEX_SET(hdr, v)                      \
            WMI_F_RMW((hdr)->noa_attributes, (v) & 0xff,            \
        WMI_UNIFIED_NOA_ATTR_INDEX);

#define	WMI_UNIFIED_NOA_ATTR_OPP_PS			0x10000
#define	WMI_UNIFIED_NOA_ATTR_OPP_PS_S		16

#define WMI_UNIFIED_NOA_ATTR_OPP_PS_GET(hdr)                         \
            WMI_F_MS((hdr)->noa_attributes, WMI_UNIFIED_NOA_ATTR_OPP_PS)

#define WMI_UNIFIED_NOA_ATTR_OPP_PS_SET(hdr)                         \
            WMI_F_RMW((hdr)->noa_attributes, 0x1,                    \
        WMI_UNIFIED_NOA_ATTR_OPP_PS);

#define	WMI_UNIFIED_NOA_ATTR_CTWIN			0xfe0000
#define	WMI_UNIFIED_NOA_ATTR_CTWIN_S		17

#define WMI_UNIFIED_NOA_ATTR_CTWIN_GET(hdr)                          \
            WMI_F_MS((hdr)->noa_attributes, WMI_UNIFIED_NOA_ATTR_CTWIN)

#define WMI_UNIFIED_NOA_ATTR_CTWIN_SET(hdr, v)                       \
            WMI_F_RMW((hdr)->noa_attributes, (v) & 0x7f,             \
        WMI_UNIFIED_NOA_ATTR_CTWIN);

#define	WMI_UNIFIED_NOA_ATTR_NUM_DESC		0xff000000
#define	WMI_UNIFIED_NOA_ATTR_NUM_DESC_S		24

#define WMI_UNIFIED_NOA_ATTR_NUM_DESC_GET(hdr)                       \
            WMI_F_MS((hdr)->noa_attributes, WMI_UNIFIED_NOA_ATTR_NUM_DESC)

#define WMI_UNIFIED_NOA_ATTR_NUM_DESC_SET(hdr, v)                    \
            WMI_F_RMW((hdr)->noa_attributes, (v) & 0xff,             \
        WMI_UNIFIED_NOA_ATTR_NUM_DESC);

        typedef struct {
            /** TIM info */
            wmi_tim_info tim_info;
            /** P2P NOA info */
            wmi_p2p_noa_info p2p_noa_info;
            /* TBD: More info elements to be added later */
        } wmi_bcn_info;

        typedef struct {
            A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_host_swba_event_fixed_param  */
            /** bitmap identifying the VDEVs, generated by the caller */
            A_UINT32 vdev_map;
            /* This TLV is followed by tim_info and p2p_noa_info for each vdev in vdevmap :
                       *     wmi_tim_info tim_info[];
                       *     wmi_p2p_noa_info p2p_noa_info[];
                       *
                       */
        } wmi_host_swba_event_fixed_param;

        #define WMI_MAX_AP_VDEV 16


        typedef struct {
            A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_tbtt_offset_event_fixed_param  */
            /** bimtap of VDEVs that has tbtt offset updated */
            A_UINT32 vdev_map;
            /* The TLVs for tbttoffset_list will follow this TLV.
                      *     tbtt offset list in the order of the LSB to MSB in the vdev_map bitmap
                      *     A_UINT32 tbttoffset_list[WMI_MAX_AP_VDEV];
                      */
        } wmi_tbtt_offset_event_fixed_param;


        /* Peer Specific commands and events */

        /* Peer Type:
         * NB: This can be left DEFAULT for the normal case, and f/w will determine BSS type based
         * on address and vdev opmode. This is largely here to allow host to indicate that
         * peer is explicitly a TDLS peer
         */
        enum wmi_peer_type {
            WMI_PEER_TYPE_DEFAULT = 0, /* Generic/Non-BSS/Self Peer */
            WMI_PEER_TYPE_BSS = 1,     /* Peer is BSS Peer entry */
            WMI_PEER_TYPE_TDLS = 2,    /* Peer is a TDLS Peer */
        };

        typedef struct {
            A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_create_cmd_fixed_param */
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
            /** peer MAC address */
            wmi_mac_addr peer_macaddr;
            /** peer type: see enum values above */
            A_UINT32 peer_type;
        } wmi_peer_create_cmd_fixed_param;

        typedef struct {
            A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_delete_cmd_fixed_param */
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
            /** peer MAC address */
            wmi_mac_addr peer_macaddr;
        } wmi_peer_delete_cmd_fixed_param;

        typedef struct {
            A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_flush_tids_cmd_fixed_param */
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
            /** peer MAC address */
            wmi_mac_addr peer_macaddr;
            /** tid bitmap identifying the tids to flush */
            A_UINT32 peer_tid_bitmap;
        } wmi_peer_flush_tids_cmd_fixed_param;

        typedef struct {
            /** rate mode . 0: disable fixed rate (auto rate)
             *   1: legacy (non 11n) rate  specified as ieee rate 2*Mbps
             *   2: ht20 11n rate  specified as mcs index
             *   3: ht40 11n rate  specified as mcs index
             */
            A_UINT32  rate_mode;
             /** 4 rate values for 4 rate series. series 0 is stored in byte 0 (LSB)
              *  and series 3 is stored at byte 3 (MSB) */
            A_UINT32  rate_series;
             /** 4 retry counts for 4 rate series. retry count for rate 0 is stored in byte 0 (LSB)
              *  and retry count for rate 3 is stored at byte 3 (MSB) */
            A_UINT32  rate_retries;
        } wmi_fixed_rate;

        typedef struct {
            /** unique id identifying the VDEV, generated by the caller */
            A_UINT32 vdev_id;
            /** peer MAC address */
            wmi_mac_addr peer_macaddr;
            /** fixed rate */
            wmi_fixed_rate peer_fixed_rate;
        } wmi_peer_fixed_rate_cmd;

        #define WMI_MGMT_TID    17

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_addba_clear_resp_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
} wmi_addba_clear_resp_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_addba_send_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** Tid number */
    A_UINT32 tid;
    /** Buffer/Window size*/
    A_UINT32 buffersize;
} wmi_addba_send_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_delba_send_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** Tid number */
    A_UINT32 tid;
    /** Is Initiator */
    A_UINT32 initiator;
    /** Reason code */
    A_UINT32 reasoncode;
} wmi_delba_send_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_addba_setresponse_cmd_fixed_param */
    /** unique id identifying the vdev, generated by the caller */
    A_UINT32 vdev_id;
    /** peer mac address */
    wmi_mac_addr peer_macaddr;
    /** Tid number */
    A_UINT32 tid;
    /** status code */
    A_UINT32 statuscode;
} wmi_addba_setresponse_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_send_singleamsdu_cmd_fixed_param */
    /** unique id identifying the vdev, generated by the caller */
    A_UINT32 vdev_id;
    /** peer mac address */
    wmi_mac_addr peer_macaddr;
    /** Tid number */
    A_UINT32 tid;
} wmi_send_singleamsdu_cmd_fixed_param;

/* Type of Station DTIM Power Save method */
enum {
    /* For NORMAL DTIM, the parameter is the number of beacon intervals and
     * also the same value as the listen interval. For this method, the
     * station will wake up based on the listen interval. If this
     * listen interval is not equal to DTIM, then the station may
     * miss certain DTIM beacons. If this value is 1, then the
     * station will wake up for every beacon.
     */
    WMI_STA_DTIM_PS_NORMAL_DTIM = 0x01,
    /* For MODULATED_DTIM, parameter is a multiple of DTIM beacons to skip.
     * When this value is 1, then the station will wake at every DTIM beacon.
     * If this value is >1, then the station will skip certain DTIM beacons.
     * This value is the multiple of DTIM intervals that the station will
     * wake up to receive the DTIM beacons.
     */
    WMI_STA_DTIM_PS_MODULATED_DTIM = 0x02,
};

/* Parameter structure for the WMI_STA_DTIM_PS_METHOD_CMDID */
typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_sta_dtim_ps_method_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** Station DTIM Power Save method as defined above */
    A_UINT32 dtim_pwrsave_method;
    /** DTIM PS value. Contents depends on the method */
    A_UINT32 value;
} wmi_sta_dtim_ps_method_cmd_fixed_param;

    /*
    * For Station UAPSD Auto Trigger feature, the Firmware monitors the
    * uAPSD uplink and downlink traffic for each uAPSD enabled WMM ACs.
    * If there is no uplink/download for the specified service interval (field service_interval),
    * firmware will auto generate a QOS-NULL trigger for that WMM-AP with the TID value
    * specified in the UP (field user_priority).
    * Firmware also monitors the responses for these QOS-NULL triggers.
    * If the peer does not have any delivery frames, it will respond with
    * QOS-NULL (EOSP=1). This feature of only using service interval is assumed to be mandatory for all
    * firmware implementation. For this basic implementation, the suspend_interval and delay_interval
    * are unused and should be set to 0.
    * When service_interval is 0, then the firmware will not send any trigger frames. This is for
    * certain host-based implementations that don't want this firmware offload.
    * Note that the per-AC intervals are required for some usage scenarios. This is why the intervals
    * are given in the array of ac_param[]. For example, Voice service interval may defaults to 20 ms
    * and rest of the AC default to 300 ms.
    *
    * The service bit, WMI_STA_UAPSD_VAR_AUTO_TRIG, will indicate that the more advanced feature
    * of variable auto trigger is supported. The suspend_interval and delay_interval is used in
    * the more advanced monitoring method.
    * If the PEER does not have any delivery enabled data frames (non QOS-NULL) for the
    * suspend interval (field suspend_interval), firmware will change its auto trigger interval
    * to delay interval (field delay_interval). This way, when there is no traffic, the station
    * will save more power by waking up less and sending less trigger frames.
    * The (service_interval < suspend_interval) and (service_interval < delay_interval).
    * If this variable auto trigger is not required, then the suspend_interval and delay_interval
    * should be 0.
    */
typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_sta_uapsd_auto_trig_param */
    /** WMM Access category from 0 to 3 */
    A_UINT32 wmm_ac;
    /** User priority to use in trigger frames. It is the TID
     *  value. This field needs to be specified and may not be
     *  equivalent to AC since some implementation may use the TSPEC
     *  to enable UAPSD and negotiate a particular user priority. */
    A_UINT32 user_priority;
    /** service interval in ms */
    A_UINT32 service_interval;
    /** Suspend interval in ms */
    A_UINT32 suspend_interval;
    /** delay interval in ms */
    A_UINT32 delay_interval;
} wmi_sta_uapsd_auto_trig_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_sta_uapsd_auto_trig_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer mac address */
    wmi_mac_addr peer_macaddr;
    /** Number of AC to specify */
    A_UINT32 num_ac;
    /*
         * Following this struc is the TLV:
         *     wmi_sta_uapsd_auto_trig_param ac_param[]; //Variable number of AC parameters (defined by field num_ac)
         */

} wmi_sta_uapsd_auto_trig_cmd_fixed_param;

/** mimo powersave state */
#define WMI_PEER_MIMO_PS_STATE                          0x1
/** enable/disable AMPDU . initial value (enabled) */
#define WMI_PEER_AMPDU                                  0x2
/** authorize/unauthorize peer. initial value is unauthorized (0)  */
#define WMI_PEER_AUTHORIZE                              0x3
/** peer channel bandwidth */
#define WMI_PEER_CHWIDTH                                0x4
/** peer NSS */
#define WMI_PEER_NSS                                    0x5
/** USE 4 ADDR */
#define WMI_PEER_USE_4ADDR                              0x6
/* set group membership status */
#define WMI_PEER_MEMBERSHIP                0x7
#define WMI_PEER_USERPOS                0x8
/*
 * A critical high-level protocol is being used with this peer. Target
 * should take appropriate measures (if possible) to ensure more
 * reliable link with minimal latency. This *may* include modifying the
 * station power save policy, enabling more RX chains, increased
 * priority of channel scheduling, etc.
 *
 * NOTE: This parameter should only be considered a hint as specific
 * behavior will depend on many factors including current network load
 * and vdev/peer configuration.
 *
 * For STA VDEV this peer corresponds to the AP's BSS peer.
 * For AP VDEV this peer corresponds to the remote peer STA.
 */
#define WMI_PEER_CRIT_PROTO_HINT_ENABLED                0x9
/* set Tx failure count threshold for the peer - Currently unused */
#define WMI_PEER_TX_FAIL_CNT_THR                        0xA
/* Enable H/W retry and Enable H/W Send CTS2S before Data */
#define WMI_PEER_SET_HW_RETRY_CTS2S                     0xB

/* Set peer advertised IBSS atim window length */
#define WMI_PEER_IBSS_ATIM_WINDOW_LENGTH                0xC

/** mimo ps values for the parameter WMI_PEER_MIMO_PS_STATE  */
#define WMI_PEER_MIMO_PS_NONE                          0x0
#define WMI_PEER_MIMO_PS_STATIC                        0x1
#define WMI_PEER_MIMO_PS_DYNAMIC                       0x2

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_set_param_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** parameter id   */
    A_UINT32 param_id;
    /** parametr value */
    A_UINT32 param_value;
} wmi_peer_set_param_cmd_fixed_param;

#define MAX_SUPPORTED_RATES 128

typedef struct {
    /** total number of rates */
    A_UINT32 num_rates;
    /**
     * rates (each 8bit value) packed into a 32 bit word.
     * the rates are filled from least significant byte to most
     * significant byte.
     */
    A_UINT32 rates[(MAX_SUPPORTED_RATES/4)+1];
} wmi_rate_set;

/* NOTE: It would bea good idea to represent the Tx MCS
 * info in one word and Rx in another word. This is split
 * into multiple words for convenience
 */
typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vht_rate_set */
    A_UINT32 rx_max_rate; /* Max Rx data rate */
    A_UINT32 rx_mcs_set;  /* Negotiated RX VHT rates */
    A_UINT32 tx_max_rate; /* Max Tx data rate */
    A_UINT32 tx_mcs_set;  /* Negotiated TX VHT rates */
}wmi_vht_rate_set;

/*
 * IMPORTANT: Make sure the bit definitions here are consistent
 * with the ni_flags definitions in wlan_peer.h
 */
#define WMI_PEER_AUTH           0x00000001  /* Authorized for data */
#define WMI_PEER_QOS            0x00000002  /* QoS enabled */
#define WMI_PEER_NEED_PTK_4_WAY 0x00000004  /* Needs PTK 4 way handshake for authorization */
#define WMI_PEER_NEED_GTK_2_WAY 0x00000010  /* Needs GTK 2 way handshake after 4-way handshake */
#define WMI_PEER_APSD           0x00000800  /* U-APSD power save enabled */
#define WMI_PEER_HT             0x00001000  /* HT enabled */
#define WMI_PEER_40MHZ          0x00002000  /* 40MHz enabld */
#define WMI_PEER_STBC           0x00008000  /* STBC Enabled */
#define WMI_PEER_LDPC           0x00010000  /* LDPC ENabled */
#define WMI_PEER_DYN_MIMOPS     0x00020000  /* Dynamic MIMO PS Enabled */
#define WMI_PEER_STATIC_MIMOPS  0x00040000  /* Static MIMO PS enabled */
#define WMI_PEER_SPATIAL_MUX    0x00200000  /* SM Enabled */
#define WMI_PEER_VHT            0x02000000  /* VHT Enabled */
#define WMI_PEER_80MHZ          0x04000000  /* 80MHz enabld */
#define WMI_PEER_PMF            0x08000000  /* Robust Management Frame Protection enabled */
/** CAUTION TODO: Place holder for WLAN_PEER_F_PS_PRESEND_REQUIRED = 0x10000000. Need to be clean up */
#define WMI_PEER_IS_P2P_CAPABLE 0x20000000  /* P2P capable peer */
#define WMI_PEER_SAFEMODE_EN    0x80000000  /* Fips Mode Enabled */

/**
 * Peer rate capabilities.
 *
 * This is of interest to the ratecontrol
 * module which resides in the firmware. The bit definitions are
 * consistent with that defined in if_athrate.c.
 *
 * @todo
 * Move this to a common header file later so there is no need to
 * duplicate the definitions or maintain consistency.
 */
#define WMI_RC_DS_FLAG          0x01    /* Dual stream flag */
#define WMI_RC_CW40_FLAG        0x02    /* CW 40 */
#define WMI_RC_SGI_FLAG         0x04    /* Short Guard Interval */
#define WMI_RC_HT_FLAG          0x08    /* HT */
#define WMI_RC_RTSCTS_FLAG      0x10    /* RTS-CTS */
#define WMI_RC_TX_STBC_FLAG     0x20    /* TX STBC */
#define WMI_RC_TX_STBC_FLAG_S   5       /* TX STBC */
#define WMI_RC_RX_STBC_FLAG     0xC0    /* RX STBC ,2 bits */
#define WMI_RC_RX_STBC_FLAG_S   6       /* RX STBC ,2 bits */
#define WMI_RC_WEP_TKIP_FLAG    0x100   /* WEP/TKIP encryption */
#define WMI_RC_TS_FLAG          0x200   /* Three stream flag */
#define WMI_RC_UAPSD_FLAG       0x400   /* UAPSD Rate Control */

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_assoc_complete_cmd_fixed_param */
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** VDEV id */
    A_UINT32 vdev_id;
    /** assoc = 1 reassoc = 0 */
    A_UINT32 peer_new_assoc;
    /** peer associd (16 bits) */
    A_UINT32 peer_associd;
    /** peer station flags: see definition above */
    A_UINT32 peer_flags;
    /** negotiated capabilities (lower 16 bits)*/
    A_UINT32 peer_caps;
    /** Listen interval */
    A_UINT32 peer_listen_intval;
    /** HT capabilties of the peer */
    A_UINT32 peer_ht_caps;
    /** maximum rx A-MPDU length */
    A_UINT32 peer_max_mpdu;
    /** mpdu density of the peer in usec(0 to 16) */
    A_UINT32 peer_mpdu_density;
    /** peer rate capabilties see flags above */
    A_UINT32 peer_rate_caps;
    /** num spatial streams */
    A_UINT32 peer_nss;
    /** VHT capabilties of the peer */
    A_UINT32 peer_vht_caps;
    /** phy mode */
    A_UINT32 peer_phymode;
    /** HT Operation Element of the peer. Five bytes packed in 2
         *  INT32 array and filled from lsb to msb.
         *  Note that the size of array peer_ht_info[] cannotbe changed
         *  without breaking WMI Compatibility. */
    A_UINT32 peer_ht_info[2];
    /** total number of negotiated legacy rate set. Also the sizeof
     *  peer_legacy_rates[] */
    A_UINT32 num_peer_legacy_rates;
    /** total number of negotiated ht rate set. Also the sizeof
     *  peer_ht_rates[] */
    A_UINT32 num_peer_ht_rates;
    /* Following this struc are the TLV's:
         *     A_UINT8 peer_legacy_rates[];
         *     A_UINT8 peer_ht_rates[];
         *     wmi_vht_rate_set peer_vht_rates; //VHT capabilties of the peer
         */
} wmi_peer_assoc_complete_cmd_fixed_param;

typedef struct {
    A_UINT32     tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_add_wds_entry_cmd_fixed_param */
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** wds MAC addr */
    wmi_mac_addr wds_macaddr;
} wmi_peer_add_wds_entry_cmd_fixed_param;

typedef struct {
    A_UINT32     tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_remove_wds_entry_cmd_fixed_param */
    /** wds MAC addr */
    wmi_mac_addr wds_macaddr;
} wmi_peer_remove_wds_entry_cmd_fixed_param;


typedef struct {
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
} wmi_peer_q_empty_callback_event;



/**
 * Channel info WMI event
 */
typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_chan_info_event_fixed_param */
     /** Error code */
    A_UINT32 err_code;
   /** Channel freq */
    A_UINT32 freq;
    /** Read flags */
    A_UINT32 cmd_flags;
    /** Noise Floor value */
    A_UINT32 noise_floor;
    /** rx clear count */
    A_UINT32   rx_clear_count;
    /** cycle count */
    A_UINT32   cycle_count;
} wmi_chan_info_event_fixed_param;

/**
 * Non wlan interference event
 */
typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_ath_dcs_cw_int */
    A_UINT32 channel; /* either number or freq in mhz*/
} ath_dcs_cw_int;

/**
 *  wlan_dcs_im_tgt_stats
 *
 */
typedef struct _wlan_dcs_im_tgt_stats {
    /** current running TSF from the TSF-1 */
    A_UINT32                  reg_tsf32;

    /** Known last frame rssi, in case of multiple stations, if
     *  and at different ranges, this would not gaurantee that
     *  this is the least rssi.
     */
    A_UINT32                  last_ack_rssi;

    /**  Sum of all the failed durations in the last one second interval.
     */
    A_UINT32                  tx_waste_time;
    /** count how many times the hal_rxerr_phy is marked, in this
     *  time period
     */
    A_UINT32                rx_time;
    A_UINT32                 phyerr_cnt;

    /**
         *  WLAN IM stats from target to host
         *
         *  Below statistics are sent from target to host periodically.
         *  These are collected at target as long as target is running
         *  and target chip is not in sleep.
         *
         */
    /** listen time from ANI */
    A_INT32   listen_time;

    /** tx frame count, MAC_PCU_TX_FRAME_CNT_ADDRESS */
    A_UINT32   reg_tx_frame_cnt;

    /** rx frame count, MAC_PCU_RX_FRAME_CNT_ADDRESS */
    A_UINT32   reg_rx_frame_cnt;

    /** rx clear count, MAC_PCU_RX_CLEAR_CNT_ADDRESS */
    A_UINT32   reg_rxclr_cnt;

    /** total cycle counts MAC_PCU_CYCLE_CNT_ADDRESS */
    A_UINT32   reg_cycle_cnt;            /* delta cycle count */

    /** extenstion channel rx clear count  */
    A_UINT32   reg_rxclr_ext_cnt;

    /** OFDM phy error counts, MAC_PCU_PHY_ERR_CNT_1_ADDRESS */
    A_UINT32   reg_ofdm_phyerr_cnt;

    /** CCK phy error count, MAC_PCU_PHY_ERR_CNT_2_ADDRESS */
    A_UINT32   reg_cck_phyerr_cnt;        /* CCK err count since last reset, read from register */

} wlan_dcs_im_tgt_stats_t;

/**
 *  wmi_dcs_interference_event_t
 *
 *  Right now this is event and stats together. Partly this is
 *  because cw interference is handled in target now. This
 *  can be done at host itself, if we can carry the NF alone
 *  as a stats event. In future this would be done and this
 *  event would carry only stats.
 */
typedef struct {
    A_UINT32    tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_dcs_interference_event_fixed_param */
    /**
     * Type of the event present, either the cw interference event, or the wlan_im stats
     */
    A_UINT32    interference_type;      /* type of interference, wlan or cw */
    /*
     * Following this struct are these TLVs. Note that they are both array of structures
     * but can have at most one element. Which TLV is empty or has one element depends
     * on the field interference_type. This is to emulate an union with cw_int and wlan_stat
     * elements (not arrays).     union { ath_dcs_cw_int cw_int; wlan_dcs_im_tgt_stats_t   wlan_stat; } int_event;
     *
     *        //cw_interference event
     *       ath_dcs_cw_int            cw_int[];  this element
     *       // wlan im interfernce stats
     *       wlan_dcs_im_tgt_stats_t   wlan_stat[];
     */
} wmi_dcs_interference_event_fixed_param;

enum wmi_peer_mcast_group_action {
    wmi_peer_mcast_group_action_add = 0,
    wmi_peer_mcast_group_action_del = 1
};
#define WMI_PEER_MCAST_GROUP_FLAG_ACTION_M   0x1
#define WMI_PEER_MCAST_GROUP_FLAG_ACTION_S   0
#define WMI_PEER_MCAST_GROUP_FLAG_WILDCARD_M 0x2
#define WMI_PEER_MCAST_GROUP_FLAG_WILDCARD_S 1
/* multicast group membership commands */
/* TODO: Converting this will be tricky since it uses an union.
   Also, the mac_addr is not aligned. We will convert to the wmi_mac_addr */
typedef struct{
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_mcast_group_cmd_fixed_param */
    A_UINT32 flags;
    wmi_mac_addr ucast_mac_addr;
    A_UINT8 mcast_ip_addr[16]; /* in network byte order */
} wmi_peer_mcast_group_cmd_fixed_param;


/** Offload Scan and Roaming related  commands */
/** The FW performs 2 different kinds of offload scans independent
 *  of host. One is Roam scan which is primarily performed  on a
 *  station VDEV after association to look for a better AP that
 *  the station VDEV can roam to. The second scan is connect scan
 *  which is mainly performed when the station is not associated
 *  and to look for a matching AP profile from a list of
 *  configured profiles. */

/**
 * WMI_ROAM_SCAN_MODE: Set Roam Scan mode
 *   the roam scan mode is one of the periodic, rssi change, both, none.
 *   None        : Disable Roam scan. No Roam scan at all.
 *   Periodic    : Scan periodically with a configurable period.
 *   Rssi change : Scan when ever rssi to current AP changes by the threshold value
 *                 set by WMI_ROAM_SCAN_RSSI_CHANGE_THRESHOLD command.
 *   Both        : Both of the above (scan when either period expires or rss to current AP changes by X amount)
 *
 */
typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_roam_scan_mode_fixed_param */
	A_UINT32 roam_scan_mode;
    A_UINT32 vdev_id;
} wmi_roam_scan_mode_fixed_param;

#define WMI_ROAM_SCAN_MODE_NONE        0x0
#define WMI_ROAM_SCAN_MODE_PERIODIC    0x1
#define WMI_ROAM_SCAN_MODE_RSSI_CHANGE 0x2
#define WMI_ROAM_SCAN_MODE_BOTH        0x3

typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_roam_scan_cmd_fixed_param */
    A_UINT32 vdev_id;
    A_UINT32 command_arg;
} wmi_roam_scan_cmd_fixed_param;

#define WMI_ROAM_SCAN_STOP_CMD 0x1

/**
 * WMI_ROAM_SCAN_RSSI_THRESHOLD : set scan rssi thresold
 *  scan rssi threshold is the rssi threshold below which the FW will start running Roam scans.
 * Applicable when WMI_ROAM_SCAN_MODE is not set to none.
 */
typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_roam_scan_rssi_threshold_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** roam scan rssi threshold */
    A_UINT32 roam_scan_rssi_thresh;
    /** When using Hw generated beacon RSSI interrupts */
    A_UINT32 roam_rssi_thresh_diff;
} wmi_roam_scan_rssi_threshold_fixed_param;

/**
 * WMI_ROAM_SCAN_PERIOD: period for roam scan.
 *  Applicable when the scan mode is Periodic or both.
 */
typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_roam_scan_period_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** roam scan period value */
    A_UINT32 roam_scan_period;
    /** Aging for Roam scans */
    A_UINT32 roam_scan_age;
} wmi_roam_scan_period_fixed_param;

/**
 * WMI_ROAM_SCAN_RSSI_CHANGE_THRESHOLD : rssi delta to trigger the roam scan.
 *   Rssi change threshold used when mode is Rssi change (or) Both.
 *   The FW will run the roam scan when ever the rssi changes (up or down) by the value set by this parameter.
 *   Note scan is triggered based on the rssi threshold condition set by WMI_ROAM_SCAN_RSSI_THRESHOLD
 */
typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_roam_scan_rssi_change_threshold_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** roam scan rssi change threshold value */
    A_UINT32 roam_scan_rssi_change_thresh;
    /** When using Hw generated beacon RSSI interrupts */
    A_UINT32 bcn_rssi_weight;
} wmi_roam_scan_rssi_change_threshold_fixed_param;

#define WMI_ROAM_SCAN_CHAN_LIST_TYPE_NONE 0x1
#define WMI_ROAM_SCAN_CHAN_LIST_TYPE_STATIC 0x2
#define WMI_ROAM_SCAN_CHAN_LIST_TYPE_DYNAMIC 0x3
/**
 * TLV for roaming channel list
 */
typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_roam_chan_list_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** WMI_CHAN_LIST_TAG */
    A_UINT32 chan_list_type;
    /** # if channels to scan */
    A_UINT32 num_chan;
/**
 * TLV (tag length value ) parameters follow the wmi_roam_chan_list
 * structure. The TLV's are:
 *     A_UINT32 channel_list[];
 **/
} wmi_roam_chan_list_fixed_param;

/** Authentication modes */
enum {
    WMI_AUTH_NONE   , /* no upper level auth */
    WMI_AUTH_OPEN   , /* open */
    WMI_AUTH_SHARED , /* shared-key */
    WMI_AUTH_8021X  , /* 802.1x */
    WMI_AUTH_AUTO   , /* Auto */
    WMI_AUTH_WPA    , /* WPA */
    WMI_AUTH_RSNA   , /* WPA2/RSNA */
    WMI_AUTH_CCKM   , /* CCK */
    WMI_AUTH_WAPI   ,/* WAPI */
    WMI_AUTH_AUTO_PSK,
    WMI_AUTH_WPA_PSK,
    WMI_AUTH_RSNA_PSK,
    WMI_AUTH_WAPI_PSK,
    WMI_AUTH_FT_RSNA, /* 11r FT */
    WMI_AUTH_FT_RSNA_PSK,
    WMI_AUTH_RSNA_PSK_SHA256,
    WMI_AUTH_RSNA_8021X_SHA256,
};

typedef struct {
    /** authentication mode (defined above)  */
    A_UINT32               rsn_authmode;
    /** unicast cipher set */
    A_UINT32               rsn_ucastcipherset;
    /** mcast/group cipher set */
    A_UINT32               rsn_mcastcipherset;
    /** mcast/group management frames cipher set */
    A_UINT32               rsn_mcastmgmtcipherset;
} wmi_rsn_params;

/** looking for a wps enabled AP */
#define WMI_AP_PROFILE_FLAG_WPS    0x1
/** looking for a secure AP  */
#define WMI_AP_PROFILE_FLAG_CRYPTO 0x2

/** To match an open AP, the rs_authmode should be set to WMI_AUTH_NONE
 *  and WMI_AP_PROFILE_FLAG_CRYPTO should be clear.
 *  To match a WEP enabled AP, the rs_authmode should be set to WMI_AUTH_NONE
 *  and WMI_AP_PROFILE_FLAG_CRYPTO should be set .
 */

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_ap_profile */
    /** flags as defined above */
    A_UINT32  flags;
	/**
     * rssi thresold value: the value of the the candidate AP should
	 * higher by this threshold than the rssi of the currrently associated AP.
	 */
	A_UINT32 rssi_threshold;
	/**
	 * ssid vlaue to be matched.
	 */
    wmi_ssid ssid;

    /**
	 * security params to be matched.
	 */
    /** authentication mode (defined above)  */
    A_UINT32               rsn_authmode;
    /** unicast cipher set */
    A_UINT32               rsn_ucastcipherset;
    /** mcast/group cipher set */
    A_UINT32               rsn_mcastcipherset;
    /** mcast/group management frames cipher set */
    A_UINT32               rsn_mcastmgmtcipherset;
} wmi_ap_profile;

/** Beacon filter wmi command info */

#define BCN_FLT_MAX_SUPPORTED_IES    256
#define BCN_FLT_MAX_ELEMS_IE_LIST    BCN_FLT_MAX_SUPPORTED_IES/32

typedef struct bss_bcn_stats {
    A_UINT32    vdev_id;
    A_UINT32    bss_bcnsdropped;
    A_UINT32    bss_bcnsdelivered;
}wmi_bss_bcn_stats_t;

typedef struct bcn_filter_stats {
    A_UINT32    bcns_dropped;
    A_UINT32    bcns_delivered;
    A_UINT32    activefilters;
    wmi_bss_bcn_stats_t bss_stats;
}wmi_bcnfilter_stats_t;

typedef struct wmi_add_bcn_filter_cmd {
    A_UINT32    tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_add_bcn_filter_cmd_fixed_param */
    A_UINT32    vdev_id;
    /*
     * Following this structure is the TLV:
     *    A_UINT32   ie_map[BCN_FLT_MAX_ELEMS_IE_LIST];
    */
} wmi_add_bcn_filter_cmd_fixed_param;

typedef struct wmi_rmv_bcn_filter_cmd {
    A_UINT32    tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_rmv_bcn_filter_cmd_fixed_param */
    A_UINT32    vdev_id;
}wmi_rmv_bcn_filter_cmd_fixed_param;

#define WMI_BCN_SEND_DTIM_ZERO         1
#define WMI_BCN_SEND_DTIM_BITCTL_SET   2
typedef struct wmi_bcn_send_from_host {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_bcn_send_from_host_cmd_fixed_param  */
    A_UINT32 vdev_id;
    A_UINT32 data_len;
    A_UINT32 frag_ptr; /* Physical address of the frame */
    A_UINT32 frame_ctrl; /* farme ctrl to setup PPDU desc */
    A_UINT32 dtim_flag;   /* to control CABQ traffic */
}wmi_bcn_send_from_host_cmd_fixed_param;

/* cmd to support bcn snd for all vaps at once */
typedef struct wmi_pdev_send_bcn {
    A_UINT32                       num_vdevs;
    wmi_bcn_send_from_host_cmd_fixed_param   bcn_cmd[1];
} wmi_pdev_send_bcn_cmd_t;

 /*
  * WMI_ROAM_AP_PROFILE:  AP profile of connected AP for roaming.
  */
typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_roam_ap_profile_fixed_param */
    /** id of AP criteria */
	A_UINT32 id;

    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;

    /*
         * Following this structure is the TLV:
         *     wmi_ap_profile ap_profile; //AP profile info
        */
} wmi_roam_ap_profile_fixed_param;;

/**
 * WMI_OFL_SCAN_ADD_AP_PROFILE: add an AP profile.
 */
typedef struct {
    /** id of AP criteria */
	A_UINT32 id;

    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;

    /** AP profile info */
    wmi_ap_profile ap_profile;

} wmi_ofl_scan_add_ap_profile;

/**
 * WMI_OFL_SCAN_REMOVE_AP_CRITERIA: remove an ap profile.
 */
typedef struct {
    /** id of AP criteria */
	A_UINT32 id;
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
} wmi_ofl_scan_remove_ap_profile;

/**
 * WMI_OFL_SCAN_PERIOD: period in msec for offload scan.
 *  0 will disable ofload scan and a very low value will perform a continous
 *  scan.
 */
typedef struct {
	/** offload scan period value, used for scans used when not connected */
	A_UINT32 ofl_scan_period;
} wmi_ofl_scan_period;



/** WMI_ROAM_EVENT: roam event triggering the host roam logic.
 * generated when ever a better AP is found in the recent roam scan (or)
 * when beacon miss is detected (or) when a DEAUTH/DISASSOC is received
 * from the current AP.
 */
typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_roam_event_fixed_param  */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** reason for roam event */
    A_UINT32 reason;
    /** associated AP's rssi calculated by FW when reason code is WMI_ROAM_REASON_LOW_RSSI*/
    A_UINT32 rssi;

} wmi_roam_event_fixed_param;

#define WMI_ROAM_REASON_BETTER_AP 0x1 /** found a better AP */
#define WMI_ROAM_REASON_BMISS     0x2 /** beacon miss detected */
#define WMI_ROAM_REASON_DEAUTH    0x2 /** deauth/disassoc received */
#define WMI_ROAM_REASON_LOW_RSSI  0x3 /** connected AP's low rssi condition detected */
#define WMI_ROAM_REASON_SUITABLE_AP 0x4 /** found another AP that matches
                                          SSID and Security profile in
                                          WMI_ROAM_AP_PROFILE, found during scan
                                          triggered upon FINAL_BMISS **/

/** WMI_PROFILE_MATCH_EVENT: offload scan
 * generated when ever atleast one of the matching profiles is found
 * in recent NLO scan. no data is carried with the event.
 */

/** P2P specific commands */

/**
 * WMI_P2P_DEV_SET_DEVICE_INFO : p2p device info, which will be used by
 * FW to generate P2P IE tobe carried in probe response frames.
 * FW will respond to probe requests while in listen state.
 */
typedef struct {
    /* number of secondary device types,supported */
    A_UINT32 num_secondary_dev_types;
    /**
     * followed by 8 bytes of primary device id and
     * num_secondary_dev_types * 8 bytes of secondary device
     * id.
     */
} wmi_p2p_dev_set_device_info;

/** WMI_P2P_DEV_SET_DISCOVERABILITY: enable/disable discoverability
 *  state. if enabled, an active STA/AP will respond to P2P probe requests on
 *  the operating channel of the VDEV.
 */

typedef struct {
    /* 1:enable disoverability, 0:disable discoverability */
    A_UINT32 enable_discoverability;
}  wmi_p2p_set_discoverability;

/** WMI_P2P_GO_SET_BEACON_IE: P2P IE to be added to
 *  beacons generated by FW. used in FW beacon mode.
 *  the FW will add this IE to beacon in addition to the beacon
 *  template set by WMI_BCN_TMPL_CMDID command.
 */
typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_p2p_go_set_beacon_ie_fixed_param  */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /* ie length */
    A_UINT32 ie_buf_len;
   /* Following this structure is the TLV byte stream of ie data of length ie_buf_len:
       *     A_UINT8 ie_data[];    // length in byte given by field num_data.
       */

}  wmi_p2p_go_set_beacon_ie_fixed_param;

/** WMI_P2P_GO_PROBE_RESP_IE: P2P IE to be added to
 *  probe response generated by FW. used in FW beacon mode.
 *  the FW will add this IE to probe response in addition to the probe response
 *  template set by WMI_PRB_TMPL_CMDID command.
 */
typedef struct {
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /* ie length */
    A_UINT32 ie_buf_len;
    /*followed by  byte stream of ie data of length ie_buf_len */
}  wmi_p2p_go_set_probe_resp_ie;

/** WMI_P2P_SET_VENDOR_IE_DATA_CMDID: Vendor specific P2P IE data, which will
 *  be used by the FW to parse the P2P NoA attribute in beacons, probe resposes
 *  and action frames received by the P2P Client.
 */
typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_p2p_set_vendor_ie_data_cmd_fixed_param */
    /** OS specific P2P IE OUI (3 bytes) + OUI type (1 byte)  */
    A_UINT32 p2p_ie_oui_type;
    /** OS specific NoA Attribute ID */
    A_UINT32 p2p_noa_attribute;
}  wmi_p2p_set_vendor_ie_data_cmd_fixed_param;

/*----P2P disc offload definition  ----*/

typedef struct {
    A_UINT32        pattern_type;
    /**
     * TLV (tag length value )  paramerters follow the pattern structure.
     * TLV can contain bssid list, ssid list and
     * ie. the TLV tags are defined above;
     */
}wmi_p2p_disc_offload_pattern_cmd;

typedef struct {
    /* unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
	/* mgmt type of the ie*/
	A_UINT32 mgmt_type;
    /* ie length */
    A_UINT32 ie_buf_len;
    /*followed by  byte stream of ie data of length ie_buf_len */
}wmi_p2p_disc_offload_appie_cmd;

typedef struct {
    /* enable/disable p2p find offload*/
    A_UINT32      enable;
	/* unique id identifying the VDEV, generated by the caller */
	A_UINT32      vdev_id;
	/* p2p find type */
	A_UINT32      disc_type;
	/* p2p find perodic */
	A_UINT32      perodic;
	/* p2p find listen channel */
	A_UINT32      listen_channel;
	/* p2p find full channel number */
    A_UINT32      num_scan_chans;
	/**
     * TLV (tag length value )  paramerters follow the pattern structure.
     * TLV  contain channel list
     */
}wmi_p2p_disc_offload_config_cmd;

/*----P2P OppPS definition  ----*/
typedef struct {
	/* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_p2p_set_oppps_cmd_fixed_param  */
    A_UINT32 tlv_header;
    /* unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /* OppPS attributes */
	/** Bit  0: Indicate enable/disable of OppPS
	 *	Bits 7-1:	Ctwindow in TUs
	 *	Bits 31-8:  Reserved
	 */
	A_UINT32	oppps_attr;
} wmi_p2p_set_oppps_cmd_fixed_param;

#define	WMI_UNIFIED_OPPPS_ATTR_ENALBED		0x1
#define	WMI_UNIFIED_OPPPS_ATTR_ENALBED_S	0

#define WMI_UNIFIED_OPPPS_ATTR_IS_ENABLED(hdr)                   \
			WMI_F_MS((hdr)->oppps_attr, WMI_UNIFIED_OPPPS_ATTR_ENALBED)

#define WMI_UNIFIED_OPPPS_ATTR_ENABLED_SET(hdr)                 \
            WMI_F_RMW((hdr)->oppps_attr, 0x1,                \
            WMI_UNIFIED_OPPPS_ATTR_ENALBED);

#define	WMI_UNIFIED_OPPPS_ATTR_CTWIN		0xfe
#define	WMI_UNIFIED_OPPPS_ATTR_CTWIN_S		1

#define WMI_UNIFIED_OPPPS_ATTR_CTWIN_GET(hdr)                        \
            WMI_F_MS((hdr)->oppps_attr, WMI_UNIFIED_OPPPS_ATTR_CTWIN)

#define WMI_UNIFIED_OPPPS_ATTR_CTWIN_SET(hdr, v)                \
            WMI_F_RMW((hdr)->oppps_attr, (v) & 0x7f,            \
            WMI_UNIFIED_OPPPS_ATTR_CTWIN);

typedef struct {
    A_UINT32 time32;     //upper 32 bits of time stamp
    A_UINT32 time0;      //lower 32 bits of time stamp
} A_TIME64;

typedef enum wmi_peer_sta_kickout_reason {
    WMI_PEER_STA_KICKOUT_REASON_UNSPECIFIED = 0,        /* default value to preserve legacy behavior */
    WMI_PEER_STA_KICKOUT_REASON_XRETRY = 1,
    WMI_PEER_STA_KICKOUT_REASON_INACTIVITY = 2,
    WMI_PEER_STA_KICKOUT_REASON_IBSS_DISCONNECT = 3,
    WMI_PEER_STA_KICKOUT_REASON_TDLS_DISCONNECT = 4,    /* TDLS peer has disappeared. All tx is failing */
} PEER_KICKOUT_REASON;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_sta_kickout_event_fixed_param  */
    /** peer mac address */
    wmi_mac_addr peer_macaddr;
    /** Reason code, defined as above */
    A_UINT32 reason;
} wmi_peer_sta_kickout_event_fixed_param;

#define WMI_WLAN_PROFILE_MAX_HIST     3
#define WMI_WLAN_PROFILE_MAX_BIN_CNT 32

typedef struct _wmi_wlan_profile_t {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_wlan_profile_t */
    A_UINT32 id;
    A_UINT32 cnt;
    A_UINT32 tot;
    A_UINT32 min;
    A_UINT32 max;
    A_UINT32 hist_intvl;
    A_UINT32 hist[WMI_WLAN_PROFILE_MAX_HIST];
} wmi_wlan_profile_t;

typedef struct _wmi_wlan_profile_ctx_t {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_wlan_profile_ctx_t */
    A_UINT32 tot; /* time in us */
    A_UINT32 tx_msdu_cnt;
    A_UINT32 tx_mpdu_cnt;
    A_UINT32 tx_ppdu_cnt;
    A_UINT32 rx_msdu_cnt;
    A_UINT32 rx_mpdu_cnt;
    A_UINT32 bin_count;
} wmi_wlan_profile_ctx_t;


typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_wlan_profile_trigger_cmd_fixed_param */
    A_UINT32 enable;
} wmi_wlan_profile_trigger_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_wlan_profile_get_prof_data_cmd_fixed_param */
    A_UINT32 value;
} wmi_wlan_profile_get_prof_data_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_wlan_profile_set_hist_intvl_cmd_fixed_param */
    A_UINT32 profile_id;
    A_UINT32 value;
} wmi_wlan_profile_set_hist_intvl_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_wlan_profile_enable_profile_id_cmd_fixed_param */
    A_UINT32 profile_id;
    A_UINT32 enable;
} wmi_wlan_profile_enable_profile_id_cmd_fixed_param;

/*Wifi header is upto 26, LLC is 8, with 14 byte duplicate in 802.3 header, that's 26+8-14=20.
146-128=18. So this means it is converted to non-QoS header. Riva FW take care of the QOS/non-QOS
when comparing wifi header.*/
/* NOTE: WOW_DEFAULT_BITMAP_PATTERN_SIZE(_DWORD) and WOW_DEFAULT_BITMASK_SIZE(_DWORD) can't be changed without breaking the compatibility */
#define WOW_DEFAULT_BITMAP_PATTERN_SIZE      146
#define WOW_DEFAULT_BITMAP_PATTERN_SIZE_DWORD 37 //Convert WOW_DEFAULT_EVT_BUF_SIZE into Int32 size
#define WOW_DEFAULT_BITMASK_SIZE             146
#define WOW_DEFAULT_BITMASK_SIZE_DWORD        37
#define WOW_MAX_BITMAP_FILTERS               32
#define WOW_DEFAULT_MAGIG_PATTERN_MATCH_CNT  16
#define WOW_ACER_EXTEND_PATTERN_MATCH_CNT    16
#define WOW_ACER_SHORT_PATTERN_MATCH_CNT     8
#define WOW_DEFAULT_EVT_BUF_SIZE             148  /* Maximum 148 bytes of the data is copied starting from header incase if the match is found.
                                                                                    The 148 comes from (128 - 14 )  payload size  + 8bytes LLC + 26bytes MAC header*/
#define WOW_DEFAULT_ACER_IOAC_PATTERN_SIZE  6
#define WOW_DEFAULT_ACER_IOAC_PATTERN_SIZE_DWORD 2
#define WOW_DEFAULT_ACER_IOAC_RANDOM_SIZE  6
#define WOW_DEFAULT_ACER_IOAC_RANDOM_SIZE_DWORD 2
#define WOW_DEFAULT_ACER_IOAC_KEEP_ALIVE_PKT_SIZE   120
#define WOW_DEFAULT_ACER_IOAC_KEEP_ALIVE_PKT_SIZE_DWORD 30

typedef enum pattern_type_e {
    WOW_PATTERN_MIN = 0,
    WOW_BITMAP_PATTERN = WOW_PATTERN_MIN,
    WOW_IPV4_SYNC_PATTERN,
    WOW_IPV6_SYNC_PATTERN,
    WOW_WILD_CARD_PATTERN,
    WOW_TIMER_PATTERN,
    WOW_MAGIC_PATTERN,
    WOW_IPV6_RA_PATTERN,
    WOW_ACER_IOAC_PKT_PATTERN,
    WOW_ACER_IOAC_TMR_PATTERN,
    WOW_PATTERN_MAX
}WOW_PATTERN_TYPE;

typedef enum event_type_e {
    WOW_BMISS_EVENT = 0,
    WOW_BETTER_AP_EVENT,
    WOW_DEAUTH_RECVD_EVENT,
    WOW_MAGIC_PKT_RECVD_EVENT,
    WOW_GTK_ERR_EVENT,
    WOW_FOURWAY_HSHAKE_EVENT,
    WOW_EAPOL_RECVD_EVENT,
    WOW_NLO_DETECTED_EVENT,
    WOW_DISASSOC_RECVD_EVENT,
    WOW_PATTERN_MATCH_EVENT,
    WOW_CSA_IE_EVENT,
    WOW_PROBE_REQ_WPS_IE_EVENT,
    WOW_AUTH_REQ_EVENT,
    WOW_ASSOC_REQ_EVENT,
    WOW_HTT_EVENT,
    WOW_RA_MATCH_EVENT,
    WOW_HOST_AUTO_SHUTDOWN_EVENT,
    WOW_ACER_IOAC_MAGIC_EVENT,
    WOW_ACER_IOAC_SHORT_EVENT,
    WOW_ACER_IOAC_EXTEND_EVENT,
    WOW_ACER_IOAC_TIMER_EVENT,
}WOW_WAKE_EVENT_TYPE;

typedef enum wake_reason_e {
    WOW_REASON_UNSPECIFIED =-1,
    WOW_REASON_NLOD = 0,
    WOW_REASON_AP_ASSOC_LOST,
    WOW_REASON_LOW_RSSI,
    WOW_REASON_DEAUTH_RECVD,
    WOW_REASON_DISASSOC_RECVD,
    WOW_REASON_GTK_HS_ERR,
    WOW_REASON_EAP_REQ,
    WOW_REASON_FOURWAY_HS_RECV,
    WOW_REASON_TIMER_INTR_RECV,
    WOW_REASON_PATTERN_MATCH_FOUND,
    WOW_REASON_RECV_MAGIC_PATTERN,
    WOW_REASON_P2P_DISC,
    WOW_REASON_WLAN_HB,
    WOW_REASON_CSA_EVENT,
    WOW_REASON_PROBE_REQ_WPS_IE_RECV,
    WOW_REASON_AUTH_REQ_RECV,
    WOW_REASON_ASSOC_REQ_RECV,
    WOW_REASON_HTT_EVENT,
    WOW_REASON_RA_MATCH,
    WOW_REASON_HOST_AUTO_SHUTDOWN,
    WOW_REASON_ACER_IOAC_MAGIC_EVENT,
    WOW_REASON_ACER_IOAC_SHORT_EVENT,
    WOW_REASON_ACER_IOAC_EXTEND_EVENT,
    WOW_REASON_ACER_IOAC_TIMER_EVENT,
    WOW_REASON_DEBUG_TEST = 0xFF,
}WOW_WAKE_REASON_TYPE;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_wow_enable_cmd_fixed_param  */
    A_UINT32    enable;
} wmi_wow_enable_cmd_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_wow_hostwakeup_from_sleep_cmd_fixed_param  */
    /** Reserved for future use */
    A_UINT32    reserved0;
} wmi_wow_hostwakeup_from_sleep_cmd_fixed_param;

typedef struct bitmap_pattern_s {
    A_UINT32     tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WOW_BITMAP_PATTERN_T */
    A_UINT32     patternbuf[WOW_DEFAULT_BITMAP_PATTERN_SIZE_DWORD];
    A_UINT32     bitmaskbuf[WOW_DEFAULT_BITMASK_SIZE_DWORD];
    A_UINT32     pattern_offset;
    A_UINT32     pattern_len;
    A_UINT32     bitmask_len;
    A_UINT32     pattern_id;  /* must be less than max_bitmap_filters */
}WOW_BITMAP_PATTERN_T;

typedef struct ipv4_sync_s {
    A_UINT32     tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WOW_IPV4_SYNC_PATTERN_T */
    A_UINT32     ipv4_src_addr;
    A_UINT32     ipv4_dst_addr;
    A_UINT32     tcp_src_prt;
    A_UINT32     tcp_dst_prt;
}WOW_IPV4_SYNC_PATTERN_T;

typedef struct ipv6_sync_s {
    A_UINT32     tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WOW_IPV6_SYNC_PATTERN_T */
    A_UINT32     ipv6_src_addr[4];
    A_UINT32     ipv6_dst_addr[4];
    A_UINT32     tcp_src_prt;
    A_UINT32     tcp_dst_prt;
}WOW_IPV6_SYNC_PATTERN_T;

typedef struct WOW_MAGIC_PATTERN_CMD
{
    A_UINT32     tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WOW_MAGIC_PATTERN_CMD */
	wmi_mac_addr macaddr;
}WOW_MAGIC_PATTERN_CMD;

typedef enum wow_ioac_pattern_type {
    WOW_IOAC_MAGIC_PATTERN = 1,
    WOW_IOAC_SHORT_PATTERN,
    WOW_IOAC_EXTEND_PATTERN,
} WOW_IOAC_PATTERN_TYPE;

typedef struct acer_ioac_pkt_pattern_s {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WOW_ACER_IOAC_PKT_PATTERN_T */
    A_UINT32 pattern_type;
    A_UINT32 pattern[WOW_DEFAULT_ACER_IOAC_PATTERN_SIZE_DWORD];
    A_UINT32 random[WOW_DEFAULT_ACER_IOAC_RANDOM_SIZE_DWORD];
    A_UINT32 pattern_len;
    A_UINT32 random_len;
} WOW_ACER_IOAC_PKT_PATTERN_T;

typedef struct acer_ioac_tmr_pattern_s {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WOW_ACER_IOAC_TMR_PATTERN_T */
    A_UINT32 wake_in_s;
    A_UINT32 vdev_id;
} WOW_ACER_IOAC_TMR_PATTERN_T;

typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_WOW_IOAC_ADD_KEEPALIVE_CMD_fixed_param */
    A_UINT32 nID;
} WMI_WOW_IOAC_ADD_KEEPALIVE_CMD_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_WOW_IOAC_DEL_KEEPALIVE_CMD_fixed_param */
    A_UINT32 nID;
} WMI_WOW_IOAC_DEL_KEEPALIVE_CMD_fixed_param;

typedef struct acer_ioac_keepalive_s {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WOW_IOAC_KEEPALIVE_T */
    A_UINT32 keepalive_pkt_buf[WOW_DEFAULT_ACER_IOAC_KEEP_ALIVE_PKT_SIZE_DWORD];
    A_UINT32 keepalive_pkt_len;
    A_UINT32 period_in_ms;
    A_UINT32 vdev_id;
} WOW_IOAC_KEEPALIVE_T;

typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_WOW_ACER_IOAC_ADD_PATTERN_CMD_fixed_param */
    A_UINT32 vdev_id;
    A_UINT32 pattern_type;
/*
 * Following this struct are these TLVs. Note that they are all array of structures
 * but can have at most one element. Which TLV is empty or has one element depends
 * on the field pattern_type. This is to emulate an union.
 *     WOW_ACER_IOAC_PKT_PATTERN_T pattern_info_acer_pkt[];
 *     WOW_ACER_IOAC_TMR_PATTERN_T pattern_info_acer_tmr[];
 */
} WMI_WOW_ACER_IOAC_ADD_PATTERN_CMD_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_WOW_ACER_IOAC_DEL_PATTERN_CMD_fixed_param */
    A_UINT32 vdev_id;
    A_UINT32 pattern_type;
} WMI_WOW_ACER_IOAC_DEL_PATTERN_CMD_fixed_param;

typedef struct {
    A_UINT32        tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_WOW_ADD_PATTERN_CMD_fixed_param */
    A_UINT32        vdev_id;
    A_UINT32        pattern_id;
    A_UINT32        pattern_type;
    /*
     * Following this struct are these TLVs. Note that they are all array of structures
     * but can have at most one element. Which TLV is empty or has one element depends
     * on the field pattern_type. This is to emulate an union.
     *     WOW_BITMAP_PATTERN_T       pattern_info_bitmap[];
     *     WOW_IPV4_SYNC_PATTERN_T    pattern_info_ipv4[];
     *     WOW_IPV6_SYNC_PATTERN_T    pattern_info_ipv6[];
     *     WOW_MAGIC_PATTERN_CMD      pattern_info_magic_pattern[];
     *     A_UINT32                   pattern_info_timeout[];
     *     A_UINT32                   ra_ratelimit_interval;
     */
}WMI_WOW_ADD_PATTERN_CMD_fixed_param;

typedef struct {
    A_UINT32        tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_WOW_DEL_PATTERN_CMD_fixed_param */
    A_UINT32        vdev_id;
    A_UINT32        pattern_id;
    A_UINT32        pattern_type;
}WMI_WOW_DEL_PATTERN_CMD_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_WOW_ADD_DEL_EVT_CMD_fixed_param  */
    A_UINT32    vdev_id;
    A_UINT32    is_add;
    A_UINT32    event_bitmap;
}WMI_WOW_ADD_DEL_EVT_CMD_fixed_param;

typedef struct  wow_event_info_s {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_WOW_EVENT_INFO_fixed_param  */
    A_UINT32    vdev_id;
    A_UINT32    flag;                              /*This is current reserved.*/
    A_INT32     wake_reason;
    A_UINT32    data_len;
}WOW_EVENT_INFO_fixed_param;

typedef enum {
    WOW_EVENT_INFO_TYPE_PACKET = 0x0001,
    WOW_EVENT_INFO_TYPE_BITMAP,
    WOW_EVENT_INFO_TYPE_GTKIGTK,
}WOW_EVENT_INFO_TYPE;

typedef struct  wow_event_info_section_s {
    A_UINT32    data_type;
    A_UINT32    data_len;
}WOW_EVENT_INFO_SECTION;

typedef struct  wow_event_info_section_packet_s {
    A_UINT8     packet[WOW_DEFAULT_EVT_BUF_SIZE];
}WOW_EVENT_INFO_SECTION_PACKET;

typedef struct  wow_event_info_section_bitmap_s {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_WOW_EVENT_INFO_SECTION_BITMAP  */
    A_UINT32    flag;                              /*This is current reserved.*/
    A_UINT32    value;                         /*This could be the pattern id for bitmap pattern.*/
    A_UINT32    org_len;                      /*The length of the orginal packet.*/
}WOW_EVENT_INFO_SECTION_BITMAP;

#define WMI_RXERR_CRC               0x01    /* CRC error on frame */
#define WMI_RXERR_DECRYPT           0x08    /* non-Michael decrypt error */
#define WMI_RXERR_MIC               0x10    /* Michael MIC decrypt error */
#define WMI_RXERR_KEY_CACHE_MISS    0x20    /* No/incorrect key matter in h/w */

typedef enum {
    PKT_PWR_SAVE_PAID_MATCH =         0x0001,
    PKT_PWR_SAVE_GID_MATCH =          0x0002,
    PKT_PWR_SAVE_EARLY_TIM_CLEAR =    0x0004,
    PKT_PWR_SAVE_EARLY_DTIM_CLEAR =   0x0008,
    PKT_PWR_SAVE_EOF_PAD_DELIM =      0x0010,
    PKT_PWR_SAVE_MACADDR_MISMATCH =   0x0020,
    PKT_PWR_SAVE_DELIM_CRC_FAIL =     0x0040,
    PKT_PWR_SAVE_GID_NSTS_ZERO =      0x0080,
    PKT_PWR_SAVE_RSSI_CHECK =         0x0100,
    PKT_PWR_SAVE_5G_EBT =             0x0200,
    WMI_PKT_PWR_SAVE_MAX =            0x0400,
} WMI_PKT_PWR_SAVE_TYPE;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_ftm_intg_cmd_fixed_param */
    A_UINT32 num_data;       /** length in byte of data[]. */
    /* This structure is used to send Factory Test Mode [FTM] command
     * from host to firmware for integrated chips which are binary blobs.
     * Following this structure is the TLV:
     *     A_UINT8 data[];    // length in byte given by field num_data.
     */
}wmi_ftm_intg_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_ftm_intg_event_fixed_param */
    A_UINT32 num_data;       /** length in byte of data[]. */
    /* This structure is used to receive Factory Test Mode [FTM] event
     * from firmware to host for integrated chips which are binary blobs.
     * Following this structure is the TLV:
     *     A_UINT8 data[];    // length in byte given by field num_data.
     */
}wmi_ftm_intg_event_fixed_param;

#define WMI_MAX_NS_OFFLOADS           2
#define WMI_MAX_ARP_OFFLOADS          2

#define WMI_ARPOFF_FLAGS_VALID              (1 << 0)    /* the tuple entry is valid */
#define WMI_ARPOFF_FLAGS_MAC_VALID          (1 << 1)    /* the target mac address is valid */
#define WMI_ARPOFF_FLAGS_REMOTE_IP_VALID    (1 << 2)    /* remote IP field is valid */

typedef struct _WMI_IPV6_ADDR {
    A_UINT8    address[16];    /* IPV6 in Network Byte Order */
} WMI_IPV6_ADDR;


typedef struct {
    A_UINT32          tlv_header;              /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_ARP_OFFLOAD_TUPLE */
    A_UINT32           flags;                  /* flags */
    A_UINT8           target_ipaddr[4];        /* IPV4 addresses of the local node*/
    A_UINT8           remote_ipaddr[4];        /* source address of the remote node requesting the ARP (qualifier) */
    wmi_mac_addr      target_mac;              /* mac address for this tuple, if not valid, the local MAC is used */
} WMI_ARP_OFFLOAD_TUPLE;

#define WMI_NSOFF_FLAGS_VALID           (1 << 0)    /* the tuple entry is valid */
#define WMI_NSOFF_FLAGS_MAC_VALID       (1 << 1)    /* the target mac address is valid */
#define WMI_NSOFF_FLAGS_REMOTE_IP_VALID (1 << 2)    /* remote IP field is valid */

#define WMI_NSOFF_MAX_TARGET_IPS    2

typedef struct {
    A_UINT32          tlv_header;                /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_NS_OFFLOAD_TUPLE */
    A_UINT32           flags;                     /* flags */
    /* NOTE: This size of array target_ipaddr[] cannot be changed without breaking WMI compatibility. */
    WMI_IPV6_ADDR     target_ipaddr[WMI_NSOFF_MAX_TARGET_IPS]; /* IPV6 target addresses of the local node  */
    WMI_IPV6_ADDR     solicitation_ipaddr;       /* multi-cast source IP addresses for receiving solicitations */
    WMI_IPV6_ADDR     remote_ipaddr;             /* address of remote node requesting the solicitation (qualifier) */
    wmi_mac_addr      target_mac;                /* mac address for this tuple, if not valid, the local MAC is used */
} WMI_NS_OFFLOAD_TUPLE;

typedef struct {
    A_UINT32                tlv_header;      /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_SET_ARP_NS_OFFLOAD_CMD_fixed_param */
    A_UINT32                flags;
    A_UINT32                vdev_id;
    /* Following this structure are the TLVs:
         *     WMI_NS_OFFLOAD_TUPLE    ns_tuples[WMI_MAX_NS_OFFLOADS];
         *     WMI_ARP_OFFLOAD_TUPLE   arp_tuples[WMI_MAX_ARP_OFFLOADS];
         */
} WMI_SET_ARP_NS_OFFLOAD_CMD_fixed_param;

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 pattern_id;
    A_UINT32 timeout;
    A_UINT32 length;
    /* Following this would be the pattern
       A_UINT8 pattern[] of length specifed by length
       field in the structure. */
} WMI_ADD_PROACTIVE_ARP_RSP_PATTERN_CMD_fixed_param;

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 pattern_id;
} WMI_DEL_PROACTIVE_ARP_RSP_PATTERN_CMD_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_tid_addba_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** Tid number */
    A_UINT32 tid;
    /** Initiator (1) or Responder (0) for this aggregation */
    A_UINT32 initiator;
    /** size of the negotiated window */
    A_UINT32 window_size;
    /** starting sequence number (only valid for initiator) */
    A_UINT32 ssn;
    /** timeout field represents the time to wait for Block Ack in
    *   initiator case and the time to wait for BAR in responder
    *   case. 0 represents no timeout. */
    A_UINT32 timeout;
    /* BA policy: immediate ACK (0) or delayed ACK (1) */
    A_UINT32 policy;
} wmi_peer_tid_addba_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_peer_tid_delba_cmd */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** Tid number */
    A_UINT32 tid;
    /** Initiator (1) or Responder (0) for this aggregation */
    A_UINT32 initiator;
} wmi_peer_tid_delba_cmd_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_tx_addba_complete_event_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** Tid number */
    A_UINT32 tid;
    /** Event status */
    A_UINT32 status;
} wmi_tx_addba_complete_event_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_tx_delba_complete_event_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
    /** Tid number */
    A_UINT32 tid;
    /** Event status */
    A_UINT32 status;
} wmi_tx_delba_complete_event_fixed_param;
/*
 * Structure to request sequence numbers for a given
 * peer station on different TIDs. The TIDs are
 * indicated in the tidBitMap, tid 0 would
 * be represented by LSB bit 0. tid 1 would be
 * represented by LSB bit 1 etc.
 * The target will retrieve the current sequence
 * numbers for the peer on all the TIDs requested
 * and send back a response in a WMI event.
 */
typedef struct
{
    A_UINT32    tlv_header;  /* TLV tag and len; tag equals
                                WMITLV_TAG_STRUC_wmi_ba_req_ssn_cmd_sub_struct_param */
    wmi_mac_addr peer_macaddr;
    A_UINT32 tidBitmap;
} wmi_ba_req_ssn;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals
     WMITLV_TAG_STRUC_wmi_ba_req_ssn_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** Number of requested SSN In the TLV wmi_ba_req_ssn[] */
    A_UINT32 num_ba_req_ssn;
/* Following this struc are the TLV's:
 *     wmi_ba_req_ssn ba_req_ssn_list; All peer and tidBitMap for which the ssn is requested
 */
} wmi_ba_req_ssn_cmd_fixed_param;

/*
 * Max transmit categories
 *
 * Note: In future if we need to increase WMI_MAX_TC definition
 * It would break the compatibility for WMI_BA_RSP_SSN_EVENTID.
 */
#define WMI_MAX_TC  8

/*
 * Structure to send response sequence numbers
 * for a give peer and tidmap.
 */
typedef struct
{
    A_UINT32    tlv_header;  /* TLV tag and len; tag equals
                                WMITLV_TAG_STRUC_wmi_ba_req_ssn_event_sub_struct_param */
    wmi_mac_addr peer_macaddr;
    /* A boolean to indicate if ssn is present */
    A_UINT32 ssn_present_for_tid[WMI_MAX_TC];
    /* The ssn from target, valid only if
     * ssn_present_for_tid[tidn] equals 1
     */
    A_UINT32 ssn_for_tid[WMI_MAX_TC];
} wmi_ba_event_ssn;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals
     WMITLV_TAG_STRUC_wmi_ba_rsp_ssn_event_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** Event status, success or failure of the overall operation */
    A_UINT32 status;
    /** Number of requested SSN In the TLV wmi_ba_req_ssn[] */
    A_UINT32 num_ba_event_ssn;
/* Following this struc are the TLV's:
 *     wmi_ba_event_ssn ba_event_ssn_list; All peer and tidBitMap for which the ssn is requested
 */
} wmi_ba_rsp_ssn_event_fixed_param;


enum wmi_aggr_state_req_type {
    WMI_DISABLE_AGGREGATION,
    WMI_ENABLE_AGGREGATION
};

/*
 * This event is generated by the COEX module
 * when esco call is begins the coex module in fw genrated this event to host to
 * disable the RX aggregation and after completion of the esco call fw will indicate to
 * enable back the Rx aggregation .
*/

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_aggr_state_trig_event_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
   /** req_type  contains values from enum
     *  wmi_aggr_state_req_type; 0 (disable) 1(enable) */
    A_UINT32  req_type;
}wmi_aggr_state_trig_event_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_install_key_complete_event_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /** MAC address used for installing   */
    wmi_mac_addr peer_macaddr;
    /** key index */
    A_UINT32 key_ix;
    /** key flags */
    A_UINT32 key_flags;
    /** Event status */
    A_UINT32 status;
} wmi_vdev_install_key_complete_event_fixed_param;

typedef enum _WMI_NLO_AUTH_ALGORITHM {
    WMI_NLO_AUTH_ALGO_80211_OPEN        = 1,
    WMI_NLO_AUTH_ALGO_80211_SHARED_KEY  = 2,
    WMI_NLO_AUTH_ALGO_WPA               = 3,
    WMI_NLO_AUTH_ALGO_WPA_PSK           = 4,
    WMI_NLO_AUTH_ALGO_WPA_NONE          = 5,
    WMI_NLO_AUTH_ALGO_RSNA              = 6,
    WMI_NLO_AUTH_ALGO_RSNA_PSK          = 7,
} WMI_NLO_AUTH_ALGORITHM;

typedef enum _WMI_NLO_CIPHER_ALGORITHM {
    WMI_NLO_CIPHER_ALGO_NONE           = 0x00,
    WMI_NLO_CIPHER_ALGO_WEP40          = 0x01,
    WMI_NLO_CIPHER_ALGO_TKIP           = 0x02,
    WMI_NLO_CIPHER_ALGO_CCMP           = 0x04,
    WMI_NLO_CIPHER_ALGO_WEP104         = 0x05,
    WMI_NLO_CIPHER_ALGO_BIP            = 0x06,
    WMI_NLO_CIPHER_ALGO_WPA_USE_GROUP  = 0x100,
    WMI_NLO_CIPHER_ALGO_RSN_USE_GROUP  = 0x100,
    WMI_NLO_CIPHER_ALGO_WEP            = 0x101,
} WMI_NLO_CIPHER_ALGORITHM;

/* SSID broadcast  type passed in NLO params */
typedef enum _WMI_NLO_SSID_BcastNwType
{
  WMI_NLO_BCAST_UNKNOWN      = 0,
  WMI_NLO_BCAST_NORMAL       = 1,
  WMI_NLO_BCAST_HIDDEN       = 2,
} WMI_NLO_SSID_BcastNwType;

#define WMI_NLO_MAX_SSIDS    16
#define WMI_NLO_MAX_CHAN     48

#define WMI_NLO_CONFIG_STOP             (0x1 << 0)
#define WMI_NLO_CONFIG_START            (0x1 << 1)
#define WMI_NLO_CONFIG_RESET            (0x1 << 2)
#define WMI_NLO_CONFIG_SLOW_SCAN        (0x1 << 4)
#define WMI_NLO_CONFIG_FAST_SCAN        (0x1 << 5)
#define WMI_NLO_CONFIG_SSID_HIDE_EN     (0x1 << 6)

/* NOTE: wmi_nlo_ssid_param structure can't be changed without breaking the compatibility */
typedef struct wmi_nlo_ssid_param
{
	A_UINT32 valid;
    wmi_ssid ssid;
} wmi_nlo_ssid_param;

/* NOTE: wmi_nlo_enc_param structure can't be changed without breaking the compatibility */
typedef struct wmi_nlo_enc_param
{
	A_UINT32 valid;
    A_UINT32 enc_type;
} wmi_nlo_enc_param;

/* NOTE: wmi_nlo_auth_param structure can't be changed without breaking the compatibility */
typedef struct wmi_nlo_auth_param
{
	A_UINT32 valid;
    A_UINT32 auth_type;
} wmi_nlo_auth_param;

/* NOTE: wmi_nlo_bcast_nw_param structure can't be changed without breaking the compatibility */
typedef struct wmi_nlo_bcast_nw_param
{
    A_UINT32 valid;
    A_UINT32 bcast_nw_type;
} wmi_nlo_bcast_nw_param;

/* NOTE: wmi_nlo_rssi_param structure can't be changed without breaking the compatibility */
typedef struct wmi_nlo_rssi_param
{
    A_UINT32 valid;
    A_INT32 rssi;
} wmi_nlo_rssi_param;

typedef struct nlo_configured_parameters {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_nlo_configured_parameters */
    wmi_nlo_ssid_param ssid;
    wmi_nlo_enc_param enc_type;
    wmi_nlo_auth_param auth_type;
    wmi_nlo_rssi_param rssi_cond;
    wmi_nlo_bcast_nw_param bcast_nw_type; /* indicates if the SSID is hidden or not */
} nlo_configured_parameters;

typedef struct wmi_nlo_config {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_nlo_config_cmd_fixed_param */
    A_UINT32    flags;
    A_UINT32    vdev_id;
    A_UINT32    fast_scan_max_cycles;
    A_UINT32    active_dwell_time;
    A_UINT32    passive_dwell_time; /* PDT in msecs */
    A_UINT32    probe_bundle_size;
    A_UINT32    rest_time;  /* ART = IRT */
    A_UINT32    max_rest_time; /* Max value that can be reached after SBM */
    A_UINT32    scan_backoff_multiplier;  /* SBM */
    A_UINT32    fast_scan_period; /* SCBM */
    A_UINT32    slow_scan_period; /* specific to windows */
    A_UINT32    no_of_ssids;
    A_UINT32    num_of_channels;
    /* The TLVs will follow.
        * nlo_configured_parameters nlo_list[];
        * A_UINT32 channel_list[];
        */

} wmi_nlo_config_cmd_fixed_param;

typedef struct wmi_nlo_event
{
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_nlo_event */
    A_UINT32    vdev_id;
}wmi_nlo_event;

#define GTK_OFFLOAD_OPCODE_MASK				0xFF000000
/** Enable GTK offload, and provided parameters KEK,KCK and replay counter values */
#define GTK_OFFLOAD_ENABLE_OPCODE			0x01000000
/** Disable GTK offload */
#define GTK_OFFLOAD_DISABLE_OPCODE			0x02000000
/** Read GTK offload parameters, generates WMI_GTK_OFFLOAD_STATUS_EVENT */
#define GTK_OFFLOAD_REQUEST_STATUS_OPCODE	0x04000000
enum wmi_chatter_mode {
    /* Chatter enter/exit happens
     * automatically based on preset
     * params
     */
    WMI_CHATTER_MODE_AUTO,
    /* Chatter enter is triggered
     * manually by the user
     */
    WMI_CHATTER_MODE_MANUAL_ENTER,
    /* Chatter exit is triggered
     * manually by the user
     */
    WMI_CHATTER_MODE_MANUAL_EXIT,
    /* Placeholder max value, always last*/
    WMI_CHATTER_MODE_MAX
};

enum wmi_chatter_query_type {
    /*query coalescing filter match counter*/
    WMI_CHATTER_QUERY_FILTER_MATCH_CNT,
    WMI_CHATTER_QUERY_MAX
};

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_chatter_set_mode_cmd_fixed_param  */
    A_UINT32 chatter_mode;
} wmi_chatter_set_mode_cmd_fixed_param;

/** maximum number of filter supported*/
#define CHATTER_MAX_COALESCING_RULES     11
/** maximum number of field tests per filter*/
#define CHATTER_MAX_FIELD_TEST           5
/** maximum field length in number of DWORDS*/
#define CHATTER_MAX_TEST_FIELD_LEN32     2

/** field test kinds*/
#define CHATTER_COALESCING_TEST_EQUAL            1
#define CHATTER_COALESCING_TEST_MASKED_EQUAL     2
#define CHATTER_COALESCING_TEST_NOT_EQUAL        3

/** packet type*/
#define CHATTER_COALESCING_PKT_TYPE_UNICAST      (1 << 0)
#define CHATTER_COALESCING_PKT_TYPE_MULTICAST    (1 << 1)
#define CHATTER_COALESCING_PKT_TYPE_BROADCAST    (1 << 2)

/** coalescing field test*/
typedef struct _chatter_pkt_coalescing_hdr_test {
    /** offset from start of mac header, for windows native wifi host driver
     * should assume standard 802.11 frame format without QoS info and address4
     * FW would account for any non-stand fields for final offset value.
     */
    A_UINT32    offset;
    A_UINT32    length;	    /* length of test field*/
    A_UINT32    test;       /*equal, not equal or masked equal*/
    A_UINT32    mask[CHATTER_MAX_TEST_FIELD_LEN32];    /*mask byte stream*/
    A_UINT32    value[CHATTER_MAX_TEST_FIELD_LEN32];   /*value byte stream*/
} chatter_pkt_coalescing_hdr_test;

/** packet coalescing filter*/
typedef struct _chatter_pkt_coalescing_filter {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_chatter_pkt_coalescing_filter */
    A_UINT32    filter_id;  /*unique id assigned by OS*/
    A_UINT32    max_coalescing_delay; /*max miliseconds 1st pkt can be hold*/
    A_UINT32    pkt_type; /*unicast/multicast/broadcast*/
    A_UINT32    num_of_test_field;    /*number of field test in table*/
    chatter_pkt_coalescing_hdr_test   test_fields[CHATTER_MAX_FIELD_TEST]; /*field test tbl*/
} chatter_pkt_coalescing_filter;

/** packet coalescing filter add command*/
typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_chatter_coalescing_add_filter_cmd_fixed_param */
    A_UINT32    num_of_filters;
    /* Following this tlv, there comes an array of structure of type chatter_pkt_coalescing_filter
     chatter_pkt_coalescing_filter rx_filter[1];*/
} wmi_chatter_coalescing_add_filter_cmd_fixed_param;
/** packet coalescing filter delete command*/
typedef struct {
    A_UINT32    tlv_header;  /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_chatter_coalescing_delete_filter_cmd_fixed_param */
    A_UINT32    filter_id; /*filter id which will be deleted*/
} wmi_chatter_coalescing_delete_filter_cmd_fixed_param;
/** packet coalescing query command*/
typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_chatter_coalescing_query_cmd_fixed_param */
    A_UINT32    type;    /*type of query*/
} wmi_chatter_coalescing_query_cmd_fixed_param;
/** chatter query reply event*/
typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_chatter_query_reply_event_fixed_param  */
    A_UINT32    type;    /*query type*/
    A_UINT32    filter_match_cnt; /*coalescing filter match counter*/
} wmi_chatter_query_reply_event_fixed_param;

/* NOTE: This constants GTK_OFFLOAD_KEK_BYTES, GTK_OFFLOAD_KCK_BYTES, and GTK_REPLAY_COUNTER_BYTES
 * cannot be changed without breaking WMI compatibility. */
#define GTK_OFFLOAD_KEK_BYTES       16
#define GTK_OFFLOAD_KCK_BYTES       16
/* NOTE: GTK_REPLAY_COUNTER_BYTES, WMI_MAX_KEY_LEN, IGTK_PN_SIZE cannot be changed in the future without breaking WMI compatibility */
#define GTK_REPLAY_COUNTER_BYTES    8
#define WMI_MAX_KEY_LEN             32
#define IGTK_PN_SIZE                6

typedef struct {
    A_UINT32    tlv_header;  /* TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_GTK_OFFLOAD_STATUS_EVENT_fixed_param */
    A_UINT32    vdev_id;     /** unique id identifying the VDEV */
    A_UINT32    flags;       /* status flags */
    A_UINT32    refresh_cnt; /* number of successful GTK refresh exchanges since last SET operation */
    A_UINT8     replay_counter[GTK_REPLAY_COUNTER_BYTES]; /* current replay counter */
    A_UINT8     igtk_keyIndex; /* Use if IGTK_OFFLOAD is defined */
    A_UINT8     igtk_keyLength; /* Use if IGTK_OFFLOAD is defined */
    A_UINT8     igtk_keyRSC[IGTK_PN_SIZE];  /* key replay sequence counter */ /* Use if IGTK_OFFLOAD is defined */
    A_UINT8     igtk_key[WMI_MAX_KEY_LEN]; /* Use if IGTK_OFFLOAD is defined */
} WMI_GTK_OFFLOAD_STATUS_EVENT_fixed_param;

typedef struct {
    A_UINT32    tlv_header;                     /** TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_GTK_OFFLOAD_CMD_fixed_param */
    A_UINT32	vdev_id;					/** unique id identifying the VDEV */
    A_UINT32    flags;                          /* control flags, GTK offload command use high byte  */
    /* The size of following 3 arrays cannot be changed without breaking WMI compatibility. */
    A_UINT8     KEK[GTK_OFFLOAD_KEK_BYTES];     /* key encryption key */
    A_UINT8     KCK[GTK_OFFLOAD_KCK_BYTES];     /* key confirmation key */
    A_UINT8     replay_counter[GTK_REPLAY_COUNTER_BYTES];  /* replay counter for re-key */
}WMI_GTK_OFFLOAD_CMD_fixed_param;

typedef struct  {
    A_UINT8    address[4];    /* IPV4 address in Network Byte Order */
} WMI_IPV4_ADDR;

typedef enum {
    WMI_STA_KEEPALIVE_METHOD_NULL_FRAME = 1,                   /* 802.11 NULL frame */
    WMI_STA_KEEPALIVE_METHOD_UNSOLICITED_ARP_RESPONSE = 2,     /* ARP response */
    WMI_STA_KEEPALIVE_METHOD_ETHERNET_LOOPBACK = 3, /*ETHERNET LOOPBACK*/
} WMI_STA_KEEPALIVE_METHOD;

typedef struct {
    A_UINT32                 tlv_header;               /* TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_STA_KEEPALVE_ARP_RESPONSE */
    WMI_IPV4_ADDR            sender_prot_addr;         /* Sender protocol address */
    WMI_IPV4_ADDR            target_prot_addr;         /* Target protocol address */
    wmi_mac_addr             dest_mac_addr;            /* destination MAC address */
} WMI_STA_KEEPALVE_ARP_RESPONSE;

typedef struct  {
    A_UINT32 tlv_header;                    /* TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_STA_KEEPALIVE_CMD_fixed_param */
    A_UINT32 vdev_id;
    A_UINT32 enable;                        /* 1 - Enable, 0 - disable */
    A_UINT32 method;                        /* keep alive method */
    A_UINT32 interval;                      /* time interval in seconds  */
    /*
        * NOTE: following this structure is the TLV for ARP Resonse:
        *     WMI_STA_KEEPALVE_ARP_RESPONSE arp_resp; // ARP response
        */
} WMI_STA_KEEPALIVE_CMD_fixed_param;

typedef struct  {
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 action;
} WMI_VDEV_WNM_SLEEPMODE_CMD_fixed_param;
typedef WMI_VDEV_WNM_SLEEPMODE_CMD_fixed_param WMI_STA_WNMSLEEP_CMD;

typedef struct {
    A_UINT32 tlv_header;           /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_set_keepalive_cmd_fixed_param */
    A_UINT32 vdev_id;
    A_UINT32 keepaliveInterval;   /* seconds */
    A_UINT32 keepaliveMethod;
} wmi_vdev_set_keepalive_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;           /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_get_keepalive_cmd_fixed_param */
    A_UINT32    vdev_id;
} wmi_vdev_get_keepalive_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_get_keepalive_event_fixed_param */
    A_UINT32 vdev_id;
    A_UINT32 keepaliveInterval;   /* seconds */
    A_UINT32 keepaliveMethod;   /* seconds */
} wmi_vdev_get_keepalive_event_fixed_param;

#define IPSEC_NATKEEPALIVE_FILTER_DISABLE 0
#define IPSEC_NATKEEPALIVE_FILTER_ENABLE  1

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 action;
} WMI_VDEV_IPSEC_NATKEEPALIVE_FILTER_CMD_fixed_param;

typedef WMI_VDEV_IPSEC_NATKEEPALIVE_FILTER_CMD_fixed_param WMI_VDEV_IPSEC_NATKEEPALIVE_FILTER_CMD;

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 mcc_tbttmode;
    wmi_mac_addr mcc_bssid;
} wmi_vdev_mcc_set_tbtt_mode_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;        /* home vdev id */
    A_UINT32 meas_token;     /* from measure request frame */
    A_UINT32 dialog_token;
    A_UINT32 number_bursts;  /* zero keep sending until cancel, bigger than 0 means times e.g. 1,2 */
    A_UINT32 burst_interval; /* unit in mill seconds, interval between consecutive burst*/
    A_UINT32 burst_cycle;   /* times cycle through within one burst */
    A_UINT32 tx_power;      /* for path frame */
    A_UINT32 off_duration;  /* uint in mill seconds, channel off duraiton for path loss frame sending */
    wmi_mac_addr dest_mac; /* multicast DA, for path loss frame */
    A_UINT32 num_chans;
} wmi_vdev_plmreq_start_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 meas_token; /* same value from req*/
} wmi_vdev_plmreq_stop_cmd_fixed_param;

typedef struct {
	/* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_p2p_set_noa_cmd_fixed_param  */
    A_UINT32 tlv_header;
    /* unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /* enable/disable NoA */
	A_UINT32 enable;
	/** number of NoA desc. In the TLV noa_descriptor[] */
	A_UINT32 num_noa;
	/**
     * TLV (tag length value )  paramerters follow the pattern structure.
     * TLV  contain NoA desc with num of num_noa
     */
} wmi_p2p_set_noa_cmd_fixed_param;

typedef enum {
    RECOVERY_SIM_ASSERT          = 0x01,
    RECOVERY_SIM_NO_DETECT       = 0x02,
    RECOVERY_SIM_CTR_EP_FULL     = 0x03,
    RECOVERY_SIM_EMPTY_POINT     = 0x04,
    RECOVERY_SIM_STACK_OV        = 0x05,
    RECOVERY_SIM_INFINITE_LOOP   = 0x06,
} RECOVERY_SIM_TYPE;

/* WMI_FORCE_FW_HANG_CMDID */
typedef struct {
    A_UINT32 tlv_header;           /* TLV tag and len; tag equals WMITLV_TAG_STRUC_WMI_FORCE_FW_HANG_CMD_fixed_param */
    A_UINT32 type;     /*0:unused 1: ASSERT, 2: not respond detect command,3:  simulate ep-full(),4:...*/
    A_UINT32 delay_time_ms;   /*0xffffffff means the simulate will delay for random time (0 ~0xffffffff ms)*/
}WMI_FORCE_FW_HANG_CMD_fixed_param;
#define WMI_MCAST_FILTER_SET 1
#define WMI_MCAST_FILTER_DELETE 2
typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 index;
    A_UINT32 action;
    wmi_mac_addr mcastbdcastaddr;
} WMI_SET_MCASTBCAST_FILTER_CMD_fixed_param;

/* GPIO Command and Event data structures */

/* WMI_GPIO_CONFIG_CMDID */
enum {
    WMI_GPIO_PULL_NONE,
    WMI_GPIO_PULL_UP,
    WMI_GPIO_PULL_DOWN,
};

enum {
    WMI_GPIO_INTTYPE_DISABLE,
    WMI_GPIO_INTTYPE_RISING_EDGE,
    WMI_GPIO_INTTYPE_FALLING_EDGE,
    WMI_GPIO_INTTYPE_BOTH_EDGE,
    WMI_GPIO_INTTYPE_LEVEL_LOW,
    WMI_GPIO_INTTYPE_LEVEL_HIGH
};

typedef struct {
    A_UINT32 tlv_header;           /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_gpio_config_cmd_fixed_param */
    A_UINT32 gpio_num;             /* GPIO number to be setup */
    A_UINT32 input;                /* 0 - Output/ 1 - Input */
    A_UINT32 pull_type;            /* Pull type defined above */
    A_UINT32 intr_mode;            /* Interrupt mode defined above (Input) */
} wmi_gpio_config_cmd_fixed_param;

/* WMI_GPIO_OUTPUT_CMDID */
typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_gpio_output_cmd_fixed_param */
    A_UINT32 gpio_num;    /* GPIO number to be setup */
    A_UINT32 set;         /* Set the GPIO pin*/
} wmi_gpio_output_cmd_fixed_param;

/* WMI_GPIO_INPUT_EVENTID */
typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_gpio_input_event_fixed_param */
    A_UINT32 gpio_num;    /* GPIO number which changed state */
} wmi_gpio_input_event_fixed_param;

/* WMI_P2P_DISC_EVENTID */
enum {
    P2P_DISC_SEARCH_PROB_REQ_HIT = 0,   /* prob req hit the p2p find pattern */
    P2P_DISC_SEARCH_PROB_RESP_HIT,      /* prob resp hit the p2p find pattern */
};

enum {
    P2P_DISC_MODE_SEARCH = 0, /* do search when p2p find offload*/
    P2P_DISC_MODE_LISTEN,     /* do listen when p2p find offload*/
	P2P_DISC_MODE_AUTO,       /* do listen and search when p2p find offload*/
};

enum {
    P2P_DISC_PATTERN_TYPE_BSSID = 0,  /* BSSID pattern */
    P2P_DISC_PATTERN_TYPE_DEV_NAME,   /* device name pattern */
};

typedef struct {
    A_UINT32    vdev_id;
    A_UINT32    reason;    /* P2P DISC wake up reason*/
} wmi_p2p_disc_event;

typedef WMI_GTK_OFFLOAD_STATUS_EVENT_fixed_param WOW_EVENT_INFO_SECTION_GTKIGTK;

typedef enum {
    WMI_FAKE_TXBFER_SEND_NDPA,
    WMI_FAKE_TXBFER_SEND_MU,
    WMI_FAKE_TXBFER_NDPA_FBTYPE,
    WMI_FAKE_TXBFER_NDPA_NCIDX,
    WMI_FAKE_TXBFER_NDPA_POLL,
    WMI_FAKE_TXBFER_NDPA_BW,
    WMI_FAKE_TXBFER_NDPA_PREAMBLE,
    WMI_FAKE_TXBFER_NDPA_RATE,
    WMI_FAKE_TXBFER_NDP_BW,
    WMI_FAKE_TXBFER_NDP_NSS,
    WMI_TXBFEE_ENABLE_UPLOAD_H,
    WMI_TXBFEE_ENABLE_CAPTURE_H,
    WMI_TXBFEE_SET_CBF_TBL,
    WMI_TXBFEE_CBF_TBL_LSIG,
    WMI_TXBFEE_CBF_TBL_SIGA1,
    WMI_TXBFEE_CBF_TBL_SIGA2,
    WMI_TXBFEE_CBF_TBL_SIGB,
    WMI_TXBFEE_CBF_TBL_PAD,
    WMI_TXBFEE_CBF_TBL_DUR,
    WMI_TXBFEE_SU_NCIDX,
    WMI_TXBFEE_CBIDX,
    WMI_TXBFEE_NGIDX,
} WMI_TXBF_PARAM_ID;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_txbf_cmd_fixed_param */
    /** parameter id   */
    A_UINT32 param_id;
    /** parameter value */
    A_UINT32 param_value;
} wmi_txbf_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_upload_h_hdr */
    A_UINT32     h_length;
    A_UINT32     cv_length;
    /* This TLV is followed by array of bytes:
     * // h_cv info buffer
     *   A_UINT8 bufp[];
     */
} wmi_upload_h_hdr;

typedef struct {
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_capture_h_event_hdr */
    A_UINT32 svd_num;
    A_UINT32 tone_num;
    A_UINT32 reserved;
} wmi_capture_h_event_hdr;

typedef struct {
    A_UINT32    tlv_header;                 /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_avoid_freq_range_desc */
    A_UINT32    start_freq;  //start frequency, not channel center freq
    A_UINT32    end_freq;   //end frequency
} wmi_avoid_freq_range_desc;

typedef struct {
    A_UINT32    tlv_header;                 /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_avoid_freq_ranges_event_fixed_param */
   //bad channel range count, multi range is allowed, 0 means all channel clear
   A_UINT32  num_freq_ranges;

   /* The TLVs will follow.
       * multi range with num_freq_ranges, LTE advance multi carrier, CDMA,etc
       *     wmi_avoid_freq_range_desc avd_freq_range[];     // message buffer, NULL terminated
       */
} wmi_avoid_freq_ranges_event_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_gtk_rekey_fail_event_fixed_param  */
    /** Reserved for future use */
    A_UINT32    reserved0;
    A_UINT32 vdev_id;
} wmi_gtk_rekey_fail_event_fixed_param;

enum wmm_ac_downgrade_policy {
    WMM_AC_DOWNGRADE_DEPRIO,
    WMM_AC_DOWNGRADE_DROP,
    WMM_AC_DOWNGRADE_INVALID,
};

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 cwmin;
    A_UINT32 cwmax;
    A_UINT32 aifs;
    A_UINT32 txoplimit;
    A_UINT32 acm;
    A_UINT32 no_ack;
} wmi_wmm_vparams;

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    wmi_wmm_vparams wmm_params[4]; /* 0 be, 1 bk, 2 vi, 3 vo */
} wmi_vdev_set_wmm_params_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32              gtxRTMask[2]; /* for HT and VHT rate masks */
    A_UINT32              userGtxMask; /* host request for GTX mask */
    A_UINT32              gtxPERThreshold; /* default: 10% */
    A_UINT32              gtxPERMargin; /* default: 2% */
    A_UINT32              gtxTPCstep; /* default: 1 */
    A_UINT32              gtxTPCMin; /* default: 5 */
    A_UINT32              gtxBWMask; /* 20/40/80/160 Mhz */
} wmi_vdev_set_gtx_params_cmd_fixed_param;

typedef struct
{
    A_UINT32 tlv_header;
    A_UINT32  vdev_id;
    A_UINT32 ac;
    A_UINT32 medium_time_us; /* per second unit, the Admitted time granted, unit in micro seconds */
    A_UINT32 downgrade_type;
} wmi_vdev_wmm_addts_cmd_fixed_param;

typedef struct
{
    A_UINT32 tlv_header;
    A_UINT32  vdev_id;
    A_UINT32 ac;
} wmi_vdev_wmm_delts_cmd_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_dfs_enable_cmd_fixed_param  */
    /** Reserved for future use */
    A_UINT32    reserved0;
} wmi_pdev_dfs_enable_cmd_fixed_param;

typedef struct {
    A_UINT32    tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_pdev_dfs_disable_cmd_fixed_param  */
    /** Reserved for future use */
    A_UINT32    reserved0;
} wmi_pdev_dfs_disable_cmd_fixed_param;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_dfs_phyerr_filter_ena_cmd_fixed_param
     */
    A_UINT32 tlv_header;

    /** Reserved for future use */
    A_UINT32 reserved0;
} wmi_dfs_phyerr_filter_ena_cmd_fixed_param;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_dfs_phyerr_filter_dis_cmd_fixed_param
     */
    A_UINT32 tlv_header;
    /** Reserved for future use */
    A_UINT32 reserved0;
} wmi_dfs_phyerr_filter_dis_cmd_fixed_param;

/** TDLS COMMANDS */

/* WMI_TDLS_SET_STATE_CMDID */
/* TDLS State */
enum wmi_tdls_state {
    /** TDLS disable */
    WMI_TDLS_DISABLE,
    /** TDLS enabled - no firmware connection tracking/notifications */
    WMI_TDLS_ENABLE_PASSIVE,
    /** TDLS enabled - with firmware connection tracking/notifications */
    WMI_TDLS_ENABLE_ACTIVE,
};

/* TDLS Options */
#define WMI_TDLS_OFFCHAN_EN             (1 << 0) /** TDLS Off Channel support */
#define WMI_TDLS_BUFFER_STA_EN          (1 << 1) /** TDLS Buffer STA support */
#define WMI_TDLS_SLEEP_STA_EN           (1 << 2) /** TDLS Sleep STA support (not currently supported) */

typedef struct {
    /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_tdls_set_state_cmd_fixed_param  */
    A_UINT32 tlv_header;
    /** unique id identifying the VDEV */
    A_UINT32 vdev_id;
    /** Enable/Disable TDLS (wmi_tdls_state) */
    A_UINT32 state;
    /** Duration (in ms) over which to calculate tx threshold and rate values */
    A_UINT32 notification_interval_ms;
    /** number of packets OVER which notify/suggest TDLS Discovery:
     *  if current tx pps counter / notification interval >= threshold
     *  then a notification will be sent to host to advise TDLS Discovery */
    A_UINT32 tx_discovery_threshold;
    /** number of packets UNDER which notify/suggest TDLS Teardown:
     *  if current tx pps counter / notification interval < threshold
     *  then a notification will be sent to host to advise TDLS Tear down */
    A_UINT32 tx_teardown_threshold;
    /** Absolute RSSI value under which notify/suggest TDLS Teardown */
    A_INT32 rssi_teardown_threshold;
    /** Peer RSSI < (AP RSSI + delta) will trigger a teardown */
    A_INT32 rssi_delta;
    /** TDLS Option Control
     * Off-Channel, Buffer STA, (later)Sleep STA support */
    A_UINT32 tdls_options;
    /* Buffering time in number of beacon intervals */
    A_UINT32 tdls_peer_traffic_ind_window;
    /* Wait time for PTR frame */
    A_UINT32 tdls_peer_traffic_response_timeout_ms;
    /* Self PUAPSD mask */
    A_UINT32 tdls_puapsd_mask;
    /* Inactivity timeout */
    A_UINT32 tdls_puapsd_inactivity_time_ms;
    /* Max of rx frame during SP */
    A_UINT32 tdls_puapsd_rx_frame_threshold;
} wmi_tdls_set_state_cmd_fixed_param;

/* WMI_TDLS_PEER_UPDATE_CMDID */

enum wmi_tdls_peer_state {
    /** tx peer TDLS link setup now starting, traffic to DA should be
     * paused (except TDLS frames) until state is moved to CONNECTED (or
     * TEARDOWN on setup failure) */
    WMI_TDLS_PEER_STATE_PEERING,
    /** tx peer TDLS link established, running (all traffic to DA unpaused) */
    WMI_TDLS_PEER_STATE_CONNECTED,
    /** tx peer TDLS link tear down started (link paused, any frames
     * queued for DA will be requeued back through the AP)*/
    WMI_TDLS_PEER_STATE_TEARDOWN,
};

/* NB: These defines are fixed, and cannot be changed without breaking WMI compatibility */
#define WMI_TDLS_MAX_SUPP_OPER_CLASSES 32
typedef struct {
    /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_tdls_peer_capabilities  */
    A_UINT32 tlv_header;
    /* Peer's QoS Info - for U-APSD */
    /* AC FLAGS  - accessed through macros below */
    /* Ack, SP, More Data Ack - accessed through macros below */
    A_UINT32 peer_qos;
    /*TDLS Peer's U-APSD Buffer STA Support*/
    A_UINT32 buff_sta_support;
    /*TDLS off channel related params */
    A_UINT32 off_chan_support;
    A_UINT32 peer_curr_operclass;
    A_UINT32 self_curr_operclass;
    /* Number of channels available for off channel operation */
    A_UINT32 peer_chan_len;
    A_UINT32 peer_operclass_len;
    A_UINT8  peer_operclass[WMI_TDLS_MAX_SUPP_OPER_CLASSES];
    /* Is peer initiator or responder of TDLS setup request */
    A_UINT32 is_peer_responder;
    /* Preferred off channel number as configured by user */
    A_UINT32 pref_offchan_num;
    /* Preferred off channel bandwidth as configured by user */
    A_UINT32 pref_offchan_bw;

    /** Followed by the variable length TLV peer_chan_list:
     *  wmi_channel peer_chan_list[].
     *  Array size would be peer_chan_len.
     *  This array is intersected channels which is supported by both peer
     *  and DUT. freq1 in chan_info shall be same as mhz, freq2 shall be 0.
     *  FW shall compute BW for an offchan based on peer's ht/vht cap
     *  received in peer_assoc cmd during change STA operation
     */
} wmi_tdls_peer_capabilities;

#define WMI_TDLS_QOS_VO_FLAG           0
#define WMI_TDLS_QOS_VI_FLAG           1
#define WMI_TDLS_QOS_BK_FLAG           2
#define WMI_TDLS_QOS_BE_FLAG           3
#define WMI_TDLS_QOS_ACK_FLAG          4
#define WMI_TDLS_QOS_SP_FLAG           5
#define WMI_TDLS_QOS_MOREDATA_FLAG     7

#define WMI_TDLS_PEER_SET_QOS_FLAG(ppeer_caps,flag) do { \
        (ppeer_caps)->peer_qos |=  (1 << flag);      \
     } while(0)
#define WMI_TDLS_PEER_GET_QOS_FLAG(ppeer_caps,flag)   \
        (((ppeer_caps)->peer_qos & (1 << flag)) >> flag)

#define WMI_SET_TDLS_PEER_VO_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_SET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_VO_FLAG)
#define WMI_GET_TDLS_PEER_VO_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_GET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_VO_FLAG)
#define WMI_SET_TDLS_PEER_VI_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_SET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_VI_FLAG)
#define WMI_GET_TDLS_PEER_VI_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_GET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_VI_FLAG)
#define WMI_SET_TDLS_PEER_BK_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_SET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_BK_FLAG)
#define WMI_GET_TDLS_PEER_BK_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_GET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_BK_FLAG)
#define WMI_SET_TDLS_PEER_BE_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_SET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_BE_FLAG)
#define WMI_GET_TDLS_PEER_BE_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_GET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_BE_FLAG)
#define WMI_SET_TDLS_PEER_ACK_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_SET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_ACK_FLAG)
#define WMI_GET_TDLS_PEER_ACK_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_GET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_ACK_FLAG)
/* SP has 2 bits */
#define WMI_SET_TDLS_PEER_SP_UAPSD(ppeer_caps,val) do { \
     (ppeer_caps)->peer_qos |=  (((val)&0x3) << WMI_TDLS_QOS_SP_FLAG); \
     } while(0)
#define WMI_GET_TDLS_PEER_SP_UAPSD(ppeer_caps) \
    (((ppeer_caps)->peer_qos & (0x3 << WMI_TDLS_QOS_SP_FLAG)) >> WMI_TDLS_QOS_SP_FLAG)

#define WMI_SET_TDLS_PEER_MORE_DATA_ACK_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_SET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_MOREDATA_FLAG)
#define WMI_GET_TDLS_PEER_MORE_DATA_ACK_UAPSD(ppeer_caps) \
    WMI_TDLS_PEER_GET_QOS_FLAG(ppeer_caps, WMI_TDLS_QOS_MOREDATA_FLAG)


#define WMI_TDLS_SELF_SET_QOS_FLAG(pset_cmd,flag) do { \
        (pset_cmd)->tdls_puapsd_mask |=  (1 << flag);      \
    } while(0)
#define WMI_TDLS_SELF_GET_QOS_FLAG(pset_cmd,flag)   \
        (((pset_cmd)->tdls_puapsd_mask & (1 << flag)) >> flag)

#define WMI_SET_TDLS_SELF_VO_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_SET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_VO_FLAG)
#define WMI_GET_TDLS_SELF_VO_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_GET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_VO_FLAG)
#define WMI_SET_TDLS_SELF_VI_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_SET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_VI_FLAG)
#define WMI_GET_TDLS_SELF_VI_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_GET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_VI_FLAG)
#define WMI_SET_TDLS_SELF_BK_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_SET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_BK_FLAG)
#define WMI_GET_TDLS_SELF__BK_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_GET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_BK_FLAG)
#define WMI_SET_TDLS_SELF_BE_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_SET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_BE_FLAG)
#define WMI_GET_TDLS_SELF_BE_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_GET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_BE_FLAG)
#define WMI_SET_TDLS_SELF_ACK_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_SET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_ACK_FLAG)
#define WMI_GET_TDLS_SELF_ACK_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_GET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_ACK_FLAG)
/* SP has 2 bits */
#define WMI_SET_TDLS_SELF_SP_UAPSD(pset_cmd,val) do { \
     (pset_cmd)->tdls_puapsd_mask |=  (((val)&0x3) << WMI_TDLS_QOS_SP_FLAG); \
    } while(0)
#define WMI_GET_TDLS_SELF_SP_UAPSD(pset_cmd) \
    (((pset_cmd)->tdls_puapsd_mask & (0x3 << WMI_TDLS_QOS_SP_FLAG)) >> WMI_TDLS_QOS_SP_FLAG)

#define WMI_SET_TDLS_SELF_MORE_DATA_ACK_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_SET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_MOREDATA_FLAG)
#define WMI_GET_TDLS_SELF_MORE_DATA_ACK_UAPSD(pset_cmd) \
    WMI_TDLS_SELF_GET_QOS_FLAG(pset_cmd, WMI_TDLS_QOS_MOREDATA_FLAG)

typedef struct {
    /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_tdls_peer_update_cmd_fixed_param  */
    A_UINT32    tlv_header;
    /** unique id identifying the VDEV */
    A_UINT32                    vdev_id;
    /** peer MAC address */
    wmi_mac_addr                peer_macaddr;
    /** new TDLS state for peer (wmi_tdls_peer_state) */
    A_UINT32                    peer_state;
    /* The TLV for wmi_tdls_peer_capabilities will follow.
     *     wmi_tdls_peer_capabilities  peer_caps;
     */
    /** Followed by the variable length TLV chan_info:
     *  wmi_channel chan_info[] */
} wmi_tdls_peer_update_cmd_fixed_param;

/** TDLS EVENTS */
enum wmi_tdls_peer_notification {
    /** tdls discovery recommended for peer (based
     * on tx bytes per second > tx_discover threshold) */
    WMI_TDLS_SHOULD_DISCOVER,
    /** tdls link tear down recommended for peer
     * due to tx bytes per second below tx_teardown_threshold
     * NB: this notification sent once */
    WMI_TDLS_SHOULD_TEARDOWN,
    /** tx peer TDLS link tear down complete */
    WMI_TDLS_PEER_DISCONNECTED,
};

enum wmi_tdls_peer_reason {
    /** tdls teardown recommended due to low transmits */
    WMI_TDLS_TEARDOWN_REASON_TX,
    /** tdls link tear down recommended due to poor RSSI */
    WMI_TDLS_TEARDOWN_REASON_RSSI,
    /** tdls link tear down recommended due to offchannel scan */
    WMI_TDLS_TEARDOWN_REASON_SCAN,
    /** tdls peer disconnected due to peer deletion */
    WMI_TDLS_DISCONNECTED_REASON_PEER_DELETE,
    /** tdls peer disconnected due to PTR timeout */
    WMI_TDLS_TEARDOWN_REASON_PTR_TIMEOUT,
    /** tdls peer disconnected due wrong PTR format */
    WMI_TDLS_TEARDOWN_REASON_BAD_PTR,
    /** tdls peer not responding */
    WMI_TDLS_TEARDOWN_REASON_NO_RESPONSE,
};

/* WMI_TDLS_PEER_EVENTID */
typedef struct {
    /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_tdls_peer_event_fixed_param  */
    A_UINT32    tlv_header;
    /** peer MAC address */
    wmi_mac_addr    peer_macaddr;
    /** TDLS peer status (wmi_tdls_peer_notification)*/
    A_UINT32        peer_status;
    /** TDLS peer reason (wmi_tdls_peer_reason) */
    A_UINT32        peer_reason;
    /** unique id identifying the VDEV */
    A_UINT32        vdev_id;
} wmi_tdls_peer_event_fixed_param;

/* NOTE: wmi_vdev_mcc_bcn_intvl_change_event_fixed_param would be deprecated. Please
 don't use this for any new implementations */
typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_vdev_mcc_bcn_intvl_change_event_fixed_param  */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /* New beacon interval to be used for the specified VDEV suggested by firmware */
    A_UINT32 new_bcn_intvl;
} wmi_vdev_mcc_bcn_intvl_change_event_fixed_param;

/* WMI_RESMGR_ADAPTIVE_OCS_ENABLE_DISABLE_CMDID */
typedef struct {
    /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_resmgr_adaptive_ocs_enable_disable_cmd_fixed_param */
    A_UINT32 tlv_header;
    /** 1: enable fw based adaptive ocs,
     *  0: disable fw based adaptive ocs
     */
    A_UINT32 enable;
} wmi_resmgr_adaptive_ocs_enable_disable_cmd_fixed_param;

/* WMI_RESMGR_SET_CHAN_TIME_QUOTA_CMDID */
typedef struct {
    /* Frequency of the channel for which the quota is set */
    A_UINT32 chan_mhz;
    /* Requested channel time quota expressed as percentage */
    A_UINT32 channel_time_quota;
} wmi_resmgr_chan_time_quota;

typedef struct {
    /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_resmgr_set_chan_time_quota_cmd_fixed_param */
    A_UINT32 tlv_header;
    /** number of channel time quota command structures
     * (wmi_resmgr_chan_time_quota) 1 or 2
     */
    A_UINT32 num_chans;
/* This TLV is followed by another TLV of array of bytes
 * A_UINT8 data[];
 * This data array contains
 * num_chans * size of(struct wmi_resmgr_chan_time_quota)
 */
} wmi_resmgr_set_chan_time_quota_cmd_fixed_param;

/* WMI_RESMGR_SET_CHAN_LATENCY_CMDID */
typedef struct {
    /* Frequency of the channel for which the latency is set */
    A_UINT32 chan_mhz;
    /* Requested channel latency in milliseconds */
    A_UINT32 latency;
} wmi_resmgr_chan_latency;

typedef struct {
    /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_resmgr_set_chan_latency_cmd_fixed_param */
    A_UINT32 tlv_header;
    /** number of channel latency command structures
     * (wmi_resmgr_chan_latency) 1 or 2
     */
    A_UINT32 num_chans;
/* This TLV is followed by another TLV of array of bytes
 * A_UINT8 data[];
 * This data array contains
 * num_chans * size of(struct wmi_resmgr_chan_latency)
 */
} wmi_resmgr_set_chan_latency_cmd_fixed_param;

/* WMI_STA_SMPS_FORCE_MODE_CMDID */

/** STA SMPS Forced Mode */
typedef enum {
    WMI_SMPS_FORCED_MODE_NONE = 0,
    WMI_SMPS_FORCED_MODE_DISABLED,
    WMI_SMPS_FORCED_MODE_STATIC,
    WMI_SMPS_FORCED_MODE_DYNAMIC
} wmi_sta_smps_forced_mode;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_sta_smps_force_mode_cmd_fixed_param */
    A_UINT32 tlv_header;
    /** Unique id identifying the VDEV */
    A_UINT32 vdev_id;
    /** The mode of SMPS that is to be forced in the FW. */
    A_UINT32 forced_mode;
} wmi_sta_smps_force_mode_cmd_fixed_param;

/** wlan HB commands */
#define WMI_WLAN_HB_ITEM_UDP            0x1
#define WMI_WLAN_HB_ITEM_TCP            0x2
#define WMI_WLAN_HB_MAX_FILTER_SIZE     32 /* should be equal to WLAN_HB_MAX_FILTER_SIZE, must be a multiple of 4 bytes */

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_hb_set_enable_cmd_fixed_param */
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 enable;
    A_UINT32 item;
    A_UINT32 session;
} wmi_hb_set_enable_cmd_fixed_param;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_hb_set_tcp_params_cmd_fixed_param */
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 srv_ip;
    A_UINT32 dev_ip;
    A_UINT32 seq;
    A_UINT32 src_port;
    A_UINT32 dst_port;
    A_UINT32 interval;
    A_UINT32 timeout;
    A_UINT32 session;
wmi_mac_addr gateway_mac;
} wmi_hb_set_tcp_params_cmd_fixed_param;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_hb_set_tcp_pkt_filter_cmd_fixed_param */
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 length;
    A_UINT32 offset;
    A_UINT32 session;
    A_UINT8  filter[WMI_WLAN_HB_MAX_FILTER_SIZE];
} wmi_hb_set_tcp_pkt_filter_cmd_fixed_param;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_hb_set_udp_params_cmd_fixed_param */
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 srv_ip;
    A_UINT32 dev_ip;
    A_UINT32 src_port;
    A_UINT32 dst_port;
    A_UINT32 interval;
    A_UINT32 timeout;
    A_UINT32 session;
    wmi_mac_addr gateway_mac;
} wmi_hb_set_udp_params_cmd_fixed_param;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_hb_set_udp_pkt_filter_cmd_fixed_param */
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    A_UINT32 length;
    A_UINT32 offset;
    A_UINT32 session;
    A_UINT8  filter[WMI_WLAN_HB_MAX_FILTER_SIZE];
} wmi_hb_set_udp_pkt_filter_cmd_fixed_param;

/** wlan HB events */
typedef enum {
    WMI_WLAN_HB_REASON_UNKNOWN      = 0,
    WMI_WLAN_HB_REASON_TCP_TIMEOUT  = 1,
    WMI_WLAN_HB_REASON_UDP_TIMEOUT  = 2,
} WMI_HB_WAKEUP_REASON;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_hb_ind_event_fixed_param */
    A_UINT32 vdev_id;    /* unique id identifying the VDEV */
    A_UINT32 session;    /* Session ID from driver */
    A_UINT32 reason;     /* wakeup reason */
} wmi_hb_ind_event_fixed_param;

/** WMI_STA_SMPS_PARAM_CMDID */
typedef enum {
    /** RSSI threshold to enter Dynamic SMPS mode from inactive mode */
    WMI_STA_SMPS_PARAM_UPPER_RSSI_THRESH = 0,
    /** RSSI threshold to enter Stalled-D-SMPS mode from D-SMPS mode or
     * to enter D-SMPS mode from Stalled-D-SMPS mode */
    WMI_STA_SMPS_PARAM_STALL_RSSI_THRESH = 1,
    /** RSSI threshold to disable SMPS modes */
    WMI_STA_SMPS_PARAM_LOWER_RSSI_THRESH = 2,
    /** Upper threshold for beacon-RSSI. Used to reduce RX chainmask. */
    WMI_STA_SMPS_PARAM_UPPER_BRSSI_THRESH = 3,
    /** Lower threshold for beacon-RSSI. Used to increase RX chainmask. */
    WMI_STA_SMPS_PARAM_LOWER_BRSSI_THRESH = 4,
    /** Enable/Disable DTIM 1chRx feature */
    WMI_STA_SMPS_PARAM_DTIM_1CHRX_ENABLE = 5
} wmi_sta_smps_param;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_sta_smps_param_cmd_fixed_param */
    A_UINT32 tlv_header;
    /** Unique id identifying the VDEV */
    A_UINT32 vdev_id;
    /** SMPS parameter (see wmi_sta_smps_param) */
    A_UINT32 param;
    /** Value of SMPS parameter */
    A_UINT32 value;
} wmi_sta_smps_param_cmd_fixed_param;

typedef struct {
    /** TLV tag and len; tag equals
     * WMITLV_TAG_STRUC_wmi_mcc_sched_sta_traffic_stats */
    A_UINT32 tlv_header;
    /* TX stats */
    A_UINT32 txBytesPushed;
    A_UINT32 txPacketsPushed;
    /* RX stats */
    A_UINT32 rxBytesRcvd;
    A_UINT32 rxPacketsRcvd;
    A_UINT32 rxTimeTotal;
    /** peer MAC address */
    wmi_mac_addr peer_macaddr;
} wmi_mcc_sched_sta_traffic_stats;

typedef struct {
    /** TLV tag and len; tag equals
     * WMITLV_TAG_STRUC_wmi_mcc_sched_traffic_stats_cmd_fixed_param */
    A_UINT32 tlv_header;
    /** Duration over which the host stats were collected */
    A_UINT32 duration;
    /** Number of stations filled in following stats array */
    A_UINT32 num_sta;
    /* Following this struct are the TLVs:
     * wmi_mcc_sched_sta_traffic_stats mcc_sched_sta_traffic_stats_list;
     */
} wmi_mcc_sched_traffic_stats_cmd_fixed_param;

typedef struct
{
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals  WMITLV_TAG_STRUC_wmi_batch_scan_enable_cmd_fixed_param */
    /* unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    /*Batch scan enable command parameters*/
    A_UINT32 scanInterval;
    A_UINT32 numScan2Batch;
    A_UINT32 bestNetworks;
    A_UINT32 rfBand;
    A_UINT32 rtt;
} wmi_batch_scan_enable_cmd_fixed_param;

typedef struct
{
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_batch_scan_enabled_event_fixed_param  */
    A_UINT32 supportedMscan;
} wmi_batch_scan_enabled_event_fixed_param;

typedef struct
{
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals  WMITLV_TAG_STRUC_wmi_batch_scan_disable_cmd_fixed_param */
/* unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    A_UINT32   param;
} wmi_batch_scan_disable_cmd_fixed_param;

typedef struct
{
    A_UINT32 tlv_header;     /* TLV tag and len; tag equals  WMITLV_TAG_STRUC_wmi_batch_scan_trigger_result_cmd_fixed_param */
    /** unique id identifying the VDEV, generated by the caller */
    A_UINT32 vdev_id;
    A_UINT32 param;
} wmi_batch_scan_trigger_result_cmd_fixed_param;

typedef struct
{
    A_UINT32 tlv_header;
    wmi_mac_addr   bssid;     /* BSSID */
    wmi_ssid   ssid;     /* SSID */
    A_UINT32   ch;           /* Channel */
    A_UINT32   rssi;         /* RSSI or Level */
    /* Timestamp when Network was found. Used to calculate age based on timestamp in GET_RSP msg header */
    A_UINT32  timestamp;
} wmi_batch_scan_result_network_info;

typedef struct
{
    A_UINT32 tlv_header;
    A_UINT32 scanId;                         /* Scan List ID. */
    /* No of AP in a Scan Result. Should be same as bestNetwork in SET_REQ msg */
    A_UINT32 numNetworksInScanList;
    A_UINT32 netWorkStartIndex;  /* indicate the start index of network info*/
} wmi_batch_scan_result_scan_list;

#define LPI_IE_BITMAP_BSSID              0x0001
#define LPI_IE_BITMAP_SSID               0x0002
#define LPI_IE_BITMAP_RSSI               0x0004
#define LPI_IE_BITMAP_CHAN               0x0008

typedef struct {
    A_UINT32 tlv_header;
    /**A_BOOL indicates LPI mgmt snooping enable/disable*/
    A_UINT32 enable;
    /**LPI snooping mode*/
    A_UINT32 snooping_mode;
    /** LPI interested IEs in snooping context */
    A_UINT32 ie_bitmap;
} wmi_lpi_mgmt_snooping_config_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_start_scan_cmd_fixed_param */
    /** Scan ID */
    A_UINT32 scan_id;
    /** Scan requestor ID */
    A_UINT32 scan_req_id;
    /** VDEV id(interface) that is requesting scan */
    A_UINT32 vdev_id;
    /** LPI interested IEs in scan context */
    A_UINT32 ie_bitmap;
    /** Scan Priority, input to scan scheduler */
    A_UINT32 scan_priority;
    /** dwell time in msec on active channels */
    A_UINT32 dwell_time_active;
    /** dwell time in msec on passive channels */
    A_UINT32 dwell_time_passive;
    /** min time in msec on the BSS channel,only valid if atleast one VDEV is active*/
    A_UINT32 min_rest_time;
    /** max rest time in msec on the BSS channel,only valid if at least one VDEV is active*/
    /** the scanner will rest on the bss channel at least min_rest_time. after min_rest_time the scanner
     *  will start checking for tx/rx activity on all VDEVs. if there is no activity the scanner will
     *  switch to off channel. if there is activity the scanner will let the radio on the bss channel
     *  until max_rest_time expires.at max_rest_time scanner will switch to off channel
     *  irrespective of activity. activity is determined by the idle_time parameter.
     */
    A_UINT32 max_rest_time;
    /** time before sending next set of probe requests.
     *   The scanner keeps repeating probe requests transmission with period specified by repeat_probe_time.
     *   The number of probe requests specified depends on the ssid_list and bssid_list
     */
    A_UINT32 repeat_probe_time;
    /** time in msec between 2 consequetive probe requests with in a set. */
    A_UINT32 probe_spacing_time;
    /** data inactivity time in msec on bss channel that will be used by scanner for measuring the inactivity  */
    A_UINT32 idle_time;
    /** maximum time in msec allowed for scan  */
    A_UINT32 max_scan_time;
    /** delay in msec before sending first probe request after switching to a channel */
    A_UINT32 probe_delay;
    /** Scan control flags */
    A_UINT32 scan_ctrl_flags;
    /** Burst duration time in msec*/
    A_UINT32 burst_duration;

    /** # if channels to scan. In the TLV channel_list[] */
    A_UINT32 num_chan;
    /** number of bssids. In the TLV bssid_list[] */
    A_UINT32 num_bssid;
    /** number of ssid. In the TLV ssid_list[] */
    A_UINT32 num_ssids;
    /** number of bytes in ie data. In the TLV ie_data[] */
    A_UINT32 ie_len;

/**
 * TLV (tag length value ) parameters follow the scan_cmd
 * structure. The TLV's are:
 *     A_UINT32 channel_list[];
 *     wmi_ssid ssid_list[];
 *     wmi_mac_addr bssid_list[];
 *     A_UINT8 ie_data[];
 */
} wmi_lpi_start_scan_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_stop_scan_cmd_fixed_param */
    /** requestor requesting cancel  */
    A_UINT32 requestor;
    /** Scan ID */
    A_UINT32 scan_id;
    /**
     * Req Type
     * req_type should be WMI_SCAN_STOP_ONE, WMI_SCN_STOP_VAP_ALL or WMI_SCAN_STOP_ALL
     * WMI_SCAN_STOP_ONE indicates to stop a specific scan with scan_id
     * WMI_SCN_STOP_VAP_ALL indicates to stop all scan requests on a specific vDev with vdev_id
     * WMI_SCAN_STOP_ALL indicates to stop all scan requests in both Scheduler's queue and Scan Engine
     */
    A_UINT32 req_type;
    /**
     * vDev ID
     * used when req_type equals to WMI_SCN_STOP_VAP_ALL, it indexed the vDev on which to stop the scan
     */
    A_UINT32 vdev_id;
} wmi_lpi_stop_scan_cmd_fixed_param;

typedef struct
{
    A_UINT32 tlv_header;
    A_UINT32 ie_bitmap;
    A_UINT32 data_len;
    /* This buffer is used to send lpi scan result data
      *  A_UINT8 data[];	 // length in byte given by field data_len.
      */
} wmi_lpi_result_event_fixed_param;

typedef struct
{
    A_UINT32 tlv_header;
    A_UINT32 timestamp;   /*timestamp of batch scan event*/
    A_UINT32 numScanLists;  /*number of scan in this event*/
    A_UINT32 isLastResult;  /*is this event a last event of the whole batch scan*/
}  wmi_batch_scan_result_event_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_p2p_noa_event_fixed_param  */
    A_UINT32 vdev_id;
    /* This TLV is followed by p2p_noa_info for vdev :
     *     wmi_p2p_noa_info p2p_noa_info;
     */
} wmi_p2p_noa_event_fixed_param;

#define WMI_RFKILL_CFG_RADIO_LEVEL_OFFSET      6
#define WMI_RFKILL_CFG_RADIO_LEVEL_MASK      0x1

#define WMI_RFKILL_CFG_GPIO_PIN_NUM_OFFSET      0
#define WMI_RFKILL_CFG_GPIO_PIN_NUM_MASK      0x3f

#define WMI_RFKILL_CFG_PIN_AS_GPIO_OFFSET      7
#define WMI_RFKILL_CFG_PIN_AS_GPIO_MASK      0xf

typedef struct {
    /** TLV tag and len; tag equals
     * */
    A_UINT32 tlv_header;
    /** gpip pin number */
    A_UINT32 gpio_pin_num;
    /** gpio interupt type */
    A_UINT32 int_type;
    /** RF radio status */
    A_UINT32 radio_state;
} wmi_rfkill_mode_param;

typedef enum {
    WMI_SET_LED_SYS_POWEROFF,
    WMI_SET_LED_SYS_S3_SUSPEND,
    WMI_SET_LED_SYS_S4_S5,
    WMI_SET_LED_SYS_DRIVER_DISABLE,
    WMI_SET_LED_SYS_WAKEUP,
    WMI_SET_LED_SYS_ALWAYS_ON, //just for test!
    WMI_SET_LED_SYS_POWERON,
} wmi_led_sys_state_param;

typedef enum {
    WMI_CONFIG_LED_TO_VDD = 0,
    WMI_CONFIG_LED_TO_GND = 1,
} wmi_config_led_connect_type;

typedef enum {
   WMI_CONFIG_LED_NOT_WITH_BT = 0,
   WMI_CONFIG_LED_WITH_BT = 1,
} wmi_config_led_with_bt_flag;

typedef enum {
   WMI_CONFIG_LED_DISABLE = 0,
   WMI_CONFIG_LED_ENABLE  = 1,
} wmi_config_led_enable_flag;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_peer_info_req_cmd_fixed_param   */
    A_UINT32 tlv_header;
    /* Set GPIO pin */
    A_UINT32 led_gpio_pin;
    /* Set connect type defined in wmi_config_led_connect_type */
    A_UINT32 connect_type;
    /* Set flag defined in wmi_config_led_with_bt_flag*/
    A_UINT32 with_bt;
    /* Set LED enablement defined in wmi_config_led_enable_flag */
    A_UINT32 led_enable;
} wmi_pdev_set_led_config_cmd_fixed_param;

/** WMI_PEER_INFO_REQ_CMDID
 *   Request FW to provide peer info */
typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_peer_info_req_cmd_fixed_param   */
    A_UINT32 tlv_header;
    /** In order to get the peer info for a single peer, host shall
     *  issue the peer_mac_address of that peer. For getting the
     *  info all peers, the host shall issue 0xFFFFFFFF as the mac
     *  address. The firmware will return the peer info for all the
     *  peers on the specified vdev_id */
    wmi_mac_addr peer_mac_address;
    /** vdev id */
    A_UINT32 vdev_id;
} wmi_peer_info_req_cmd_fixed_param;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_peer_info */
    A_UINT32 tlv_header;
    /** mac addr of the peer */
    wmi_mac_addr peer_mac_address;
    /** data_rate of the peer */
    A_UINT32 data_rate;
    /** rssi of the peer */
    A_UINT32 rssi;
    /** tx fail count */
    A_UINT32 tx_fail_cnt;
} wmi_peer_info;

/** FW response with the peer info */
typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_peer_info_event_fixed_param   */
    A_UINT32 tlv_header;
    /** number of peers in peer_info */
    A_UINT32 num_peers;
    /* This TLV is followed by another TLV of array of structs
     * wmi_peer_info peer_info[];
     */
} wmi_peer_info_event_fixed_param;

/** FW response when tx failure count has reached threshold
 *  for a peer */
typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_peer_tx_fail_cnt_thr_event_fixed_param */
    A_UINT32 tlv_header;
    /** vdev id*/
    A_UINT32 vdev_id;
    /** mac address */
    wmi_mac_addr peer_mac_address;
    /** tx failure count- will eventually be removed and not used * */
    A_UINT32 tx_fail_cnt;
    /** seq number of the nth tx_fail_event */
     A_UINT32 seq_no;
} wmi_peer_tx_fail_cnt_thr_event_fixed_param;

enum wmi_rmc_mode {
    /** Disable RMC */
    WMI_RMC_MODE_DISABLED = 0,
    /** Enable RMC */
    WMI_RMC_MODE_ENABLED = 1,
};

/** Enable reliable multicast transmitter functionality. Upon
 *  receiving this, the FW shall mutlicast frames with
 *  reliablity. This is a vendor
 *  proprietary feature. */
typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_rmc_set_mode_cmd_fixed_param   */
    A_UINT32 tlv_header;
    /** vdev id*/
    A_UINT32 vdev_id;
    /** enable_rmc contains values from enum wmi_rmc_mode;
     *  Default value: 0 (disabled) */
    A_UINT32 enable_rmc;
} wmi_rmc_set_mode_cmd_fixed_param;

/** Configure transmission periodicity of action frames in a
 *  reliable multicast network for the multicast transmitter */
typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_rmc_set_action_period_cmd_fixed_param */
    A_UINT32 tlv_header;
    /** vdev id */
    A_UINT32 vdev_id;
    /** time period in milliseconds. Default: 300 ms.
     An action frame indicating the current leader is transmitted by the
     RMC transmitter once every 'periodity_msec' */
    A_UINT32 periodicity_msec;
} wmi_rmc_set_action_period_cmd_fixed_param;

/** Optimise Leader selection process in RMC functionality. For
 *  Enhancement/Debug purposes only */
typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_rmc_config_cmd_fixed_param   */
    A_UINT32 tlv_header;
    /** vdev id */
    A_UINT32 vdev_id;
    /** flags ::
     *  0x0001 - Enable beacon averaging
     *  0x0002 - Force leader selection
     *  0x0004 - Enable Timer based leader switch
     *  0x0008 - Use qos/NULL based for multicast reliability */
    A_UINT32 flags;
    /**  control leader change timeperiod (in seconds) */
    A_UINT32 peridocity_leader_switch;
    /** control activity timeout value for data rx (in seconds) */
    A_UINT32 data_activity_timeout;
    /** mac address of leader */
    wmi_mac_addr forced_leader_mac_addr;
} wmi_rmc_config_cmd_fixed_param;

/** Multi-hop forwarding is generally implemented in
 *  the kernel. To decrease system power consumption, the
 *  driver can enable offloading this to the chipset. In
 *  order for the offload, the firmware needs the routing table.
 *  The host shall plumb the routing table into FW. The firmware
 *  shall perform an IP address lookup and forward the packet to
 *  the next hop using next hop's mac address. This is a vendor
 *  proprietary feature. */
enum wmi_mhf_ofl_mode {
    /** Disable multihop forwarding offload */
    WMI_MHF_OFL_MODE_DISABLED = 0,
    /** Enable multihop forwarding offload */
    WMI_MHF_OFL_MODE_ENABLED = 1,
};

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_mhf_offload_set_mode_cmd_fixed_param */
    A_UINT32 tlv_header;
    /** vdev id*/
    A_UINT32 vdev_id;
    /** enable_mhf_ofl contains values from enum
     *  wmi_mhf_ofl_mode; Default value: 0 (disabled) */
    A_UINT32 enable_mhf_ofl;
} wmi_mhf_offload_set_mode_cmd_fixed_param;

enum wmi_mhf_ofl_table_action {
   /** Create forwarding offload table in FW */
   WMI_MHF_OFL_TBL_CREATE = 0,
   /** Append to existing multihop forwarding offload table */
   WMI_MHF_OFL_TBL_APPEND = 1,
   /** Flush entire multihop forwarding offload table in FW */
   WMI_MHF_OFL_TBL_FLUSH = 2,
};

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_mhf_offload_plumb_routing_table_cmd_fixed_param */
    A_UINT32 tlv_header;
    /** vdev id*/
    A_UINT32 vdev_id;
    /** action corresponds to values from enum
     *  wmi_mhf_ofl_table_action */
    A_UINT32 action;
    /** number of entries in the table */
    A_UINT32 num_entries;
/** Followed by the variable length TLV
 *  wmi_mhf_offload_routing_table_entry entries[] */
}wmi_mhf_offload_plumb_routing_table_cmd;

typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_mhf_offload_routing_table_entry */
    A_UINT32 tlv_header;
    /** Destination node's IP address */
    WMI_IPV4_ADDR dest_ipv4_addr;
    /** Next hop node's MAC address */
    wmi_mac_addr next_hop_mac_addr;
}wmi_mhf_offload_routing_table_entry;

typedef struct {
    /** tlv tag and len, tag equals
     * WMITLV_TAG_STRUC_wmi_dfs_radar_event */
    A_UINT32 tlv_header;

    /** full 64 tsf timestamp get from MAC tsf timer indicates
     * the time that the radar event uploading to host, split
     * it to high 32 bit and lower 32 bit in fulltsf_high and
     * full_tsf_low
     */
    A_UINT32 upload_fullts_low;
    A_UINT32 upload_fullts_high;

    /** timestamp indicates the time when DFS pulse is detected
     * equal to ppdu_end_ts - radar_pusle_summary_ts_offset
     */
    A_UINT32 pulse_detect_ts;

    /** the duaration of the pulse in us */
    A_UINT32 pulse_duration;

    /** the center frequency of the radar pulse detected, KHz */
    A_UINT32 pulse_center_freq;

    /** bandwidth of current DFS channel, MHz */
    A_UINT32 ch_bandwidth;

    /** center channel frequency1 of current DFS channel, MHz */
    A_UINT16 ch_center_freq1;

    /** center channel frequency2 of current DFS channel, MHz,
     * reserved for 160 BW mode
     */
    A_UINT16 ch_center_freq2;

    /** flag to indicate if this pulse is chirp */
    A_UINT8  pulse_is_chirp;

    /** RSSI recorded in the ppdu */
    A_UINT8  rssi;

    /** extened RSSI info */
    A_UINT8  rssi_ext;

    /** For 4-byte aligment padding */
    A_UINT8 reserved;

} wmi_dfs_radar_event_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_thermal_mgmt_cmd_fixed_param  */

    /*Thermal thresholds*/
    A_UINT32 lower_thresh_degreeC; /* in degree C*/
    A_UINT32 upper_thresh_degreeC; /* in degree C*/

    /*Enable/Disable Thermal Monitoring for Mitigation*/
    A_UINT32 enable;
} wmi_thermal_mgmt_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_thermal_mgmt_event_fixed_param  */

    A_UINT32 temperature_degreeC;/* temperature in degree C*/
} wmi_thermal_mgmt_event_fixed_param;

/**
 * This command is sent from WLAN host driver to firmware to
 * request firmware to configure auto shutdown timer in fw
 * 0 - Disable <1-19600>-Enabled and timer value is seconds (86400 seconds = 1 day maximum>
 */
typedef struct {
    A_UINT32 tlv_header;    /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_host_auto_shutdown_cfg_cmd_param  */
    A_UINT32 timer_value;   /** timer value; 0=disable */
} wmi_host_auto_shutdown_cfg_cmd_fixed_param;

enum wmi_host_auto_shutdown_reason {
    WMI_HOST_AUTO_SHUTDOWN_REASON_UNKNOWN = 0,
    WMI_HOST_AUTO_SHUTDOWN_REASON_TIMER_EXPIRY = 1,
    WMI_HOST_AUTO_SHUTDOWN_REASON_MAX,
};

/* WMI_HOST_AUTO_SHUTDOWN_EVENTID  */
typedef struct{
    A_UINT32    tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_host_auto_shutdown_event_fixed_param  */
    A_UINT32    shutdown_reason; /* value: wmi_host_auto_shutdown_reason */
} wmi_host_auto_shutdown_event_fixed_param;


typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_nan_cmd_param */
    A_UINT32 data_len; /** length in byte of data[]. */
    /* This structure is used to send REQ binary blobs
     * from application/service to firmware where Host drv is pass through .
     * Following this structure is the TLV:
     *     A_UINT8 data[];    // length in byte given by field data_len.
     */
} wmi_nan_cmd_param;

typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_nan_event_hdr */
    A_UINT32 data_len; /** length in byte of data[]. */
    /* This structure is used to send REQ binary blobs
     * from firmware to application/service where Host drv is pass through .
     * Following this structure is the TLV:
     *     A_UINT8 data[];    // length in byte given by field data_len.
     */
} wmi_nan_event_hdr;

typedef struct {
    A_UINT32 tlv_header;
    A_UINT32 num_data;
    /* followed by WMITLV_TAG_ARRAY_BYTE */
} wmi_diag_data_container_event_fixed_param;

enum {
    WMI_PDEV_PARAM_TXPOWER_REASON_NONE = 0,
    WMI_PDEV_PARAM_TXPOWER_REASON_SAR,
    WMI_PDEV_PARAM_TXPOWER_REASON_MAX
};

#define PDEV_PARAM_TXPOWER_VALUE_MASK  0x000000FF
#define PDEV_PARAM_TXPOWER_VALUE_SHIFT 0

#define PDEV_PARAM_TXPOWER_REASON_MASK  0x0000FF00
#define PDEV_PARAM_TXPOWER_REASON_SHIFT 8

#define SET_PDEV_PARAM_TXPOWER_VALUE(txpower_param, value)     \
    ((txpower_param) &= ~PDEV_PARAM_TXPOWER_VALUE_MASK, (txpower_param) |= ((value) << PDEV_PARAM_TXPOWER_VALUE_SHIFT))

#define SET_PDEV_PARAM_TXPOWER_REASON(txpower_param, value)     \
    ((txpower_param) &= ~PDEV_PARAM_TXPOWER_REASON_MASK, (txpower_param) |= ((value) << PDEV_PARAM_TXPOWER_REASON_SHIFT))

#define GET_PDEV_PARAM_TXPOWER_VALUE(txpower_param)     \
    (((txpower_param) & PDEV_PARAM_TXPOWER_VALUE_MASK) >> PDEV_PARAM_TXPOWER_VALUE_SHIFT)

#define GET_PDEV_PARAM_TXPOWER_REASON(txpower_param)     \
    (((txpower_param) & PDEV_PARAM_TXPOWER_REASON_MASK) >> PDEV_PARAM_TXPOWER_REASON_SHIFT)

/**
 * This command is sent from WLAN host driver to firmware to
 * notify the current modem power state. Host would receive a
 * message from modem when modem is powered on. Host driver
 * would then send this command to firmware. Firmware would then
 * power on WCI-2 (UART) interface for LTE/MWS Coex.
 *
 * This command is only applicable for APQ platform which has
 * modem on the platform. If firmware doesn't support MWS Coex,
 * this command can be dropped by firmware.
 *
 * This is a requirement from modem team that WCN can't toggle
 * UART before modem is powered on.
 */
typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_modem_power_state_cmd_param */
    A_UINT32 tlv_header;

    /** Modem power state parameter */
    A_UINT32 modem_power_state;
} wmi_modem_power_state_cmd_param;

enum {
    WMI_MODEM_STATE_OFF = 0,
    WMI_MODEM_STATE_ON
};

#define WMI_PEER_ESTIMATED_LINKSPEED_INVALID    0xFFFFFFFF

typedef struct {
    /* TLV tag and len; tag equals WMITLV_TAG_STRUC_ wmi_peer_get_estimated_linkspeed_cmd_fixed_param */
    A_UINT32 tlv_header;
    /** MAC address of the peer for which the estimated link speed is required. */
    wmi_mac_addr peer_macaddr;
} wmi_peer_get_estimated_linkspeed_cmd_fixed_param;

typedef struct {
    /* TLV tag and len; tag equals WMITLV_TAG_STRUC_ wmi_peer_estimated_linkspeed_event_fixed_param */
    A_UINT32 tlv_header;
    /** MAC address of the peer for which the estimated link speed is required.
     */
    wmi_mac_addr peer_macaddr;
  /* Estimated link speed in kbps.
   * When est_linkspeed_kbps is not valid, the value is set to WMI_PEER_ESTIMATED_LINKSPEED_INVALID.
   */
    A_UINT32 est_linkspeed_kbps;
} wmi_peer_estimated_linkspeed_event_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /* TLV tag and len; tag equals */
    /* vdev ID */
    A_UINT32 vdev_id;
    A_UINT32 data_len; /** length in byte of data[]. */
    /* This structure is used to send REQ binary blobs
     * from application/service to firmware where Host drv is pass through .
     * Following this structure is the TLV:
     *     A_UINT8 data[];    // length in byte given by field data_len.
     */
} wmi_req_stats_ext_cmd_fixed_param;

typedef struct {
    A_UINT32 tlv_header; /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_stats1_event_fix_param */
    A_UINT32 vdev_id; /** vdev ID */
    A_UINT32 data_len; /** length in byte of data[]. */
    /* This structure is used to send REQ binary blobs
     * from firmware to application/service where Host drv is pass through .
     * Following this structure is the TLV:
     *     A_UINT8 data[];    // length in byte given by field data_len.
     */
} wmi_stats_ext_event_fixed_param;

typedef struct {
    /* TLV tag and len; tag equals WMITLV_TAG_STRUC_ wmi_peer_state_event_fixed_param */
    A_UINT32 tlv_header;
    A_UINT32 vdev_id; /* vdev ID */
    /* MAC address of the peer for which the estimated link speed is required.*/
    wmi_mac_addr peer_macaddr;
    A_UINT32 state; /* peer state */
} wmi_peer_state_event_fixed_param;

enum {
    WMI_2G4_HT40_OBSS_SCAN_PASSIVE = 0,    /** scan_type: passive */
    WMI_2G4_HT40_OBSS_SCAN_ACTIVE, /** scan_type: active */
};

typedef struct {
    /**
     * TLV tag and len;
     * tag equals WMITLV_TAG_STRUC_wmi_obss_scan_enalbe_cmd_fixed_param
     */
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
    /**
     * active or passive. if active all the channels are actively scanned.
     *  if passive then all the channels are passively scanned
     */
    A_UINT32 scan_type;
    /**
     * FW can perform multiple scans with in a  OBSS scan interval.
     * For each scan,
     *  if the scan is passive then obss_scan_passive_dwell is minimum dwell to be used for each channel  ,
     *  if the scan is active then obss_scan_active_dwell is minimum dwell to be used for each channel .
     *   The unit for these 2 parameters is TUs.
     */
    A_UINT32 obss_scan_passive_dwell;
    A_UINT32 obss_scan_active_dwell;
    /**
     * OBSS scan interval . FW needs to perform one or more OBSS scans within this interval and fulfill the
     *  both min and total per channel dwell time requirement
     */
    A_UINT32 bss_channel_width_trigger_scan_interval;
    /**
     * FW can perform multiple scans with in a  OBSS scan interval.
     * For each scan,
     * the total per channel dwell time across all scans with in OBSS scan interval should be
     * atleast obss_scan_passive_total_per channel for passive scas and obss_scan_active_total_per channel
     * for active scans and ,
     *   The unit for these 2 parameters is TUs.
     */
    A_UINT32 obss_scan_passive_total_per_channel;
    A_UINT32 obss_scan_active_total_per_channel;
    A_UINT32 bss_width_channel_transition_delay_factor; /** parameter to check exemption from scan */
    A_UINT32 obss_scan_activity_threshold; /** parameter to check exemption from scan */
    /** following two parameters used by FW to fill IEs when sending 20/40 coexistence action frame to AP */
    A_UINT32 forty_mhz_intolerant; /** STA 40M bandwidth intolerant capability */
    A_UINT32 current_operating_class; /** STA current operating class */
    /** length of 2.4GHz channel list to scan at, channel list in tlv->channels[] */
    A_UINT32 channel_len;
    /** length of optional ie data to append to probe reqest when active scan, ie data in tlv->ie_field[] */
    A_UINT32 ie_len;
} wmi_obss_scan_enable_cmd_fixed_param;

typedef struct {
    /**
    * TLV tag and len;
    * tag equals WMITLV_TAG_STRUC_wmi_obss_scan_disalbe_cmd_fixed_param
    */
    A_UINT32 tlv_header;
    A_UINT32 vdev_id;
} wmi_obss_scan_disable_cmd_fixed_param;

typedef struct {
    /** TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_offload_prb_rsp_tx_status_event_fixed_param */
    A_UINT32 tlv_header;
    /** unique id identifying the VDEV */
    A_UINT32 vdev_id;
    /** prb rsp tx status, values defined in enum WMI_FRAME_TX_STATUS */
    A_UINT32 tx_status;
}wmi_offload_prb_rsp_tx_status_event_fixed_param;

typedef enum {
    WMI_FRAME_TX_OK,            /* frame tx ok */
    WMI_FRAME_TX_XRETRY,        /* excessivley retried */
    WMI_FRAME_TX_DROP,          /* frame dropped by FW due to resources */
    WMI_FRAME_TX_FILTERED,      /* frame filtered by hardware */
} WMI_FRAME_TX_STATUS;

/**
 * This command is sent from WLAN host driver to firmware to
 * request firmware to send the latest channel avoidance range
 * to host.
 *
 * This command is only applicable for APQ platform which has
 * modem on the platform. If firmware doesn't support MWS Coex,
 * this command can be dropped by firmware.
 *
 * Host would send this command to firmware to request a channel
 * avoidance information update.
 */
typedef struct {
    /** TLV tag and len; tag equals
     *  WMITLV_TAG_STRUC_wmi_chan_avoid_update_cmd_param */
    A_UINT32 tlv_header;
} wmi_chan_avoid_update_cmd_param;

/* ExtScan operation mode */
typedef enum {
   WMI_EXTSCAN_MODE_NONE          = 0x0000,
   WMI_EXTSCAN_MODE_START         = 0x0001,    // ExtScan/TableMonitoring operation started
   WMI_EXTSCAN_MODE_STOP          = 0x0002,    // ExtScan/TableMonitoring operation stopped
   WMI_EXTSCAN_MODE_IGNORED       = 0x0003,    // ExtScan command ignored due to error
} wmi_extscan_operation_mode;

/* Channel Mask */
typedef enum {
   WMI_CHANNEL_BAND_UNSPECIFIED = 0x0000,
   WMI_CHANNEL_BAND_24          = 0x0001,    // 2.4 channel
   WMI_CHANNEL_BAND_5_NON_DFS   = 0x0002,    // 5G Channels (No DFS channels)
   WMI_CHANNEL_BAND_DFS         = 0x0004,    // DFS channels
} wmi_channel_band_mask;

typedef enum {
    WMI_EXTSCAN_CYCLE_STARTED_EVENT     = 0x0001,
    WMI_EXTSCAN_CYCLE_COMPLETED_EVENT   = 0x0002,
    WMI_EXTSCAN_BUCKET_STARTED_EVENT    = 0x0004,
    WMI_EXTSCAN_BUCKET_COMPLETED_EVENT  = 0x0008,
    WMI_EXTSCAN_BUCKET_FAILED_EVENT     = 0x0010,
    WMI_EXTSCAN_BUCKET_OVERRUN_EVENT    = 0x0020,

    WMI_EXTSCAN_EVENT_MAX               = 0x8000
} wmi_extscan_event_type;

#define WMI_EXTSCAN_CYCLE_EVENTS_MASK    (WMI_EXTSCAN_CYCLE_STARTED_EVENT   | \
                                          WMI_EXTSCAN_CYCLE_COMPLETED_EVENT)

#define WMI_EXTSCAN_BUCKET_EVENTS_MASK   (WMI_EXTSCAN_BUCKET_STARTED_EVENT   | \
                                          WMI_EXTSCAN_BUCKET_COMPLETED_EVENT | \
                                          WMI_EXTSCAN_BUCKET_FAILED_EVENT    | \
                                          WMI_EXTSCAN_BUCKET_OVERRUN_EVENT)

typedef enum {
    WMI_EXTSCAN_NO_FORWARDING         = 0x0000,
    WMI_EXTSCAN_FORWARD_FRAME_TO_HOST = 0x0001
} wmi_extscan_forwarding_flags;

typedef enum {
    WMI_EXTSCAN_USE_MSD = 0x0001,    // Use Motion Sensor Detection */
} wmi_extscan_configuration_flags;

typedef enum {
    WMI_EXTSCAN_STATUS_OK    = 0,
    WMI_EXTSCAN_STATUS_ERROR = 0x80000000,
    WMI_EXTSCAN_STATUS_INVALID_PARAMETERS,
    WMI_EXTSCAN_STATUS_INTERNAL_ERROR
} wmi_extscan_start_stop_status;

typedef struct {
    /** Request ID - to identify command. Cannot be 0 */
    A_UINT32     request_id;
    /** Requestor ID - client requesting ExtScan */
    A_UINT32     requestor_id;
    /** VDEV id(interface) that is requesting scan */
    A_UINT32     vdev_id;
} wmi_extscan_command_id;

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_ARRAY_STRUC */
    /** channel number */
    A_UINT32    channel;

    /** dwell time in msec - use defaults if 0 */
    A_UINT32    min_dwell_time;
    A_UINT32    max_dwell_time;
    /** passive/active channel and other flags */
    A_UINT32    control_flags;                        // 0 => active, 1 => passive scan; ignored for DFS
} wmi_extscan_bucket_channel;

/* Scan Bucket specification */
typedef struct {
    A_UINT32        tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_ARRAY_STRUC */
    /** Bucket ID  - 0-based */
    A_UINT32        bucket_id;
    /** ExtScan events subscription - events to be reported to client (see wmi_extscan_event_type) */
    A_UINT32        notify_extscan_events;
    /** Options to forward scan results - see wmi_extscan_forwarding_flags */
    A_UINT32        forwarding_flags;
    /** ExtScan configuration flags - wmi_extscan_configuration_flags */
    A_UINT32        configuration_flags;
    /** multiplier to be applied to the periodic scan's base period */
    A_UINT32        base_period_multiplier;
    /** dwell time in msec on active channels - use defaults if 0 */
    A_UINT32        min_dwell_time_active;
    A_UINT32        max_dwell_time_active;
    /** dwell time in msec on passive channels - use defaults if 0 */
    A_UINT32        min_dwell_time_passive;
    A_UINT32        max_dwell_time_passive;
    /** see wmi_channel_band_mask; when equal to WMI_CHANNEL_UNSPECIFIED, use channel list */
    A_UINT32        channel_band;
    /** number of channels (if channel_band is WMI_CHANNEL_UNSPECIFIED) */
    A_UINT32        num_channels;
/** Followed by the variable length TLV chan_list:
 *  wmi_extscan_bucket_channel chan_list[] */
} wmi_extscan_bucket;

typedef struct {
    A_UINT32     tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_start_cmd_fixed_param */
    /** Request ID - to identify command. Cannot be 0 */
    A_UINT32     request_id;
    /** Requestor ID - client requesting ExtScan */
    A_UINT32     requestor_id;
    /** VDEV id(interface) that is requesting scan */
    A_UINT32     vdev_id;
    /** table ID - to allow support for multiple simultaneous requests */
    A_UINT32     table_id;
    /** Base period (milliseconds) used by scan buckets to define periodicity of the scans */
    A_UINT32     base_period;
    /** Maximum number of iterations to run - one iteration is the scanning of the least frequent bucket */
    A_UINT32     max_iterations;
    /** Options to forward scan results - see wmi_extscan_forwarding_flags */
    A_UINT32     forwarding_flags;
    /** ExtScan configuration flags - wmi_extscan_configuration_flags */
    A_UINT32     configuration_flags;
    /** ExtScan events subscription - bitmask indicating which events should be send to client (see wmi_extscan_event_type) */
    A_UINT32     notify_extscan_events;
    /** Scan Priority, input to scan scheduler */
    A_UINT32     scan_priority;
    /** Maximum number of BSSIDs to cache on each scan cycle */
    A_UINT32     max_bssids_per_scan_cycle;
    /** Minimum RSSI value to report */
    A_UINT32     min_rssi;
    /** Maximum table usage in percentage */
    A_UINT32     max_table_usage;
    /** default dwell time in msec on active channels */
    A_UINT32     min_dwell_time_active;
    A_UINT32     max_dwell_time_active;
    /** default dwell time in msec on passive channels */
    A_UINT32     min_dwell_time_passive;
    A_UINT32     max_dwell_time_passive;
    /** min time in msec on the BSS channel,only valid if atleast one VDEV is active*/
    A_UINT32     min_rest_time;
    /** max rest time in msec on the BSS channel,only valid if at least one VDEV is active*/
    /** the scanner will rest on the bss channel at least min_rest_time. after min_rest_time the scanner
     *  will start checking for tx/rx activity on all VDEVs. if there is no activity the scanner will
     *  switch to off channel. if there is activity the scanner will let the radio on the bss channel
     *  until max_rest_time expires.at max_rest_time scanner will switch to off channel
     *  irrespective of activity. activity is determined by the idle_time parameter.
     */
    A_UINT32     max_rest_time;
    /** time before sending next set of probe requests.
     *   The scanner keeps repeating probe requests transmission with period specified by repeat_probe_time.
     *   The number of probe requests specified depends on the ssid_list and bssid_list
     */
    /** Max number of probes to be sent */
    A_UINT32     n_probes;
    /** time in msec between 2 sets of probe requests. */
    A_UINT32     repeat_probe_time;
    /** time in msec between 2 consequetive probe requests with in a set. */
    A_UINT32     probe_spacing_time;
    /** data inactivity time in msec on bss channel that will be used by scanner for measuring the inactivity  */
    A_UINT32     idle_time;
    /** maximum time in msec allowed for scan  */
    A_UINT32     max_scan_time;
    /** delay in msec before sending first probe request after switching to a channel */
    A_UINT32     probe_delay;
    /** Scan control flags */
    A_UINT32     scan_ctrl_flags;
    /** Burst duration time in msec*/
    A_UINT32     burst_duration;

    /** number of bssids in the TLV bssid_list[] */
    A_UINT32     num_bssid;
    /** number of ssid in the TLV ssid_list[] */
    A_UINT32     num_ssids;
    /** number of bytes in TLV ie_data[] */
    A_UINT32     ie_len;
    /** number of buckets in the TLV bucket_list[] */
    A_UINT32     num_buckets;
    /** number of channels in channel_list[] determined by the
        sum of wmi_extscan_bucket.num_channels in array  */

/**
 * TLV (tag length value ) parameters follow the extscan_cmd
 * structure. The TLV's are:
 *     wmi_ssid                   ssid_list[];
 *     wmi_mac_addr               bssid_list[];
 *     A_UINT8                    ie_data[];
 *     wmi_extscan_bucket         bucket_list[];
 *     wmi_extscan_bucket_channel channel_list[];
 */
} wmi_extscan_start_cmd_fixed_param;

typedef struct {
    A_UINT32     tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_stop_cmd_fixed_param */
    /** Request ID - to match running command. 0 matches any request */
    A_UINT32     request_id;
    /** Requestor ID - client requesting stop */
    A_UINT32     requestor_id;
    /** VDEV id(interface) that is requesting scan */
    A_UINT32     vdev_id;
    /** table ID - to allow support for multiple simultaneous requests */
    A_UINT32     table_id;
} wmi_extscan_stop_cmd_fixed_param;

enum wmi_extscan_get_cached_results_flags {
    WMI_EXTSCAN_GET_CACHED_RESULTS_FLAG_NONE        = 0x0000,
    WMI_EXTSCAN_GET_CACHED_RESULTS_FLAG_FLUSH_TABLE = 0x0001
};

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_get_cached_results_cmd_fixed_param */
    /** request ID - used to correlate command with events */
    A_UINT32    request_id;
    /** Requestor ID - client that requested results */
    A_UINT32    requestor_id;
    /** VDEV id(interface) that is requesting scan */
    A_UINT32    vdev_id;
    /** table ID - to allow support for multiple simultaneous requests */
    A_UINT32    table_id;
    /** maximum number of results to be returned  */
    A_UINT32    max_results;
    /** flush BSSID list - wmi_extscan_get_cached_results_flags */
    A_UINT32    control_flags;    // enum wmi_extscan_get_cached_results_flags
} wmi_extscan_get_cached_results_cmd_fixed_param;

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_get_wlan_change_results_cmd_fixed_param */
    /** request ID - used to correlate command with events */
    A_UINT32    request_id;
    /** Requestor ID - client that requested results */
    A_UINT32    requestor_id;
    /** VDEV id(interface) that is requesting scan */
    A_UINT32    vdev_id;
    /** table ID - to allow support for multiple simultaneous requests */
    A_UINT32    table_id;
} wmi_extscan_get_wlan_change_results_cmd_fixed_param;

typedef struct {
    A_UINT32        tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_ARRAY_STRUC */
    /**bssid */
    wmi_mac_addr    bssid;
    /**channel number */
    A_UINT32        channel;
    /**upper RSSI limit */
    A_UINT32        upper_rssi_limit;
    /**lower RSSI limit */
    A_UINT32        lower_rssi_limit;
} wmi_extscan_wlan_change_bssid_param;

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_configure_wlan_change_monitor_cmd_fixed_param */
    /** Request ID - to identify command. Cannot be 0 */
    A_UINT32    request_id;
    /** Requestor ID - client requesting wlan change monitoring */
    A_UINT32    requestor_id;
    /** VDEV id(interface) that is requesting scan */
    A_UINT32    vdev_id;
    /** table ID - to allow support for multiple simultaneous tables */
    A_UINT32    table_id;
    /** operation mode: start/stop */
    A_UINT32    mode;    // wmi_extscan_operation_mode
    /** number of rssi samples to store */
    A_UINT32    max_rssi_samples;
    /** number of samples to use to calculate RSSI average */
    A_UINT32    rssi_averaging_samples;
    /** number of scans to confirm loss of contact with RSSI */
    A_UINT32    lost_ap_scan_count;
    /** number of out-of-range BSSIDs necessary to send event */
    A_UINT32    max_out_of_range_count;
    /** total number of bssid signal descriptors (in all pages) */
    A_UINT32    total_entries;
    /** index of the first bssid entry found in the TLV wlan_change_descriptor_list*/
    A_UINT32    first_entry_index;
    /** number of bssid signal descriptors in this page */
    A_UINT32    num_entries_in_page;
    /* Following this structure is the TLV:
     *     wmi_extscan_wlan_change_bssid_param wlan_change_descriptor_list[];    // number of elements given by field num_page_entries.
     */
} wmi_extscan_configure_wlan_change_monitor_cmd_fixed_param;

typedef struct {
    A_UINT32        tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_ARRAY_STRUC */
    /**bssid */
    wmi_mac_addr    bssid;
    /**RSSI threshold for reporting */
    A_UINT32        min_rssi;
    /**channel number */
    A_UINT32        channel;
} wmi_extscan_hotlist_entry;

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_configure_hotlist_monitor_cmd_fixed_param */
    /** Request ID - to identify command. Cannot be 0 */
    A_UINT32    request_id;
    /** Requestor ID - client requesting hotlist monitoring */
    A_UINT32    requestor_id;
    /** VDEV id(interface) that is requesting scan */
    A_UINT32    vdev_id;
    /** table ID - to allow support for multiple simultaneous tables */
    A_UINT32    table_id;
    /** operation mode: start/stop */
    A_UINT32    mode;    // wmi_extscan_operation_mode
    /**total number of bssids (in all pages) */
    A_UINT32    total_entries;
    /**index of the first bssid entry found in the TLV wmi_extscan_hotlist_entry*/
    A_UINT32    first_entry_index;
    /**number of bssids in this page */
    A_UINT32    num_entries_in_page;
    /* Following this structure is the TLV:
     *     wmi_extscan_hotlist_entry hotlist[];    // number of elements given by field num_page_entries.
     */
} wmi_extscan_configure_hotlist_monitor_cmd_fixed_param;

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_ARRAY_STRUC */
    /** table ID - to allow support for multiple simultaneous tables */
    A_UINT32    table_id;
    /** size in bytes of scan cache entry */
    A_UINT32    scan_cache_entry_size;
    /** maximum number of scan cache entries */
    A_UINT32    max_scan_cache_entries;
    /** maximum number of buckets per extscan request */
    A_UINT32    max_buckets;
    /** maximum number of BSSIDs that will be stored in each scan (best n/w as per RSSI) */
    A_UINT32    max_bssid_per_scan;
    /** table usage level at which indication must be sent to host */
    A_UINT32    max_table_usage_threshold;
} wmi_extscan_cache_capabilities;

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_ARRAY_STRUC */
    /** table ID - to allow support for multiple simultaneous tables */
    A_UINT32    table_id;
    /** size in bytes of wlan change entry */
    A_UINT32    wlan_change_entry_size;
    /** maximum number of entries in wlan change table */
    A_UINT32    max_wlan_change_entries;
    /** number of RSSI samples used for averaging RSSI */
    A_UINT32    max_rssi_averaging_samples;
    /** number of BSSID/RSSI entries (BSSID pointer, RSSI, timestamp) that device can hold */
    A_UINT32    max_rssi_history_entries;
} wmi_extscan_wlan_change_monitor_capabilities;

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_ARRAY_STRUC */
    /** table ID - to allow support for multiple simultaneous tables */
    A_UINT32    table_id;
    /** size in bytes of hotlist entry */
    A_UINT32    wlan_hotlist_entry_size;
    /** maximum number of entries in wlan change table */
    A_UINT32    max_hotlist_entries;
} wmi_extscan_hotlist_monitor_capabilities;

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_set_capabilities_cmd_fixed_param */
    /** Request ID - matches request ID used to start hot list monitoring */
    A_UINT32    request_id;
    /** Requestor ID - client requesting stop */
    A_UINT32    requestor_id;
    /** number of extscan caches */
    A_UINT32    num_extscan_cache_tables;
    /** number of wlan change lists */
    A_UINT32    num_wlan_change_monitor_tables;
    /** number of hotlists */
    A_UINT32    num_hotlist_monitor_tables;
    /** if one sided rtt data collection is supported */
    A_UINT32    rtt_one_sided_supported;
    /** if 11v data collection is supported */
    A_UINT32    rtt_11v_supported;
    /** if 11mc data collection is supported */
    A_UINT32    rtt_ftm_supported;
    /** number of extscan cache capabilities (one per table)  */
    A_UINT32    num_extscan_cache_capabilities;
    /** number of wlan change  capabilities (one per table)  */
    A_UINT32    num_extscan_wlan_change_capabilities;
    /** number of extscan hotlist capabilities (one per table)  */
    A_UINT32    num_extscan_hotlist_capabilities;
    /* Following this structure is the TLV:
     *     wmi_extscan_cache_capabilities               extscan_cache_capabilities; // number of capabilities given by num_extscan_caches
     *     wmi_extscan_wlan_change_monitor_capabilities wlan_change_capabilities;   // number of capabilities given by num_wlan_change_monitor_tables
     *     wmi_extscan_hotlist_monitor_capabilities     hotlist_capabilities;       // number of capabilities given by num_hotlist_monitor_tables
     */
} wmi_extscan_set_capabilities_cmd_fixed_param;

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_get_capabilities_cmd_fixed_param */
    /** Request ID - matches request ID used to start hot list monitoring */
    A_UINT32    request_id;
    /** Requestor ID - client requesting capabilities */
    A_UINT32    requestor_id;
} wmi_extscan_get_capabilities_cmd_fixed_param;

typedef struct {
    A_UINT32     tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_start_stop_event_fixed_param */
    /** Request ID of the operation that was started/stopped */
    A_UINT32     request_id;
    /** Requestor ID of the operation that was started/stopped */
    A_UINT32     requestor_id;
    /** VDEV id(interface) of the operation that was started/stopped */
    A_UINT32     vdev_id;
    /** extscan WMI command */
    A_UINT32     command;
    /** operation mode: start/stop */
    A_UINT32     mode;      // wmi_extscan_operation_mode
    /**success/failure */
    A_UINT32     status;    // enum wmi_extscan_start_stop_status
    /** table ID - to allow support for multiple simultaneous requests */
    A_UINT32     table_id;
} wmi_extscan_start_stop_event_fixed_param;

typedef struct {
    A_UINT32     tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_operation_event_fixed_param */
    /** Request ID of the extscan operation that is currently running */
    A_UINT32     request_id;
    /** Requestor ID of the extscan operation that is currently running */
    A_UINT32     requestor_id;
    /** VDEV id(interface) of the extscan operation that is currently running */
    A_UINT32     vdev_id;
    /** scan event (wmi_scan_event_type) */
    A_UINT32     event;    // wmi_extscan_event_type
    /** table ID - to allow support for multiple simultaneous requests */
    A_UINT32     table_id;
    /**number of buckets */
    A_UINT32     num_buckets;
    /* Following this structure is the TLV:
     *     A_UINT32    bucket_id[];    // number of elements given by field num_buckets.
     */
} wmi_extscan_operation_event_fixed_param;

/* Types of extscan tables */
typedef enum {
    EXTSCAN_TABLE_NONE    = 0,
    EXTSCAN_TABLE_BSSID   = 1,
    EXTSCAN_TABLE_RSSI    = 2,
} wmi_extscan_table_type;

typedef struct {
    A_UINT32     tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_table_usage_event_fixed_param */
    /** Request ID of the extscan operation that is currently running */
    A_UINT32     request_id;
    /** Requestor ID of the extscan operation that is currently running */
    A_UINT32     requestor_id;
    /** VDEV id(interface) of the extscan operation that is currently running */
    A_UINT32     vdev_id;
    /** table ID - to allow support for multiple simultaneous tables */
    A_UINT32     table_id;
    /**see wmi_extscan_table_type for table reporting usage */
    A_UINT32     table_type;
    /**number of entries in use */
    A_UINT32     entries_in_use;
    /**maximum number of entries in table */
    A_UINT32     maximum_entries;
} wmi_extscan_table_usage_event_fixed_param;

typedef struct {
    A_UINT32    tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_ARRAY_STRUC */
    /**RSSI */
    A_UINT32    rssi;
    /**time stamp in seconds */
    A_UINT32    tstamp;
} wmi_extscan_rssi_info;

typedef struct {
    A_UINT32        tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_ARRAY_STRUC */
    /**bssid */
    wmi_mac_addr    bssid;
    /**ssid */
    wmi_ssid        ssid;
    /**channel number */
    A_UINT32        channel;
    /* capabilities */
    A_UINT32        capabilities;
    /* beacon interval in TUs */
    A_UINT32        beacon_interval;
    /**time stamp in seconds - time last seen */
    A_UINT32        tstamp;
    /**flags - _tExtScanEntryFlags */
    A_UINT32        flags;
    /**RTT in ns */
    A_UINT32        rtt;
    /**rtt standard deviation */
    A_UINT32        rtt_sd;
    /* rssi information */
    A_UINT32        number_rssi_samples;
    /** IE length */
    A_UINT32        ie_length;             // length of IE data
} wmi_extscan_wlan_descriptor;

typedef struct {
    A_UINT32     tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_cached_results_event_fixed_param */
    /** Request ID of the WMI_EXTSCAN_GET_CACHED_RESULTS_CMDID */
    A_UINT32     request_id;
    /** Requestor ID of the WMI_EXTSCAN_GET_CACHED_RESULTS_CMDID */
    A_UINT32     requestor_id;
    /** VDEV id(interface) of the WMI_EXTSCAN_GET_CACHED_RESULTS_CMDID */
    A_UINT32     vdev_id;
    /** Request ID of the extscan operation that is currently running */
    A_UINT32     extscan_request_id;
    /** Requestor ID of the extscan operation that is currently running */
    A_UINT32     extscan_requestor_id;
    /** VDEV id(interface) of the extscan operation that is currently running */
    A_UINT32     extscan_vdev_id;
    /** table ID - to allow support for multiple simultaneous tables */
    A_UINT32     table_id;
    /**current time stamp in seconds. Used to provide a baseline for the relative timestamps returned for each block and entry */
    A_UINT32     current_tstamp;
    /**total number of bssids (in all pages) */
    A_UINT32     total_entries;
    /**index of the first bssid entry found in the TLV wmi_extscan_wlan_descriptor*/
    A_UINT32     first_entry_index;
    /**number of bssids in this page */
    A_UINT32     num_entries_in_page;
    /* Followed by the variable length TLVs
     *     wmi_extscan_wlan_descriptor    bssid_list[]
     *     wmi_extscan_rssi_info          rssi_list[]
     *     A_UINT8                        ie_list[]
     */
} wmi_extscan_cached_results_event_fixed_param;

typedef enum {
    EXTSCAN_WLAN_CHANGE_FLAG_NONE         = 0x00,
    EXTSCAN_WLAN_CHANGE_FLAG_OUT_OF_RANGE = 0x01,
    EXTSCAN_WLAN_CHANGE_FLAG_AP_LOST      = 0x02,
} wmi_extscan_wlan_change_flags;

typedef struct {
    A_UINT32        tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_ARRAY_STRUC */
    /**bssid */
    wmi_mac_addr    bssid;
    /**time stamp in seconds */
    A_UINT32        tstamp;
    /**upper RSSI limit */
    A_UINT32        upper_rssi_limit;
    /**lower RSSI limit */
    A_UINT32        lower_rssi_limit;
    /** channel */
    A_UINT32        channel;    /* in MHz */
    /**current RSSI average */
    A_UINT32        rssi_average;
    /**flags - wmi_extscan_wlan_change_flags */
    A_UINT32        flags;
    /**legnth of RSSI history to follow (number of values) */
    A_UINT32        num_rssi_samples;
} wmi_extscan_wlan_change_result_bssid;

typedef struct {
    A_UINT32     tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_wlan_change_results_event_fixed_param */
    /** Request ID of the WMI_EXTSCAN_GET_WLAN_CHANGE_RESULTS_CMDID command that requested the results */
    A_UINT32     request_id;
    /** Requestor ID of the WMI_EXTSCAN_GET_WLAN_CHANGE_RESULTS_CMDID command that requested the results */
    A_UINT32     requestor_id;
    /** VDEV id(interface) of the WMI_EXTSCAN_GET_WLAN_CHANGE_RESULTS_CMDID command that requested the results */
    A_UINT32     vdev_id;
    /** Request ID of the WMI_EXTSCAN_CONFIGURE_WLAN_CHANGE_MONITOR_CMDID command that configured the table */
    A_UINT32     config_request_id;
    /** Requestor ID of the WMI_EXTSCAN_CONFIGURE_WLAN_CHANGE_MONITOR_CMDID command that configured the table */
    A_UINT32     config_requestor_id;
    /** VDEV id(interface) of the WMI_EXTSCAN_CONFIGURE_WLAN_CHANGE_MONITOR_CMDID command that configured the table */
    A_UINT32     config_vdev_id;
    /** table ID - to allow support for multiple simultaneous tables */
    A_UINT32     table_id;
    /**number of entries with RSSI out of range or BSSID not detected */
    A_UINT32     change_count;
    /**total number of bssid signal descriptors (in all pages) */
    A_UINT32     total_entries;
    /**index of the first bssid signal descriptor entry found in the TLV wmi_extscan_wlan_descriptor*/
    A_UINT32     first_entry_index;
    /**number of bssids signal descriptors in this page */
    A_UINT32     num_entries_in_page;
    /* Following this structure is the TLV:
     *     wmi_extscan_wlan_change_result_bssid bssid_signal_descriptor_list[];    // number of descriptors given by field num_entries_in_page.
     * Following this structure is the list of RSSI values (each is an A_UINT8):
     *     A_UINT8 rssi_list[];    // last N RSSI values.
     */
} wmi_extscan_wlan_change_results_event_fixed_param;

enum _tExtScanEntryFlags
{
    WMI_HOTLIST_FLAG_NONE     = 0x00,
    WMI_HOTLIST_FLAG_PRESENCE = 0x01
};

typedef struct {
    A_UINT32     tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_hotlist_match_event_fixed_param */
    /** Request ID of the WMI_EXTSCAN_CONFIGURE_HOTLIST_MONITOR_CMDID that configured the table */
    A_UINT32     config_request_id;
    /** Requestor ID of the WMI_EXTSCAN_CONFIGURE_HOTLIST_MONITOR_CMDID that configured the table */
    A_UINT32     config_requestor_id;
    /** VDEV id(interface) of the WMI_EXTSCAN_CONFIGURE_HOTLIST_MONITOR_CMDID that configured the table */
    A_UINT32     config_vdev_id;
    /** table ID - to allow support for multiple simultaneous tables */
    A_UINT32     table_id;
    /**total number of bssids (in all pages) */
    A_UINT32     total_entries;
    /**index of the first bssid entry found in the TLV wmi_extscan_wlan_descriptor*/
    A_UINT32     first_entry_index;
    /**number of bssids in this page */
    A_UINT32     num_entries_in_page;
    /* Following this structure is the TLV:
     *     wmi_extscan_wlan_descriptor hotlist_match[];    // number of descriptors given by field num_entries_in_page.
     */
} wmi_extscan_hotlist_match_event_fixed_param;

typedef struct {
    A_UINT32     tlv_header; /* TLV tag and len; tag equals WMITLV_TAG_STRUC_wmi_extscan_capabilities_event_fixed_param */
    /** Request ID of the WMI_EXTSCAN_GET_CAPABILITIES_CMDID */
    A_UINT32     request_id;
    /** Requestor ID of the WMI_EXTSCAN_GET_CAPABILITIES_CMDID */
    A_UINT32     requestor_id;
    /** VDEV id(interface) of the WMI_EXTSCAN_GET_CAPABILITIES_CMDID */
    A_UINT32     vdev_id;
    /** number of extscan caches */
    A_UINT32     num_extscan_cache_tables;
    /** number of wlan change lists */
    A_UINT32     num_wlan_change_monitor_tables;
    /** number of hotlists */
    A_UINT32     num_hotlist_monitor_tables;
    /** if one sided rtt data collection is supported */
    A_UINT32     rtt_one_sided_supported;
    /** if 11v data collection is supported */
    A_UINT32     rtt_11v_supported;
    /** if 11mc data collection is supported */
    A_UINT32     rtt_ftm_supported;
    /** number of extscan cache capabilities (one per table)  */
    A_UINT32     num_extscan_cache_capabilities;
    /** number of wlan change  capabilities (one per table)  */
    A_UINT32     num_extscan_wlan_change_capabilities;
    /** number of extscan hotlist capabilities (one per table)  */
    A_UINT32     num_extscan_hotlist_capabilities;
    /* Following this structure are the TLVs describing the capabilities of of the various types of lists. The FW theoretically
     * supports multiple lists of each type.
     *
     *     wmi_extscan_cache_capabilities               extscan_cache_capabilities[] // capabilities of extscan cache (BSSID/RSSI lists)
     *     wmi_extscan_wlan_change_monitor_capabilities wlan_change_capabilities[]   // capabilities of wlan_change_monitor_tables
     *     wmi_extscan_hotlist_monitor_capabilities     hotlist_capabilities[]       // capabilities of hotlist_monitor_tables
     */
} wmi_extscan_capabilities_event_fixed_param;

#ifdef __cplusplus
}
#endif

#endif /*_WMI_UNIFIED_H_*/

/**@}*/
