/**
 ****************************************************************************************
 *
 * @file user_periph_setup.c
 *
 * @brief Peripherals setup and initialization.
 *
 * Copyright (C) 2015. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration
#include "user_periph_setup.h"       // peripheral configuration
#include "global_io.h"
#include "gpio.h"
#include "uart.h"                    // UART initialization
#include "fk_oled.h"
#include "spi.h"
#include "spi_flash.h"
//dgh add
#include "my_int.h"
#include "my_heart.h"
#include "my_timer.h"
#if DEVELOPMENT_DEBUG

void GPIO_reservations(void)
{
/*
* Globally reserved GPIOs reservation
*/

/*
* Application specific GPIOs reservation. Used only in Development mode (#if DEVELOPMENT_DEBUG)

i.e.
    RESERVE_GPIO(DESCRIPTIVE_NAME, GPIO_PORT_0, GPIO_PIN_1, PID_GPIO);    //Reserve P_01 as Generic Purpose I/O
*/

#ifdef CFG_PRINTF_UART2
    RESERVE_GPIO(UART2_TX, UART2_TX_GPIO_PORT, UART2_TX_GPIO_PIN, PID_UART2_TX);
    RESERVE_GPIO(UART2_RX, UART2_RX_GPIO_PORT, UART2_RX_GPIO_PIN, PID_UART2_RX);
#endif
	//lcd
	RESERVE_GPIO(GPIO,LCD_CS_PORT, LCD_CS_PIN, PID_GPIO);
	RESERVE_GPIO(GPIO,LCD_CD_PORT, LCD_CD_PIN, PID_GPIO);
	RESERVE_GPIO(GPIO,LCD_RST_PORT, LCD_RST_PIN, PID_GPIO);
	//spi
	RESERVE_GPIO(SPI_CLK, SPI_PORT, SPI_PIN_CLK, PID_SPI_CLK);
  RESERVE_GPIO(SPI_DO, SPI_PORT, SPI_PIN_DO, PID_SPI_DO); 
	RESERVE_GPIO(SPI_EN, SPI_PORT, SPI_PIN_CS, PID_SPI_EN);
	RESERVE_GPIO(SPI_DI, SPI_PORT, SPI_PIN_DI, PID_SPI_DI);
	//MOTOR
	//RESERVE_GPIO(GPIO, MOTOR_PORT, MOTOR_PIN, PID_GPIO);
	//button
	RESERVE_GPIO(GPIO, BUTTON_PORT, BUTTON_PIN1, PID_GPIO);
	//st2378
	RESERVE_GPIO(st,st2378_port, st2378_cl, PID_I2C_SCL);
	RESERVE_GPIO(st, st2378_port, st2378_da, PID_I2C_SDA);
	//uart
	RESERVE_GPIO(UART2_TX, uart_port, uart_tx, PID_UART2_TX);
  RESERVE_GPIO(UART2_RX, uart_port, uart_rx, PID_UART2_RX);
	//stm32ldo
	RESERVE_GPIO(stm32ldo, stm32_port, stm32_pin, PID_GPIO);
	//adc
	RESERVE_GPIO(adc, adc_port, adc_pin, PID_ADC);
	//back light
	RESERVE_GPIO(back_light, backlight_port, backlight_pin, PID_GPIO);
	//
  RESERVE_GPIO(SHOCK, SHOCK_PORT, SHOCK_PIN, PID_GPIO);
	
//	RESERVE_GPIO(SPI_EN, 0, 3, PID_SPI_EN);
}

#endif //DEVELOPMENT_DEBUG

