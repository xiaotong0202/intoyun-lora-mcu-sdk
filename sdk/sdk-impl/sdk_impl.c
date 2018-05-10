/*
 * Copyright (c) 2013-2018 Molmc Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <limits.h>
#include "iot_import.h"
#include "iot_export.h"
#include "iotx_comm_if_api.h"

uint32_t millis(void)
{
    return HAL_UptimeMs();
}

void delay(uint32_t ms)
{
    uint32_t start_millis = HAL_UptimeMs();
    uint32_t current_millis = 0, elapsed_millis = 0;

    if (ms == 0) return;
    while (1) {
        current_millis = HAL_UptimeMs();
        if (current_millis < start_millis) {
            elapsed_millis =  UINT_MAX - start_millis + current_millis;
        } else {
            elapsed_millis = current_millis - start_millis;
        }

        if (elapsed_millis > ms) {
            break;
        }
        IOT_SYSTEM_Loop();
    }
}

uint32_t timerGetId(void)
{
    return HAL_UptimeMs();
}

bool timerIsEnd(uint32_t timerID, uint32_t time)
{
    uint32_t current_millis = HAL_UptimeMs();
    uint32_t elapsed_millis = 0;

    //Check for wrapping
    if (current_millis < timerID) {
        elapsed_millis =  UINT_MAX-timerID + current_millis;
    } else {
        elapsed_millis = current_millis - timerID;
    }

    if (elapsed_millis >= time) {
        return true;
    }
    return false;
}

const iot_system_if_t System =
{
    IOT_SYSTEM_Init,
    IOT_SYSTEM_Loop,
    IOT_SYSTEM_SetEventCallback,
    IOT_SYSTEM_SetDeviceInfo,
    IOT_SYSTEM_GetModuleInfo,
    IOT_SYSTEM_Reboot,
    IOT_SYSTEM_Restore,
    IOT_SYSTEM_Sleep,
    IOT_SYSTEM_EnterDFU,
    IOT_SYSTEM_PutPipe,
};

const iot_cloud_if_t Cloud =
{
    IOT_Comm_Connect,
    IOT_Comm_IsConnected,
    IOT_Comm_Disconnect,
    IOT_Comm_SendData,
#if CONFIG_CLOUD_DATAPOINT_ENABLED == 1
    IOT_DataPoint_DefineBool,
    IOT_DataPoint_DefineNumber,
    IOT_DataPoint_DefineEnum,
    IOT_DataPoint_DefineString,
    IOT_DataPoint_DefineBinary,

    IOT_DataPoint_ReadBool,
    IOT_DataPoint_ReadNumberInt32,
    IOT_DataPoint_ReadNumberDouble,
    IOT_DataPoint_ReadEnum,
    IOT_DataPoint_ReadString,
    IOT_DataPoint_ReadBinary,

    IOT_DataPoint_WriteBool,
    IOT_DataPoint_WriteNumberInt32,
    IOT_DataPoint_WriteNumberDouble,
    IOT_DataPoint_WriteEnum,
    IOT_DataPoint_WriteString,
    IOT_DataPoint_WriteBinary,

    IOT_DataPoint_SendBool,
    IOT_DataPoint_SendNumberInt32,
    IOT_DataPoint_SendNumberDouble,
    IOT_DataPoint_SendEnum,
    IOT_DataPoint_SendString,
    IOT_DataPoint_SendBinary,
    IOT_DataPoint_SendAllDatapointManual,
#endif
};

const iot_lorawan_if_t LoRaWan =
{
    //LoRaWan通讯接口
    IOT_LoRaWAN_SendConfirmed,
    IOT_LoRaWAN_SendUnconfirmed,
    IOT_LoRaWAN_GetMacSendStatus,
    IOT_LoRaWAN_MacReceive,
    IOT_LoRaWAN_SetMacClassType,
    IOT_LoRaWAN_GetMacClassType,
    //LoRaWan参数设置接口
    IOT_LoRaWAN_GetMacDeviceAddr,
    IOT_LoRaWAN_GetMacDeviceEui,
    IOT_LoRaWAN_GetMacAppEui,
    IOT_LoRaWAN_SetMacOTAAParams,
    IOT_LoRaWAN_SetMacABPParams,
    IOT_LoRaWAN_SetMacPowerIndex,
    IOT_LoRaWAN_SetMacDatarate,
    IOT_LoRaWAN_GetMacDatarate,
    IOT_LoRaWAN_SetMacAdr,
    IOT_LoRaWAN_GetMacAdr,
    IOT_LoRaWAN_SetMacDutyCyclePrescaler,
    IOT_LoRaWAN_GetMacDutyCyclePrescaler,
    IOT_LoRaWAN_SetMacChannelFreq,
    IOT_LoRaWAN_GetMacChannelFreq,
    IOT_LoRaWAN_SetMacChannelDRRange,
    IOT_LoRaWAN_GetMacChannelDRRange,
    IOT_LoRaWAN_SetMacChannelEnable,
    IOT_LoRaWAN_GetMacChannelEnable,
    IOT_LoRaWAN_SetMacConfirmedTrials,
    IOT_LoRaWAN_GetMacConfirmedTrials,
    IOT_LoRaWAN_SetMacUnconfirmedTrials,
    IOT_LoRaWAN_GetMacUncomfirmedTrials,
    IOT_LoRaWAN_SetMacJoinTrials,
    IOT_LoRaWAN_GetMacJoinTrials,
    IOT_LoRaWAN_SetMacUplinkCount,
    IOT_LoRaWAN_GetMacUplinkCount,
    IOT_LoRaWAN_SetMacDownlinkCount,
    IOT_LoRaWAN_GetMacDownlinkCount,
    IOT_LoRaWAN_SetMacRX2Params,
    IOT_LoRaWAN_GetMacRX2Params,
    IOT_LoRaWAN_SetMacRX1Delay,
    IOT_LoRaWAN_GetMacRX1Delay,
    IOT_LoRaWAN_GetMacMargin,
    IOT_LoRaWAN_GetMacGatewayNumber,
    IOT_LoRaWAN_GetMacSnr,
    IOT_LoRaWAN_GetMacRssi,
};

const iot_lora_if_t LoRa =
{
    //P2P通讯接口
    IOT_LoRa_RadioSendData,
    IOT_LoRa_GetRadioSendStatus,
    IOT_LoRa_SetRadioRx,
    IOT_LoRa_RadioRx,
    //P2P参数设置接口
    IOT_LoRa_RadioStartCad,
    IOT_LoRa_GetRadioRssi,
    IOT_LoRa_GetRadioSnr,
    IOT_LoRa_RadioSleep,
    IOT_LoRa_SetRadioFreq,
    IOT_LoRa_GetRadioFreq,
    IOT_LoRa_SetRadioMaxPayloadLen,
    IOT_LoRa_GetRadioMaxPayloadLen,
    IOT_LoRa_SetRadioMode,
    IOT_LoRa_GetRadioMode,
    IOT_LoRa_SetRadioSf,
    IOT_LoRa_GetRadioSf,
    IOT_LoRa_SetRadioBw,
    IOT_LoRa_GetRadioBw,
    IOT_LoRa_SetRadioCoderate,
    IOT_LoRa_GetRadioCoderate,
    IOT_LoRa_SetRadioPreambleLen,
    IOT_LoRa_GetRadioPreambleLen,
    IOT_LoRa_SetRadioFixLenOn,
    IOT_LoRa_GetRadioFixLenOn,
    IOT_LoRa_SetRadioCrcEnabled,
    IOT_LoRa_GetRadioCrcEnabled,
    IOT_LoRa_SetRadioFreqHopOn,
    IOT_LoRa_GetRadioFreqHopOn,
    IOT_LoRa_SetRadioHopPeriod,
    IOT_LoRa_GetRadioHopPeriod,
    IOT_LoRa_SetRadioIqInverted,
    IOT_LoRa_GetRadioIqInverted,
    IOT_LoRa_SetRadioRxContinuous,
    IOT_LoRa_GetRadioRxContinuous,
    IOT_LoRa_SetRadioTxPower,
    IOT_LoRa_GetRadioTxPower,
    IOT_LoRa_SetRadioFixPayloadLen,
    IOT_LoRa_GetRadioFixPayloadLen,
    IOT_LoRa_SetRadioSymbTimeout,
    IOT_LoRa_GetRadioSymbTimeout,
    IOT_LoRa_GetRadioReadRegister,
    IOT_LoRa_SetRadioWriteRegister,
};

const iot_log_if_t Log =
{
    molmc_log_level_set,
    molmc_log_set_output
};

#if CONFIG_SYSTEM_KEY_ENABLE == 1
const iot_keys_if_t Key =
{
    IOT_KEY_Init,
    IOT_KEY_SetParams,
    IOT_KEY_Register,
    IOT_KEY_ClickCb,
    IOT_KEY_DoubleClickCb,
    IOT_KEY_PressStartCb,
    IOT_KEY_PressStopCb,
    IOT_KEY_PressDuringCb,
    IOT_KEY_Loop,
};
#endif

#if CONFIG_SYSTEM_TIMER_ENABLE == 1
const iot_timers_if_t Timer =
{
    IOT_TIMER_Register,
    IOT_TIMER_ChangePeriod,
    IOT_TIMER_Start,
    IOT_TIMER_Stop,
    IOT_TIMER_Reset,
    IOT_TIMER_Loop,
};
#endif

