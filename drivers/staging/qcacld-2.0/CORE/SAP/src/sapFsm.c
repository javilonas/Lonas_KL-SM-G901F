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

/*===========================================================================

                      s a p F s m . C

  OVERVIEW:

  This software unit holds the implementation of the WLAN SAP Finite
  State Machine modules

  DEPENDENCIES:

  Are listed for each API below.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE


  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



  when        who     what, where, why
----------    ---    --------------------------------------------------------
2010-03-15         Created module

===========================================================================*/


/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "sapInternal.h"
// Pick up the SME API definitions
#include "sme_Api.h"
// Pick up the PMC API definitions
#include "pmcApi.h"
#include "wlan_nv.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  External declarations for global context
 * -------------------------------------------------------------------------*/
#ifdef FEATURE_WLAN_CH_AVOID
extern sapSafeChannelType safeChannels[];
#endif /* FEATURE_WLAN_CH_AVOID */

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
#ifdef SOFTAP_CHANNEL_RANGE
static VOS_STATUS sapGetChannelList(ptSapContext sapContext, v_U8_t **channelList,
                                 v_U8_t  *numberOfChannels);
#endif

/*==========================================================================
  FUNCTION    sapGet5GHzChannelList

  DESCRIPTION
    Function for initializing list of  2.4/5 Ghz [NON-DFS/DFS] available
    channels in the current regulatory domain.

  DEPENDENCIES
    NA.

  PARAMETERS

    IN
    sapContext: SAP Context

  RETURN VALUE
    NA

  SIDE EFFECTS
============================================================================*/
static VOS_STATUS sapGet5GHzChannelList(ptSapContext sapContext);

/*==========================================================================
  FUNCTION    sapStartDfsCacTimer

  DESCRIPTION
    Function to start the DFS CAC timer when SAP is started on DFS Channel
  DEPENDENCIES
    NA.

  PARAMETERS

    IN
    sapContext: SAP Context
  RETURN VALUE
    DFS Timer start status
  SIDE EFFECTS
============================================================================*/

int sapStartDfsCacTimer(ptSapContext sapContext);

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
* -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/

/*==========================================================================
  FUNCTION    sapEventInit

  DESCRIPTION
    Function for initializing sWLAN_SAPEvent structure

  DEPENDENCIES
    NA.

  PARAMETERS

    IN
    sapEvent    : State machine event

  RETURN VALUE

    None

  SIDE EFFECTS
============================================================================*/
static inline void sapEventInit(ptWLAN_SAPEvent sapEvent)
{
   sapEvent->event = eSAP_MAC_SCAN_COMPLETE;
   sapEvent->params = 0;
   sapEvent->u1 = 0;
   sapEvent->u2 = 0;
}

/*
 * This Function Checks if a given channel is AVAILABLE or USABLE
 * for DFS operation.
 */
static v_BOOL_t
sapDfsIsChannelInNolList(ptSapContext sapContext, v_U8_t channelNumber)
{
    int i;
    unsigned long timeElapsedSinceLastRadar,timeWhenRadarFound,currentTime = 0;

    for (i =0 ; i< sapContext->SapDfsInfo.numCurrentRegDomainDfsChannels; i++)
    {
        if(sapContext->SapDfsInfo.sapDfsChannelNolList[i]
                                 .dfs_channel_number == channelNumber)
        {
            if ( (sapContext->SapDfsInfo.sapDfsChannelNolList[i]
                        .radar_status_flag == eSAP_DFS_CHANNEL_USABLE)
                                           ||
                 (sapContext->SapDfsInfo.sapDfsChannelNolList[i]
                        .radar_status_flag == eSAP_DFS_CHANNEL_AVAILABLE) )
            {
                /*
                 * Allow SAP operation on this channel
                 * either the DFS channel has not been used
                 * for SAP operation or it is available for
                 * SAP operation since it is past Non-Occupancy-Period
                 * so, return FALSE.
                 */
                VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_LOW,
                          "%s[%d]: Channel = %d"
                           "Not in NOL LIST, CHANNEL AVAILABLE",
                           __func__, __LINE__, sapContext->SapDfsInfo
                                                 .sapDfsChannelNolList[i]
                                                 .dfs_channel_number);
                return VOS_FALSE;
            }
            else if (sapContext->SapDfsInfo.sapDfsChannelNolList[i]
                        .radar_status_flag == eSAP_DFS_CHANNEL_UNAVAILABLE)
            {
                /*
                 * If a DFS Channel is UNAVAILABLE then
                 * check to see if it is past Non-occupancy-period
                 * of 30 minutes. If it is past 30 mins then
                 * mark the channel as AVAILABLE and return FALSE
                 * as the channel is not anymore in NON-Occupancy-Period.
                 */
                timeWhenRadarFound = sapContext->SapDfsInfo
                                     .sapDfsChannelNolList[i]
                                     .radar_found_timestamp;
                currentTime = vos_timer_get_system_time();
                timeElapsedSinceLastRadar = currentTime - timeWhenRadarFound;
                if (timeElapsedSinceLastRadar >=  SAP_DFS_NON_OCCUPANCY_PERIOD)
                {
                    sapContext->SapDfsInfo.sapDfsChannelNolList[i]
                           .radar_status_flag = eSAP_DFS_CHANNEL_AVAILABLE;

                    VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_LOW,
                              "%s[%d]:Channel=%d"
                               "Not in NOL LIST,CHANNEL AVAILABLE",
                               __func__, __LINE__, sapContext->SapDfsInfo
                                                   .sapDfsChannelNolList[i]
                                                   .dfs_channel_number);
                    return VOS_FALSE;
                }
                else
                {
                    /*
                     * Channel is not still available for SAP operation
                     * so return TRUE; As the Channel is still
                     * in Non-occupancy-Period.
                     */
                    VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_LOW,
                              "%s[%d]:Channel=%d"
                              "still in NOL LIST,CHANNEL UNAVAILABLE",
                               __func__, __LINE__, sapContext->SapDfsInfo
                                                  .sapDfsChannelNolList[i]
                                                  .dfs_channel_number);
                    return VOS_TRUE;
                }
            }
        }
    }
    return VOS_FALSE;
}

/*==========================================================================
  FUNCTION    sapGotoChannelSel

  DESCRIPTION
    Function for initiating scan request for SME

  DEPENDENCIES
    NA.

  PARAMETERS

    IN
    sapContext  : Sap Context value
    sapEvent    : State machine event

  RETURN VALUE
    The VOS_STATUS code associated with performing the operation

    VOS_STATUS_SUCCESS: Success

  SIDE EFFECTS
============================================================================*/
VOS_STATUS
sapGotoChannelSel
(
    ptSapContext sapContext,
    ptWLAN_SAPEvent sapEvent
)
{
    /* Initiate a SCAN request */
    eHalStatus halStatus;
    tCsrScanRequest scanRequest;/* To be initialised if scan is required */
    v_U32_t    scanRequestID = 0;
    VOS_STATUS vosStatus = VOS_STATUS_SUCCESS;

#ifdef SOFTAP_CHANNEL_RANGE
    v_U8_t     *channelList = NULL;
    v_U8_t     numOfChannels = 0 ;
#endif
    tHalHandle hHal;
    tANI_U8   channel;

    hHal = (tHalHandle)vos_get_context( VOS_MODULE_ID_SME, sapContext->pvosGCtx);
    if (NULL == hHal)
    {
        /* we have a serious problem */
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_FATAL,
                   "In %s, invalid hHal", __func__);
        return VOS_STATUS_E_FAULT;
    }

    /*If STA-AP concurrency is enabled take the concurrent connected channel first. In other cases wpa_supplicant should take care */
    if (vos_get_concurrency_mode() == VOS_STA_SAP)
    {
        channel = sme_GetConcurrentOperationChannel(hHal);

        if (channel)
        { /*if a valid channel is returned then use concurrent channel.
                  Else take whatever comes from configuartion*/
            sapContext->channel = channel;
            sme_SelectCBMode(hHal,
                             sapConvertSapPhyModeToCsrPhyMode(sapContext->csrRoamProfile.phyMode),
                             channel);
        }
    }

    if (sapContext->channel == AUTO_CHANNEL_SELECT)
    {
        vos_mem_zero(&scanRequest, sizeof(scanRequest));

        /* Set scanType to Passive scan */
        scanRequest.scanType = eSIR_PASSIVE_SCAN;

        /* Set min and max channel time to zero */
        scanRequest.minChnTime = 0;
        scanRequest.maxChnTime = 0;

        /* Set BSSType to default type */
        scanRequest.BSSType = eCSR_BSS_TYPE_ANY;

#ifndef SOFTAP_CHANNEL_RANGE
        /*Scan all the channels */
        scanRequest.ChannelInfo.numOfChannels = 0;

        scanRequest.ChannelInfo.ChannelList = NULL;

        scanRequest.requestType = eCSR_SCAN_REQUEST_FULL_SCAN;//eCSR_SCAN_REQUEST_11D_SCAN;

#else

        sapGetChannelList(sapContext, &channelList, &numOfChannels);

        /*Scan the channels in the list*/
        scanRequest.ChannelInfo.numOfChannels = numOfChannels;

        scanRequest.ChannelInfo.ChannelList = channelList;

        scanRequest.requestType = eCSR_SCAN_SOFTAP_CHANNEL_RANGE;

        sapContext->channelList = channelList;

#endif
        /* Set requestType to Full scan */

        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, calling sme_ScanRequest", __func__);

        halStatus = sme_ScanRequest(hHal,
                            sapContext->sessionId,
                            &scanRequest,
                            &scanRequestID,//, when ID == 0 11D scan/active scan with callback, min-maxChntime set in csrScanRequest()?
                            &WLANSAP_ScanCallback,//csrScanCompleteCallback callback,
                            sapContext);//void * pContext scanRequestID filled up
        if (eHAL_STATUS_SUCCESS != halStatus)
        {
            VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, "%s:sme_ScanRequest  fail %d!!!", __func__, halStatus);
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "SoftAP Configuring for default channel, Ch= %d", sapContext->channel);
            /* In case of error, switch to default channel */
            sapContext->channel = SAP_DEFAULT_CHANNEL;

