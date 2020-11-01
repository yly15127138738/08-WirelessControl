#include <iocc2530.h>
#include "hal_mcu.h"
#include "hal_assert.h"
#include "hal_board.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include <stdio.h>
#include "info.h"
#include "uart1.h"
#include "clock.h"

#define RF_CHANNEL            20                                // 2.4 GHz RF channel
#define PAN_ID                0x2000
#define SEND_ADDR             0x2530
#define RECV_ADDR             0x2522
#define COMMAND               0x10

#define NODE_TYPE            0                                 //0:���սڵ㣬��0�����ͽڵ�
char temp;

void io_init(void)
{
  P2SEL &= ~0x01;
  P2DIR &= ~0x01;
}

static basicRfCfg_t basicRfConfig;
int ledstatus = 0;
void rfSendData(void)
{
    uint8 pTxData[] = 'l';
    char tx[]="�����뿪������  ����oΪ��  ����cΪ��";
    int i;
     static unsigned char counter = 0;                         //��¼K4�����µĴ���
    // Keep Receiver off when not needed to save power
    basicRfReceiveOff();


    // Main loop
    while (TRUE) {     
     hal_led_on(1);
     UartSendString(tx,sizeof tx);
     halMcuWaitMs(1500);
     pTxData[0]= temp;
     basicRfSendPacket(0xffff, (uint8*)pTxData, sizeof pTxData);//�㲥�������ݰ�   
     hal_led_off(1);
     halMcuWaitMs(50);
    }
}

void rfRecvData(void)
{
  uint8 pRxData[128];
  int rlen;
   basicRfReceiveOn();
   printf("{data=LED is off}");                                 //��ʾLED��ʼ״̬              
    // Main loop
    while (TRUE) {
        while(!basicRfPacketIsReady());
        rlen = basicRfReceive(pRxData, sizeof pRxData, NULL);
        if(rlen > 0 && pRxData[0] == 'o') {

            hal_led_on(1);   
            printf("{   LED is on  }");
          } 
        if(rlen > 0 && pRxData[0] == 'c') {
             hal_led_off(1);
             printf("{  LED is off}");
          }
        }
    
}

void main(void)
{
    halMcuInit();

    io_init();
    uart1_init();
    hal_led_init();
    
    hal_uart_init();
    
    //lcd_dis();
    
    if (FAILED == halRfInit()) {
        HAL_ASSERT(FALSE);
    }

    // Config basicRF
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
#ifdef SECURITY_CCM
    basicRfConfig.securityKey = key;
#endif

    
    // Initialize BasicRF
#if NODE_TYPE
    basicRfConfig.myAddr = SEND_ADDR;
#else
    basicRfConfig.myAddr = RECV_ADDR; 
#endif
    
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
#if NODE_TYPE
  rfSendData();
#else
  rfRecvData();   
#endif
}


#pragma vector = URX0_VECTOR
__interrupt void UART0_ISR(void)
{
  URX0IF = 0;       //���жϱ�־
  temp = U0DBUF;    //�����ڽ��ջ�������ݴ�ŵ�temp��,temp��char���ͣ���8λ����λ
}
