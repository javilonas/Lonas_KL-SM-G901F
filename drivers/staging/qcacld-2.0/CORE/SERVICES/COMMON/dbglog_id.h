/*
 * Copyright (c) 2013 The Linux Foundation. All rights reserved.
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

#ifndef _DBGLOG_ID_H_
#define _DBGLOG_ID_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The target state machine framework will send dbglog messages on behalf on
 * other modules. We do this do avoid each target module adding identical
 * dbglog code for state transitions and event processing. We also don't want
 * to force each module to define the the same XXX_DBGID_SM_MSG with the same
 * value below. Instead we use a special ID that the host dbglog code
 * recognizes as a message sent by the SM on behalf on another module.
 */
#define DBGLOG_DBGID_SM_FRAMEWORK_PROXY_DBGLOG_MSG 1000

/*
 * The nomenclature for the debug identifiers is MODULE_DESCRIPTION.
 * Please ensure that the definition of any new debugid introduced is captured
 * between the <MODULE>_DBGID_DEFINITION_START and
 * <MODULE>_DBGID_DEFINITION_END defines. The structure is required for the
 * parser to correctly pick up the values for different debug identifiers.
 */

/*
* The target state machine framework will send dbglog messages on behalf on
* other modules. We do this do avoid each module adding identical dbglog code
* for state transitions and event processing. We also don't want to force each
* module to define the the same XXX_DBGID_SM_MSG with the same value below.
* Instead we use a special ID that the host dbglog code recognizes as a
* message sent by the SM on behalf on another module.
*/
#define DBGLOG_DBGID_SM_FRAMEWORK_PROXY_DBGLOG_MSG 1000


/* INF debug identifier definitions */
#define INF_DBGID_DEFINITION_START                    0
#define INF_ASSERTION_FAILED                          1
#define INF_TARGET_ID                                 2
#define INF_TARGET_MEM_REMAING			              3
#define INF_TARGET_MEM_EXT_REMAING                    4
#define INF_TARGET_MEM_ALLOC_TRACK                    5
#define INF_TARGET_MEM_ALLOC_RAM                      6
#define INF_DBGID_DEFINITION_END                      7

/* WMI debug identifier definitions */
#define WMI_DBGID_DEFINITION_START                    0
#define WMI_CMD_RX_XTND_PKT_TOO_SHORT                 1
#define WMI_EXTENDED_CMD_NOT_HANDLED                  2
#define WMI_CMD_RX_PKT_TOO_SHORT                      3
#define WMI_CALLING_WMI_EXTENSION_FN                  4
#define WMI_CMD_NOT_HANDLED                           5
#define WMI_IN_SYNC                                   6
#define WMI_TARGET_WMI_SYNC_CMD                       7
#define WMI_SET_SNR_THRESHOLD_PARAMS                  8
#define WMI_SET_RSSI_THRESHOLD_PARAMS                 9
#define WMI_SET_LQ_TRESHOLD_PARAMS                   10
#define WMI_TARGET_CREATE_PSTREAM_CMD                11
#define WMI_WI_DTM_INUSE                             12
#define WMI_TARGET_DELETE_PSTREAM_CMD                13
#define WMI_TARGET_IMPLICIT_DELETE_PSTREAM_CMD       14
#define WMI_TARGET_GET_BIT_RATE_CMD                  15
#define WMI_GET_RATE_MASK_CMD_FIX_RATE_MASK_IS       16
#define WMI_TARGET_GET_AVAILABLE_CHANNELS_CMD        17
#define WMI_TARGET_GET_TX_PWR_CMD                    18
#define WMI_FREE_EVBUF_WMIBUF                        19
#define WMI_FREE_EVBUF_DATABUF                       20
#define WMI_FREE_EVBUF_BADFLAG                       21
#define WMI_HTC_RX_ERROR_DATA_PACKET                 22
#define WMI_HTC_RX_SYNC_PAUSING_FOR_MBOX             23
#define WMI_INCORRECT_WMI_DATA_HDR_DROPPING_PKT      24
#define WMI_SENDING_READY_EVENT                      25
#define WMI_SETPOWER_MDOE_TO_MAXPERF                 26
#define WMI_SETPOWER_MDOE_TO_REC                     27
#define WMI_BSSINFO_EVENT_FROM                       28
#define WMI_TARGET_GET_STATS_CMD                     29
#define WMI_SENDING_SCAN_COMPLETE_EVENT              30
#define WMI_SENDING_RSSI_INDB_THRESHOLD_EVENT        31
#define WMI_SENDING_RSSI_INDBM_THRESHOLD_EVENT       32
#define WMI_SENDING_LINK_QUALITY_THRESHOLD_EVENT     33
#define WMI_SENDING_ERROR_REPORT_EVENT               34
#define WMI_SENDING_CAC_EVENT                        35
#define WMI_TARGET_GET_ROAM_TABLE_CMD                36
#define WMI_TARGET_GET_ROAM_DATA_CMD                 37
#define WMI_SENDING_GPIO_INTR_EVENT                  38
#define WMI_SENDING_GPIO_ACK_EVENT                   39
#define WMI_SENDING_GPIO_DATA_EVENT                  40
#define WMI_CMD_RX                                   41
#define WMI_CMD_RX_XTND                              42
#define WMI_EVENT_SEND                               43
#define WMI_EVENT_SEND_XTND                          44
#define WMI_CMD_PARAMS_DUMP_START                    45
#define WMI_CMD_PARAMS_DUMP_END                      46
#define WMI_CMD_PARAMS                               47
#define WMI_EVENT_ALLOC_FAILURE                      48
#define WMI_DBGID_DCS_PARAM_CMD                      49
#define WMI_SEND_EVENT_WRONG_TLV                     50
#define WMI_SEND_EVENT_NO_TLV_DEF                    51
#define WMI_DBGID_DEFINITION_END                     52

/*  PM Message definition*/
#define PS_STA_DEFINITION_START                     0
#define PS_STA_PM_ARB_REQUEST                       1
#define PS_STA_DELIVER_EVENT                        2
#define PS_STA_PSPOLL_SEQ_DONE                      3
#define PS_STA_COEX_MODE                            4
#define PS_STA_PSPOLL_ALLOW                         5
#define PS_STA_SET_PARAM                            6
#define PS_STA_SPECPOLL_TIMER_STARTED               7
#define PS_STA_SPECPOLL_TIMER_STOPPED               8
#define PS_STA_AVG_CHANNEL_CONGESTION               9
#define PS_STA_DEFINITION_END                       10

/** RESMGR dbg ids */
/* TODO: 1. Segregate IDs as per sub-module. (Give 100 per sub-module?)
 *       2. Add chmgr IDs over here.
 *       3. Update prints in dbglog_host.c
 *       4. Deprecate WLAN_MODULE_RESMGR_CHAN_MANAGER */
