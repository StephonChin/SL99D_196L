/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "Arduino.h"

#include "m2mnet/include/m2m.h"
#include "m2mnet/include/m2m_api.h"
#include "m2mnet/include/m2m_port.h"

#include "m2mnet/config/config.h"
//#include "lights_control/com/mcu_com.h"
//#include "lights_control/led_entrance.h"

#include "system.h" 
#include "app_m2m_handle.h"
#include "application_cmd_handle.h"

void setup()
{
	
	application_setup();
	sys_setup();
    m2m_setup();
}


void loop()
{		
	application_loop();
	m2m_loop();
	system_loop();
}

