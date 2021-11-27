#include "time.h"



void Clock_Config(void){

     ErrorStatus        State;
     uint32_t           PLL_M;      
     uint32_t           PLL_N;
     uint32_t           PLL_P;
     uint32_t           PLL_Q;

 

    /*����ǰ������RCC����Ϊ��ʼֵ*/
     RCC_DeInit();
     /*����ѡ�� �ⲿ����HSE����Ϊ ʱ��Դ��������ȴ��ⲿ����*/
     RCC_HSEConfig(RCC_HSE_ON);
     /*�ȴ��ⲿ��������ȶ�״̬*/
     while( RCC_WaitForHSEStartUp() != SUCCESS );
     /*
     **����Ҫѡ��PLLʱ����Ϊϵͳʱ�ӣ����������Ҫ��PLLʱ�ӽ�������
     */
     /*ѡ���ⲿ������ΪPLL��ʱ��Դ*/
     /* ����һ��Ϊֹ������ HSE_VALUE = 8 MHz.
        PLL_VCO input clock = (HSE_VALUE or HSI_VALUE / PLL_M)��
        �����ĵ������ֵ�������� 1~2MHz����������� PLL_M = 8��
        �� PLL_VCO input clock = 1MHz */
     PLL_M=8;  
     /* ����һ��Ϊֹ������ PLL_VCO input clock = 1 MHz.
        PLL_VCO output clock = (PLL_VCO input clock) * PLL_N,
        ���ֵҪ��������ϵͳʱ�ӣ����� �� PLL_N = 336,
        �� PLL_VCO output clock = 336 MHz.*/       
     PLL_N=336;
     /* ����һ��Ϊֹ������ PLL_VCO output clock = 336 MHz.
        System Clock = (PLL_VCO output clock)/PLL_P ,
        ��Ϊ����Ҫ SystemClock = 168 Mhz������� PLL_P = 2.
     */
     PLL_P=2;
     /*���ϵ����������SD����д��USB�ȹ��ܣ���ʱ���ã������ĵ�����ʱ����Ϊ7*/
     PLL_Q         =    7;
     /* ����PLL������ʹ�ܣ���� 168Mhz �� System Clock ʱ��*/
     RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q);
     RCC_PLLCmd(ENABLE);
     /*������һ���������Ѿ����ú���PLLʱ�ӡ�������������Syetem Clock*/
     /*ѡ��PLLʱ����Ϊϵͳʱ��Դ*/
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);


	 /*������һ���������Ѿ����ú���ϵͳʱ�ӣ�Ƶ��Ϊ 168MHz. �������ǿ��Զ� AHB��APB������ȵ� ʱ�ӽ�������*/
     /*ʱ�ӵĽṹ��ο��û��ֲ�*/
     /*�������� AHBʱ�ӣ�HCLK��. Ϊ�˻�ýϸߵ�Ƶ�ʣ����Ƕ� SYSCLK 1��Ƶ���õ�HCLK*/
     RCC_HCLKConfig(RCC_HCLK_Div1);
     /*APBxʱ�ӣ�PCLK����AHBʱ�ӣ�HCLK����Ƶ�õ��������������� PCLK*/
     /*APB1ʱ������. 4��Ƶ���� PCLK1 = 42 MHz*/
     RCC_PCLK1Config(RCC_HCLK_Div4);
     /*APB2ʱ������. 2��Ƶ���� PCLK2 = 84 MHz*/
     RCC_PCLK2Config(RCC_HCLK_Div2);
	/*****��������******/

 

/*���Ϻ������Դ�����˵����Щ�⺯��������*/

}

 