#define RESMGR_DEFINITION_START                     0
#define RESMGR_OCS_ALLOCRAM_SIZE                    1
#define RESMGR_OCS_RESOURCES                        2
#define RESMGR_LINK_CREATE                          3
#define RESMGR_LINK_DELETE                          4
#define RESMGR_OCS_CHREQ_CREATE                     5
#define RESMGR_OCS_CHREQ_DELETE                     6
#define RESMGR_OCS_CHREQ_START                      7
#define RESMGR_OCS_CHREQ_STOP                       8
#define RESMGR_OCS_SCHEDULER_INVOKED                9
#define RESMGR_OCS_CHREQ_GRANT                      10
#define RESMGR_OCS_CHREQ_COMPLETE                   11
#define RESMGR_OCS_NEXT_TSFTIME                     12
#define RESMGR_OCS_TSF_TIMEOUT_US                   13
#define RESMGR_OCS_CURR_CAT_WINDOW                  14
#define RESMGR_OCS_CURR_CAT_WINDOW_REQ              15
#define RESMGR_OCS_CURR_CAT_WINDOW_TIMESLOT         16
#define RESMGR_OCS_CHREQ_RESTART                    17
#define RESMGR_OCS_CLEANUP_CH_ALLOCATORS            18
#define RESMGR_OCS_PURGE_CHREQ                      19
#define RESMGR_OCS_CH_ALLOCATOR_FREE                20
#define RESMGR_OCS_RECOMPUTE_SCHEDULE               21
#define RESMGR_OCS_NEW_CAT_WINDOW_REQ               22
#define RESMGR_OCS_NEW_CAT_WINDOW_TIMESLOT          23
#define RESMGR_OCS_CUR_CH_ALLOC                     24
#define RESMGR_OCS_WIN_CH_ALLOC                     25
#define RESMGR_OCS_SCHED_CH_CHANGE                  26
#define RESMGR_OCS_CONSTRUCT_CAT_WIN                27
#define RESMGR_OCS_CHREQ_PREEMPTED                  28
#define RESMGR_OCS_CH_SWITCH_REQ                    29
#define RESMGR_OCS_CHANNEL_SWITCHED                 30
#define RESMGR_OCS_CLEANUP_STALE_REQS               31
#define RESMGR_OCS_CHREQ_UPDATE                     32
#define RESMGR_OCS_REG_NOA_NOTIF                    33
#define RESMGR_OCS_DEREG_NOA_NOTIF                  34
#define RESMGR_OCS_GEN_PERIODIC_NOA                 35
#define RESMGR_OCS_RECAL_QUOTAS                     36
#define RESMGR_OCS_GRANTED_QUOTA_STATS              37
#define RESMGR_OCS_ALLOCATED_QUOTA_STATS            38
#define RESMGR_OCS_REQ_QUOTA_STATS                  39
#define RESMGR_OCS_TRACKING_TIME_FIRED              40
#define RESMGR_VC_ARBITRATE_ATTRIBUTES              41
#define RESMGR_OCS_LATENCY_STRICT_TIME_SLOT         42
#define RESMGR_OCS_CURR_TSF                         43
#define RESMGR_OCS_QUOTA_REM                        44
#define RESMGR_OCS_LATENCY_CASE_NO                  45
#define RESMGR_OCS_WIN_CAT_DUR                      46
#define RESMGR_VC_UPDATE_CUR_VC                     47
#define RESMGR_VC_REG_UNREG_LINK                    48
#define RESMGR_VC_PRINT_LINK                        49
#define RESMGR_OCS_MISS_TOLERANCE                   50
#define RESMGR_DYN_SCH_ALLOCRAM_SIZE                51
#define RESMGR_DYN_SCH_ENABLE                       52
#define RESMGR_DYN_SCH_ACTIVE                       53
#define RESMGR_DYN_SCH_CH_STATS_START               54
#define RESMGR_DYN_SCH_CH_SX_STATS                  55
#define RESMGR_DYN_SCH_TOT_UTIL_PER                 56
#define RESMGR_DYN_SCH_HOME_CH_QUOTA                57
#define RESMGR_OCS_REG_RECAL_QUOTA_NOTIF            58
#define RESMGR_OCS_DEREG_RECAL_QUOTA_NOTIF          59
#define RESMGR_DYN_SCH_CH_STATS_END                 60
#define RESMGR_DEFINITION_END                       61

/* RESMGR CHNMGR debug ids */
#define RESMGR_CHMGR_DEFINITION_START               0
#define RESMGR_CHMGR_PAUSE_COMPLETE                 1
#define RESMGR_CHMGR_CHANNEL_CHANGE                 2
#define RESMGR_CHMGR_RESUME_COMPLETE                3
#define RESMGR_CHMGR_VDEV_PAUSE                     4
#define RESMGR_CHMGR_VDEV_UNPAUSE                   5
#define RESMGR_CHMGR_CTS2S_TX_COMP                  6
#define RESMGR_CHMGR_CFEND_TX_COMP                  7
#define RESMGR_CHMGR_DEFINITION_END                 8

/* VDEV manager debug ids */
#define VDEV_MGR_DEFINITION_START                   0
#define VDEV_MGR_FIRST_BMISS_DETECTED               1
#define VDEV_MGR_FINAL_BMISS_DETECTED               2
#define VDEV_MGR_BCN_IN_SYNC                        3
#define VDEV_MGR_AP_KEEPALIVE_IDLE                  4
#define VDEV_MGR_AP_KEEPALIVE_INACTIVE              5
#define VDEV_MGR_AP_KEEPALIVE_UNRESPONSIVE          6
#define VDEV_MGR_AP_TBTT_CONFIG                     7
#define VDEV_MGR_FIRST_BCN_RECEIVED                 8
#define VDEV_MGR_VDEV_START                         9
#define VDEV_MGR_VDEV_UP                            10
#define VDEV_MGR_PEER_AUTHORIZED                    11
#define VDEV_MGR_OCS_HP_LP_REQ_POSTED               12
#define VDEV_MGR_VDEV_START_OCS_HP_REQ_COMPLETE     13
#define VDEV_MGR_VDEV_START_OCS_HP_REQ_STOP         14
#define VDEV_MGR_HP_START_TIME                      15
#define VDEV_MGR_VDEV_PAUSE_DELAY_UPDATE            16
#define VDEV_MGR_VDEV_PAUSE_FAIL                    17
#define VDEV_MGR_GEN_PERIODIC_NOA                   18
#define VDEV_MGR_OFF_CHAN_GO_CH_REQ_SETUP           19
#define VDEV_MGR_DEFINITION_END                     20

/* WHAL debug identifier definitions */
#define WHAL_DBGID_DEFINITION_START                 0
#define WHAL_ERROR_ANI_CONTROL                      1
#define WHAL_ERROR_CHIP_TEST1                       2
#define WHAL_ERROR_CHIP_TEST2                       3
#define WHAL_ERROR_EEPROM_CHECKSUM                  4
#define WHAL_ERROR_EEPROM_MACADDR                   5
#define WHAL_ERROR_INTERRUPT_HIU                    6
#define WHAL_ERROR_KEYCACHE_RESET                   7
#define WHAL_ERROR_KEYCACHE_SET                     8
#define WHAL_ERROR_KEYCACHE_TYPE                    9
#define WHAL_ERROR_KEYCACHE_TKIPENTRY              10
#define WHAL_ERROR_KEYCACHE_WEPLENGTH              11
#define WHAL_ERROR_PHY_INVALID_CHANNEL             12
#define WHAL_ERROR_POWER_AWAKE                     13
#define WHAL_ERROR_POWER_SET                       14
#define WHAL_ERROR_RECV_STOPDMA                    15
#define WHAL_ERROR_RECV_STOPPCU                    16
#define WHAL_ERROR_RESET_CHANNF1                   17
#define WHAL_ERROR_RESET_CHANNF2                   18
#define WHAL_ERROR_RESET_PM                        19
#define WHAL_ERROR_RESET_OFFSETCAL                 20
#define WHAL_ERROR_RESET_RFGRANT                   21
#define WHAL_ERROR_RESET_RXFRAME                   22
#define WHAL_ERROR_RESET_STOPDMA                   23
#define WHAL_ERROR_RESET_ERRID                     24
#define WHAL_ERROR_RESET_ADCDCCAL1                 25
#define WHAL_ERROR_RESET_ADCDCCAL2                 26
#define WHAL_ERROR_RESET_TXIQCAL                   27
#define WHAL_ERROR_RESET_RXIQCAL                   28
#define WHAL_ERROR_RESET_CARRIERLEAK               29
#define WHAL_ERROR_XMIT_COMPUTE                    30
#define WHAL_ERROR_XMIT_NOQUEUE                    31
#define WHAL_ERROR_XMIT_ACTIVEQUEUE                32
#define WHAL_ERROR_XMIT_BADTYPE                    33
#define WHAL_ERROR_XMIT_STOPDMA                    34
#define WHAL_ERROR_INTERRUPT_BB_PANIC              35
#define WHAL_ERROR_PAPRD_MAXGAIN_ABOVE_WINDOW      36
#define WHAL_ERROR_QCU_HW_PAUSE_MISMATCH           37
#define WHAL_ERROR_POWER_RFLP_CONFIG               38
#define WHAL_ERROR_POWER_RFLP_SYNTHBYPASS_CONFIG   39
#define WHAL_ERROR_POWER_RFLP_BIAS2X_CONFIG        40
#define WHAL_ERROR_POWER_RFLP_PLLBYPASS_CONFIG     41
#define WHAL_ERROR_POWER_RFLP_OFF1CHAN_CONFIG      42
#define WHAL_ERROR_POWER_ANTENNA_LMIT              43
#define WHAL_ERROR_POWER_REGDMN_TX_LMIT            44
#define WHAL_ERROR_POWER_MODE_SCALED_PWR           45
#define WHAL_ERROR_POWER_EDGE_PWR_TPSCALE          46
#define WHAL_ERROR_POWER_CHAN_REGALLOW             47
#define WHAL_ERROR_WAIT_REG_TIMEOUT                48
#define WHAL_ERROR_XTAL_SET                        49
#define WHAL_DBGID_DEFINITION_END                  50