#ifdef SOFTAP_CHANNEL_RANGE
            if(sapContext->channelList != NULL)
            {
                sapContext->channel = sapContext->channelList[0];
                vos_mem_free(sapContext->channelList);
                sapContext->channelList = NULL;
            }
#endif
            /* Fill in the event structure */
            sapEventInit(sapEvent);
            /* Handle event */
            vosStatus = sapFsm(sapContext, sapEvent);
        }
        else
        {
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, return from sme_ScanRequest, scanRequestID=%d, Ch= %d",
                   __func__, scanRequestID, sapContext->channel);
        }

    }
    else
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, for configured channel, Ch= %d", __func__, sapContext->channel);
        /* Fill in the event structure */
        // Eventhough scan was not done, means a user set channel was chosen
        sapEventInit(sapEvent);
        /* Handle event */
        vosStatus = sapFsm(sapContext, sapEvent);
    }

    /* If scan failed, get default channel and advance state machine as success with default channel */
    /* Have to wait for the call back to be called to get the channel cannot advance state machine here as said above */
    VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, before exiting sapGotoChannelSel channel=%d", __func__, sapContext->channel);

    return VOS_STATUS_SUCCESS;
}// sapGotoChannelSel

/*==========================================================================
  FUNCTION    sapGotoStarting

  DESCRIPTION
    Function for initiating start bss request for SME

  DEPENDENCIES
    NA.

  PARAMETERS

    IN
    sapContext  : Sap Context value
    sapEvent    : State machine event
    bssType     : Type of bss to start, INRA AP
    status      : Return the SAP status here

  RETURN VALUE
    The VOS_STATUS code associated with performing the operation

    VOS_STATUS_SUCCESS: Success

  SIDE EFFECTS
============================================================================*/
VOS_STATUS
sapGotoStarting
(
    ptSapContext sapContext,
    ptWLAN_SAPEvent sapEvent,
    eCsrRoamBssType bssType
)
{
    /* tHalHandle */
    tHalHandle hHal = VOS_GET_HAL_CB(sapContext->pvosGCtx);
    eHalStatus halStatus;
    tANI_U32 type, subType;
    VOS_STATUS status = VOS_STATUS_E_FAILURE;

    /*- - - - - - - - TODO:once configs from hdd available - - - - - - - - -*/
    char key_material[32]={ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1,};
    tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
    sapContext->key_type = 0x05;
    sapContext->key_length = 32;
    vos_mem_copy(sapContext->key_material, key_material, sizeof(key_material));  /* Need a key size define */

    VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s", __func__);
    if (NULL == hHal)
    {
        /* we have a serious problem */
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_FATAL,
                   "In %s, invalid hHal", __func__);
        return VOS_STATUS_E_FAULT;
    }

    /* No Need to Req for Power with power offload enabled */
    if(!pMac->psOffloadEnabled)
    {
       //TODO: What shall we do if failure????
       halStatus = pmcRequestFullPower( hHal,
                            WLANSAP_pmcFullPwrReqCB,
                            sapContext,
                            eSME_REASON_OTHER);
    }

    if (sapContext->csrRoamProfile.csrPersona == VOS_P2P_GO_MODE)
        status = vos_get_vdev_types(VOS_P2P_GO_MODE, &type, &subType);
    else
        status = vos_get_vdev_types(VOS_STA_SAP_MODE, &type, &subType);

    if (VOS_STATUS_SUCCESS != status)
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_FATAL, "failed to get vdev type");
        return VOS_STATUS_E_FAILURE;
    }
    /* Open SME Session for Softap */
    halStatus = sme_OpenSession(hHal,
                        &WLANSAP_RoamCallback,
                        sapContext,
                        sapContext->self_mac_addr,
                        &sapContext->sessionId,
                        type, subType);

    if(eHAL_STATUS_SUCCESS != halStatus )
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, "Error: In %s calling sme_RoamConnect status = %d", __func__, halStatus);
        return VOS_STATUS_E_FAILURE;
    }

    return VOS_STATUS_SUCCESS;
}// sapGotoStarting

/*==========================================================================
  FUNCTION    sapGotoDisconnecting

  DESCRIPTION
    Processing of SAP FSM Disconnecting state

  DEPENDENCIES
    NA.

  PARAMETERS

    IN
    sapContext  : Sap Context value
    status      : Return the SAP status here

  RETURN VALUE
    The VOS_STATUS code associated with performing the operation

    VOS_STATUS_SUCCESS: Success

  SIDE EFFECTS
============================================================================*/
VOS_STATUS
sapGotoDisconnecting
(
    ptSapContext sapContext
)
{
    eHalStatus halStatus;
    tHalHandle hHal;

    hHal = VOS_GET_HAL_CB(sapContext->pvosGCtx);
    if (NULL == hHal)
    {
        /* we have a serious problem */
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                   "In %s, invalid hHal", __func__);
        return VOS_STATUS_E_FAULT;
    }

    sapFreeRoamProfile(&sapContext->csrRoamProfile);
    halStatus = sme_RoamStopBss(hHal, sapContext->sessionId);
    if(eHAL_STATUS_SUCCESS != halStatus )
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, "Error: In %s calling sme_RoamStopBss status = %d", __func__, halStatus);
        return VOS_STATUS_E_FAILURE;
    }

    return VOS_STATUS_SUCCESS;
}

static eHalStatus sapRoamSessionCloseCallback(void *pContext)
{
    ptSapContext sapContext = (ptSapContext)pContext;
    return sapSignalHDDevent(sapContext, NULL,
                    eSAP_STOP_BSS_EVENT, (v_PVOID_t) eSAP_STATUS_SUCCESS);
}

/*==========================================================================
  FUNCTION    sapGotoDisconnected

  DESCRIPTION
    Function for setting the SAP FSM to Disconnection state

  DEPENDENCIES
    NA.

  PARAMETERS

    IN
    sapContext  : Sap Context value
    sapEvent    : State machine event
    status      : Return the SAP status here

  RETURN VALUE
    The VOS_STATUS code associated with performing the operation

    VOS_STATUS_SUCCESS: Success

  SIDE EFFECTS
============================================================================*/
VOS_STATUS
sapGotoDisconnected
(
    ptSapContext sapContext
)
{
    VOS_STATUS vosStatus = VOS_STATUS_E_FAILURE;
    tWLAN_SAPEvent sapEvent;
    // Processing has to be coded
    // Clean up stations from TL etc as AP BSS is shut down then set event
    sapEvent.event = eSAP_MAC_READY_FOR_CONNECTIONS;// hardcoded
    sapEvent.params = 0;
    sapEvent.u1 = 0;
    sapEvent.u2 = 0;
    /* Handle event */
    vosStatus = sapFsm(sapContext, &sapEvent);

    return vosStatus;
}

