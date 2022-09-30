#include "my_heart.h"
#include "gpio.h"
#include "spi.h"
#include "user_periph_setup.h"


/*******************************************************************************
*	        Global Variables												   *
*******************************************************************************/
#define DELAY_COUNT 2

unsigned long AFE44xx_Current_Register_Settings[36] = {
  CONTROL0_VAL,           //Reg0:CONTROL0: CONTROL REGISTER 0
  LED2STC_VAL,            //Reg1:REDSTARTCOUNT: SAMPLE RED START COUNT
  LED2ENDC_VAL,           //Reg2:REDENDCOUNT: SAMPLE RED END COUNT
  LED2LEDSTC_VAL,         //Reg3:REDLEDSTARTCOUNT: RED LED START COUNT
  LED2LEDENDC_VAL,        //Reg4:REDLEDENDCOUNT: RED LED END COUNT
  ALED2STC_VAL,           //Reg5:AMBREDSTARTCOUNT: SAMPLE AMBIENT RED START COUNT
  ALED2ENDC_VAL,          //Reg6:AMBREDENDCOUNT: SAMPLE AMBIENT RED END COUNT
  LED1STC_VAL,            //Reg7:IRSTARTCOUNT: SAMPLE IR START COUNT
  LED1ENDC_VAL,           //Reg8:IRENDCOUNT: SAMPLE IR END COUNT
  LED1LEDSTC_VAL,         //Reg9:IRLEDSTARTCOUNT: IR LED START COUNT
  LED1LEDENDC_VAL,        //Reg10:IRLEDENDCOUNT: IR LED END COUNT
  ALED1STC_VAL,           //Reg11:AMBIRSTARTCOUNT: SAMPLE AMBIENT IR START COUNT
  ALED1ENDC_VAL,          //Reg12:AMBIRENDCOUNT: SAMPLE AMBIENT IR END COUNT
  LED2CONVST_VAL,         //Reg13:REDCONVSTART: REDCONVST
  LED2CONVEND_VAL,        //Reg14:REDCONVEND: RED CONVERT END COUNT
  ALED2CONVST_VAL,        //Reg15:AMBREDCONVSTART: RED AMBIENT CONVERT START COUNT
  ALED2CONVEND_VAL,       //Reg16:AMBREDCONVEND: RED AMBIENT CONVERT END COUNT
  LED1CONVST_VAL,         //Reg17:IRCONVSTART: IR CONVERT START COUNT
  LED1CONVEND_VAL,        //Reg18:IRCONVEND: IR CONVERT END COUNT
  ALED1CONVST_VAL,        //Reg19:AMBIRCONVSTART: IR AMBIENT CONVERT START COUNT
  ALED1CONVEND_VAL,       //Reg20:AMBIRCONVEND: IR AMBIENT CONVERT END COUNT
  ADCRSTSTCT0_VAL,        //Reg21:ADCRESETSTCOUNT0: ADC RESET 0 START COUNT
  ADCRSTENDCT0_VAL,       //Reg22:ADCRESETENDCOUNT0: ADC RESET 0 END COUNT
  ADCRSTSTCT1_VAL,        //Reg23:ADCRESETSTCOUNT1: ADC RESET 1 START COUNT
  ADCRSTENDCT1_VAL,       //Reg24:ADCRESETENDCOUNT1: ADC RESET 1 END COUNT
  ADCRSTSTCT2_VAL,        //Reg25:ADCRESETENDCOUNT2: ADC RESET 2 START COUNT
  ADCRSTENDCT2_VAL,       //Reg26:ADCRESETENDCOUNT2: ADC RESET 2 END COUNT
  ADCRSTSTCT3_VAL,        //Reg27:ADCRESETENDCOUNT3: ADC RESET 3 START COUNT
  ADCRSTENDCT3_VAL,       //Reg28:ADCRESETENDCOUNT3: ADC RESET 3 END COUNT
  PRP,                    //Reg29:PRPCOUNT: PULSE REPETITION PERIOD COUNT
  CONTROL1_VAL,           //Reg30:CONTROL1: CONTROL REGISTER 1 //timer enabled, averages=3, RED and IR LED pulse ON PD_ALM AND LED_ALM pins
  0x00000,                //Reg31:?: ??          
  (ENSEPGAIN + STAGE2EN_LED1 + STG2GAIN_LED1_0DB + CF_LED1_5P + RF_LED1_1M),                      //Reg32:TIAGAIN: TRANS IMPEDANCE AMPLIFIER GAIN SETTING REGISTER
  (AMBDAC_0uA + FLTRCNRSEL_500HZ + STAGE2EN_LED2 + STG2GAIN_LED2_0DB + CF_LED2_5P + RF_LED2_1M),  //Reg33:TIA_AMB_GAIN: TRANS IMPEDANCE AAMPLIFIER AND AMBIENT CANELLATION STAGE GAIN
  (LEDCNTRL_VAL),                                                                                 //Reg34:LEDCNTRL: LED CONTROL REGISTER
#ifndef __AFE4403__
  (TX_REF_1 + RST_CLK_ON_PD_ALM_PIN_DISABLE + ADC_BYP_DISABLE + TXBRGMOD_H_BRIDGE + DIGOUT_TRISTATE_DISABLE + XTAL_ENABLE + EN_FAST_DIAG + PDN_TX_OFF + PDN_RX_OFF + PDN_AFE_OFF)                 //Reg35:CONTROL2: CONTROL REGISTER 2 //bit 9
#else
    (TX_REF_1 + TXBRGMOD_H_BRIDGE + DIGOUT_TRISTATE_DISABLE + XTAL_ENABLE + EN_FAST_DIAG + PDN_TX_OFF + PDN_RX_OFF + PDN_AFE_OFF)                 //Reg35:CONTROL2: CONTROL REGISTER 2 //bit 9  
#endif    
};

