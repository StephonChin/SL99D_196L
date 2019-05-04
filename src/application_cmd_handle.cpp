	/*********************************************************
** sample 
*********************************************************/
#include <string.h>
#include "Arduino.h"

#include "m2mnet/include/m2m_type.h"
#include "m2mnet/include/m2m.h"
#include "m2mnet/include/m2m_api.h"
#include "m2mnet/include/m2m_log.h"
#include "m2mnet/config/config.h"
#include "m2mnet/include/app_implement.h"
#include "m2mnet/include/util.h"
#include "m2mnet/include/m2m_app.h"
#include "app_m2m_handle.h"
#include "app_config.h"
#include "lights_control/led_entrance.h"

#include "lights_control/com/app_com.h"
/****** C++ to c declear*******************************************************/

extern "C" int app_cmd_handle(u8 cmd,u8*p_data,int recv_len,M2M_packet_T **pp_ack_data);

void application_setup(void){
	Led_Program_Config();

}
void application_wifi_connet_loop(void){
	Led_Entrance(); 
}


void application_loop(void){
	Led_Entrance();

}
int app_cmd_handle(u8 cmd,u8*p_data,int recv_len,M2M_packet_T **pp_ack_data){
	int ret = 0;
	//Lm2m_ota_data putota;
	//Lm2m_ota_data getota;

	switch (cmd){	
		
		case WIFI_CMD_APP_UART_SEND_RQ:
			///if(p_data && recv_len)
			//	ret = Serial.write(p_data,recv_len);
			//break;
    case WIFI_CMD_APP_LED_TWINKLE_RQ:
    	//printf("===%d===",recv_len);

    	
	  //m2m_bytes_dump((u8*)"-----receive data:", p_data, recv_len);
	  //printf("====WARN==== enter app_cmd_handle!!!");
      ret = app_cmd_handler( p_data, recv_len, pp_ack_data);
	}

	return ret;
}