/*==========================================================================
  FUNCTION    sapSignalHDDevent

  DESCRIPTION
    Function for HDD to send the event notification using callback

  DEPENDENCIES
    NA.

  PARAMETERS

    IN
    sapContext  : Sap Context value
    pCsrRoamInfo : Pointer to CSR roam information
    sapHddevent      : SAP HDD event
    context          : to pass the element for future support

  RETURN VALUE
    The VOS_STATUS code associated with performing the operation

    VOS_STATUS_SUCCESS: Success

  SIDE EFFECTS
============================================================================*/
VOS_STATUS
sapSignalHDDevent
(
    ptSapContext sapContext, /* sapContext value */
    tCsrRoamInfo *pCsrRoamInfo,
    eSapHddEvent sapHddevent,
    void         *context
)
{
    VOS_STATUS  vosStatus = VOS_STATUS_SUCCESS;
    tSap_Event sapApAppEvent; /* This now encodes ALL event types */
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Format the Start BSS Complete event to return... */
    if (NULL == sapContext->pfnSapEventCallback)
    {
        VOS_ASSERT(0);
        return VOS_STATUS_E_FAILURE;
    }

    switch (sapHddevent)
    {
        case eSAP_STA_ASSOC_IND:
            //  TODO - Indicate the assoc request indication to OS
            sapApAppEvent.sapHddEventCode = eSAP_STA_ASSOC_IND;

            vos_mem_copy( &sapApAppEvent.sapevt.sapAssocIndication.staMac, pCsrRoamInfo->peerMac,sizeof(tSirMacAddr));
            sapApAppEvent.sapevt.sapAssocIndication.staId = pCsrRoamInfo->staId;
            sapApAppEvent.sapevt.sapAssocIndication.status = 0;
            // Required for indicating the frames to upper layer
            sapApAppEvent.sapevt.sapAssocIndication.beaconLength = pCsrRoamInfo->beaconLength;
            sapApAppEvent.sapevt.sapAssocIndication.beaconPtr = pCsrRoamInfo->beaconPtr;
            sapApAppEvent.sapevt.sapAssocIndication.assocReqLength = pCsrRoamInfo->assocReqLength;
            sapApAppEvent.sapevt.sapAssocIndication.assocReqPtr = pCsrRoamInfo->assocReqPtr;
            sapApAppEvent.sapevt.sapAssocIndication.fWmmEnabled = pCsrRoamInfo->wmmEnabledSta;
            if ( pCsrRoamInfo->u.pConnectedProfile != NULL )
            {
               sapApAppEvent.sapevt.sapAssocIndication.negotiatedAuthType = pCsrRoamInfo->u.pConnectedProfile->AuthType;
               sapApAppEvent.sapevt.sapAssocIndication.negotiatedUCEncryptionType = pCsrRoamInfo->u.pConnectedProfile->EncryptionType;
               sapApAppEvent.sapevt.sapAssocIndication.negotiatedMCEncryptionType = pCsrRoamInfo->u.pConnectedProfile->mcEncryptionType;
               sapApAppEvent.sapevt.sapAssocIndication.fAuthRequired = pCsrRoamInfo->fAuthRequired;
            }
            break;
       case eSAP_START_BSS_EVENT:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, SAP event callback event = %s",
                __func__, "eSAP_START_BSS_EVENT");
            sapApAppEvent.sapHddEventCode = eSAP_START_BSS_EVENT;
            sapApAppEvent.sapevt.sapStartBssCompleteEvent.status = (eSapStatus )context;
            if(pCsrRoamInfo != NULL ){
                sapApAppEvent.sapevt.sapStartBssCompleteEvent.staId = pCsrRoamInfo->staId;
            }
            else
                sapApAppEvent.sapevt.sapStartBssCompleteEvent.staId = 0;
            sapApAppEvent.sapevt.sapStartBssCompleteEvent.operatingChannel = (v_U8_t)sapContext->channel;
            sapApAppEvent.sapevt.sapStartBssCompleteEvent.sessionId =
                    sapContext->sessionId;
            break;

        case eSAP_STOP_BSS_EVENT:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, SAP event callback event = %s",
                       __func__, "eSAP_STOP_BSS_EVENT");
            sapApAppEvent.sapHddEventCode = eSAP_STOP_BSS_EVENT;
            sapApAppEvent.sapevt.sapStopBssCompleteEvent.status = (eSapStatus )context;
            break;

        case eSAP_STA_ASSOC_EVENT:
        case eSAP_STA_REASSOC_EVENT:
        {
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, SAP event callback event = %s",
                __func__, "eSAP_STA_ASSOC_EVENT");
            if (pCsrRoamInfo->fReassocReq)
                sapApAppEvent.sapHddEventCode = eSAP_STA_REASSOC_EVENT;
            else
                sapApAppEvent.sapHddEventCode = eSAP_STA_ASSOC_EVENT;

            //TODO: Need to fill the SET KEY information and pass to HDD
            vos_mem_copy( &sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.staMac,
                         pCsrRoamInfo->peerMac,sizeof(tSirMacAddr));
            sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.staId = pCsrRoamInfo->staId ;
            sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.statusCode = pCsrRoamInfo->statusCode;
            sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.iesLen = pCsrRoamInfo->rsnIELen;
            vos_mem_copy(sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.ies, pCsrRoamInfo->prsnIE,
                        pCsrRoamInfo->rsnIELen);

            if(pCsrRoamInfo->addIELen)
            {
                v_U8_t  len = sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.iesLen;
                sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.iesLen
                                                        += pCsrRoamInfo->addIELen;
                vos_mem_copy(&sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.ies[len], pCsrRoamInfo->paddIE,
                            pCsrRoamInfo->addIELen);
            }

            sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.wmmEnabled = pCsrRoamInfo->wmmEnabledSta;
            sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.status = (eSapStatus )context;
            sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.timingMeasCap = pCsrRoamInfo->timingMeasCap;
            //TODO: Need to fill sapAuthType
            //sapApAppEvent.sapevt.sapStationAssocReassocCompleteEvent.SapAuthType = pCsrRoamInfo->pProfile->negotiatedAuthType;
            break;
        }

        case eSAP_STA_DISASSOC_EVENT:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, SAP event callback event = %s",
                       __func__, "eSAP_STA_DISASSOC_EVENT");
            sapApAppEvent.sapHddEventCode = eSAP_STA_DISASSOC_EVENT;

            vos_mem_copy( &sapApAppEvent.sapevt.sapStationDisassocCompleteEvent.staMac,
                          pCsrRoamInfo->peerMac, sizeof(tSirMacAddr));
            sapApAppEvent.sapevt.sapStationDisassocCompleteEvent.staId = pCsrRoamInfo->staId;
            if (pCsrRoamInfo->reasonCode == eCSR_ROAM_RESULT_FORCED)
                sapApAppEvent.sapevt.sapStationDisassocCompleteEvent.reason = eSAP_USR_INITATED_DISASSOC;
            else
                sapApAppEvent.sapevt.sapStationDisassocCompleteEvent.reason = eSAP_MAC_INITATED_DISASSOC;

            sapApAppEvent.sapevt.sapStationDisassocCompleteEvent.statusCode = pCsrRoamInfo->statusCode;
            sapApAppEvent.sapevt.sapStationDisassocCompleteEvent.status = (eSapStatus )context;
            break;

        case eSAP_STA_SET_KEY_EVENT:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, SAP event callback event = %s",
                       __func__, "eSAP_STA_SET_KEY_EVENT");
            sapApAppEvent.sapHddEventCode = eSAP_STA_SET_KEY_EVENT;
            sapApAppEvent.sapevt.sapStationSetKeyCompleteEvent.status = (eSapStatus )context;
            vos_mem_copy(&sapApAppEvent.sapevt.sapStationSetKeyCompleteEvent.peerMacAddr,
                         pCsrRoamInfo->peerMac,sizeof(tSirMacAddr));
            break;

        case eSAP_STA_DEL_KEY_EVENT :
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, SAP event callback event = %s",
                       __func__, "eSAP_STA_DEL_KEY_EVENT");
            sapApAppEvent.sapHddEventCode = eSAP_STA_DEL_KEY_EVENT;
            sapApAppEvent.sapevt.sapStationDeleteKeyCompleteEvent.status = (eSapStatus )context;
            //TODO: Should we need to send the key information
            //sapApAppEvent.sapevt.sapStationDeleteKeyCompleteEvent.keyId = ;
            break;

        case eSAP_STA_MIC_FAILURE_EVENT :
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, SAP event callback event = %s",
                        __func__, "eSAP_STA_MIC_FAILURE_EVENT");
            sapApAppEvent.sapHddEventCode = eSAP_STA_MIC_FAILURE_EVENT;
            vos_mem_copy( &sapApAppEvent.sapevt.sapStationMICFailureEvent.srcMacAddr,
                          pCsrRoamInfo->u.pMICFailureInfo->srcMacAddr,
                          sizeof(tSirMacAddr));
            vos_mem_copy( &sapApAppEvent.sapevt.sapStationMICFailureEvent.staMac,
                          pCsrRoamInfo->u.pMICFailureInfo->taMacAddr,
                          sizeof(tSirMacAddr));
            vos_mem_copy( &sapApAppEvent.sapevt.sapStationMICFailureEvent.dstMacAddr,
                          pCsrRoamInfo->u.pMICFailureInfo->dstMacAddr,
                          sizeof(tSirMacAddr));
            sapApAppEvent.sapevt.sapStationMICFailureEvent.multicast = pCsrRoamInfo->u.pMICFailureInfo->multicast;
            sapApAppEvent.sapevt.sapStationMICFailureEvent.IV1 = pCsrRoamInfo->u.pMICFailureInfo->IV1;
            sapApAppEvent.sapevt.sapStationMICFailureEvent.keyId = pCsrRoamInfo->u.pMICFailureInfo->keyId;
            vos_mem_copy( sapApAppEvent.sapevt.sapStationMICFailureEvent.TSC,
                          pCsrRoamInfo->u.pMICFailureInfo->TSC,
                          SIR_CIPHER_SEQ_CTR_SIZE);
            break;

        case eSAP_ASSOC_STA_CALLBACK_EVENT:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, SAP event callback event = %s",
                       __func__, "eSAP_ASSOC_STA_CALLBACK_EVENT");
            break;

        case eSAP_WPS_PBC_PROBE_REQ_EVENT:
            sapApAppEvent.sapHddEventCode = eSAP_WPS_PBC_PROBE_REQ_EVENT;

            vos_mem_copy( &sapApAppEvent.sapevt.sapPBCProbeReqEvent.WPSPBCProbeReq,
                          pCsrRoamInfo->u.pWPSPBCProbeReq,
                          sizeof(tSirWPSPBCProbeReq));
            break;

       case eSAP_INDICATE_MGMT_FRAME:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                                 "In %s, SAP event callback event = %s",
                                __func__, "eSAP_INDICATE_MGMT_FRAME");
            sapApAppEvent.sapHddEventCode = eSAP_INDICATE_MGMT_FRAME;
            sapApAppEvent.sapevt.sapManagementFrameInfo.nFrameLength
                                           = pCsrRoamInfo->nFrameLength;
            sapApAppEvent.sapevt.sapManagementFrameInfo.pbFrames
                                           = pCsrRoamInfo->pbFrames;
            sapApAppEvent.sapevt.sapManagementFrameInfo.frameType
                                           = pCsrRoamInfo->frameType;
            sapApAppEvent.sapevt.sapManagementFrameInfo.rxChan
                                           = pCsrRoamInfo->rxChan;

            break;
       case eSAP_REMAIN_CHAN_READY:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                                 "In %s, SAP event callback event = %s",
                                __func__, "eSAP_REMAIN_CHAN_READY");
           sapApAppEvent.sapHddEventCode = eSAP_REMAIN_CHAN_READY;
            break;
       case eSAP_SEND_ACTION_CNF:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                                 "In %s, SAP event callback event = %s",
                                __func__, "eSAP_SEND_ACTION_CNF");
            sapApAppEvent.sapHddEventCode = eSAP_SEND_ACTION_CNF;
            sapApAppEvent.sapevt.sapActionCnf.actionSendSuccess = (eSapStatus)context;
            break;

       case eSAP_DISCONNECT_ALL_P2P_CLIENT:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                             "In %s, SAP event callback event = %s",
                            __func__, "eSAP_DISCONNECT_ALL_P2P_CLIENT");
            sapApAppEvent.sapHddEventCode = eSAP_DISCONNECT_ALL_P2P_CLIENT;
            sapApAppEvent.sapevt.sapActionCnf.actionSendSuccess = (eSapStatus)context;
            break;

       case eSAP_MAC_TRIG_STOP_BSS_EVENT :
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                             "In %s, SAP event callback event = %s",
                            __func__, "eSAP_MAC_TRIG_STOP_BSS_EVENT");
            sapApAppEvent.sapHddEventCode = eSAP_MAC_TRIG_STOP_BSS_EVENT;
            sapApAppEvent.sapevt.sapActionCnf.actionSendSuccess = (eSapStatus)context;
            break;


        case eSAP_UNKNOWN_STA_JOIN:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                       "In %s, SAP event callback event = %s",
                       __func__, "eSAP_UNKNOWN_STA_JOIN");
            sapApAppEvent.sapHddEventCode = eSAP_UNKNOWN_STA_JOIN;
            vos_mem_copy((v_PVOID_t)sapApAppEvent.sapevt.sapUnknownSTAJoin.macaddr.bytes,
                         (v_PVOID_t)context, sizeof(v_MACADDR_t));
            break;

        case eSAP_MAX_ASSOC_EXCEEDED:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                    "In %s, SAP event callback event = %s",
                    __func__, "eSAP_MAX_ASSOC_EXCEEDED");
            sapApAppEvent.sapHddEventCode = eSAP_MAX_ASSOC_EXCEEDED;
            vos_mem_copy((v_PVOID_t)sapApAppEvent.sapevt.sapMaxAssocExceeded.macaddr.bytes,
                    (v_PVOID_t)pCsrRoamInfo->peerMac, sizeof(v_MACADDR_t));
            break;

        case eSAP_CHANNEL_CHANGE_EVENT:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                    "In %s, SAP event callback event = %s",
                    __func__, "eSAP_CHANNEL_CHANGE_EVENT");
            sapApAppEvent.sapHddEventCode = eSAP_CHANNEL_CHANGE_EVENT;
            sapApAppEvent.sapevt.sapChannelChange.operatingChannel =
               sapContext->SapDfsInfo.target_channel;

        default:
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, "In %s, SAP Unknown callback event = %d",
                       __func__,sapHddevent);
            break;
    }
    vosStatus = (*sapContext->pfnSapEventCallback)
                (
                 &sapApAppEvent,
                 sapContext->pUsrContext//userdataforcallback - hdd opaque handle
                 );

    return vosStatus;

} /* sapSignalApAppStartBssEvent */