#define COEX_DEBUGID_START              0
#define BTCOEX_DBG_MCI_1                            1
#define BTCOEX_DBG_MCI_2                            2
#define BTCOEX_DBG_MCI_3                            3
#define BTCOEX_DBG_MCI_4                            4
#define BTCOEX_DBG_MCI_5                            5
#define BTCOEX_DBG_MCI_6                            6
#define BTCOEX_DBG_MCI_7                            7
#define BTCOEX_DBG_MCI_8                            8
#define BTCOEX_DBG_MCI_9                            9
#define BTCOEX_DBG_MCI_10                           10
#define COEX_WAL_BTCOEX_INIT                        11
#define COEX_WAL_PAUSE                              12
#define COEX_WAL_RESUME                             13
#define COEX_UPDATE_AFH                             14
#define COEX_HWQ_EMPTY_CB                           15
#define COEX_MCI_TIMER_HANDLER                      16
#define COEX_MCI_RECOVER                            17
#define ERROR_COEX_MCI_ISR                          18
#define ERROR_COEX_MCI_GPM                          19
#define COEX_ProfileType                            20
#define COEX_LinkID                                 21
#define COEX_LinkState                              22
#define COEX_LinkRole                               23
#define COEX_LinkRate                               24
#define COEX_VoiceType                              25
#define COEX_TInterval                              26
#define COEX_WRetrx                                 27
#define COEX_Attempts                               28
#define COEX_PerformanceState                       29
#define COEX_LinkType                               30
#define COEX_RX_MCI_GPM_VERSION_QUERY               31
#define COEX_RX_MCI_GPM_VERSION_RESPONSE            32
#define COEX_RX_MCI_GPM_STATUS_QUERY                33
#define COEX_STATE_WLAN_VDEV_DOWN                   34
#define COEX_STATE_WLAN_VDEV_START                  35
#define COEX_STATE_WLAN_VDEV_CONNECTED              36
#define COEX_STATE_WLAN_VDEV_SCAN_STARTED           37
#define COEX_STATE_WLAN_VDEV_SCAN_END               38
#define COEX_STATE_WLAN_DEFAULT                     39
#define COEX_CHANNEL_CHANGE                         40
#define COEX_POWER_CHANGE                           41
#define COEX_CONFIG_MGR                             42
#define COEX_TX_MCI_GPM_BT_CAL_REQ                  43
#define COEX_TX_MCI_GPM_BT_CAL_GRANT                44
#define COEX_TX_MCI_GPM_BT_CAL_DONE                 45
#define COEX_TX_MCI_GPM_WLAN_CAL_REQ                46
#define COEX_TX_MCI_GPM_WLAN_CAL_GRANT              47
#define COEX_TX_MCI_GPM_WLAN_CAL_DONE               48
#define COEX_TX_MCI_GPM_BT_DEBUG                    49
#define COEX_TX_MCI_GPM_VERSION_QUERY               50
#define COEX_TX_MCI_GPM_VERSION_RESPONSE            51
#define COEX_TX_MCI_GPM_STATUS_QUERY                52
#define COEX_TX_MCI_GPM_HALT_BT_GPM                 53
#define COEX_TX_MCI_GPM_WLAN_CHANNELS               54
#define COEX_TX_MCI_GPM_BT_PROFILE_INFO             55
#define COEX_TX_MCI_GPM_BT_STATUS_UPDATE            56
#define COEX_TX_MCI_GPM_BT_UPDATE_FLAGS             57
#define COEX_TX_MCI_GPM_UNKNOWN                     58
#define COEX_TX_MCI_SYS_WAKING                      59
#define COEX_TX_MCI_LNA_TAKE                        60
#define COEX_TX_MCI_LNA_TRANS                       61
#define COEX_TX_MCI_SYS_SLEEPING                    62
#define COEX_TX_MCI_REQ_WAKE                        63
#define COEX_TX_MCI_REMOTE_RESET                    64
#define COEX_TX_MCI_TYPE_UNKNOWN                    65
#define COEX_WHAL_MCI_RESET                         66
#define COEX_POLL_BT_CAL_DONE_TIMEOUT               67
#define COEX_WHAL_PAUSE                             68
#define COEX_RX_MCI_GPM_BT_CAL_REQ                  69
#define COEX_RX_MCI_GPM_BT_CAL_DONE                 70
#define COEX_RX_MCI_GPM_BT_CAL_GRANT                71
#define COEX_WLAN_CAL_START                         72
#define COEX_WLAN_CAL_RESULT                        73
#define COEX_BtMciState                             74
#define COEX_BtCalState                             75
#define COEX_WlanCalState                           76
#define COEX_RxReqWakeCount                         77
#define COEX_RxRemoteResetCount                     78
#define COEX_RESTART_CAL                            79
#define COEX_SENDMSG_QUEUE                          80
#define COEX_RESETSEQ_LNAINFO_TIMEOUT               81
#define COEX_MCI_ISR_IntRaw                         82
#define COEX_MCI_ISR_Int1Raw                        83
#define COEX_MCI_ISR_RxMsgRaw                       84
#define COEX_WHAL_COEX_RESET                        85
#define COEX_WAL_COEX_INIT                          86
#define COEX_TXRX_CNT_LIMIT_ISR                     87
#define COEX_CH_BUSY                                88
#define COEX_REASSESS_WLAN_STATE                    89
#define COEX_BTCOEX_WLAN_STATE_UPDATE               90
#define COEX_BT_NUM_OF_PROFILES                     91
#define COEX_BT_NUM_OF_HID_PROFILES                 92
#define COEX_BT_NUM_OF_ACL_PROFILES                 93
#define COEX_BT_NUM_OF_HI_ACL_PROFILES              94
#define COEX_BT_NUM_OF_VOICE_PROFILES               95
#define COEX_WLAN_AGGR_LIMIT                        96
#define COEX_BT_LOW_PRIO_BUDGET                     97
#define COEX_BT_HI_PRIO_BUDGET                      98
#define COEX_BT_IDLE_TIME                           99
#define COEX_SET_COEX_WEIGHT                        100
#define COEX_WLAN_WEIGHT_GROUP                      101
#define COEX_BT_WEIGHT_GROUP                        102
#define COEX_BT_INTERVAL_ALLOC                      103
#define COEX_BT_SCHEME                              104
#define COEX_BT_MGR                                 105
#define COEX_BT_SM_ERROR                            106
#define COEX_SYSTEM_UPDATE                          107
#define COEX_LOW_PRIO_LIMIT                         108
#define COEX_HI_PRIO_LIMIT                          109
#define COEX_BT_INTERVAL_START                      110
#define COEX_WLAN_INTERVAL_START                    111
#define COEX_NON_LINK_BUDGET                        112
#define COEX_CONTENTION_MSG                         113
#define COEX_SET_NSS                                114
#define COEX_SELF_GEN_MASK                          115
#define COEX_PROFILE_ERROR                          116
#define COEX_WLAN_INIT                              117
#define COEX_BEACON_MISS                            118
#define COEX_BEACON_OK                              119
#define COEX_BTCOEX_SCAN_ACTIVITY                   120
#define COEX_SCAN_ACTIVITY                          121
#define COEX_FORCE_QUIETTIME                        122
#define COEX_BT_MGR_QUIETTIME                       123
#define COEX_BT_INACTIVITY_TRIGGER                  124
#define COEX_BT_INACTIVITY_REPORTED                 125
#define COEX_TX_MCI_GPM_WLAN_PRIO                   126
#define COEX_TX_MCI_GPM_BT_PAUSE_PROFILE            127
#define COEX_TX_MCI_GPM_WLAN_SET_ACL_INACTIVITY     128
#define COEX_RX_MCI_GPM_BT_ACL_INACTIVITY_REPORT    129
#define COEX_GENERIC_ERROR                          130
#define COEX_RX_RATE_THRESHOLD                      131
#define COEX_RSSI                                   132

