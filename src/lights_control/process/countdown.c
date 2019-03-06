/*
 * FILENAME		countdown.c
 * DESCRIPTION
 *				count down timer
 *				get the real time from APP after connected
 *				there are two mode to count down
 *					1. count down hour to turn off
 *					2. hour and minute to turn off
 */
#include "countdown.h"


//global parameters
cntdwn_T	cntdwn_data;

/*
 * FUNCTION NAME		Timer_Count_Down
 */
void Timer_Count_Down(void)
{
	//real time
	cntdwn_data.real_msec++;
	if (cntdwn_data.real_msec >= 100)
	{
		cntdwn_data.real_msec = 0;
		cntdwn_data.real_sec++;
		if (cntdwn_data.real_sec >= 60)
		{
			cntdwn_data.real_sec = 0;
			cntdwn_data.real_min++;
			if (cntdwn_data.real_min >= 60)
			{
				cntdwn_data.real_min = 0;
				cntdwn_data.real_hour++;
				if (cntdwn_data.real_hour >= 24)
				{
					cntdwn_data.real_hour = 0;
				}
			}
		}
	}

	if (cntdwn_data.off_hour == cntdwn_data.real_hour && cntdwn_data.off_min == cntdwn_data.real_min
	&& cntdwn_data.real_sec == 0 && cntdwn_data.real_msec == 0)
	{
		cntdwn_data.status = CNTDWN_TURN_OFF;
	}

	if (cntdwn_data.on_hour == cntdwn_data.real_hour && cntdwn_data.on_min == cntdwn_data.real_min
	&& cntdwn_data.real_sec == 0 && cntdwn_data.real_msec == 0)
	{
		cntdwn_data.status = CNTDWN_TURN_ON;
	}
}