/*==========================================================================
  FUNCTION    sapFsm

  DESCRIPTION
    SAP State machine entry function

  DEPENDENCIES
    NA.

  PARAMETERS

    IN
    sapContext  : Sap Context value
    sapEvent    : State machine event
    status      : Return the SAP status here

  RETURN VALUE
    The VOS_STATUS code associated with performing the operation

    VOS_STATUS_SUCCESS: Success

  SIDE EFFECTS
============================================================================*/
VOS_STATUS
sapFsm
(
    ptSapContext sapContext,    /* sapContext value */
    ptWLAN_SAPEvent sapEvent   /* State machine event */
)
{
   /* Retrieve the phy link state machine structure
     * from the sapContext value
     */
    eSapFsmStates_t stateVar = sapContext->sapsMachine; /*state var that keeps track of state machine*/
    tCsrRoamInfo    *roamInfo = (tCsrRoamInfo *)(sapEvent->params);
    v_U32_t msg = sapEvent->event;  /* State machine input event message */
    VOS_STATUS vosStatus = VOS_STATUS_E_FAILURE;

    switch (stateVar)
    {
        case eSAP_DISCONNECTED:
            if ((msg == eSAP_HDD_START_INFRA_BSS))
            {
                /* Transition from eSAP_DISCONNECTED to eSAP_CH_SELECT (both without substates) */
                VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, new from state %s => %s",
                            __func__, "eSAP_DISCONNECTED", "eSAP_CH_SELECT");

                /* There can be one SAP Session for softap */
                if (sapContext->isSapSessionOpen == eSAP_TRUE)
                {
                   VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_FATAL,
                        "%s:SME Session is already opened\n",__func__);
                   return VOS_STATUS_E_EXISTS;
                }

                sapContext->sessionId = 0xff;

                if ((sapContext->channel == AUTO_CHANNEL_SELECT) &&
                    (sapContext->isScanSessionOpen == eSAP_FALSE))
                {
                    tANI_U32 type, subType;
                    tHalHandle hHal = VOS_GET_HAL_CB(sapContext->pvosGCtx);
                    if (NULL == hHal)
                    {
                        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                            "In %s, NULL hHal in state %s, msg %d",
                            __func__, "eSAP_DISCONNECTED", msg);
                    }
                    else if(VOS_STATUS_SUCCESS == vos_get_vdev_types(VOS_STA_MODE,
                           &type, &subType)) {
                           /* Open SME Session for scan */
                           if(eHAL_STATUS_SUCCESS  != sme_OpenSession(hHal,
                                 NULL, sapContext, sapContext->self_mac_addr,
                                 &sapContext->sessionId, type, subType))
                           {
                                 VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                                     "Error: In %s calling sme_OpenSession", __func__);
                           } else {
                                 sapContext->isScanSessionOpen = eSAP_TRUE;
                           }
                    }
                }
                /* Set SAP device role */
                sapContext->sapsMachine = eSAP_CH_SELECT;

                /* Perform sme_ScanRequest */
                vosStatus = sapGotoChannelSel(sapContext, sapEvent);

                /* Transition from eSAP_DISCONNECTED to eSAP_CH_SELECT (both without substates) */
                VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, from state %s => %s",
                           __func__, "eSAP_DISCONNECTED", "eSAP_CH_SELECT");
            }
            if (msg == eSAP_DFS_CHANNEL_CAC_START)
            {
               /* No need of state check here, caller is expected to perform
                * the checks before sending the event
                */
               sapContext->sapsMachine = eSAP_DFS_CAC_WAIT;

               VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_DEBUG,
               "ENTERTRED eSAP_DISCONNECTED-->eSAP_DFS_CAC_WAIT\n");
               sapStartDfsCacTimer(sapContext);
            }
            if (msg == eSAP_CHANNEL_SELECTION_FAILED)
            {
                 /* Set SAP device role */
                sapContext->sapsMachine = eSAP_CH_SELECT;

                /* Perform sme_ScanRequest */
                vosStatus = sapGotoChannelSel(sapContext, sapEvent);
            }
            else
            {
                 VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, "In %s, in state %s, event msg %d",
                             __func__, "eSAP_DISCONNECTED", msg);
            }
            break;

        case eSAP_CH_SELECT:
            if (sapContext->isScanSessionOpen == eSAP_TRUE)
            {
                 /* scan completed, so close the session */
                 tHalHandle  hHal = VOS_GET_HAL_CB(sapContext->pvosGCtx);
                 if (NULL == hHal)
                 {
                     VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, "In %s, NULL hHal in state %s, msg %d",
                             __func__, "eSAP_CH_SELECT", msg);
                 } else {
                     if(eHAL_STATUS_SUCCESS != sme_CloseSession(hHal,
                                      sapContext->sessionId, NULL, NULL))
                     {
                         VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, "In %s CloseSession error event msg %d",
                                __func__, msg);
                     } else {
                         sapContext->isScanSessionOpen = eSAP_FALSE;
                     }
                 }
                 sapContext->sessionId = 0xff;
            }

            if (msg == eSAP_MAC_SCAN_COMPLETE)
            {
                 /* Transition from eSAP_CH_SELECT to eSAP_STARTING (both without substates) */
                 VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, from state %s => %s",
                            __func__, "eSAP_CH_SELECT", "eSAP_STARTING");
                 // Channel selected. Now can sapGotoStarting
                 sapContext->sapsMachine = eSAP_STARTING;
                 // Specify the channel
                 sapContext->csrRoamProfile.ChannelInfo.numOfChannels = 1;
                 sapContext->csrRoamProfile.ChannelInfo.ChannelList = &sapContext->csrRoamProfile.operationChannel;
                 sapContext->csrRoamProfile.operationChannel = (tANI_U8)sapContext->channel;
                 vosStatus = sapGotoStarting( sapContext, sapEvent, eCSR_BSS_TYPE_INFRA_AP);
                 /* Transition from eSAP_CH_SELECT to eSAP_STARTING (both without substates) */
                 VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, from state %s => %s",
                             __func__, "eSAP_CH_SELECT", "eSAP_STARTING");
            }
            else
            {
                VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, "In %s, in state %s, invalid event msg %d",
                            __func__, "eSAP_CH_SELECT", msg);
            }
            break;

        case eSAP_DFS_CAC_WAIT:
            if (msg == eSAP_DFS_CHANNEL_CAC_START)
            {
                VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, from state %s => %s",
                            __func__, "eSAP_CH_SELECT", "eSAP_DFS_CAC_WAIT");
                sapStartDfsCacTimer(sapContext);
            }
            else if (msg == eSAP_DFS_CHANNEL_CAC_RADAR_FOUND)
            {
               /* Radar found while performing channel availability
                * check, need to switch the channel again
                */
               eCsrPhyMode phyMode =
                  sapConvertSapPhyModeToCsrPhyMode(sapContext->csrRoamProfile.phyMode);
               tHalHandle hHal =
                  (tHalHandle)vos_get_context(VOS_MODULE_ID_SME, sapContext->pvosGCtx);

               /* SAP to be moved to DISCONNECTING state */
               sapContext->sapsMachine = eSAP_DISCONNECTING;

               VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO,
                  "ENTERTRED CAC WAIT STATE-->eSAP_DISCONNECTING\n");
                if (NULL == hHal)
                {
                   VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                               "In %s, NULL hHal in state %s, msg %d",
                               __func__, "eSAP_DFS_CAC_WAIT", msg);
                }
                else if (sapContext->SapDfsInfo.target_channel)
                {
                   sme_SelectCBMode(hHal, phyMode,
                                   sapContext->SapDfsInfo.target_channel);
                }

               /*
                * eSAP_DFS_CHANNEL_CAC_RADAR_FOUND:
                * A Radar is found on current DFS Channel
                * while in CAC WAIT period So, do a channel switch
                * to randomly selected  target channel.
                * Send the Channel change message to SME/PE.
                * sap_radar_found_status is set to 1
                */

               WLANSAP_ChannelChangeRequest((v_PVOID_t)sapContext,
                              sapContext->SapDfsInfo.target_channel);
            }
            else if (msg == eSAP_DFS_CHANNEL_CAC_END)
            {
               /*
                * eSAP_DFS_CHANNEL_CAC_END:
                * CAC Period elapsed and there was no radar
                * found so, SAP can continue beaconing.
                * sap_radar_found_status is set to 0
                */
               sapContext->SapDfsInfo.sap_radar_found_status = VOS_FALSE;

               /* Start beaconing on the new channel */
               WLANSAP_StartBeaconReq((v_PVOID_t)sapContext);

               /* Transition from eSAP_STARTING to eSAP_STARTED
                * (both without substates)
                */
               VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                         "In %s, from state channel = %d %s => %s",
                         __func__,sapContext->channel, "eSAP_STARTING",
                         "eSAP_STARTED");

               sapContext->sapsMachine = eSAP_STARTED;

               /*Action code for transition */
               vosStatus = sapSignalHDDevent(sapContext, roamInfo,
                                             eSAP_START_BSS_EVENT,
                                             (v_PVOID_t)eSAP_STATUS_SUCCESS);

               /* Transition from eSAP_STARTING to eSAP_STARTED
                * (both without substates)
                */
               VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                         "In %s, from state %s => %s",
                         __func__, "eSAP_DFS_CAC_WAIT", "eSAP_STARTED");
            }
            else
            {
                VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                          "In %s, in state %s, invalid event msg %d",
                          __func__, "eSAP_DFS_CAC_WAIT", msg);
            }
            break;

        case eSAP_STARTING:
            if (msg == eSAP_MAC_START_BSS_SUCCESS )
            {
                /* Transition from eSAP_STARTING to eSAP_STARTED (both without substates) */
                VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, from state channel = %d %s => %s",
                            __func__,sapContext->channel, "eSAP_STARTING", "eSAP_STARTED");

                 sapContext->sapsMachine = eSAP_STARTED;

                 /*Action code for transition */
                 vosStatus = sapSignalHDDevent( sapContext, roamInfo, eSAP_START_BSS_EVENT, (v_PVOID_t)eSAP_STATUS_SUCCESS);

                 /* Transition from eSAP_STARTING to eSAP_STARTED (both without substates) */
                 VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, from state %s => %s",
                            __func__, "eSAP_STARTING", "eSAP_STARTED");

                 /* The upper layers have been informed that AP is up and
                  * running, however, the AP is still not beaconing, until
                  * CAC is done if the operating channel is DFS
                  */
                 if (vos_nv_getChannelEnabledState(sapContext->channel) == NV_CHANNEL_DFS)
                 {
                     /* Move the device in CAC_WAIT_STATE */
                     sapContext->sapsMachine = eSAP_DFS_CAC_WAIT;

                     /* TODO: Need to stop the OS transmit queues, so that no traffic
                      * can flow down the stack
                      */

                     /* Start CAC wait timer */
                     sapStartDfsCacTimer(sapContext);
                 }
             }
             else if (msg == eSAP_MAC_START_FAILS)
             {
                 /*Transition from STARTING to DISCONNECTED (both without substates)*/
                 VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, "In %s, from state %s => %s",
                            __func__, "eSAP_STARTING", "eSAP_DISCONNECTED");

                 /*Action code for transition */
                 vosStatus = sapSignalHDDevent( sapContext, NULL, eSAP_START_BSS_EVENT,(v_PVOID_t) eSAP_STATUS_FAILURE);
                 vosStatus =  sapGotoDisconnected(sapContext);

                 /*Advance outer statevar */
                 sapContext->sapsMachine = eSAP_DISCONNECTED;
             }
             else if (msg == eSAP_HDD_STOP_INFRA_BSS)
             {
                 /*Transition from eSAP_STARTING to eSAP_DISCONNECTING (both without substates)*/
                 VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, from state %s => %s",
                             __func__, "eSAP_STARTING", "eSAP_DISCONNECTING");

                 /*Advance outer statevar */
                 sapContext->sapsMachine = eSAP_DISCONNECTED;
                 vosStatus = sapSignalHDDevent( sapContext, NULL, eSAP_START_BSS_EVENT, (v_PVOID_t)eSAP_STATUS_FAILURE);
                 vosStatus = sapGotoDisconnected(sapContext);
                 /* Close the SME session*/

                 if (eSAP_TRUE == sapContext->isSapSessionOpen)
                 {
                    tHalHandle hHal = VOS_GET_HAL_CB(sapContext->pvosGCtx);
                    if (NULL == hHal)
                    {
                       VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                                  "In %s, NULL hHal in state %s, msg %d",
                                  __func__, "eSAP_STARTING", msg);
                    }
                    else if (eHAL_STATUS_SUCCESS ==
                         sme_CloseSession(hHal,
                                         sapContext->sessionId, NULL, NULL))
                     {
                         sapContext->isSapSessionOpen = eSAP_FALSE;
                     }
                 }
             }
             else if (msg == eSAP_OPERATING_CHANNEL_CHANGED)
             {
                 /* The operating channel has changed, update hostapd */
                 sapContext->channel =
                     (tANI_U8)sapContext->SapDfsInfo.target_channel;

                 sapContext->sapsMachine = eSAP_STARTED;

                 VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                           "In %s, from state %s => %s",
                           __func__, "eSAP_STARTING", "eSAP_STARTED");

                 /* Indicate change in the state to upper layers */
                 vosStatus = sapSignalHDDevent(sapContext, roamInfo,
                                               eSAP_START_BSS_EVENT,
                                               (v_PVOID_t)eSAP_STATUS_SUCCESS);
             }
             else
             {
                 VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                             "In %s, in state %s, invalid event msg %d",
                             __func__, "eSAP_STARTING", msg);
                 /* Intentionally left blank */
             }
             break;

        case eSAP_STARTED:
            if (msg == eSAP_HDD_STOP_INFRA_BSS)
            {
                /* Transition from eSAP_STARTED to eSAP_DISCONNECTING (both without substates) */
                VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, from state %s => %s",
                           __func__, "eSAP_STARTED", "eSAP_DISCONNECTING");
                sapContext->sapsMachine = eSAP_DISCONNECTING;
                vosStatus = sapGotoDisconnecting(sapContext);
            }
            else if (eSAP_DFS_CHNL_SWITCH_ANNOUNCEMENT_START == msg)
            {
                /* Radar is seen on the current operating channel
                 * send CSA IE for all associated stations
                 */
                VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                          "In %s, Send CSA IE Request", __func__);

                /* Request for CSA IE transmission */
                WLANSAP_DfsSendCSAIeRequest(sapContext);
            }
            else
            {
                VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, "In %s, in state %s, invalid event msg %d",
                           __func__, "eSAP_STARTED", msg);
            }
            break;

        case eSAP_DISCONNECTING:
            if (msg == eSAP_MAC_READY_FOR_CONNECTIONS)
            {
                /* Transition from eSAP_DISCONNECTING to eSAP_DISCONNECTED (both without substates) */
                VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH, "In %s, from state %s => %s",
                          __func__, "eSAP_DISCONNECTING", "eSAP_DISCONNECTED");

                sapContext->sapsMachine = eSAP_DISCONNECTED;
                /* Close the SME session*/
                if (eSAP_TRUE == sapContext->isSapSessionOpen)
                {
                    tHalHandle hHal = VOS_GET_HAL_CB(sapContext->pvosGCtx);

                    if (NULL == hHal)
                    {
                        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                                   "In %s, NULL hHal in state %s, msg %d",
                                   __func__, "eSAP_DISCONNECTING", msg);
                    }
                    else
                    {
                        sapContext->isSapSessionOpen = eSAP_FALSE;
                        if (!HAL_STATUS_SUCCESS(
                            sme_CloseSession(hHal,
                                     sapContext->sessionId,
                                     sapRoamSessionCloseCallback, sapContext)))
                        {
                            vosStatus = sapSignalHDDevent(sapContext, NULL,
                                              eSAP_STOP_BSS_EVENT,
                                              (v_PVOID_t) eSAP_STATUS_SUCCESS);
                        }
                    }
                }
            }
            else if (msg == eWNI_SME_CHANNEL_CHANGE_REQ)
            {
               /* Most likely, radar has been detected and SAP wants to
                * change the channel
                */
                vosStatus =
                        WLANSAP_ChannelChangeRequest((v_PVOID_t)sapContext,
                        sapContext->SapDfsInfo.target_channel);

                VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO,
                          "In %s, Sending DFS eWNI_SME_CHANNEL_CHANGE_REQ",
                          __func__);
            }
            else
            {
                VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                           "In %s, in state %s, invalid event msg %d",
                          __func__, "eSAP_DISCONNECTING", msg);
            }
            break;
      }
      return vosStatus;
}// sapFsm


