#include "my_adc.h"
#include "battery.h"
#include "adc.h"

#include "user_custs1_def.h"
#include "string.h"
#include "prf_utils.h"
#include "attm_db.h"
#include "ke_task.h"
#include "custs1_task.h"
#include "LIS2DS12_ACC_driver.h"

extern bool dghflag;
unsigned int adc_heart = 0;
unsigned char power_rate=100;
unsigned char power_level=3;

void adc_test(void)
{
	adc_calibrate();
  power_rate = battery_get_lvl(BATT_CR2032);
	power_level=power_rate/25;
}
