#include "data_process.h"
#define data_buff_num_max 3
#define data_num_max 10

void heart_process(unsigned int data,unsigned char *heart_rate)
{
	static bool data_full_flag=0,start_flag=0;
	static unsigned char peak_num=0, data_num=0, data_buff_tail=0;
	static unsigned int data_buff[data_buff_num_max]={0},point_num=0;
	static unsigned char peak_to_peak[10]={0};
	unsigned char i,j,t;
	unsigned short period=0;
	static unsigned char up_flag=0;
	if(start_flag)point_num++;
	if(peak_num==10)
	{
		peak_num=0;
		for(i=0;i<10;i++)            // 冒泡排序
		{
			for(j=0;j<10-i;j++)
			{
				if(peak_to_peak[j]>peak_to_peak[j+1])
				{
					t=peak_to_peak[j];
					peak_to_peak[j]=peak_to_peak[j+1];
					peak_to_peak[j+1]=t;
				}
			}
		}
		for(i=1;i<9;i++)     //去掉最高值和最低值求平均值
		{
			period+=peak_to_peak[i];
		}
		period=period/8;
		*heart_rate=3000/period;
	}
	else
	{
		if(data_num==data_num_max)
		{
			if(data_full_flag)
			{
				if(data_buff[data_buff_tail]>data_buff[data_buff_tail==(data_buff_num_max-1)?0:(data_buff_tail+1)])
				{
					up_flag++;
				}
				else
				{
					if(up_flag==2)
					{
						if(start_flag==0)start_flag=1;
						else
						{
							peak_to_peak[peak_num]=point_num/10;
							peak_num++;
						}
					}
					up_flag=0;
				}
			}
			data_num=0;
			data_buff_tail++;
			if(data_buff_tail==data_buff_num_max)
			{
				if(data_full_flag==0)data_full_flag=1;
				data_buff_tail=0;
			}
		}
		data_num++;
		data_buff[data_buff_tail]+=data;
	}
}