void set_pad_functions(void)        // set gpio port function mode
{
#ifdef CFG_PRINTF_UART2
    GPIO_ConfigurePin(UART2_TX_GPIO_PORT, UART2_TX_GPIO_PIN, OUTPUT, PID_UART2_TX, false);
    GPIO_ConfigurePin(UART2_RX_GPIO_PORT, UART2_RX_GPIO_PIN, INPUT, PID_UART2_RX, false);
#endif
	//lcd
	GPIO_ConfigurePin(LCD_CS_PORT, LCD_CS_PIN, OUTPUT, PID_GPIO, true);
	GPIO_ConfigurePin(LCD_CD_PORT, LCD_CD_PIN, OUTPUT, PID_GPIO, false);
	GPIO_ConfigurePin(LCD_RST_PORT, LCD_RST_PIN, OUTPUT, PID_GPIO, false);
	GPIO_ConfigurePin(backlight_port, backlight_pin, OUTPUT, PID_GPIO, false);
	//spi
	GPIO_ConfigurePin( SPI_PORT, SPI_PIN_CLK, OUTPUT, PID_SPI_CLK, false );
	GPIO_ConfigurePin( SPI_PORT, SPI_PIN_CS, OUTPUT, PID_SPI_EN, true );
  GPIO_ConfigurePin( SPI_PORT, SPI_PIN_DO, OUTPUT, PID_SPI_DO, false ); 	
	GPIO_ConfigurePin( SPI_PORT, SPI_PIN_DI, INPUT, PID_SPI_DI, true );
	//MOTOR
	GPIO_ConfigurePin( MOTOR_PORT, MOTOR_PIN, OUTPUT, PID_GPIO, false );
	//button
	GPIO_ConfigurePin( BUTTON_PORT, BUTTON_PIN1, INPUT, PID_GPIO, false );
	//st2378
	GPIO_ConfigurePin(st2378_port, st2378_cl,INPUT, PID_I2C_SCL, false ); 	
	GPIO_ConfigurePin(st2378_port, st2378_da,INPUT, PID_I2C_SDA, false );
	//stm32ldo
	GPIO_ConfigurePin(stm32_port, stm32_pin,OUTPUT, PID_GPIO, false );
	//uart
	GPIO_ConfigurePin(uart_port, uart_tx, OUTPUT, PID_UART2_TX, false);
  GPIO_ConfigurePin(uart_port, uart_rx, INPUT, PID_UART2_RX, false);
	
	//Υπ¶―
	GPIO_ConfigurePin(SHOCK_PORT, SHOCK_PIN, OUTPUT, PID_GPIO, false);
/*
* Configure application ports.
i.e.
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_1, OUTPUT, PID_GPIO, false ); // Set P_01 as Generic purpose Output
*/
}
//unsigned char test_data[2048]={0};
void periph_init(void)
{
		volatile unsigned short Init_i, j;
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));

    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);

    //rom patch
    patch_func();

    //Init pads
    set_pad_functions();

    // (Re)Initialize peripherals
    // i.e.
    //  uart_init(UART_BAUDRATE_115K2, 3);

#ifdef CFG_PRINTF_UART2
    SetBits16(CLK_PER_REG, UART2_ENABLE, 1);
    uart2_init(UART_BAUDRATE_115K2, 3);
#endif
			
		SetBits16(CLK_PER_REG, UART2_ENABLE, 1);	//enable uart2
    uart2_init(UART_BAUDRATE_115K2, 3);
		
   // Enable the pads
    SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);
		SPI_Pad_t spi_cs_pad;
		spi_cs_pad.port = GPIO_PORT_0;
		spi_cs_pad.pin = GPIO_PIN_3;
		spi_init(&spi_cs_pad, SPI_MODE_8BIT, SPI_ROLE_MASTER, SPI_CLK_IDLE_POL_LOW, SPI_PHA_MODE_0, SPI_MINT_DISABLE, SPI_XTAL_DIV_2);
		spi_flash_init(0x40000, 256);
//		spi_flash_read_data(test_data,0x3d000,20);
		OLED_Init(); 
//		spi_flash_read_data(test_data,0x3d000,2048);
//		oled_display(3);
		my_int_init();//int button
		my_timer_init();
		
}