#define COEX_WLAN_VDEV_NOTIF_START                  133
#define COEX_WLAN_VDEV_NOTIF_UP                     134
#define COEX_WLAN_VDEV_NOTIF_DOWN                   135
#define COEX_WLAN_VDEV_NOTIF_STOP                   136
#define COEX_WLAN_VDEV_NOTIF_ADD_PEER               137
#define COEX_WLAN_VDEV_NOTIF_DELETE_PEER            138
#define COEX_WLAN_VDEV_NOTIF_CONNECTED_PEER         139
#define COEX_WLAN_VDEV_NOTIF_PAUSE                  140
#define COEX_WLAN_VDEV_NOTIF_UNPAUSED               141
#define COEX_STATE_WLAN_VDEV_PEER_ADD               142
#define COEX_STATE_WLAN_VDEV_CONNECTED_PEER         143
#define COEX_STATE_WLAN_VDEV_DELETE_PEER            144
#define COEX_STATE_WLAN_VDEV_PAUSE                  145
#define COEX_STATE_WLAN_VDEV_UNPAUSED               146
#define COEX_SCAN_CALLBACK                          147
#define COEX_RC_SET_CHAINMASK                       148
#define COEX_TX_MCI_GPM_WLAN_SET_BT_RXSS_THRES      149
#define COEX_TX_MCI_GPM_BT_RXSS_THRES_QUERY         150
#define COEX_BT_RXSS_THRES                          151
#define COEX_BT_PROFILE_ADD_RMV                     152
#define COEX_BT_SCHED_INFO                          153
#define COEX_TRF_MGMT                               154
#define COEX_SCHED_START                            155
#define COEX_SCHED_RESULT                           156
#define COEX_SCHED_ERROR                            157
#define COEX_SCHED_PRE_OP                           158
#define COEX_SCHED_POST_OP                          159
#define COEX_RX_RATE                                160
#define COEX_ACK_PRIORITY                           161
#define COEX_STATE_WLAN_VDEV_UP                     162
#define COEX_STATE_WLAN_VDEV_PEER_UPDATE            163
#define COEX_STATE_WLAN_VDEV_STOP                   164
#define COEX_WLAN_PAUSE_PEER                        165
#define COEX_WLAN_UNPAUSE_PEER                      166
#define COEX_WLAN_PAUSE_INTERVAL_START              167
#define COEX_WLAN_POSTPAUSE_INTERVAL_START          168
#define COEX_TRF_FREERUN                            169
#define COEX_TRF_SHAPE_PM                           170
#define COEX_TRF_SHAPE_PSP                          171
#define COEX_TRF_SHAPE_S_CTS                        172
#define COEX_CHAIN_CONFIG                           173
#define COEX_SYSTEM_MONITOR                         174
#define COEX_SINGLECHAIN_INIT                       175
#define COEX_MULTICHAIN_INIT                        176
#define COEX_SINGLECHAIN_DBG_1                      177
#define COEX_SINGLECHAIN_DBG_2                      178
#define COEX_SINGLECHAIN_DBG_3                      179
#define COEX_MULTICHAIN_DBG_1                       180
#define COEX_MULTICHAIN_DBG_2                       181
#define COEX_MULTICHAIN_DBG_3                       182
#define COEX_PSP_TX_CB                              183
#define COEX_PSP_RX_CB                              184
#define COEX_PSP_STAT_1                             185
#define COEX_PSP_SPEC_POLL                          186
#define COEX_PSP_READY_STATE                        187
#define COEX_PSP_TX_STATUS_STATE                    188
#define COEX_PSP_RX_STATUS_STATE_1                  189
#define COEX_PSP_NOT_READY_STATE                    190
#define COEX_PSP_DISABLED_STATE                     191
#define COEX_PSP_ENABLED_STATE                      192
#define COEX_PSP_SEND_PSPOLL                        193
#define COEX_PSP_MGR_ENTER                          194
#define COEX_PSP_MGR_RESULT                         195
#define COEX_PSP_NONWLAN_INTERVAL                   196
#define COEX_PSP_STAT_2                             197
#define COEX_PSP_RX_STATUS_STATE_2                  198
#define COEX_PSP_ERROR                              199
#define COEX_T2BT                                   200
#define COEX_BT_DURATION                            201
#define COEX_TX_MCI_GPM_WLAN_SCHED_INFO_TRIG        202
#define COEX_TX_MCI_GPM_WLAN_SCHED_INFO_TRIG_RSP    203
#define COEX_TX_MCI_GPM_SCAN_OP                     204
#define COEX_TX_MCI_GPM_BT_PAUSE_GPM_TX             205
#define COEX_CTS2S_SEND                             206
#define COEX_CTS2S_RESULT                           207
#define COEX_ENTER_OCS                              208
#define COEX_EXIT_OCS                               209
#define COEX_UPDATE_OCS                             210
#define COEX_STATUS_OCS                             211
#define COEX_STATS_BT                               212

#define COEX_MWS_WLAN_INIT                          213
#define COEX_MWS_WBTMR_SYNC                         214
#define COEX_MWS_TYPE2_RX                           215
#define COEX_MWS_TYPE2_TX                           216
#define COEX_MWS_WLAN_CHAVD                         217
#define COEX_MWS_WLAN_CHAVD_INSERT                  218
#define COEX_MWS_WLAN_CHAVD_MERGE                   219
#define COEX_MWS_WLAN_CHAVD_RPT                     220
#define COEX_MWS_CP_MSG_SEND                        221
#define COEX_MWS_CP_ESCAPE                          222
#define COEX_MWS_CP_UNFRAME                         223
#define COEX_MWS_CP_SYNC_UPDATE                     224
#define COEX_MWS_CP_SYNC                            225
#define COEX_MWS_CP_WLAN_STATE_IND                  226
#define COEX_MWS_CP_SYNCRESP_TIMEOUT                227
#define COEX_MWS_SCHEME_UPDATE                      228
#define COEX_MWS_WLAN_EVENT                         229
#define COEX_MWS_UART_UNESCAPE                      230
#define COEX_MWS_UART_ENCODE_SEND                   231
#define COEX_MWS_UART_RECV_DECODE                   232
#define COEX_MWS_UL_HDL                             233
#define COEX_MWS_REMOTE_EVENT                       234
#define COEX_MWS_OTHER                              235
#define COEX_MWS_ERROR                              236
#define COEX_MWS_ANT_DIVERSITY                      237