eSapStatus
sapconvertToCsrProfile(tsap_Config_t *pconfig_params, eCsrRoamBssType bssType, tCsrRoamProfile *profile)
{
    //Create Roam profile for SoftAP to connect
    profile->BSSType = eCSR_BSS_TYPE_INFRA_AP;
    profile->SSIDs.numOfSSIDs = 1;
    profile->csrPersona = pconfig_params->persona;
    profile->disableDFSChSwitch = pconfig_params->disableDFSChSwitch;

    vos_mem_zero(profile->SSIDs.SSIDList[0].SSID.ssId,
                 sizeof(profile->SSIDs.SSIDList[0].SSID.ssId));

    //Flag to not broadcast the SSID information
    profile->SSIDs.SSIDList[0].ssidHidden =  pconfig_params->SSIDinfo.ssidHidden;

    profile->SSIDs.SSIDList[0].SSID.length = pconfig_params->SSIDinfo.ssid.length;
    vos_mem_copy(&profile->SSIDs.SSIDList[0].SSID.ssId, pconfig_params->SSIDinfo.ssid.ssId,
                  sizeof(pconfig_params->SSIDinfo.ssid.ssId));

    profile->negotiatedAuthType = eCSR_AUTH_TYPE_OPEN_SYSTEM;

    if (pconfig_params->authType == eSAP_OPEN_SYSTEM)
    {
        profile->negotiatedAuthType = eCSR_AUTH_TYPE_OPEN_SYSTEM;
    }
    else if (pconfig_params->authType == eSAP_SHARED_KEY)
    {
        profile->negotiatedAuthType = eCSR_AUTH_TYPE_SHARED_KEY;
    }
    else
    {
        profile->negotiatedAuthType = eCSR_AUTH_TYPE_AUTOSWITCH;
    }

    profile->AuthType.numEntries = 1;
    profile->AuthType.authType[0] = eCSR_AUTH_TYPE_OPEN_SYSTEM;

    //Always set the Encryption Type
    profile->EncryptionType.numEntries = 1;
    profile->EncryptionType.encryptionType[0] = pconfig_params->RSNEncryptType;

    profile->mcEncryptionType.numEntries = 1;
    profile->mcEncryptionType.encryptionType[0] = pconfig_params->mcRSNEncryptType;

    if (pconfig_params->privacy & eSAP_SHARED_KEY)
    {
        profile->AuthType.authType[0] = eCSR_AUTH_TYPE_SHARED_KEY;
    }

    profile->privacy = pconfig_params->privacy;
    profile->fwdWPSPBCProbeReq = pconfig_params->fwdWPSPBCProbeReq;

    if (pconfig_params->authType == eSAP_SHARED_KEY)
    {
        profile->csr80211AuthType = eSIR_SHARED_KEY;
    }
    else if (pconfig_params->authType == eSAP_OPEN_SYSTEM)
    {
        profile->csr80211AuthType = eSIR_OPEN_SYSTEM;
    }
    else
    {
        profile->csr80211AuthType = eSIR_AUTO_SWITCH;
    }

    //Initialize we are not going to use it
    profile->pWPAReqIE = NULL;
    profile->nWPAReqIELength = 0;

    //set the RSN/WPA IE
    profile->pRSNReqIE = NULL;
    profile->nRSNReqIELength = pconfig_params->RSNWPAReqIELength;
    if (pconfig_params->RSNWPAReqIELength)
    {
        profile->pRSNReqIE = vos_mem_malloc(pconfig_params->RSNWPAReqIELength);
        if( NULL == profile->pRSNReqIE )
        {
           VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR, " %s Fail to alloc memory", __func__);
           return eSAP_STATUS_FAILURE;
        }
        vos_mem_copy(profile->pRSNReqIE, pconfig_params->pRSNWPAReqIE, pconfig_params->RSNWPAReqIELength);
        profile->nRSNReqIELength = pconfig_params->RSNWPAReqIELength;
    }

    // Turn off CB mode
    profile->CBMode = eCSR_CB_OFF;

    //set the phyMode to accept anything
    //Best means everything because it covers all the things we support
    profile->phyMode = pconfig_params->SapHw_mode; /*eCSR_DOT11_MODE_BEST*/

    //Configure beaconInterval
    profile->beaconInterval = (tANI_U16)pconfig_params->beacon_int;

    // set DTIM period
    profile->dtimPeriod = pconfig_params->dtim_period;

    //set Uapsd enable bit
    profile->ApUapsdEnable = pconfig_params->UapsdEnable;

    //Enable protection parameters
    profile->protEnabled       = pconfig_params->protEnabled;
    profile->obssProtEnabled   = pconfig_params->obssProtEnabled;
    profile->cfg_protection    = pconfig_params->ht_capab;

    //country code
    if (pconfig_params->countryCode[0])
        vos_mem_copy(profile->countryCode, pconfig_params->countryCode, WNI_CFG_COUNTRY_CODE_LEN);
    profile->ieee80211d = pconfig_params->ieee80211d;
    //wps config info
    profile->wps_state = pconfig_params->wps_state;