void Enable_AFE44xx_SPI_Read (void)
{
	GPIO_SetInactive(heart_open_port, heart_open_pin);	// pull CS low
	spi_set_bitmode(SPI_MODE_8BIT);
//	spi_access(0x00000001);
	spi_access(0);
	spi_access(0);
	spi_access(0);
	spi_access(1);
	GPIO_SetActive(heart_open_port, heart_open_pin);	// pull CS high

}

void Disable_AFE44xx_SPI_Read (void)
{
	GPIO_SetInactive(heart_open_port, heart_open_pin);	// pull CS low
	spi_set_bitmode(SPI_MODE_8BIT);
	spi_access(0);
	spi_access(0);
	spi_access(0);
	spi_access(0);
	GPIO_SetActive(heart_open_port, heart_open_pin);	// pull CS high
}

void AFE44xx_Reg_Write (unsigned char reg_address, unsigned long data)
{
	GPIO_SetInactive(heart_open_port, heart_open_pin);	// pull CS low
	spi_set_bitmode(SPI_MODE_8BIT);
	spi_access(reg_address);
	spi_access(data >>16);
	spi_access(((data & 0x00FFFF) >>8));
	spi_access(data & 0x0000FF);
	GPIO_SetActive(heart_open_port, heart_open_pin);	// pull CS high
}

unsigned long AFE44xx_Reg_Read(unsigned char reg_address)
{
	unsigned long data_read=0;
	GPIO_SetInactive(heart_open_port, heart_open_pin);	// pull CS low
	spi_set_bitmode(SPI_MODE_8BIT);
	spi_access(reg_address);
	data_read+=spi_access(0);
	data_read+=spi_access(0);
	data_read+=spi_access(0);
	GPIO_SetActive(heart_open_port, heart_open_pin);	// pull CS high
	return data_read;
}

