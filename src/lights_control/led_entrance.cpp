/*****************************************************************
  *
  * FunctionName  led_entrance.c
  * Date          26 NOV 2018
  * Author        Desheng.Chin
  * Brief         Process the program code of led part
  *
*******************************************************************/
#include "led_entrance.h"
#include <EEPROM.h>


#define EEPROM_CONF_SIZE		2048


//file parameters
os_timer_t    MyTimer;
bool          TimerUpdateFlag;



/**
  * FunctionName    Timer_Call_Back
  */
void Timer_Call_Back(void *pArg)
{
  TimerUpdateFlag = true;
}


/**
  * FunctionNamw  Led_program_config
  * Brief         the configuration for led needs
  *               ==> change the print port to uart0
  *               ==> intialize the timer to 80ms period
  *               ==> initialize the i2s,uart,pio
  */
void Led_Program_Config(void)
{
  //Timer init
  os_timer_setfn(&MyTimer, Timer_Call_Back, NULL);
  os_timer_arm(&MyTimer, 10, true); // 10 milliseconds

  Uart_Init();

  EEPROM.begin(EEPROM_CONF_SIZE);

  //Led dma init
  Ws2812_Init();

  //Display data init
  User_Data_Init();
}



/**
  * FunctionName    Led_entrance
  * Date            26 NOV. 2018
  * Author          DS.Chin
  * Input           None
  * Output          None
  * Brief           the entrace of led code
  */
void Led_Entrance(void)
{
	//static uint8_t  ShowStep;
	static uint8_t  ModeStep;
	
	if (TimerUpdateFlag == true){
		TimerUpdateFlag = false;

		Key_Scan();

		Data_Process();

		ModeStep++;
		if (ModeStep >= 6){
			ModeStep = 0;
			Display_Control();
		}

		Ws2812_Show();
	}
}









