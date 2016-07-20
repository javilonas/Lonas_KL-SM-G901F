/*
 * Copyright (c) 2012-2014 The Linux Foundation. All rights reserved.
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

#if !defined( HDD_CFG80211_H__ )
#define HDD_CFG80211_H__


/**===========================================================================

  \file  wlan_hdd_cfg80211.h

  \brief cfg80211 functions declarations

  ==========================================================================*/

/* $HEADER$ */


//value for initial part of frames and number of bytes to be compared
#define GAS_INITIAL_REQ "\x04\x0a"
#define GAS_INITIAL_REQ_SIZE 2

#define GAS_INITIAL_RSP "\x04\x0b"
#define GAS_INITIAL_RSP_SIZE 2

#define GAS_COMEBACK_REQ "\x04\x0c"
#define GAS_COMEBACK_REQ_SIZE 2

#define GAS_COMEBACK_RSP "\x04\x0d"
#define GAS_COMEBACK_RSP_SIZE 2

#define P2P_PUBLIC_ACTION_FRAME "\x04\x09\x50\x6f\x9a\x09"
#define P2P_PUBLIC_ACTION_FRAME_SIZE 6

#define P2P_ACTION_FRAME "\x7f\x50\x6f\x9a\x09"
#define P2P_ACTION_FRAME_SIZE 5

#define SA_QUERY_FRAME_REQ "\x08\x00"
#define SA_QUERY_FRAME_REQ_SIZE 2

#define SA_QUERY_FRAME_RSP "\x08\x01"
#define SA_QUERY_FRAME_RSP_SIZE 2

#define HDD_P2P_WILDCARD_SSID "DIRECT-" //TODO Put it in proper place;
#define HDD_P2P_WILDCARD_SSID_LEN 7

#define WNM_BSS_ACTION_FRAME "\x0a\x07"
#define WNM_BSS_ACTION_FRAME_SIZE 2

#define WPA_OUI_TYPE   "\x00\x50\xf2\x01"
#define BLACKLIST_OUI_TYPE   "\x00\x50\x00\x00"
#define WHITELIST_OUI_TYPE   "\x00\x50\x00\x01"
#define WPA_OUI_TYPE_SIZE  4

#define WLAN_BSS_MEMBERSHIP_SELECTOR_HT_PHY 127
#define BASIC_RATE_MASK   0x80
#define RATE_MASK         0x7f

#ifdef WLAN_ENABLE_AGEIE_ON_SCAN_RESULTS
/* GPS application requirement */
#define QCOM_VENDOR_IE_ID 221
#define QCOM_OUI1         0x00
#define QCOM_OUI2         0xA0
#define QCOM_OUI3         0xC6
#define QCOM_VENDOR_IE_AGE_TYPE  0x100
#define QCOM_VENDOR_IE_AGE_LEN   11

#ifdef FEATURE_WLAN_TDLS
#define WLAN_IS_TDLS_SETUP_ACTION(action) \
         ((SIR_MAC_TDLS_SETUP_REQ <= action) && (SIR_MAC_TDLS_SETUP_CNF >= action))
#if !defined (TDLS_MGMT_VERSION2)
#define TDLS_MGMT_VERSION2 0
#endif
#endif

#define MAX_CHANNEL (MAX_2_4GHZ_CHANNEL + NUM_5GHZ_CHANNELS)

typedef struct {
   u8 element_id;
   u8 len;
   u8 oui_1;
   u8 oui_2;
   u8 oui_3;
   u32 type;
   u32 age;
}__attribute__((packed)) qcom_ie_age ;
#endif

/* Vendor id to be used in vendor specific command and events
 * to user space.
 * NOTE: The authoritative place for definition of QCA_NL80211_VENDOR_ID,
 * vendor subcmd definitions prefixed with QCA_NL80211_VENDOR_SUBCMD, and
 * qca_wlan_vendor_attr is open source file src/common/qca-vendor.h in
 * git://w1.fi/srv/git/hostap.git; the values here are just a copy of that
 */

#define QCA_NL80211_VENDOR_ID                          0x001374