#ifdef WLAN_FEATURE_11W
    // MFP capable/required
    profile->MFPCapable = pconfig_params->mfpCapable ? 1 : 0;
    profile->MFPRequired = pconfig_params->mfpRequired ? 1 : 0;
#endif

    return eSAP_STATUS_SUCCESS; /* Success.  */
}

/**
 * FUNCTION: sapConvertSapPhyModeToCsrPhyMode
 * Called internally by SAP
 */
eCsrPhyMode sapConvertSapPhyModeToCsrPhyMode( eSapPhyMode sapPhyMode )
{
    switch (sapPhyMode)
    {
      case (eSAP_DOT11_MODE_abg):
         return eCSR_DOT11_MODE_abg;
      case (eSAP_DOT11_MODE_11b):
         return eCSR_DOT11_MODE_11b;
      case (eSAP_DOT11_MODE_11g):
         return eCSR_DOT11_MODE_11g;
      case (eSAP_DOT11_MODE_11n):
         return eCSR_DOT11_MODE_11n;
      case (eSAP_DOT11_MODE_11g_ONLY):
         return eCSR_DOT11_MODE_11g_ONLY;
      case (eSAP_DOT11_MODE_11n_ONLY):
         return eCSR_DOT11_MODE_11n_ONLY;
      case (eSAP_DOT11_MODE_11b_ONLY):
         return eCSR_DOT11_MODE_11b_ONLY;
#ifdef WLAN_FEATURE_11AC
      case (eSAP_DOT11_MODE_11ac_ONLY):
         return eCSR_DOT11_MODE_11ac_ONLY;
      case (eSAP_DOT11_MODE_11ac):
         return eCSR_DOT11_MODE_11ac;
#endif
      default:
         return eCSR_DOT11_MODE_AUTO;
    }
}

void sapFreeRoamProfile(tCsrRoamProfile *profile)
{
   if(profile->pRSNReqIE)
   {
      vos_mem_free(profile->pRSNReqIE);
      profile->pRSNReqIE = NULL;
   }
}


void
sapSortMacList(v_MACADDR_t *macList, v_U8_t size)
{
    v_U8_t outer, inner;
    v_MACADDR_t temp;
    v_SINT_t nRes = -1;

    if ((NULL == macList) || (size >= MAX_ACL_MAC_ADDRESS))
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                      "In %s, either buffer is NULL or size = %d is more."
                      , __func__, size);
        return;
    }

    for(outer = 0; outer < size; outer++)
    {
        for(inner = 0; inner < size - 1; inner++)
        {
            nRes = vos_mem_compare2((macList + inner)->bytes, (macList + inner + 1)->bytes, sizeof(v_MACADDR_t));
            if (nRes > 0)
            {
                vos_mem_copy(temp.bytes, (macList + inner + 1)->bytes, sizeof(v_MACADDR_t));
                vos_mem_copy((macList + inner + 1)->bytes, (macList + inner)->bytes, sizeof(v_MACADDR_t));
                vos_mem_copy((macList + inner)->bytes, temp.bytes, sizeof(v_MACADDR_t));
             }
        }
    }
}

eSapBool
sapSearchMacList(v_MACADDR_t *macList, v_U8_t num_mac, v_U8_t *peerMac, v_U8_t *index)
{
    v_SINT_t nRes = -1;
    v_S7_t nStart = 0, nEnd, nMiddle;
    nEnd = num_mac - 1;

    while (nStart <= nEnd)
    {
        nMiddle = (nStart + nEnd) / 2;
        nRes = vos_mem_compare2(&macList[nMiddle], peerMac, sizeof(v_MACADDR_t));

        if (0 == nRes)
        {
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                    "search SUCC");
            // "index equals NULL" means the caller does not need the
            // index value of the peerMac being searched
            if (index != NULL)
            {
                *index = (v_U8_t) nMiddle;
                VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                        "index %d", *index);
            }
            return eSAP_TRUE;
        }
        if (nRes < 0)
            nStart = nMiddle + 1;
        else
            nEnd = nMiddle - 1;
    }

    VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
            "search not succ");
    return eSAP_FALSE;
}

void
sapAddMacToACL(v_MACADDR_t *macList, v_U8_t *size, v_U8_t *peerMac)
{
    v_SINT_t nRes = -1;
    int i;
    VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,"add acl entered");
    for (i=((*size)-1); i>=0; i--)
    {
        nRes = vos_mem_compare2(&macList[i], peerMac, sizeof(v_MACADDR_t));
        if (nRes > 0)
        {
            /* Move alphabetically greater mac addresses one index down to allow for insertion
               of new mac in sorted order */
            vos_mem_copy((macList+i+1)->bytes,(macList+i)->bytes, sizeof(v_MACADDR_t));
        }
        else
        {
            break;
        }
    }
    //This should also take care of if the element is the first to be added in the list
    vos_mem_copy((macList+i+1)->bytes, peerMac, sizeof(v_MACADDR_t));
    // increment the list size
    (*size)++;
}

void
sapRemoveMacFromACL(v_MACADDR_t *macList, v_U8_t *size, v_U8_t index)
{
    int i;
    VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,"remove acl entered");
    /* return if the list passed is empty. Ideally this should never happen since this funcn is always
       called after sapSearchMacList to get the index of the mac addr to be removed and this will
       only get called if the search is successful. Still no harm in having the check */
    if (macList==NULL) return;
    for (i=index; i<((*size)-1); i++)
    {
        /* Move mac addresses starting from "index" passed one index up to delete the void
           created by deletion of a mac address in ACL */
        vos_mem_copy((macList+i)->bytes,(macList+i+1)->bytes, sizeof(v_MACADDR_t));
    }
    // The last space should be made empty since all mac addesses moved one step up
    vos_mem_zero((macList+(*size)-1)->bytes, sizeof(v_MACADDR_t));
    //reduce the list size by 1
    (*size)--;
}

void sapPrintACL(v_MACADDR_t *macList, v_U8_t size)
{
    int i;
    v_BYTE_t *macArray;
    VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,"print acl entered");

    if ((NULL == macList) || (size == 0) || (size >= MAX_ACL_MAC_ADDRESS))
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                    "In %s, either buffer is NULL or size %d is incorrect."
                    , __func__, size);
        return;
    }

    for (i=0; i<size; i++)
    {
        macArray = (macList+i)->bytes;
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                "** ACL entry %i - "MAC_ADDRESS_STR, i,
                MAC_ADDR_ARRAY(macArray));
    }
    return;
}

