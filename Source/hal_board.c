
#include <iocc2530.h>
#include <stdio.h>


void hal_led_init(void)
{
    P1SEL &= ~0x0D;          //����P1.0 P1.2 P1.3Ϊ��ͨ I/O ��
    P1DIR |= 0x0D;           //����P1.0 P1.2 P1.3 Ϊ���

    P2SEL &= ~0x01;
    P2DIR |= 0x01;
    
    P1 |= 0x0D;
    P2_0 |= 0x01;
}

void hal_led_off(int leds)
{
  if (leds & 0x01) {
    P1_0 |= 0x01;
  }
  if (leds & 0x02) {
    P1_2 |= 0x04;
  }
  if (leds & 0x04) {
    P1_3 |= 0x08;
  }
  if (leds & 0x08) {
    P2_0 |= 0x01;
  }
}
void hal_led_on(int leds)
{
    if (leds & 0x01) {
    P1_0 &= ~0x01;
  }
  if (leds & 0x02) {
    P1_2 &= ~0x04;
  }
  if (leds & 0x04) {
    P1_3 &= ~0x08;
  }
  if (leds & 0x08) {
    P2_0 &= ~0x01;
  }
}



/****************************************************************
*�������� ����ʼ������1										
*��ڲ��� ����												
*�� �� ֵ ����							
*˵    �� ��19200-8-n-1						
****************************************************************/
void hal_uart_init(void)
{
  P1SEL |=  0xC0;                                               //��ʼ��UART1�˿�
  PERCFG |= 0x02;                                               //ѡ��UART1Ϊ��ѡλ��һ
  P2SEL &= ~0x20;                                               //P1������Ϊ����1
  U1CSR = 0xC0;                                                 //����ΪUARTģʽ,����ʹ�ܽ�����
   
  U1GCR = 10;                  
  U1BAUD = 59;                                                  //����������Ϊ38400
  
  U1UCR |= 0|0;                                     //����ֹͣλ����żУ��
}

__near_func int putchar(int ch)
{
    U1DBUF = ch;
    while(UTX1IF == 0);
    UTX1IF = 0;
    return ch;
}