enum qca_nl80211_vendor_subcmds {
    QCA_NL80211_VENDOR_SUBCMD_UNSPEC = 0,
    QCA_NL80211_VENDOR_SUBCMD_TEST = 1,
    /* subcmds 2..9 not yet allocated */
    QCA_NL80211_VENDOR_SUBCMD_AVOID_FREQUENCY = 10,
    QCA_NL80211_VENDOR_SUBCMD_DFS_CAPABILITY =  11,
    QCA_NL80211_VENDOR_SUBCMD_NAN =  12,
    QCA_NL80211_VENDOR_SUBCMD_STATS_EXT = 13,
    /* subcommands for link layer statistics start here */
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_SET = 14,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_GET = 15,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_CLR = 16,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_RADIO_RESULTS = 17,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_IFACE_RESULTS = 18,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_PEERS_RESULTS = 19,
    /* subcommands for extscan start here */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_START = 20,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_STOP = 21,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_GET_VALID_CHANNELS = 22,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_GET_CAPABILITIES = 23,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_GET_CACHED_RESULTS = 24,
    /* Used when report_threshold is reached in scan cache. */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SCAN_RESULTS_AVAILABLE = 25,
    /* Used to report scan results when each probe rsp. is received,
     * if report_events enabled in wifi_scan_cmd_params.
     */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_FULL_SCAN_RESULT = 26,
    /* Indicates progress of scanning state-machine. */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SCAN_EVENT = 27,
    /* Indicates BSSID Hotlist. */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_HOTLIST_AP_FOUND = 28,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SET_BSSID_HOTLIST = 29,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_RESET_BSSID_HOTLIST = 30,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SIGNIFICANT_CHANGE = 31,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SET_SIGNIFICANT_CHANGE = 32,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_RESET_SIGNIFICANT_CHANGE = 33,
    /* EXT TDLS */
    QCA_NL80211_VENDOR_SUBCMD_TDLS_ENABLE = 34,
    QCA_NL80211_VENDOR_SUBCMD_TDLS_DISABLE = 35,
    QCA_NL80211_VENDOR_SUBCMD_TDLS_GET_STATUS = 36,
    QCA_NL80211_VENDOR_SUBCMD_TDLS_STATE = 37,
    /* Get supported features */
    QCA_NL80211_VENDOR_SUBCMD_GET_SUPPORTED_FEATURES = 38,

    /* Set scanning_mac_oui */
    QCA_NL80211_VENDOR_SUBCMD_SCANNING_MAC_OUI = 39,
    /* Set nodfs_flag */
    QCA_NL80211_VENDOR_SUBCMD_NO_DFS_FLAG = 40,

    /* Get Concurrency Matrix */
    QCA_NL80211_VENDOR_SUBCMD_GET_CONCURRENCY_MATRIX = 42,
};

enum qca_nl80211_vendor_subcmds_index {
#if defined(FEATURE_WLAN_CH_AVOID) || defined(FEATURE_WLAN_FORCE_SAP_SCC)
    QCA_NL80211_VENDOR_SUBCMD_AVOID_FREQUENCY_INDEX = 0,
#endif /* FEATURE_WLAN_CH_AVOID || FEATURE_WLAN_FORCE_SAP_SCC */

#ifdef WLAN_FEATURE_NAN
    QCA_NL80211_VENDOR_SUBCMD_NAN_INDEX,
#endif /* WLAN_FEATURE_NAN */

#ifdef WLAN_FEATURE_STATS_EXT
    QCA_NL80211_VENDOR_SUBCMD_STATS_EXT_INDEX,
#endif /* WLAN_FEATURE_STATS_EXT */

#ifdef FEATURE_WLAN_EXTSCAN
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_START_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_STOP_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_GET_CAPABILITIES_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_GET_CACHED_RESULTS_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SCAN_RESULTS_AVAILABLE_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_FULL_SCAN_RESULT_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SCAN_EVENT_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_HOTLIST_AP_FOUND_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SET_BSSID_HOTLIST_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_RESET_BSSID_HOTLIST_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SIGNIFICANT_CHANGE_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SET_SIGNIFICANT_CHANGE_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_RESET_SIGNIFICANT_CHANGE_INDEX,
#endif /* FEATURE_WLAN_EXTSCAN */

#ifdef WLAN_FEATURE_LINK_LAYER_STATS
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_SET_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_GET_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_CLR_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_LL_RADIO_STATS_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_LL_IFACE_STATS_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_LL_PEER_INFO_STATS_INDEX,
#endif /* WLAN_FEATURE_LINK_LAYER_STATS */
    /* EXT TDLS */
    QCA_NL80211_VENDOR_SUBCMD_TDLS_STATE_CHANGE_INDEX,
};