VOS_STATUS
sapIsPeerMacAllowed(ptSapContext sapContext, v_U8_t *peerMac)
{
    if (eSAP_ALLOW_ALL == sapContext->eSapMacAddrAclMode)
              return VOS_STATUS_SUCCESS;

    if (sapSearchMacList(sapContext->acceptMacList, sapContext->nAcceptMac, peerMac, NULL))
        return VOS_STATUS_SUCCESS;

    if (sapSearchMacList(sapContext->denyMacList, sapContext->nDenyMac, peerMac, NULL))
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                  "In %s, Peer "MAC_ADDRESS_STR" in deny list",
                  __func__, MAC_ADDR_ARRAY(peerMac));
        return VOS_STATUS_E_FAILURE;
    }

    // A new station CAN associate, unless in deny list. Less stringent mode
    if (eSAP_ACCEPT_UNLESS_DENIED == sapContext->eSapMacAddrAclMode)
        return VOS_STATUS_SUCCESS;

    // A new station CANNOT associate, unless in accept list. More stringent mode
    if (eSAP_DENY_UNLESS_ACCEPTED == sapContext->eSapMacAddrAclMode)
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                  "In %s, Peer "MAC_ADDRESS_STR" denied, Mac filter mode is eSAP_DENY_UNLESS_ACCEPTED",
                  __func__,  MAC_ADDR_ARRAY(peerMac));
        return VOS_STATUS_E_FAILURE;
    }

    /* The new STA is neither in accept list nor in deny list. In this case, deny the association
     * but send a wifi event notification indicating the mac address being denied
     */
    if (eSAP_SUPPORT_ACCEPT_AND_DENY == sapContext->eSapMacAddrAclMode)
    {
        sapSignalHDDevent(sapContext, NULL, eSAP_UNKNOWN_STA_JOIN, (v_PVOID_t)peerMac);
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                  "In %s, Peer "MAC_ADDRESS_STR" denied, Mac filter mode is eSAP_SUPPORT_ACCEPT_AND_DENY",
                  __func__, MAC_ADDR_ARRAY(peerMac));
        return VOS_STATUS_E_FAILURE;
    }
    return VOS_STATUS_SUCCESS;
}

#ifdef SOFTAP_CHANNEL_RANGE
static VOS_STATUS sapGetChannelList(ptSapContext sapContext,
                                 v_U8_t **channelList, v_U8_t *numberOfChannels)
{
    v_U32_t startChannelNum;
    v_U32_t endChannelNum;
    v_U32_t operatingBand;
    v_U8_t  loopCount;
    v_U8_t *list;
    v_U8_t channelCount;
    v_U8_t bandStartChannel;
    v_U8_t bandEndChannel ;
    v_U32_t enableLTECoex;
    tHalHandle hHal = VOS_GET_HAL_CB(sapContext->pvosGCtx);
    v_U32_t dfsMasterCap = FALSE;
#ifdef FEATURE_WLAN_CH_AVOID
    v_U8_t i;
#endif

    if (NULL == hHal)
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
             "Invalid HAL pointer from pvosGCtx on sapGetChannelList");
        *numberOfChannels = 0;
        *channelList = NULL;
        return VOS_STATUS_E_FAULT;
    }

    if ( eCSR_BAND_ALL == sapContext->scanBandPreference)
    {
        ccmCfgGetInt(hHal, WNI_CFG_SAP_CHANNEL_SELECT_START_CHANNEL, &startChannelNum);
        ccmCfgGetInt(hHal, WNI_CFG_SAP_CHANNEL_SELECT_END_CHANNEL, &endChannelNum);
        ccmCfgGetInt(hHal, WNI_CFG_SAP_CHANNEL_SELECT_OPERATING_BAND, &operatingBand);

        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO,
                 "%s:sapGetChannelList: startChannel %d,EndChannel %d,Operatingband:%d",
                 __func__,startChannelNum,endChannelNum,operatingBand);

        switch(operatingBand)
        {
            case eSAP_RF_SUBBAND_2_4_GHZ:
               bandStartChannel = RF_CHAN_1;
               bandEndChannel = RF_CHAN_14;
               break;

            case eSAP_RF_SUBBAND_5_LOW_GHZ:
               bandStartChannel = RF_CHAN_36;
               bandEndChannel = RF_CHAN_64;
               break;

            case eSAP_RF_SUBBAND_5_MID_GHZ:
               bandStartChannel = RF_CHAN_100;
               bandEndChannel = RF_CHAN_140;
               break;

            case eSAP_RF_SUBBAND_5_HIGH_GHZ:
               bandStartChannel = RF_CHAN_149;
               bandEndChannel = RF_CHAN_165;
               break;

            case eSAP_RF_SUBBAND_5_ALL_GHZ:
               bandStartChannel = RF_CHAN_36;
               bandEndChannel = RF_CHAN_165;
               break;

            default:
               VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                 "sapGetChannelList:OperatingBand not valid ");
               /* assume 2.4 GHz */
               bandStartChannel = RF_CHAN_1;
               bandEndChannel = RF_CHAN_14;
               break;
        }
    }
    else
    {
        if ( sapContext->allBandScanned == eSAP_FALSE )
        {
            //first band scan
            sapContext->currentPreferredBand = sapContext->scanBandPreference;
        }
        else
        {
            //scan next band
            if ( eCSR_BAND_24 == sapContext->scanBandPreference )
                sapContext->currentPreferredBand = eCSR_BAND_5G;
            else
                sapContext->currentPreferredBand = eCSR_BAND_24;
        }
        switch(sapContext->currentPreferredBand)
        {
            case eCSR_BAND_24:
               bandStartChannel = RF_CHAN_1;
               bandEndChannel = RF_CHAN_14;
               startChannelNum = 1;
               endChannelNum = 14;
               break;

            case eCSR_BAND_5G:
               bandStartChannel = RF_CHAN_36;
               bandEndChannel = RF_CHAN_165;
               startChannelNum = 36;
               endChannelNum = 165;
               break;

            default:
               VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                 "sapGetChannelList:bandPreference not valid ");
               /* assume 2.4 GHz */
               bandStartChannel = RF_CHAN_1;
               bandEndChannel = RF_CHAN_14;
               startChannelNum = 1;
               endChannelNum = 14;
               break;
        }
    }

    ccmCfgGetInt(hHal, WNI_CFG_ENABLE_LTE_COEX, &enableLTECoex);

    /*Check if LTE coex is enabled and 2.4GHz is selected*/
    if (enableLTECoex && (bandStartChannel == RF_CHAN_1)
       && (bandEndChannel == RF_CHAN_14))
    {
        /*Set 2.4GHz upper limit to channel 9 for LTE COEX*/
        bandEndChannel = RF_CHAN_9;
    }
    /* Allocate the max number of channel supported */
    list = (v_U8_t *)vos_mem_malloc(NUM_5GHZ_CHANNELS);
    if (NULL == list)
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                   "%s: Unable to allocate channel list", __func__);
        *numberOfChannels = 0;
        *channelList = NULL;
        return VOS_STATUS_E_RESOURCES;
    }

    /*Search for the Active channels in the given range */
    ccmCfgGetInt(hHal, WNI_CFG_DFS_MASTER_ENABLED, &dfsMasterCap);
    channelCount = 0;
    for (loopCount = bandStartChannel; loopCount <= bandEndChannel;
                    loopCount++)
    {
        if((startChannelNum <= rfChannels[loopCount].channelNum)&&
            (endChannelNum >= rfChannels[loopCount].channelNum ))
        {
            if ((NV_CHANNEL_ENABLE == regChannels[loopCount].enabled) ||
                (dfsMasterCap &&
                 (NV_CHANNEL_DFS == regChannels[loopCount].enabled))) {
#ifdef FEATURE_WLAN_CH_AVOID
                for( i = 0; i < NUM_20MHZ_RF_CHANNELS; i++ )
                {
                    if( (safeChannels[i].channelNumber ==
                                rfChannels[loopCount].channelNum) )
                    {
                        /* Check if channel is safe */
                        if(VOS_TRUE == safeChannels[i].isSafe)
                        {
#endif
                            list[channelCount] =
                                     rfChannels[loopCount].channelNum;
                            channelCount++;
#ifdef FEATURE_WLAN_CH_AVOID
                        }
                        break;
                    }
                }
#endif
            }
        }
    }
    if (0 == channelCount)
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
        "sapGetChannelList:No active channels present in the given range for the current region");
        /*LTE COEX: channel range outside the restricted 2.4GHz band limits*/
        if (enableLTECoex && (startChannelNum > bandEndChannel))
        {
            VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_FATAL,
            "sapGetChannelList:SAP cannot be started as LTE COEX restricted 2.4GHz limits");
        }
    }

    /* return the channel list and number of channels to scan*/
    *numberOfChannels = channelCount;
    if(channelCount != 0)
    {
       *channelList = list;
    }
    else
    {
       *channelList = NULL;
        vos_mem_free(list);
    }

    for (loopCount = 0; loopCount <channelCount; loopCount ++ )
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_DEBUG,
             "%s: channel number: %d",
             __func__,list[loopCount]);
    }
    return VOS_STATUS_SUCCESS;
}
#endif

/*
 * Function for initializing list of  2.4/5 Ghz [NON-DFS/DFS]
 * available channels in the current regulatory domain.
 */
static VOS_STATUS sapGet5GHzChannelList(ptSapContext sapContext)
{
    v_U8_t count = 0;
    int i;
    if (NULL == sapContext)
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
              "Invalid sapContext pointer on sapGetChannelList");
        return VOS_STATUS_E_FAULT;
    }

    sapContext->SapAllChnlList.channelList =
                (v_U8_t *)vos_mem_malloc(WNI_CFG_VALID_CHANNEL_LIST_LEN);
    if (NULL == sapContext->SapAllChnlList.channelList)
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
                   " Memory Allocation failed sapGetChannelList");
                 return VOS_STATUS_E_FAULT;
    }

    for( i = RF_CHAN_36; i <= RF_CHAN_165; i++ )
    {
        if( regChannels[i].enabled == NV_CHANNEL_ENABLE ||
            regChannels[i].enabled == NV_CHANNEL_DFS )
        {
            sapContext->SapAllChnlList.channelList[count] =
                                          rfChannels[i].channelNum;
            VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_LOW,
                      "%s[%d] CHANNEL = %d",__func__, __LINE__,
                      sapContext->SapAllChnlList.channelList[count]);
            count++;
        }
    }

    sapContext->SapAllChnlList.numChannel = count;
    VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_LOW,
              "%s[%d] NUMBER OF CHANNELS count = %d"
              "sapContext->SapAllChnlList.numChannel = %d",
              __func__,__LINE__,count,sapContext->SapAllChnlList.numChannel);
    return VOS_STATUS_SUCCESS;
}