#define COEX_P2P_GO                                 238
#define COEX_P2P_CLIENT                             239
#define COEX_SCC_1                                  240
#define COEX_SCC_2                                  241
#define COEX_MCC_1                                  242
#define COEX_MCC_2                                  243
#define COEX_TRF_SHAPE_NOA                          244
#define COEX_NOA_ONESHOT                            245
#define COEX_NOA_PERIODIC                           246
#define COEX_LE_1                                   247
#define COEX_LE_2                                   248
#define COEX_ANT_1                                  249
#define COEX_ANT_2                                  250
#define COEX_ENTER_NOA                              251
#define COEX_EXIT_NOA                               252
#define COEX_BT_SCAN_PROTECT                        253

#define COEX_DEBUG_ID_END                           254

#define SCAN_START_COMMAND_FAILED                   0
#define SCAN_STOP_COMMAND_FAILED                    1
#define SCAN_EVENT_SEND_FAILED                      2
#define SCAN_ENGINE_START                           3
#define SCAN_ENGINE_CANCEL_COMMAND                  4
#define SCAN_ENGINE_STOP_DUE_TO_TIMEOUT             5
#define SCAN_EVENT_SEND_TO_HOST                     6
#define SCAN_FWLOG_EVENT_ADD                        7
#define SCAN_FWLOG_EVENT_REM                        8
#define SCAN_FWLOG_EVENT_PREEMPTED                  9
#define SCAN_FWLOG_EVENT_RESTARTED                  10
#define SCAN_FWLOG_EVENT_COMPLETED                  11

#define BEACON_EVENT_SWBA_SEND_FAILED               0
#define BEACON_EVENT_EARLY_RX_BMISS_STATUS          1
#define BEACON_EVENT_EARLY_RX_SLEEP_SLOP            2
#define BEACON_EVENT_EARLY_RX_CONT_BMISS_TIMEOUT    3
#define BEACON_EVENT_EARLY_RX_PAUSE_SKIP_BCN_NUM    4
#define BEACON_EVENT_EARLY_RX_CLK_DRIFT             5
#define BEACON_EVENT_EARLY_RX_AP_DRIFT              6
#define BEACON_EVENT_EARLY_RX_BCN_TYPE              7

#define RATECTRL_DBGID_DEFINITION_START             0
#define RATECTRL_DBGID_ASSOC                        1
#define RATECTRL_DBGID_NSS_CHANGE                   2
#define RATECTRL_DBGID_CHAINMASK_ERR                3
#define RATECTRL_DBGID_UNEXPECTED_FRAME             4
#define RATECTRL_DBGID_WAL_RCQUERY                  5
#define RATECTRL_DBGID_WAL_RCUPDATE                 6
#define RATECTRL_DBGID_GTX_UPDATE                   7
#define RATECTRL_DBGID_DEFINITION_END               8

#define AP_PS_DBGID_DEFINITION_START                0
#define AP_PS_DBGID_UPDATE_TIM                      1
#define AP_PS_DBGID_PEER_STATE_CHANGE               2
#define AP_PS_DBGID_PSPOLL                          3
#define AP_PS_DBGID_PEER_CREATE                     4
#define AP_PS_DBGID_PEER_DELETE                     5
#define AP_PS_DBGID_VDEV_CREATE                     6
#define AP_PS_DBGID_VDEV_DELETE                     7
#define AP_PS_DBGID_SYNC_TIM                        8
#define AP_PS_DBGID_NEXT_RESPONSE                   9
#define AP_PS_DBGID_START_SP                        10
#define AP_PS_DBGID_COMPLETED_EOSP                  11
#define AP_PS_DBGID_TRIGGER                         12
#define AP_PS_DBGID_DUPLICATE_TRIGGER               13
#define AP_PS_DBGID_UAPSD_RESPONSE                  14
#define AP_PS_DBGID_SEND_COMPLETE                   15
#define AP_PS_DBGID_SEND_N_COMPLETE                 16
#define AP_PS_DBGID_DETECT_OUT_OF_SYNC_STA          17
#define AP_PS_DBGID_DELIVER_CAB                     18
#define AP_PS_DBGID_NO_CLIENT                       27
#define AP_PS_DBGID_CLIENT_IN_PS_ACTIVE             28
#define AP_PS_DBGID_CLIENT_IN_PS_NON_ACTIVE         29
#define AP_PS_DBGID_CLIENT_IN_AWAKE                 30

/* WLAN_MODULE_MGMT_TXRX Debugids*/
#define MGMT_TXRX_DBGID_DEFINITION_START            0
#define MGMT_TXRX_FORWARD_TO_HOST                   1
#define MGMT_TXRX_DBGID_DEFINITION_END              2

#define WAL_DBGID_DEFINITION_START                  0
#define WAL_DBGID_FAST_WAKE_REQUEST                 1
#define WAL_DBGID_FAST_WAKE_RELEASE                 2
#define WAL_DBGID_SET_POWER_STATE                   3
#define WAL_DBGID_CHANNEL_CHANGE_FORCE_RESET        5
#define WAL_DBGID_CHANNEL_CHANGE                    6
#define WAL_DBGID_VDEV_START                        7
#define WAL_DBGID_VDEV_STOP                         8
#define WAL_DBGID_VDEV_UP                           9
#define WAL_DBGID_VDEV_DOWN                         10
#define WAL_DBGID_SW_WDOG_RESET                     11
#define WAL_DBGID_TX_SCH_REGISTER_TIDQ              12
#define WAL_DBGID_TX_SCH_UNREGISTER_TIDQ            13
#define WAL_DBGID_TX_SCH_TICKLE_TIDQ                14


#define WAL_DBGID_XCESS_FAILURES                    15
#define WAL_DBGID_AST_ADD_WDS_ENTRY                 16
#define WAL_DBGID_AST_DEL_WDS_ENTRY                 17
#define WAL_DBGID_AST_WDS_ENTRY_PEER_CHG            18
#define WAL_DBGID_AST_WDS_SRC_LEARN_FAIL            19
#define WAL_DBGID_STA_KICKOUT                       20
#define WAL_DBGID_BAR_TX_FAIL                       21
#define WAL_DBGID_BAR_ALLOC_FAIL                    22
#define WAL_DBGID_LOCAL_DATA_TX_FAIL                23
#define WAL_DBGID_SECURITY_PM4_QUEUED               24
#define WAL_DBGID_SECURITY_GM1_QUEUED               25
#define WAL_DBGID_SECURITY_PM4_SENT                 26
#define WAL_DBGID_SECURITY_ALLOW_DATA               27
#define WAL_DBGID_SECURITY_UCAST_KEY_SET            28
#define WAL_DBGID_SECURITY_MCAST_KEY_SET            29
#define WAL_DBGID_SECURITY_ENCR_EN                  30
#define WAL_DBGID_BB_WDOG_TRIGGERED                 31
#define WAL_DBGID_RX_LOCAL_BUFS_LWM                 32
#define WAL_DBGID_RX_LOCAL_DROP_LARGE_MGMT          33
#define WAL_DBGID_VHT_ILLEGAL_RATE_PHY_ERR_DETECTED 34
#define WAL_DBGID_DEV_RESET                         35
#define WAL_DBGID_TX_BA_SETUP                       36
#define WAL_DBGID_RX_BA_SETUP                       37
#define WAL_DBGID_DEV_TX_TIMEOUT                    38
#define WAL_DBGID_DEV_RX_TIMEOUT                    39
#define WAL_DBGID_STA_VDEV_XRETRY                   40
#define WAL_DBGID_DCS                               41
#define WAL_DBGID_MGMT_TX_FAIL                      42
#define WAL_DBGID_SET_M4_SENT_MANUALLY              43
#define WAL_DBGID_PROCESS_4_WAY_HANDSHAKE           44
#define WAL_DBGID_WAL_CHANNEL_CHANGE_START          45
#define WAL_DBGID_WAL_CHANNEL_CHANGE_COMPLETE       46
#define WAL_DBGID_WHAL_CHANNEL_CHANGE_START         47
#define WAL_DBGID_WHAL_CHANNEL_CHANGE_COMPLETE      48
#define WAL_DBGID_TX_MGMT_DESCID_SEQ_TYPE_LEN       49
#define WAL_DBGID_TX_DATA_MSDUID_SEQ_TYPE_LEN       50
#define WAL_DBGID_TX_DISCARD                        51
#define WAL_DBGID_TX_MGMT_COMP_DESCID_STATUS        52
#define WAL_DBGID_TX_DATA_COMP_MSDUID_STATUS        53
#define WAL_DBGID_RESET_PCU_CYCLE_CNT               54
#define WAL_DBGID_SETUP_RSSI_INTERRUPTS             55
#define WAL_DBGID_BRSSI_CONFIG                      56
#define WAL_DBGID_CURRENT_BRSSI_AVE                 57
#define WAL_DBGID_BCN_TX_COMP                       58
#define WAL_DBGID_RX_REENTRY                        59
#define WAL_DBGID_SET_HW_CHAINMASK                  60
#define WAL_DBGID_SET_HW_CHAINMASK_TXRX_STOP_FAIL   61
#define WAL_DBGID_GET_HW_CHAINMASK                  62
#define WAL_DBGID_SMPS_DISABLE                      63
#define WAL_DBGID_SMPS_ENABLE_HW_CNTRL              64
#define WAL_DBGID_SMPS_SWSEL_CHAINMASK              65
#define WAL_DBGID_DEFINITION_END                    66