enum qca_wlan_vendor_attr {
    QCA_WLAN_VENDOR_ATTR_INVALID = 0,
    /* used by QCA_NL80211_VENDOR_SUBCMD_DFS_CAPABILITY */
    QCA_WLAN_VENDOR_ATTR_DFS = 1,
    /* used by QCA_NL80211_VENDOR_SUBCMD_NAN */
    QCA_WLAN_VENDOR_ATTR_NAN = 2,
    /* used by QCA_NL80211_VENDOR_SUBCMD_STATS_EXT */
    QCA_WLAN_VENDOR_ATTR_STATS_EXT = 3,
    QCA_WLAN_VENDOR_ATTR_IFINDEX = 4,
    /* keep last */
    QCA_WLAN_VENDOR_ATTR_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_MAX =
    QCA_WLAN_VENDOR_ATTR_AFTER_LAST - 1
};

enum qca_wlan_vendor_attr_get_supported_features {
    QCA_WLAN_VENDOR_ATTR_FEATURE_SET_INVALID = 0,
    /* Unsigned 32-bit value */
    QCA_WLAN_VENDOR_ATTR_FEATURE_SET = 1,
    /* keep last */
    QCA_WLAN_VENDOR_ATTR_FEATURE_SET_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_FEATURE_SET_MAX =
        QCA_WLAN_VENDOR_ATTR_FEATURE_SET_AFTER_LAST - 1,
};

/* NL attributes for data used by
 * QCA_NL80211_VENDOR_SUBCMD_GET_CONCURRENCY_MATRIX sub command.
 */
enum qca_wlan_vendor_attr_get_concurrency_matrix {
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_INVALID = 0,
    /* Unsigned 32-bit value */
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_CONFIG_PARAM_SET_SIZE_MAX = 1,
    /* Unsigned 32-bit value */
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_RESULTS_SET_SIZE = 2,
    /* An array of SET_SIZE x Unsigned 32bit values representing
     * concurrency combinations.
     */
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_RESULTS_SET = 3,
    /* keep last */
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_MAX =
        QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_AFTER_LAST - 1,
};

/* Feature defines */
#define WIFI_FEATURE_INFRA              0x0001   /* Basic infrastructure mode */
#define WIFI_FEATURE_INFRA_5G           0x0002   /* Support for 5 GHz Band */
#define WIFI_FEATURE_HOTSPOT            0x0004   /* Support for GAS/ANQP */
#define WIFI_FEATURE_P2P                0x0008   /* Wifi-Direct */
#define WIFI_FEATURE_SOFT_AP            0x0010   /* Soft AP */
#define WIFI_FEATURE_EXTSCAN            0x0020   /* Extended Scan APIs */
#define WIFI_FEATURE_NAN                0x0040   /* Neighbor Awareness
                                                    Networking */
#define WIFI_FEATURE_D2D_RTT            0x0080   /* Device-to-device RTT */
#define WIFI_FEATURE_D2AP_RTT           0x0100   /* Device-to-AP RTT */
#define WIFI_FEATURE_BATCH_SCAN         0x0200   /* Batched Scan (legacy) */
#define WIFI_FEATURE_PNO                0x0400   /* Preferred network offload */
#define WIFI_FEATURE_ADDITIONAL_STA     0x0800   /* Support for two STAs */
#define WIFI_FEATURE_TDLS               0x1000   /* Tunnel directed link
                                                    setup */
#define WIFI_FEATURE_TDLS_OFFCHANNEL    0x2000   /* Support for TDLS off
                                                    channel */
#define WIFI_FEATURE_EPR                0x4000   /* Enhanced power reporting */
#define WIFI_FEATURE_AP_STA             0x8000   /* Support for AP STA
                                                    Concurrency */
/* Add more features here */

#ifdef FEATURE_WLAN_CH_AVOID
#define HDD_MAX_AVOID_FREQ_RANGES   4
typedef struct sHddAvoidFreqRange
{
   u32 startFreq;
   u32 endFreq;
} tHddAvoidFreqRange;

