#include<STC15F2K60S2.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int

#define cstFocs      11059200L
#define cstBaud1     9600

sbit sbtBeep = P3 ^ 4;

bit btBeepFlag;

static char	buffer[32];
char ptr	= 0;

unsigned pitch;
unsigned tick;
unsigned char bias;
unsigned char tickcnt = 0;
void T0_Process() interrupt 1
{
		if(btBeepFlag){
			tick = pitch;
		}			
		P00 = 0x0;
		if(tickcnt % (256 / bias) == 0){
			tick += 1;
			P0 = 0x1;
		}
		
		
    TH0 = ( 65535 - tick ) / 256;
    TL0 = ( 65535 - tick ) % 256;
		
		if( btBeepFlag)
    {
        sbtBeep = ~sbtBeep;
    }
    else
        sbtBeep = 0;      
		
		tickcnt++;
}



void Uart1_Init( void )
{
    AUXR = 0X80; //�����Ĵ��� ��ʱ��ʱ��0���ٶ��Ǵ�ͳ��12��������Ƶ
    SCON |= 0X50; //�������
    TL1 = ( 65536 - ( cstFocs / 4 / cstBaud1 ) );
    TH1 = ( 65536 - ( cstFocs / 4 / cstBaud1 ) ) >> 8;
    AUXR |= 0X40; //�����Ĵ��� ��ʱ��ʱ��1���ٶ��Ǵ�ͳ��12��������Ƶ
    RI = 0;      //�����жϱ�־λ
    TI = 0;      //�����жϱ�־λ
    TR1 = 1;     //������ʱ��1
    ES = 1;      //�����ж�����λ
    EA = 1;      //���ж�����λ
    PS = 1 ;    //����1�жϸ����ȼ�
}



void Uart1_Process() interrupt 4 using 1
{
    if(RI)
    {
        RI = 0;
				if(SBUF == 0xFF)ptr = -2;
				else if(SBUF == 0xAA && ptr == -2)ptr = -1;
				else{
					buffer[++ptr] = SBUF;
					if(ptr == 4){
						pitch = ((unsigned)buffer[0]<<8) + ((unsigned)buffer[1] & 0xff);
						btBeepFlag	= buffer[2]& 0x1 ;
						bias = buffer[3];
						ptr = -2;
						
						tickcnt = 0;
					}
				}
				//P00 = bias >= 31;
				//P01 = bias >= 63;
				//P02 = bias >= 95;
				//P03 = bias >= 127;
				//P04 = bias >= 159;
				//P05 = bias >= 191;
				//P06 = bias >= 223;
				//P07 = bias >= 255;
    }

}

void Init()
{
		btBeepFlag = 0;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P2M0 = 0xff;
    P2M1 = 0x00;
    P0M0 = 0xff;
    P0M1 = 0x00;

    TMOD = 0x01; //��ʱ��0����ʽ1
    ET0 = 1; //������ʱ���ж�
    TH0 = ( 65535 - 1000 ) / 256;
    TL0 = ( 65535 - 1000 ) % 256;
    TR0 = 1; //������ʱ��
		P0 = 0;
    Uart1_Init();
}

void main()
{
    Init();
    while( 1 )
    {
        
    }
}