/*
 * This function randomly selects the channel to switch after the detection
 * of radar
 * param sapContext - sap context
 * dfs_event - Dfs information from DFS
 * return - channel to which AP wishes to switch
 */
v_U8_t sapIndicateRadar(ptSapContext sapContext,tSirSmeDfsEventInd *dfs_event)
{
    v_U8_t available_chan_idx[WNI_CFG_VALID_CHANNEL_LIST_LEN];
    int available_chan_count, numGChannels = 0, numAChannels = 0;
    v_U8_t total_num_channels = 0;
    v_U8_t target_channel = 0;
    int i;
    v_BOOL_t isChannelNol = VOS_FALSE;

    if (NULL == sapContext || NULL == dfs_event)
    {
        /* Invalid sap context of dfs event passed */
        return 0;
    }

    if (!dfs_event->dfs_radar_status)
    {
        /*dfs status does not indicate a radar on the channel-- False Alarm*/
        return 0;
    }

    if (sapContext->csrRoamProfile.disableDFSChSwitch)
    {
       return sapContext->channel;
    }

    /* set the Radar Found flag in SapDfsInfo */
    sapContext->SapDfsInfo.sap_radar_found_status = VOS_TRUE;

    /* We need to generate Channel Switch IE if the radar is found in the
     * operating state
     */
    if (eSAP_STARTED == sapContext->sapsMachine)
        sapContext->SapDfsInfo.csaIERequired = VOS_TRUE;

    sapGet5GHzChannelList(sapContext);
    total_num_channels = sapContext->SapAllChnlList.numChannel;

    /*
     * Find how many G channels are present in the channel list
     */
    for(i = 0; i < RF_CHAN_14; i++ )
    {
        if( regChannels[i].enabled )
        {
            numGChannels++;
        }
    }
    numAChannels = (total_num_channels - numGChannels);

    /*
     * Mark the current channel on which Radar is found
     * in the  NOL list as eSAP_DFS_CHANNEL_UNAVAILABLE.
     */

    for (i = 0; i<= sapContext->SapDfsInfo.numCurrentRegDomainDfsChannels; i++)
    {
        if (sapContext->SapDfsInfo.sapDfsChannelNolList[i]
                         .dfs_channel_number ==
                                               dfs_event->ieee_chan_number)
        {
            // Capture the Radar Found timestamp on the Current Channel in ms.
            sapContext->SapDfsInfo.sapDfsChannelNolList[i]
                         .radar_found_timestamp = vos_timer_get_system_time();
            // Mark the Channel to be UNAVAILABLE for next 30 mins.
            sapContext->SapDfsInfo.sapDfsChannelNolList[i]
                         .radar_status_flag = eSAP_DFS_CHANNEL_UNAVAILABLE;

            VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_HIGH,
                      "%s[%d]: Channel = %d Added to NOL LIST",
                       __func__, __LINE__, dfs_event->ieee_chan_number);
        }
    }

    /*
     * (1) skip static turbo channel as it will require STA to be in
     * static turbo to work.
     * (2) skip channel which's marked with radar detction
     * (3) WAR: we allow user to config not to use any DFS channel
     * (4) When we pick a channel, skip excluded 11D channels
     * (5) Create the available channel list with the above rules
     */

    for (i = 0, available_chan_count = 0; i< total_num_channels; i++)
    {
        if (sapContext->SapAllChnlList.channelList[i] ==
                                       dfs_event->ieee_chan_number)
        {
            continue;//skip the channel on which radar is found
        }

        /*
         * Now Check if the channel is DFS and if
         * the channel is not in NOL list and add
         * it available_chan_idx otherwise skip this
         * channel index.
         */

        if (vos_nv_getChannelEnabledState(sapContext->SapAllChnlList
                                           .channelList[i]) ==
                                                            NV_CHANNEL_DFS)
        {
            isChannelNol = sapDfsIsChannelInNolList(sapContext,
                                 sapContext->SapAllChnlList.channelList[i]);
            if (VOS_TRUE == isChannelNol)
            {
                /*
                 * Skip this channel since it is still in
                 * DFS Non-Occupancy-Period which is 30 mins.
                 */
                VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_LOW,
                          "%s[%d]: Channel = %d Present in NOL LIST",
                           __func__, __LINE__,
                           sapContext->SapAllChnlList.channelList[i]);

                continue;
            }
        }
        available_chan_idx[available_chan_count++] =
                           sapContext->SapAllChnlList.channelList[i];
    }

    if(available_chan_count)
    {
        v_U32_t random_byte = 0;

        /* logic to generate a random index */
        get_random_bytes(&random_byte,1);
        i = (random_byte + jiffies) % available_chan_count;

        /*
         * Pick the channel from the random index
         * in available_chan_idx list
         */
        target_channel = (available_chan_idx[i]);
        VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_LOW,
                  "%s[%d]: Target channel Index = %d target_channel = %d",
                  __func__,__LINE__, i, target_channel);
        return target_channel;
    }

    return 0;
}

/*
 * CAC timer callback function.
 * Post eSAP_DFS_CHANNEL_CAC_END event to sapFsm().
 */
void sapDfsCacTimerCallback(void *data)
{
    ptSapContext sapContext = (ptSapContext)data;
    tWLAN_SAPEvent sapEvent;

    /* Check to ensure that SAP is in DFS WAIT state*/
    if (sapContext->sapsMachine == eSAP_DFS_CAC_WAIT)
    {
        vos_timer_destroy(&sapContext->SapDfsInfo.sap_dfs_cac_timer);
        sapContext->SapDfsInfo.is_dfs_cac_timer_running = 0;
        /*
         * CAC Complete, post eSAP_DFS_CHANNEL_CAC_END to sapFsm
         */
        VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_DEBUG,
                  "%s[%d]: Sending eSAP_DFS_CHANNEL_CAC_END for target_channel = %d",
                  __func__,__LINE__, sapContext->channel);
        sapEvent.event = eSAP_DFS_CHANNEL_CAC_END;
        sapEvent.params = 0;
        sapEvent.u1 = 0;
        sapEvent.u2 = 0;
    }
    else if (sapContext->sapsMachine == eSAP_DFS_CAC_WAIT)
    {
        vos_timer_destroy(&sapContext->SapDfsInfo.sap_dfs_cac_timer);
        sapContext->SapDfsInfo.is_dfs_cac_timer_running = 0;

        /*
         * CAC Complete, post eSAP_DFS_CHANNEL_CAC_END to sapFsm
         */
        sapEvent.event = eSAP_DFS_CHANNEL_CAC_END;
        sapEvent.params = 0;
        sapEvent.u1 = 0;
        sapEvent.u2 = 0;
    }

    sapFsm(sapContext, &sapEvent);
}

/*
 * Function to start the DFS CAC Timer
 * when SAP is started on a DFS channel
 */
int sapStartDfsCacTimer(ptSapContext sapContext)
{
    VOS_STATUS status;
    v_U32_t cacTimeOut;
    v_REGDOMAIN_t regDomain;
    if (sapContext == NULL)
    {
        return 0;
    }
    if (sapContext->SapDfsInfo.ignore_cac)
    {
        /*
         * If User has set to ignore the CAC
         * so, continue without CAC Timer.
         */
        return 2;
    }
    cacTimeOut = DEFAULT_CAC_TIMEOUT;
    vos_nv_getRegDomainFromCountryCode(&regDomain,
                    sapContext->csrRoamProfile.countryCode, COUNTRY_QUERY);
    if ((regDomain == REGDOMAIN_ETSI) &&
       (IS_ETSI_WEATHER_CH(sapContext->channel)))
    {
        cacTimeOut = ETSI_WEATHER_CH_CAC_TIMEOUT;
    }
    VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_DEBUG,
                  "%s[%d]: SAP_DFS_CHANNEL_CAC_START on CH - %d, CAC TIMEOUT - %d sec",
                  __func__, __LINE__, sapContext->channel, cacTimeOut/1000);

    vos_timer_init(&sapContext->SapDfsInfo.sap_dfs_cac_timer,
                   VOS_TIMER_TYPE_SW,
                   sapDfsCacTimerCallback, (v_PVOID_t)sapContext);

    /*Start the CAC timer*/
    status = vos_timer_start(&sapContext->SapDfsInfo.sap_dfs_cac_timer, cacTimeOut);
    if (status == VOS_STATUS_SUCCESS)
    {
        sapContext->SapDfsInfo.is_dfs_cac_timer_running = VOS_TRUE;
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * This function initializes the NOL list
 * parameters required to track the radar
 * found DFS channels in the current Reg. Domain .
 */
VOS_STATUS sapInitDfsChannelNolList(ptSapContext sapContext)
{
    v_U8_t count = 0;
    int i;
    if (NULL == sapContext)
    {
        VOS_TRACE( VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_ERROR,
              "Invalid sapContext pointer on sapInitDfsChannelNolList");
        return VOS_STATUS_E_FAULT;
    }
    for ( i = RF_CHAN_36; i <= RF_CHAN_165; i++ )
    {
        if ( regChannels[i].enabled == NV_CHANNEL_DFS )
        {
            sapContext->SapDfsInfo.sapDfsChannelNolList[count]
               .dfs_channel_number = rfChannels[i].channelNum;

            VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_LOW,
                      "%s[%d] CHANNEL = %d", __func__, __LINE__,
                       sapContext->SapDfsInfo
                       .sapDfsChannelNolList[count].dfs_channel_number);

            sapContext->SapDfsInfo.sapDfsChannelNolList[count]
               .radar_status_flag = eSAP_DFS_CHANNEL_USABLE;
            sapContext->SapDfsInfo.sapDfsChannelNolList[count]
               .radar_found_timestamp = 0;
            count++;
        }
    }
    sapContext->SapDfsInfo.numCurrentRegDomainDfsChannels = count;

    VOS_TRACE(VOS_MODULE_ID_SAP, VOS_TRACE_LEVEL_INFO_LOW,
              "%s[%d] NUMBER OF DFS CHANNELS = %d",
              __func__, __LINE__,
              sapContext->SapDfsInfo.numCurrentRegDomainDfsChannels);

    return VOS_STATUS_SUCCESS;
}