typedef struct sHddAvoidFreqList
{
   u32 avoidFreqRangeCount;
   tHddAvoidFreqRange avoidFreqRange[HDD_MAX_AVOID_FREQ_RANGES];
} tHddAvoidFreqList;
#endif /* FEATURE_WLAN_CH_AVOID */

struct cfg80211_bss* wlan_hdd_cfg80211_update_bss_db( hdd_adapter_t *pAdapter,
                                      tCsrRoamInfo *pRoamInfo
                                      );

#ifdef FEATURE_WLAN_LFR
int wlan_hdd_cfg80211_pmksa_candidate_notify(
                    hdd_adapter_t *pAdapter, tCsrRoamInfo *pRoamInfo,
                    int index, bool preauth );
#endif

#ifdef FEATURE_WLAN_LFR_METRICS
VOS_STATUS wlan_hdd_cfg80211_roam_metrics_preauth(hdd_adapter_t *pAdapter,
                                                  tCsrRoamInfo *pRoamInfo);

VOS_STATUS wlan_hdd_cfg80211_roam_metrics_preauth_status(
    hdd_adapter_t *pAdapter, tCsrRoamInfo *pRoamInfo, bool preauth_status);

VOS_STATUS wlan_hdd_cfg80211_roam_metrics_handover(hdd_adapter_t *pAdapter,
                                                   tCsrRoamInfo *pRoamInfo);
#endif

#ifdef FEATURE_WLAN_WAPI
void wlan_hdd_cfg80211_set_key_wapi(hdd_adapter_t* pAdapter,
              u8 key_index, const u8 *mac_addr, u8 *key , int key_Len);
#endif
struct wiphy *wlan_hdd_cfg80211_wiphy_alloc(int priv_size);

int wlan_hdd_cfg80211_scan( struct wiphy *wiphy,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0))
                            struct net_device *dev,
#endif
                            struct cfg80211_scan_request *request);

int wlan_hdd_cfg80211_init(struct device *dev,
                               struct wiphy *wiphy,
                               hdd_config_t *pCfg
                                         );

int wlan_hdd_cfg80211_register( struct wiphy *wiphy);
void wlan_hdd_cfg80211_post_voss_start(hdd_adapter_t* pAdapter);

void wlan_hdd_cfg80211_pre_voss_stop(hdd_adapter_t* pAdapter);

#ifdef CONFIG_ENABLE_LINUX_REG
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0))
void wlan_hdd_linux_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request);
#else
int wlan_hdd_linux_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request);
#endif
#else
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0))
void wlan_hdd_crda_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request);
#else
int wlan_hdd_crda_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request);
#endif
#endif

extern v_VOID_t hdd_connSetConnectionState( hdd_station_ctx_t *pHddStaCtx,
                                        eConnectionState connState );
VOS_STATUS wlan_hdd_validate_operation_channel(hdd_adapter_t *pAdapter,int channel);
#ifdef FEATURE_WLAN_TDLS
int wlan_hdd_cfg80211_send_tdls_discover_req(struct wiphy *wiphy,
                            struct net_device *dev, u8 *peer);
#endif
#ifdef WLAN_FEATURE_GTK_OFFLOAD
extern void wlan_hdd_cfg80211_update_replayCounterCallback(void *callbackContext,
                            tpSirGtkOffloadGetInfoRspParams pGtkOffloadGetInfoRsp);
#endif
void* wlan_hdd_change_country_code_cb(void *pAdapter);
void hdd_select_cbmode( hdd_adapter_t *pAdapter,v_U8_t operationChannel);

v_U8_t* wlan_hdd_cfg80211_get_ie_ptr(v_U8_t *pIes, int length, v_U8_t eid);

#if defined(QCA_WIFI_2_0) && defined(QCA_WIFI_FTM) \
    && !defined(QCA_WIFI_ISOC) && defined(CONFIG_NL80211_TESTMODE)
void wlan_hdd_testmode_rx_event(void *buf, size_t buf_len);
#endif

#ifdef QCA_WIFI_2_0
void hdd_suspend_wlan(void (*callback)(void *callbackContext, boolean suspended),
                      void *callbackContext);
void hdd_resume_wlan(void);
#endif

#ifdef FEATURE_WLAN_CH_AVOID
int wlan_hdd_send_avoid_freq_event(hdd_context_t *pHddCtx,
                                   tHddAvoidFreqList *pAvoidFreqList);
#endif

#endif