void AFE44xx_Default_Reg_Init(void)
{
  Disable_AFE44xx_SPI_Read();
  
  AFE44xx_Reg_Write((unsigned char)PRPCOUNT, (unsigned long)PRP);
  AFE44xx_Reg_Write((unsigned char)LED2STC, (unsigned long)LED2STC_VAL);
  AFE44xx_Reg_Write((unsigned char)LED2ENDC, (unsigned long)LED2ENDC_VAL);
  AFE44xx_Reg_Write((unsigned char)LED2LEDSTC, (unsigned long)LED2LEDSTC_VAL);
  AFE44xx_Reg_Write((unsigned char)LED2LEDENDC, (unsigned long)LED2LEDENDC_VAL);
  AFE44xx_Reg_Write((unsigned char)ALED2STC, (unsigned long)ALED2STC_VAL);
  AFE44xx_Reg_Write((unsigned char)ALED2ENDC, (unsigned long)ALED2ENDC_VAL);
  AFE44xx_Reg_Write((unsigned char)LED1STC, (unsigned long)LED1STC_VAL);
  AFE44xx_Reg_Write((unsigned char)LED1ENDC, (unsigned long)LED1ENDC_VAL);
  AFE44xx_Reg_Write((unsigned char)LED1LEDSTC, (unsigned long)LED1LEDSTC_VAL);
  AFE44xx_Reg_Write((unsigned char)LED1LEDENDC, (unsigned long)LED1LEDENDC_VAL);
  AFE44xx_Reg_Write((unsigned char)ALED1STC, (unsigned long)ALED1STC_VAL);
  AFE44xx_Reg_Write((unsigned char)ALED1ENDC, (unsigned long)ALED1ENDC_VAL);
  AFE44xx_Reg_Write((unsigned char)LED2CONVST, (unsigned long)LED2CONVST_VAL);
  AFE44xx_Reg_Write((unsigned char)LED2CONVEND, (unsigned long)LED2CONVEND_VAL);
  AFE44xx_Reg_Write((unsigned char)ALED2CONVST, (unsigned long)ALED2CONVST_VAL);
  AFE44xx_Reg_Write((unsigned char)ALED2CONVEND, (unsigned long)ALED2CONVEND_VAL);
  AFE44xx_Reg_Write((unsigned char)LED1CONVST, (unsigned long)LED1CONVST_VAL);
  AFE44xx_Reg_Write((unsigned char)LED1CONVEND, (unsigned long)LED1CONVEND_VAL);
  AFE44xx_Reg_Write((unsigned char)ALED1CONVST, (unsigned long)ALED1CONVST_VAL);
  AFE44xx_Reg_Write((unsigned char)ALED1CONVEND, (unsigned long)ALED1CONVEND_VAL);
  AFE44xx_Reg_Write((unsigned char)ADCRSTSTCT0, (unsigned long)ADCRSTSTCT0_VAL);
  AFE44xx_Reg_Write((unsigned char)ADCRSTENDCT0, (unsigned long)ADCRSTENDCT0_VAL);
  AFE44xx_Reg_Write((unsigned char)ADCRSTSTCT1, (unsigned long)ADCRSTSTCT1_VAL);
  AFE44xx_Reg_Write((unsigned char)ADCRSTENDCT1, (unsigned long)ADCRSTENDCT1_VAL);
  AFE44xx_Reg_Write((unsigned char)ADCRSTSTCT2, (unsigned long)ADCRSTSTCT2_VAL);
  AFE44xx_Reg_Write((unsigned char)ADCRSTENDCT2, (unsigned long)ADCRSTENDCT2_VAL);
  AFE44xx_Reg_Write((unsigned char)ADCRSTSTCT3, (unsigned long)ADCRSTSTCT3_VAL);
  AFE44xx_Reg_Write((unsigned char)ADCRSTENDCT3, (unsigned long)ADCRSTENDCT3_VAL);
  
  AFE44xx_Reg_Write((unsigned char)CONTROL0, AFE44xx_Current_Register_Settings[0]);            //0x00
  AFE44xx_Reg_Write((unsigned char)CONTROL2, AFE44xx_Current_Register_Settings[35]);           //0x23
  AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);            //0x20
  AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);       //0x21
  AFE44xx_Reg_Write((unsigned char)LEDCNTRL, AFE44xx_Current_Register_Settings[34]);           //0x22
  AFE44xx_Reg_Write((unsigned char)CONTROL1, AFE44xx_Current_Register_Settings[30]);           //0x1E
  
#ifdef __AFE4403__
  AFE44xx_Reg_Write((unsigned char)CONTROL3, 0);           //0x31
  AFE44xx_Reg_Write((unsigned char)PDNCYCLESTC, 0);        //0x32
  AFE44xx_Reg_Write((unsigned char)PDNCYCLEENDC, 0);       //0x33
#endif
  Enable_AFE44xx_SPI_Read();
}

