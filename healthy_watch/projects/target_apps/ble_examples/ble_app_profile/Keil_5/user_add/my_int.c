
#include "my_int.h"
#include "gpio.h"
#include "user_periph_setup.h"
#include "user_custs1_def.h"
#include "string.h"
#include "prf_utils.h"
#include "attm_db.h"
#include "ke_task.h"
#include "custs1_task.h"

#define BUTTON1_TEXT  "BUTTON_1"

unsigned char Touch_release = 0;
void my_int_init(void)
{
    GPIO_RegisterCallback(GPIO0_IRQn, my_int_button1_handler);
	  GPIO_EnableIRQ( BUTTON_PORT, BUTTON_PIN1, GPIO0_IRQn, 1, 1, 10 );
}

void my_int_button1_handler(void)
{
	static bool first_flag=1;
	if(first_flag)
	{
		first_flag=0;
		return ;
	}
	Touch_release =1;
}

