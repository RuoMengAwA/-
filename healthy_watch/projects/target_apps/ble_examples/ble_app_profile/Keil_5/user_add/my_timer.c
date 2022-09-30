#include "my_timer.h"
#include "pwm.h"
#include "user_custs1_def.h"
#include "string.h"
#include "prf_utils.h"
#include "attm_db.h"
#include "ke_task.h"
#include "custs1_task.h"
#include "gpio.h"
#include "user_periph_setup.h"       // peripheral configuration
#include "adc.h"
#include "battery.h"

#include "spi_flash.h"
#include "LIS2DS12_ACC_driver.h"
#include "app_easy_gap.h"
#include "my_adc.h"
#include "fk_oled.h"


extern struct custs1_env_tag custs1_env;
extern void oled_off(void);
extern void oled_on(void);
void my_timer_init(void)
{
  // Stop timer for enter settings
    timer0_stop();

    // register callback function for SWTIM_IRQn irq
    timer0_register_callback(timer0_general_user_callback_function);

    // Enable TIMER0 clock
    set_tmr_enable(CLK_PER_REG_TMR_ENABLED);

    // Sets TIMER0,TIMER2 clock division factor to 8, so TIM0 Fclk is F = 16MHz/8 = 2Mhz
    set_tmr_div(CLK_PER_REG_TMR_DIV_8);

    // clear PWM settings register to not generate PWM
    timer0_set_pwm_high_counter(0);
    timer0_set_pwm_low_counter(0);
    
    timer0_init(TIM0_CLK_32K, PWM_MODE_ONE, TIM0_CLK_DIV_BY_10);

    timer0_set_pwm_on_counter(32);          //10ms

    // Enable SWTIM_IRQn irq
    timer0_enable_irq();
    
    // Start Timer0
    timer0_start();
}

/**
 ****************************************************************************************
 * @brief  Timer0 general user callback function
 * 
 ****************************************************************************************
 */ 
extern unsigned char Touch_release; //�����ɿ���־λ
volatile unsigned char OLED_dis_flag;  //OLED��ʾ���ر�־λ
volatile unsigned char OLED_Page;			//OLED��Ļ��ҳ��
unsigned char Long_Time_Tch;  //������־λ
unsigned int Time_Counter = 0;    //����ʱ�����
unsigned char OLED_Test = 1;
unsigned char ADC_flag = 0;//����sdk-capture��ʶ���10ms-captureһ��
unsigned char	heart_beat_flag = 0;//���ʲɼ���ʶ,30ms����һ��
unsigned char blood_oxygen_flag=0;
unsigned char write_flash_flag = 0;//дflash��ʶ��30sдһ��
unsigned int Timer = 0;
unsigned int DisplayTimer = 0;
unsigned char hour,minute,seconds;
unsigned int heart_beat_flag_timer=0;
bool StepFlag=0;
bool FlashReadFlag=0;
bool close_flag=0;
extern void Delay_1ms(unsigned int Del_1ms);
unsigned char dghData[20];
void Delay_1ms(unsigned int Del_1ms)		//??1ms,??17us;??????16us
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

void timer_process()
{
	if(Touch_release==1)  //���������ɿ�
	{
		Touch_release = 0;
		if(Time_Counter>100)  //�������
		{
			Time_Counter = 0;	//����ʱ���������
			if(Long_Time_Tch ==0)
			{
				Long_Time_Tch =1;
			}
			else
			{
				Long_Time_Tch =0;
			}
			if(!close_flag&&OLED_Page==1)  //��һ����ʱ��ſ��Թػ�
			{
				close_flag=1;
				Write(Command,0x28);     //display off 
				GPIO_SetActive(backlight_port, backlight_pin);
				GPIO_SetActive(SHOCK_PORT, SHOCK_PIN);
				Delay_1ms(3000);
				GPIO_SetInactive(SHOCK_PORT, SHOCK_PIN);
				OLED_Page=7;//Ԥװ��1��
			}
			else if(close_flag==1)  //����Ѿ����˻�
			{
				close_flag=0;
				GPIO_SetInactive(backlight_port, backlight_pin);  //������
				OLED_Page=1;//Ԥװ
			  Write(Command,0x29);     //display ON
				GPIO_SetActive(SHOCK_PORT, SHOCK_PIN);
				Delay_1ms(3000);
				GPIO_SetInactive(SHOCK_PORT, SHOCK_PIN);
				OLED_dis_flag = 1;//OLED��ʾ���ش�
			}
		}
		else if(close_flag==0)
		{
			DisplayTimer=0;
			Time_Counter = 0;//����ʱ���������
			OLED_Page++;//OLED��ʾ��ҳ����1
			if(OLED_Page>6)
			{
				OLED_Page = 1;
			}
			if(!close_flag)
			{
				OLED_dis_flag = 1;//OLED��ʾ���ش�
			}
		}
	}



}



void timer0_general_user_callback_function(void) 
{
		DisplayTimer++;
		if(DisplayTimer==1500)OLED_Page=7;
	  if(DisplayTimer>1502)DisplayTimer=1501;
		Timer ++;
		if(Timer == 100)
		{
			Timer = 0;
			seconds++;
			if(seconds==0||seconds==20||seconds==40)StepFlag=1;
			if(seconds==25||seconds==55||close_flag==1)
			{
				GPIO_SetInactive(stm32_port,stm32_pin);
			}
			if(seconds==28||seconds==58||close_flag==0)
			{
//				GPIO_SetInactive(stm32_port,stm32_pin);
				GPIO_SetActive(stm32_port,stm32_pin);
			}
			if(seconds==30)
			{
				heart_beat_flag = 1;
				write_flash_flag = 1;
				adc_test();
			}else if(seconds == 60)
			{
				blood_oxygen_flag=1;
//				spi_flash_read_data(dghData, 0x03fe00,20);
				dghData[5]++;                                      //deal with time
				if(dghData[5]==0x0a)
				{
					dghData[5]=0;
					if(dghData[4]==0x05)
					{
						dghData[4]=0;
						dghData[3]++;
						if((dghData[2]*10+dghData[3])==24)
						{
							dghData[2]=0;
							dghData[3]=0;
							dghData[9]++;
							if((dghData[8]*10+dghData[9])==30)
							{
								dghData[8]=0;
								dghData[9]=0;
								dghData[7]++;
								if((dghData[6]*10+dghData[7])==12)
								{
									dghData[6]=0;
									dghData[7]=0;
								}
								else 
								{
									if(dghData[7]==0x0a)
									{
										dghData[7]=0;
										dghData[6]++;
									}
								}
							}
							else
							{
								if(dghData[9]==0x0a)
								{
									dghData[9]=0;
									dghData[8]++;
								}
							}
						}
						else
						{
							if(dghData[3]==0x0a)
							{
								dghData[3]=0;
								dghData[2]++;
							}
						}
					}
					else
					{
						dghData[4]++;
					}
				}
//				spi_flash_write_data(dghData, 0x03fe00,10);
				seconds = 0;
				minute ++;
				if(minute == 60)
				{
					hour ++;
					minute = 0;
				}
			}
		}
	if(GPIO_GetPinStatus(BUTTON_PORT,BUTTON_PIN1))//�ж��жϹܽŵ�״̬�����Ϊ����Time_Counter++;
	{
		Time_Counter++;   //��������֮��ʼ����
		if(Time_Counter>=1000)  //10s����10s��λ
		{
			NVIC_SystemReset();   //10���λ
		}
	}
	


}