#define ANI_DBGID_POLL                               0
#define ANI_DBGID_CONTROL                            1
#define ANI_DBGID_OFDM_PARAMS                        2
#define ANI_DBGID_CCK_PARAMS                         3
#define ANI_DBGID_RESET                              4
#define ANI_DBGID_RESTART                            5
#define ANI_DBGID_OFDM_LEVEL                         6
#define ANI_DBGID_CCK_LEVEL                          7
#define ANI_DBGID_FIRSTEP                            8
#define ANI_DBGID_CYCPWR                             9
#define ANI_DBGID_MRC_CCK                           10
#define ANI_DBGID_SELF_CORR_LOW                     11
#define ANI_DBGID_ENABLE                            12

#define ANI_DBGID_CURRENT_LEVEL                     13
#define ANI_DBGID_POLL_PERIOD                       14
#define ANI_DBGID_LISTEN_PERIOD                     15
#define ANI_DBGID_OFDM_LEVEL_CFG                    16
#define ANI_DBGID_CCK_LEVEL_CFG                     17

/* OFFLOAD Manager Debugids*/
#define OFFLOAD_MGR_DBGID_DEFINITION_START             0
#define OFFLOADMGR_REGISTER_OFFLOAD                    1
#define OFFLOADMGR_DEREGISTER_OFFLOAD                  2
#define OFFLOADMGR_NO_REG_DATA_HANDLERS                3
#define OFFLOADMGR_NO_REG_EVENT_HANDLERS               4
#define OFFLOADMGR_REG_OFFLOAD_FAILED                  5
#define OFFLOADMGR_DEREG_OFFLOAD_FAILED                6
#define OFFLOADMGR_ENTER_FAILED                        7
#define OFFLOADMGR_EXIT_FAILED                         8
#define OFFLOADMGR_DBGID_DEFINITION_END                9

/*Resource Debug IDs*/
#define RESOURCE_DBGID_DEFINITION_START             0
#define RESOURCE_PEER_ALLOC                         1
#define RESOURCE_PEER_FREE                          2
#define RESOURCE_PEER_ALLOC_WAL_PEER                3
#define RESOURCE_PEER_NBRHOOD_MGMT_ALLOC            4
#define RESOURCE_PEER_NBRHOOD_MGMT_INFO             5
#define RESOURCE_DBGID_DEFINITION_END               6

/* DCS debug IDs*/
#define WLAN_DCS_DBGID_INIT                         0
#define WLAN_DCS_DBGID_WMI_CWINT                    1
#define WLAN_DCS_DBGID_TIMER                        2
#define WLAN_DCS_DBGID_CMDG                         3
#define WLAN_DCS_DBGID_CMDS                         4
#define WLAN_DCS_DBGID_DINIT                        5

/*P2P Module ids*/
#define P2P_DBGID_DEFINITION_START                          0
#define P2P_DEV_REGISTER                                    1
#define P2P_HANDLE_NOA                                      2
#define P2P_UPDATE_SCHEDULE_OPPS                            3
#define P2P_UPDATE_SCHEDULE                                 4
#define P2P_UPDATE_START_TIME                               5
#define P2P_UPDATE_START_TIME_DIFF_TSF32                    6
#define P2P_UPDATE_START_TIME_FINAL                         7
#define P2P_SETUP_SCHEDULE_TIMER                            8
#define P2P_PROCESS_SCHEDULE_AFTER_CALC                     9
#define P2P_PROCESS_SCHEDULE_STARTED_TIMER                  10
#define P2P_CALC_SCHEDULES_FIRST_CALL_ALL_NEXT_EVENT        11
#define P2P_CALC_SCHEDULES_FIRST_VALUE                      12
#define P2P_CALC_SCHEDULES_EARLIEST_NEXT_EVENT              13
#define P2P_CALC_SCHEDULES_SANITY_COUNT                     14
#define P2P_CALC_SCHEDULES_CALL_ALL_NEXT_EVENT_FROM_WHILE_LOOP 15
#define P2P_CALC_SCHEDULES_TIMEOUT_1                        16
#define P2P_CALC_SCHEDULES_TIMEOUT_2                        17
#define P2P_FIND_ALL_NEXT_EVENTS_REQ_EXPIRED                18
#define P2P_FIND_ALL_NEXT_EVENTS_REQ_ACTIVE                 19
#define P2P_FIND_NEXT_EVENT_REQ_NOT_STARTED                 20
#define P2P_FIND_NEXT_EVENT_REQ_COMPLETE_NON_PERIODIC       21
#define P2P_FIND_NEXT_EVENT_IN_MID_OF_NOA                   22
#define P2P_FIND_NEXT_EVENT_REQ_COMPLETE                    23
#define P2P_SCHEDULE_TIMEOUT                                24
#define P2P_CALC_SCHEDULES_ENTER                            25
#define P2P_PROCESS_SCHEDULE_ENTER                          26
#define P2P_FIND_ALL_NEXT_EVENTS_INDIVIDUAL_REQ_AFTER_CHANGE    27
#define P2P_FIND_ALL_NEXT_EVENTS_INDIVIDUAL_REQ_BEFORE_CHANGE   28
#define P2P_FIND_ALL_NEXT_EVENTS_ENTER                      29
#define P2P_FIND_NEXT_EVENT_ENTER                           30
#define P2P_NOA_GO_PRESENT                                  31
#define P2P_NOA_GO_ABSENT                                   32
#define P2P_GO_NOA_NOTIF                                    33
#define P2P_GO_TBTT_OFFSET                                  34
#define P2P_GO_GET_NOA_INFO                                 35
#define P2P_GO_ADD_ONE_SHOT_NOA                             36
#define P2P_GO_GET_NOA_IE                                   37
#define P2P_GO_BCN_TX_COMP                                  38
#define P2P_DBGID_DEFINITION_END                            39


