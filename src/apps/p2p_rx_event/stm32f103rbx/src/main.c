#include "intoyun_interface.h"
#include "stm32f1xx_hal.h"

#define LED_PIN             GPIO_PIN_0
#define LED_GPIO_PORT       GPIOB
#define LED_ON	 	          HAL_GPIO_WritePin(LED_GPIO_PORT,LED_PIN, GPIO_PIN_RESET)
#define LED_OFF		          HAL_GPIO_WritePin(LED_GPIO_PORT,LED_PIN, GPIO_PIN_SET)
#define LED_TOG		          HAL_GPIO_TogglePin(LED_GPIO_PORT,LED_PIN)

#define WAKEUP_PIN          GPIO_PIN_9
#define WAKEUP_GPIO_PORT    GPIOB

#define RADIO_BUFFER_SIZE  10

uint8_t radioBuffer[RADIO_BUFFER_SIZE] = {1,2,3,4};
int rssiVal = 0;

void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = LED_PIN | WAKEUP_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(WAKEUP_GPIO_PORT, WAKEUP_PIN, GPIO_PIN_SET);
}

//不运行lorawan协议
void system_event_callback( system_event_t event, int param, uint8_t *data, uint16_t datalen)
{
    if(event == event_lora_radio_status){
        switch(param)
        {
        case ep_lora_radio_tx_done:
            log_v("radio tx done\r\n");
            break;

        case ep_lora_radio_tx_fail:
            log_v("radio tx fail\r\n");
            break;

        case ep_lora_radio_rx_timeout:
            log_v("radio rx timeout\r\n");
            break;

        case ep_lora_radio_rx_error:
            log_v("radio rx error\r\n");
            break;

        case ep_lora_radio_module_wakeup:
            log_v("radio system wakeup\r\n");
            break;

        case ep_lora_radio_rx_done:
            log_v("radio receive data len=%d\r\n",datalen);
            log_v("radio receive data : \r\n");
            for(uint8_t i=0;i<datalen;i++)
            {
                log_v("%d \r\n",data[i]);
            }
            log_v("\r\n");
            break;

        default:
            break;
        }
    }
}

void userInit(void)
{
    GPIO_Init();
    log_v("lorawan slave mode\r\n");
    delay(100);
    System.setEventCallback(system_event_callback);
    delay(10);
    //不运行lorawan协议
    if(!System.setProtocol(PROTOCOL_P2P))
    {
        log_v("exe mac pause fail\r\n");
    }
    delay(50);
    if(!LoRa.radioSetFreq(433175000))
    {
        log_v("set radio freq fail\r\n");
    }
    delay(100);
    if(!LoRa.radioStartRx(0)) //设为连续接收模式
    {
        log_v("set radio rx mode fail\r\n");
    }
}

void userHandle(void)
{
}

int main(void)
{
    System.init();
    userInit();
    while(1)
    {
        System.loop();
        userHandle();
    }
}