//CSA modules DBGIDs
#define CSA_DBGID_DEFINITION_START 0
#define CSA_OFFLOAD_POOL_INIT 1
#define CSA_OFFLOAD_REGISTER_VDEV 2
#define CSA_OFFLOAD_DEREGISTER_VDEV 3
#define CSA_DEREGISTER_VDEV_ERROR 4
#define CSA_OFFLOAD_BEACON_RECEIVED 5
#define CSA_OFFLOAD_BEACON_CSA_RECV 6
#define CSA_OFFLOAD_CSA_RECV_ERROR_IE 7
#define CSA_OFFLOAD_CSA_TIMER_ERROR 8
#define CSA_OFFLOAD_CSA_TIMER_EXP 9
#define CSA_OFFLOAD_WMI_EVENT_ERROR 10
#define CSA_OFFLOAD_WMI_EVENT_SENT 11
#define CSA_OFFLOAD_WMI_CHANSWITCH_RECV 12
#define CSA_DBGID_DEFINITION_END 13

/* Chatter module DBGIDs */
#define WLAN_CHATTER_DBGID_DEFINITION_START 0
#define WLAN_CHATTER_ENTER 1
#define WLAN_CHATTER_EXIT 2
#define WLAN_CHATTER_FILTER_HIT 3
#define WLAN_CHATTER_FILTER_MISS 4
#define WLAN_CHATTER_FILTER_FULL 5
#define WLAN_CHATTER_FILTER_TM_ADJ  6
#define WLAN_CHATTER_BUFFER_FULL    7
#define WLAN_CHATTER_TIMEOUT        8
#define WLAN_CHATTER_MC_FILTER_ADD  9
#define WLAN_CHATTER_MC_FILTER_DEL  10
#define WLAN_CHATTER_MC_FILTER_ALLOW  11
#define WLAN_CHATTER_MC_FILTER_DROP  12
#define WLAN_CHATTER_COALESCING_FILTER_ADD      13
#define WLAN_CHATTER_COALESCING_FILTER_DEL      14
#define WLAN_CHATTER_DBGID_DEFINITION_END       15

#define WOW_DBGID_DEFINITION_START 0
#define WOW_ENABLE_CMDID 1
#define WOW_RECV_DATA_PKT 2
#define WOW_WAKE_HOST_DATA 3
#define WOW_RECV_MGMT 4
#define WOW_WAKE_HOST_MGMT 5
#define WOW_RECV_EVENT 6
#define WOW_WAKE_HOST_EVENT 7
#define WOW_INIT 8
#define WOW_RECV_MAGIC_PKT 9
#define WOW_RECV_BITMAP_PATTERN 10
#define WOW_AP_VDEV_DISALLOW    11
#define WOW_STA_VDEV_DISALLOW   12
#define WOW_P2PGO_VDEV_DISALLOW 13
#define WOW_NS_OFLD_ENABLE       14
#define WOW_ARP_OFLD_ENABLE      15
#define WOW_NS_ARP_OFLD_DISABLE  16
#define WOW_NS_RECEIVED          17
#define WOW_NS_REPLIED           18
#define WOW_ARP_RECEIVED         19
#define WOW_ARP_REPLIED          20
#define WOW_BEACON_OFFLOAD_TX    21
#define WOW_BEACON_OFFLOAD_CFG   22
#define WOW_DBGID_DEFINITION_END 23

/* SWBMISS module DBGIDs */
#define SWBMISS_DBGID_DEFINITION_START  0
#define SWBMISS_ENABLED                 1
#define SWBMISS_DISABLED                2
#define SWBMISS_DBGID_DEFINITION_END    3

/* WLAN module DBGIDS */
#define ROAM_DBGID_DEFINITION_START 0
#define ROAM_MODULE_INIT           1
#define ROAM_DEV_START             2
#define ROAM_CONFIG_RSSI_THRESH    3
#define ROAM_CONFIG_SCAN_PERIOD    4
#define ROAM_CONFIG_AP_PROFILE     5
#define ROAM_CONFIG_CHAN_LIST      6
#define ROAM_CONFIG_SCAN_PARAMS    7
#define ROAM_CONFIG_RSSI_CHANGE    8
#define ROAM_SCAN_TIMER_START      9
#define ROAM_SCAN_TIMER_EXPIRE    10
#define ROAM_SCAN_TIMER_STOP      11
#define ROAM_SCAN_STARTED         12
#define ROAM_SCAN_COMPLETE        13
#define ROAM_SCAN_CANCELLED       14
#define ROAM_CANDIDATE_FOUND      15
#define ROAM_RSSI_ACTIVE_SCAN     16
#define ROAM_RSSI_ACTIVE_ROAM     17
#define ROAM_RSSI_GOOD            18
#define ROAM_BMISS_FIRST_RECV     19
#define ROAM_DEV_STOP             20
#define ROAM_FW_OFFLOAD_ENABLE    21
#define ROAM_CANDIDATE_SSID_MATCH 22
#define ROAM_CANDIDATE_SECURITY_MATCH 23
#define ROAM_LOW_RSSI_INTERRUPT   24
#define ROAM_HIGH_RSSI_INTERRUPT  25
#define ROAM_SCAN_REQUESTED       26
#define ROAM_BETTER_CANDIDATE_FOUND 27
#define ROAM_BETTER_AP_EVENT 28
#define ROAM_CANCEL_LOW_PRIO_SCAN 29
#define ROAM_FINAL_BMISS_RECVD    30
#define ROAM_CONFIG_SCAN_MODE     31
#define ROAM_BMISS_FINAL_SCAN_ENABLE 32
#define ROAM_SUITABLE_AP_EVENT    33
#define ROAM_RSN_IE_PARSE_ERROR   34
#define ROAM_WPA_IE_PARSE_ERROR   35
#define ROAM_SCAN_CMD_FROM_HOST   36
#define ROAM_DBGID_DEFINITION_END 37

/* DATA_TXRX module DBGIDs*/
#define DATA_TXRX_DBGID_DEFINITION_START         0
#define DATA_TXRX_DBGID_RX_DATA_SEQ_LEN_INFO     1
#define DATA_TXRX_DBGID_DEFINITION_END           2

/* TDLS module DBGIDs*/
#define TDLS_DBGID_DEFINITION_START             0
#define TDLS_DBGID_VDEV_CREATE                  1
#define TDLS_DBGID_VDEV_DELETE                  2
#define TDLS_DBGID_ENABLED_PASSIVE              3
#define TDLS_DBGID_ENABLED_ACTIVE               4
#define TDLS_DBGID_DISABLED                     5
#define TDLS_DBGID_CONNTRACK_TIMER              6
#define TDLS_DBGID_WAL_SET                      7
#define TDLS_DBGID_WAL_GET                      8
#define TDLS_DBGID_WAL_PEER_UPDATE_SET          9
#define TDLS_DBGID_WAL_PEER_UPDATE_EVT         10
#define TDLS_DBGID_WAL_VDEV_CREATE             11
#define TDLS_DBGID_WAL_VDEV_DELETE             12
#define TDLS_DBGID_WLAN_EVENT                  13
#define TDLS_DBGID_WLAN_PEER_UPDATE_SET        14
#define TDLS_DBGID_PEER_EVT_DRP_THRESH         15
#define TDLS_DBGID_PEER_EVT_DRP_RATE           16
#define TDLS_DBGID_PEER_EVT_DRP_RSSI           17
#define TDLS_DBGID_PEER_EVT_DISCOVER           18
#define TDLS_DBGID_PEER_EVT_DELETE             19
#define TDLS_DBGID_PEER_CAP_UPDATE             20
#define TDLS_DBGID_UAPSD_SEND_PTI_FRAME        21
#define TDLS_DBGID_UAPSD_SEND_PTI_FRAME2PEER   22
#define TDLS_DBGID_UAPSD_START_PTR_TIMER       23
#define TDLS_DBGID_UAPSD_CANCEL_PTR_TIMER      24
#define TDLS_DBGID_UAPSD_PTR_TIMER_TIMEOUT     25
#define TDLS_DBGID_UAPSD_STA_PS_EVENT_HANDLER  26
#define TDLS_DBGID_UAPSD_PEER_EVENT_HANDLER    27
#define TDLS_DBGID_UAPSD_PS_DEFAULT_SETTINGS   28
#define TDLS_DBGID_UAPSD_GENERIC               29


/* TXBF Module IDs */
#define TXBFEE_DBGID_START                      0
#define TXBFEE_DBGID_NDPA_RECEIVED              1
#define TXBFEE_DBGID_HOST_CONFIG_TXBFEE_TYPE    2
#define TXBFER_DBGID_SEND_NDPA                  3
#define TXBFER_DBGID_GET_NDPA_BUF_FAIL          4
#define TXBFER_DBGID_SEND_NDPA_FAIL             5
#define TXBFER_DBGID_GET_NDP_BUF_FAIL           6
#define TXBFER_DBGID_SEND_NDP_FAIL              7
#define TXBFER_DBGID_GET_BRPOLL_BUF_FAIL        8
#define TXBFER_DBGID_SEND_BRPOLL_FAIL           9
#define TXBFER_DBGID_HOST_CONFIG_CMDID         10
#define TXBFEE_DBGID_HOST_CONFIG_CMDID         11
#define TXBFEE_DBGID_ENABLE_UPLOAD_H           12
#define TXBFEE_DBGID_UPLOADH_CV_TAG            13
#define TXBFEE_DBGID_UPLOADH_H_TAG             14
#define TXBFEE_DBGID_CAPTUREH_RECEIVED         15
#define TXBFEE_DBGID_PACKET_IS_STEERED         16
#define TXBFEE_UPLOADH_EVENT_ALLOC_MEM_FAIL    17
#define TXBFEE_DBGID_SW_WAR_AID_ZERO           18
#define TXBFEE_DBGID_END                       19

/* SMPS module DBGIDs */
#define STA_SMPS_DBGID_DEFINITION_START                 0
#define STA_SMPS_DBGID_CREATE_PDEV_INSTANCE             1
#define STA_SMPS_DBGID_CREATE_VIRTUAL_CHAN_INSTANCE     2
#define STA_SMPS_DBGID_DELETE_VIRTUAL_CHAN_INSTANCE     3
#define STA_SMPS_DBGID_CREATE_STA_INSTANCE              4
#define STA_SMPS_DBGID_DELETE_STA_INSTANCE              5
#define STA_SMPS_DBGID_VIRTUAL_CHAN_SMPS_START          6
#define STA_SMPS_DBGID_VIRTUAL_CHAN_SMPS_STOP           7
#define STA_SMPS_DBGID_SEND_SMPS_ACTION_FRAME           8
#define STA_SMPS_DBGID_HOST_FORCED_MODE                 9
#define STA_SMPS_DBGID_FW_FORCED_MODE                   10
#define STA_SMPS_DBGID_RSSI_THRESHOLD_CROSSED           11
#define STA_SMPS_DBGID_SMPS_ACTION_FRAME_COMPLETION     12
#define STA_SMPS_DBGID_DTIM_EBT_EVENT_CHMASK_UPDATE     13
#define STA_SMPS_DBGID_DTIM_CHMASK_UPDATE               14
#define STA_SMPS_DBGID_DTIM_BEACON_EVENT_CHMASK_UPDATE  15
#define STA_SMPS_DBGID_DTIM_POWER_STATE_CHANGE          16
#define STA_SMPS_DBGID_DTIM_CHMASK_UPDATE_SLEEP         17
#define STA_SMPS_DBGID_DTIM_CHMASK_UPDATE_AWAKE         18

#define STA_SMPS_DBGID_DEFINITION_END                   18

/* RTT module DBGIDs*/
#define RTT_CALL_FLOW                 0
#define RTT_REQ_SUB_TYPE              1
#define RTT_MEAS_REQ_HEAD             2
#define RTT_MEAS_REQ_BODY             3
#define RTT_INIT_GLOBAL_STATE         6
#define RTT_REPORT                    8
#define RTT_ERROR_REPORT              10
#define RTT_TIMER_STOP                11
#define RTT_SEND_TM_FRAME             12
#define RTT_V3_RESP_CNT               13
#define RTT_V3_RESP_FINISH            14
#define RTT_CHANNEL_SWITCH_REQ        15
#define RTT_CHANNEL_SWITCH_GRANT      16
#define RTT_CHANNEL_SWITCH_COMPLETE   17
#define RTT_CHANNEL_SWITCH_PREEMPT    18
#define RTT_CHANNEL_SWITCH_STOP       19
#define RTT_TIMER_START               20
/* WLAN HB module DBGIDs */
#define WLAN_HB_DBGID_DEFINITION_START                  0
#define WLAN_HB_DBGID_INIT                              1
#define WLAN_HB_DBGID_TCP_GET_TXBUF_FAIL                2
#define WLAN_HB_DBGID_TCP_SEND_FAIL                     3
#define WLAN_HB_DBGID_BSS_PEER_NULL                     4
#define WLAN_HB_DBGID_UDP_GET_TXBUF_FAIL                5
#define WLAN_HB_DBGID_UDP_SEND_FAIL                     6
#define WLAN_HB_DBGID_WMI_CMD_INVALID_PARAM             7
#define WLAN_HB_DBGID_WMI_CMD_INVALID_OP                8
#define WLAN_HB_DBGID_WOW_NOT_ENTERED                   9
#define WLAN_HB_DBGID_ALLOC_SESS_FAIL                   10
#define WLAN_HB_DBGID_CTX_NULL                          11
#define WLAN_HB_DBGID_CHKSUM_ERR                        12
#define WLAN_HB_DBGID_UDP_TX                            13
#define WLAN_HB_DBGID_TCP_TX                            14
#define WLAN_HB_DBGID_DEFINITION_END                    15

/* Thermal Manager DBGIDs*/
#define THERMAL_MGR_DBGID_DEFINITION_START   0
#define THERMAL_MGR_NEW_THRESH               1
#define THERMAL_MGR_THRESH_CROSSED           2
#define THERMAL_MGR_DBGID_DEFINITION_END     3

/* WLAN PHYERR DFS(parse/filter) DBGIDs */
#define WLAN_PHYERR_DFS_DBGID_DEFINITION_START    0
#define WLAN_PHYERR_DFS_PHYERR_INFO_CHAN_BUFLEN   1
#define WLAN_PHYERR_DFS_PHYERR_INFO_PPDU          2
#define WLAN_PHYERR_DFS_DBDID_RADAR_SUMMARY       3
#define WLAN_PHYERR_DFS_DBDID_SEARCH_FFT          4
#define WLAN_PHTERR_DFS_DBDID_FILTER_STATUS       5
#define WLAN_PHYERR_DFS_DBGID_DEFINITION_END      6

/* RMC DBGIDs */
#define RMC_DBGID_DEFINITION_START             0
#define RMC_SM_INIT_ERR                        1
#define RMC_VDEV_ALLOC_ERR                     2
#define RMC_CREATE_INSTANCE                    3
#define RMC_DELETE_INSTANCE                    4
#define RMC_NEW_PRI_LEADER                     5
#define RMC_NEW_SEC_LEADER                     6
#define RMC_NO_LDR_CHANGE                      7
#define RMC_LDR_INFORM_SENT                    8
#define RMC_PEER_ADD                           9
#define RMC_PEER_DELETE                        10
#define RMC_PEER_UNKNOWN                       11
#define RMC_PRI_LDR_RSSI_UPDATE                12
#define RMC_SEC_LDR_RSSI_UPDATE                13
#define RMC_SET_MODE                           14
#define RMC_SET_ACTION_PERIOD                  15
#define RMC_DBGID_DEFINITION_END               16

#ifdef __cplusplus
}
#endif

#endif /* _DBGLOG_ID_H_ */
