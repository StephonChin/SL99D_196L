/*
 * esp_function.c
 * description: esp8266 function.
 *  Created on: 2018-6-13
 *      Author: skylli
 * Time list:
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Arduino.h"
#include <ArduinoOTA.h>
#include "osapi.h"
#include "ets_sys.h"
#include "lwip/inet.h"

#include <stdbool.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266httpUpdate.h>

#include "app_config.h"
#include "m2mnet/include/m2m_type.h"
#include "m2mnet/include/util.h"
#include "m2mnet/include/m2m_log.h"
#include "m2mnet/include/m2m_api.h"
#include "m2mnet/include/m2m_app.h"
#include "m2mnet/include/m2m_port.h"

#include "lights_control/leddrv/ws2812_dma.h"
#include "system.h"

#include "m2mnet/platform/esp/config_hardware.h"
#include "user_interface.h"
#include "application_cmd_handle.h"
#include "config.h"
#include <EEPROM.h>
#include "app_m2m_handle.h"


#define EEPROM_VER	(0X11)
#define DIFF(a,b) ((a>b)?(a-b):(b-a))
#define resttime 5000
#define MAXWAITTIME (3*60*1000)
static u8 *p_g_ota_url = NULL;
SYS_cnn_status g_sys_cnn = SYS_CNN_LOST_CONNECT;
static BOOL en_broadcast = TRUE;

/* c interface */
extern "C" void sys_setup(void);
extern "C" void sys_factory_reset(void);
extern "C" int sys_smartconfig_auto_connet(void);
extern "C" void sys_sta_smartconfig(void);
extern "C" void sys_uart_report_status(int stat);
extern "C" int sys_ssid_pw_reset(LM2M_router_conf *p_router);

extern "C"  SYS_cnn_status sys_connect_status_hanle(size_t net);
extern "C"	int sys_cmd_handle(size_t net,u8 cmd,u8*p_data,int recv_len,M2M_packet_T **pp_ack_data);

extern "C"	int sys_eeprom_read(int address, u8 *p_buf, int len);
extern "C"	int sys_eeprom_write(int address, u8 *p_buf, int len);
extern "C"  BOOL sys_in_ap_mode(void);

int sys_host_config(size_t net,u8 *p_data, int recv_len);



// function
// runing mode 
static EEPROM_conf_T sys_conf;

int sys_eeprom_read(int address, u8 *p_buf, int len){
	int i;
	

	for(i=0;i<len;i++){
		p_buf[i] =  EEPROM.read(address+i);
	}
	
	return i;
}
int sys_eeprom_write(int address, u8 *p_buf, int len){
	int i;

	
	//EEPROM.begin(len);
	for(i=0;i<len;i++){
		EEPROM.write( address + i, p_buf[i]);
	}
	//dma_int_off_irq();
	EEPROM.commit();
	return i;
}
/**********************************************
** description: 保存秘钥.
** args:    
**      
**          p_key;p_keylen;
********/
u32 esp8266_secretKey_write(u8 *p_key,u16 p_keylen)
{ 
	 if( sys_conf.ver_mak != EEPROM_VER )
     return 1;
    mcpy(sys_conf.p_key,p_key,p_keylen);
	sys_conf.keylen=p_keylen;
	sys_eeprom_write( EEPROM_CONF_ADDRESS, (u8*)&sys_conf,  sizeof(EEPROM_conf_T));

	return 0;
}
u32 esp8266_secretKey_read(u8 *p_key,u16 *p_keylen)
{
 if( sys_conf.ver_mak != EEPROM_VER || sys_conf.keylen>32 || sys_conf.keylen<1)
   return 1;
 mcpy(p_key,sys_conf.p_key,sys_conf.keylen);
 *p_keylen=sys_conf.keylen;
  return 0;

}

int sys_wifi_mode_set(LESP_WIFI_MODE_T mod){
	sys_conf.wifi_mod = mod;
	sys_eeprom_write( EEPROM_CONF_ADDRESS, (u8*)&sys_conf,  sizeof(EEPROM_conf_T));
}


void sys_gpio_init(void){
	randomSeed(analogRead(0));
	//Serial.begin(9600);

    
	pinMode(SREST_PIN, OUTPUT);
	digitalWrite (SREST_PIN, 1);
	pinMode(REST_PIN, OUTPUT);
	digitalWrite(REST_PIN, 1);
	pinMode(REST_PIN, INPUT);
	pinMode(LED_WIFI_CONN_PIN, OUTPUT);

}
static void sys_broadcast_set(BOOL status){
	en_broadcast = status;
}
BOOL sys_broadcast_enable(void){
	return en_broadcast;
}


void sys_wifi_init(void){
	int ret = 0;
	char r[3], w[3];
	sys_broadcast_set(TRUE);
	switch( sys_conf.wifi_mod ){
		
		case WIFI_MODE_RST_SMT:
		
			m2m_log_debug("WIFI_MODE_RST_SMT");
			WiFi.mode(WIFI_STA);
			sys_sta_smartconfig();
		//  sys_conf.wifi_mod = WIFI_MODE_STA_SMT;
		//  sys_eeprom_write( EEPROM_CONF_ADDRESS, (u8*)&sys_conf, sizeof(EEPROM_conf_T));
			sys_broadcast_set(TRUE);
			break;
		case WIFI_MODE_RST_AP:
			{
				u8 mac[8];
				char ssid[128];
 				memset(ssid, 0, 128);			
				memset( mac, 0, 8);
				
				m2m_log_debug("WIFI_MODE_RST_AP");
				WiFi.mode( WIFI_AP );
				WiFi.macAddress( mac );
				sprintf(ssid, "%s%02x%02x", SOFTAP_SSID_,mac[4],mac[5]);
				m2m_log_debug("ssid is : %s", ssid);
				
				if(WiFi.softAP(ssid)){
					m2m_log_error("Ready");
					
				}else m2m_log_debug("failt");

				m2m_log_debug("start ap mode.");

				sys_broadcast_set(TRUE);
			}
			break;
		// case WIFI_MODE_STA_PW:
			
		// 	m2m_log_debug("WIFI_MODE_STA_PW");
		// 	if(sys_conf.ssidlen && sys_conf.pwlen){
		// 		u8 *p = NULL;
		// 		u8 ssid[32];
		// 		u8 pw[32];
		// 		u32 old_time = m2m_current_time_get();
		// 		mmemset(ssid, 0, 32);				
		// 		mmemset(pw, 0, 32);

		// 		WiFi.mode(WIFI_STA);
		// 		sys_eeprom_read( ( EEPROM_CONF_ADDRESS + (int)((u8*)sys_conf.p_ssid_pw -  (u8*)&sys_conf.ver_mak) ), ssid,  sys_conf.ssidlen);
		// 		sys_eeprom_read( ( EEPROM_CONF_ADDRESS + (int)((u8*)sys_conf.p_ssid_pw -  (u8*)&sys_conf.ver_mak) + sys_conf.ssidlen ), pw,  sys_conf.pwlen);
				
		// 		m2m_log_debug("ssid %s", ssid);
		// 		m2m_log_debug("pw %s", pw);
		// 		//m2m_bytes_dump((u8*)"ssid is ", ssid, (int)sys_conf.ssidlen);
		// 		//m2m_bytes_dump((u8*)"password  is ", pw, (int)sys_conf.pwlen);
				
		// 		WiFi.begin((const char*)ssid, (const char*)pw);
				
		// 		while (WiFi.status() != WL_CONNECTED){ // Wait for the Wi-Fi to connect
		// 		    delay(50);
		// 		    //Serial.print('.');
		// 			sys_factory_reset();
		// 			u32 curr_tm = m2m_current_time_get();
		// 			if( DIFF(curr_tm,old_time) > (MAX_SMARTCONFIG_TIME * 1000) ){
		// 					break;
		// 			}
		// 		  }
		// 	}
		// 	break;
		case WIFI_MODE_STA_SMT:
			
			m2m_log_debug("WIFI_MODE_STA_SMT");
			WiFi.mode(WIFI_STA);
			sys_smartconfig_auto_connet();
			break;
	}
}

void sys_led_flash(void){
	static u8 led_status = 0;
	
	led_status = (led_status)?0:1;
	digitalWrite(LED_WIFI_CONN_PIN,led_status);
}
void sys_status_led_flash(SYS_cnn_status status){
	static u32 old_tm = 0;

	//m2m_printf("<%d>",status);
	u32 c_time = m2m_current_time_get();
	switch(status){
		
		case SYS_CNN_OTAING:
			if( DIFF(old_tm, c_time ) >= 300){
				old_tm = c_time;
				sys_led_flash();
			}
			break;
		
		case SYS_CNN_CONFIGING_STA://在STA模式下 快闪
			if( DIFF(old_tm, c_time ) >= 500){
				old_tm = c_time;
				sys_led_flash();
			}
			break;
		case SYS_CNN_CONFIGING_AP://在AP模式下 慢闪
			if( DIFF(old_tm, c_time ) >= 1500){
				old_tm = c_time;
				sys_led_flash();
			}
			break;
		case SYS_CNN_LOST_CONNECT://没连接路由 灭
			digitalWrite(LED_WIFI_CONN_PIN,1);
			break;
		case SYS_CNN_ONLINE://在线常亮
			digitalWrite(LED_WIFI_CONN_PIN,0);
			break;
	}
}
void sys_uart_report_status(int stat){
	static int old_stat = 0;
	char buf[5];
	int i;
	
	if( old_stat == stat )
		return;
	
	buf[0] = VERSION;
	buf[1] = CMD;
	buf[2] = IDX;
	buf[3] = LEN;
	buf[4] = stat;
	for(i=0;i<5;i++)
		 Serial.print(buf[i]);

	old_stat = stat;


}
static void sys_cnn_status_set(SYS_cnn_status status){
	g_sys_cnn = status;
}
BOOL sys_in_ap_mode(void){
	if (WiFi.getMode() == WIFI_AP)
		return TRUE;
	else
		return FALSE;

}

SYS_cnn_status sys_connect_status_hanle(){
    //static bool s_fag = 0;
	SYS_cnn_status ret_status = SYS_CNN_MAX;
	switch(g_sys_cnn){
		case SYS_CNN_LOST_CONNECT:
			if(WiFi.getMode() == WIFI_STA && WiFi.status() !=  WL_CONNECTED)
				break;
			g_sys_cnn = SYS_CNN_ONLINE;
			ret_status = SYS_CNN_ONLINE;
		case SYS_CNN_ONLINE:
             if(WiFi.getMode() == WIFI_STA && WiFi.status() !=  WL_CONNECTED){
				g_sys_cnn = SYS_CNN_LOST_CONNECT;
				ret_status =  SYS_CNN_LOST_CONNECT;
            }
                break;
		case SYS_CNN_CONFIGING_AP:
		      	g_sys_cnn = SYS_CNN_LOST_CONNECT;	
						break;
	}
    if( sys_in_ap_mode() ){

        g_sys_cnn = SYS_CNN_CONFIGING_AP;
        ret_status = SYS_CNN_CONFIGING_AP;
//        	if(!s_fag ){
//			    ret_status = SYS_CNN_CONFIGING_AP;
//			    s_fag = 1;
//			}
    }

	if( ret_status < SYS_CNN_MAX){
		sys_uart_report_status(ret_status);//ret_status run led
    }
	sys_status_led_flash(g_sys_cnn);
	return ret_status;
}
int sys_eeprom_factory_reset(void){
	u8 *p = NULL;
	int len =  sizeof( SYS_Host_info_t ) + strlen(TST_SERVER_HOST);
	SYS_Host_info_t *p_host = (SYS_Host_info_t*) mmalloc( len +1 );
	
	if(!p_host)
		return  M2M_ERR_NULL;

	// todo
  sys_conf.ver_mak == EEPROM_VER;
	memcpy(sys_conf.p_key,"123",sizeof("123")); 
	sys_conf.keylen=strlen("123");
	sys_eeprom_write( EEPROM_CONF_ADDRESS, (u8*)&sys_conf,  sizeof(EEPROM_conf_T));
	
	// todo add aes aeskey
	// host reset to factory configure.
	p_host->port = TST_SERVER_PORT;
	p_host->len = strlen(TST_SERVER_HOST);
	mcpy( (u8*)p_host->cname, (u8*)TST_SERVER_HOST, strlen(TST_SERVER_HOST) );
	p = (u8*)PRODUCT_SERVER_ID;
	STR_2_HEX_ARRAY_INV( p_host->s_id.id, ID_LEN, p, strlen(PRODUCT_SERVER_ID));
	byte_printf((u8*)"---freset id: ", (u8*)p_host->s_id.id, ID_LEN );
	//m2m_printf("\nreset host %s\n", p_host->cname);
	sys_host_config(NULL, (u8*)p_host, len);
	mfree(p_host);
	return 0;
}
void  sys_factory_reset(void){
	static u32 last_tm = 0 ; 
	u32 curr_tm = 0;


	if(0 == digitalRead(REST_PIN)){

		//m2m_printf("[%d]", digitalRead(REST_PIN));
		//m2m_printf("(%d)\n", last_tm);
	
		curr_tm = m2m_current_time_get();
		if(DIFF( curr_tm,last_tm ) > resttime){
			
			sys_eeprom_factory_reset();
			// WiFi.disconnect();
			// WiFi.setAutoConnect(false);
			// mmemset(sys_conf.p_ssid_pw, 0,  64);
			
			if(sys_conf.wifi_mod == WIFI_MODE_RST_SMT)
				sys_wifi_mode_set(WIFI_MODE_RST_AP);
			else
				sys_wifi_mode_set(WIFI_MODE_RST_SMT);
			delay(500);
           // digitalWrite (SREST_PIN, 0);
			system_restore();
			ESP.reset();
			}    
	}else 	
		last_tm = m2m_current_time_get(); 
			   
} 

/*wifi specti*/
/*
*  Calling it will instruct module to switch to the station mode 
*  and connect to the last used access point basing on configuration saved in flash memory.
*/
int sys_smartconfig_auto_connet(void)
{
  // set to sta
  u8 flag = 0;
  u32 tmp_tm = 0, last_tm = 0, old_time = 0 ; 
  WiFi.mode(WIFI_STA);
  WiFi.begin();
	g_sys_cnn=SYS_CNN_LOST_CONNECT;
  old_time = m2m_current_time_get();
  do{
    int wstatus = WiFi.status();
    if ( wstatus == WL_CONNECTED)
    {
      m2m_log_debug("AutoConfig Success");
      m2m_log_debug("SSID:%s\r\n", WiFi.SSID().c_str());
      m2m_log_debug("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.printDiag(Serial);
      return true;
      //break;
    }else{
      	delay(10);
	  	tmp_tm = millis();
		if( DIFF_(tmp_tm ,last_tm ) > 500){

		flag = flag ==0 ? 1:0; 
	    digitalWrite( LED_WIFI_CONN_PIN, flag);
	    last_tm = millis();
  	  }
    }
	u32 curr_tm = m2m_current_time_get();
	
	if( DIFF(curr_tm,old_time) > (MAX_SMARTCONFIG_TIME * 1000) ){
			break;
	}
    sys_factory_reset();
	application_wifi_connet_loop();
  }while(1);
  m2m_log_error("sys_smartconfig_auto_connet Faild!" );
  return true;
  //WiFi.printDiag(Serial);
}


/*
* Start smart configuration mode by sniffing for special packets 
*  that contain SSID and password of desired Access Point. Depending on result either true or 
*  false is returned.
*/
void sys_sta_smartconfig(void) {
  // u8 _configing = 1;
  // SYS_cnn_status _led_state = SYS_CNN_CONFIGING_STA;
	g_sys_cnn=SYS_CNN_CONFIGING_STA;
  // u32 last_tm = 0, curr_tm =0; 
  int ret = 0;
  // config in output mode   
  digitalWrite(LED_WIFI_CONN_PIN,0);
  
  //Init WiFi as Station, start SmartConfig
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
#if 0
  last_tm = m2m_current_time_get();
  while( WiFi.status() != WL_CONNECTED) {      //wait app to connect

		if( WiFi.smartConfigDone()){
			ret = WiFi.setAutoConnect(true);
			//break;
		}
		delay(10);
		
		application_wifi_connet_loop();
		sys_status_led_flash( _led_state );
		// key rest 
		sys_factory_reset();
	#if 1
		curr_tm = m2m_current_time_get();
		if( _configing && DIFF(curr_tm,last_tm ) > (MAX_SMARTCONFIG_TIME * 1000) ){
			_configing = 0;
			_led_state = SYS_CNN_LOST_CONNECT;
			WiFi.stopSmartConfig();
			break;
		}
	#endif

		}
#endif
  //Configure module to automatically connect on power on to the last used access point.
  #if 0
  //Wait for WiFi to connect to AP
  WiFi.begin();
  last_tm = m2m_current_time_get();
  while (WiFi.status() != WL_CONNECTED) {  //phone connecting the AP
  	delay(30);
	application_wifi_connet_loop();
  	sys_status_led_flash( SYS_CNN_CONFIGING_STA );
  	if( DIFF(m2m_current_time_get(),last_tm ) > (1000) ){
		  break;
	   }
  }
  local_ip_save();
  // light up led.
  sys_status_led_flash(SYS_CNN_LOST_CONNECT);  
#endif
}
void sys_conf_printf(void){
	m2m_log("version %d ", sys_conf.ver_mak);
	m2m_log("wifi mode %d", sys_conf.wifi_mod);
// m2m_log("ssi_password %s", sys_conf.p_ssid_pw);
	m2m_log("server port  %d cname %s", sys_conf.host.port, sys_conf.host.cname);
	byte_printf((u8*)"server host id is ", (u8*)sys_conf.host.s_id.id, ID_LEN);
}	
void sys_setup(void){
	
	//EEPROM.begin(EEPROM_CONF_SIZE_MX);
	sys_eeprom_read(EEPROM_CONF_ADDRESS, (u8*)&sys_conf, sizeof(EEPROM_conf_T) );
	if( sys_conf.ver_mak != EEPROM_VER ){
		sys_eeprom_factory_reset();
	}
	sys_gpio_init();
	sys_wifi_init();
    local_ip_save();
	sys_conf_printf();
	// increaset reset counter.
	sys_conf.reset_cnt++;
	sys_eeprom_write( EEPROM_CONF_ADDRESS, (u8*)&sys_conf,  sizeof(EEPROM_conf_T));
}

// system 
int sys_ssid_pw_reset(LM2M_router_conf *p_router){

	u8 *p = NULL;
	u8 old_wifi_mod = sys_conf.wifi_mod;
	if(!p_router  || !p_router->ssidlen || ( p_router->ssidlen + p_router->passwordlen )  > 64 )
		return -1;
	u8 flag = 0;
    u32 last_tm = 0;
	//sys_conf.ssidlen =  p_router->ssidlen;
//	sys_conf.pwlen = p_router->passwordlen;
  m2m_log("set ssid and pwd!ssidlen=%d,pwdlen=%d,pwd=%.*s\r\n",p_router->ssidlen,p_router->passwordlen,p_router->passwordlen+p_router->ssidlen,p_router->p_ssid_pw);
	char *ssid=(char*)malloc(p_router->ssidlen+1);
	mmemset((u8*)ssid,0,p_router->ssidlen);
	memcpy(ssid,p_router->p_ssid_pw,p_router->ssidlen);
	ssid[p_router->ssidlen]='\0';
	char *pwd=(char*)malloc(p_router->passwordlen+1);
	mmemset((u8*)pwd,0,p_router->passwordlen);
	memcpy(pwd,&p_router->p_ssid_pw[p_router->ssidlen],p_router->passwordlen);
	pwd[p_router->passwordlen]='\0';
	m2m_log("ssid:%s,pwd:%s,\r\n",ssid,pwd);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid,pwd);
	u32 curr_tm = m2m_current_time_get()+30*1000;
	 while(WiFi.status() != WL_CONNECTED){
			delay(10);
	   if(DIFF_(millis(),last_tm ) > 1500){
	    flag = flag ==0 ? 1:0; 
	    digitalWrite( LED_WIFI_CONN_PIN, flag);
	    last_tm = millis();
  	  }
		
			if( m2m_current_time_get()>curr_tm ){
			break;
		}
	}
	//m2m_printf("\n--->ssid and password is %s\n", p_router->p_ssid_pw);
#if 1
	m2m_log("connect successs!\r\n");
	//WiFi.disconnect();
	//WiFi.setAutoConnect(false); 
	
	 // light up led.
  sys_status_led_flash(SYS_CNN_LOST_CONNECT);  
	sys_conf.wifi_mod = WIFI_MODE_STA_SMT;
	sys_eeprom_write( EEPROM_CONF_ADDRESS, (u8*)&sys_conf, sizeof(EEPROM_conf_T));
	free(ssid);
	free(pwd);
//	sys_conf.wifi_mod = old_wifi_mod;
#endif
    
	return 0;
}

int sys_host_config(size_t net,u8 *p_data, int recv_len){
	SYS_Host_info_t host;

	_RETURN_EQUAL_0( p_data,  M2M_ERR_INVALID);
	_RETURN_EQUAL_0( recv_len,  M2M_ERR_INVALID);

	
	mcpy( (u8*)&sys_conf.host, (u8*)p_data, sizeof(SYS_Host_info_t));
	
	sys_eeprom_write( EEPROM_CONF_ADDRESS, (u8*)&sys_conf, sizeof(EEPROM_conf_T));
	
	sys_eeprom_write( EEPROM_CONF_ADDRESS + sizeof(EEPROM_conf_T), (u8*)(p_data +  sizeof(SYS_Host_info_t) ), recv_len - sizeof(SYS_Host_info_t) );
	m2m_log_debug("Host cname %s have been write to eeprom",  &p_data[ sizeof(SYS_Host_info_t)] );

	
	u8 *p = &p_data[sizeof(SYS_Host_info_t)];
	mcpy( (u8*)&host, (u8*)p_data, sizeof(SYS_Host_info_t));
	if(net)
		m2m_net_host_update(net, &host.s_id, p, host.port);

	
	return recv_len;
}
/**
**  读取 eeprom 获取 host， 注意 活的的 host 必须 调用 mfree 进行销毁。
****/
SYS_Host_info_t *sys_host_alloc(M2M_id_T *p_id    ){

	SYS_Host_info_t *p_host = NULL;
	_RETURN_EQUAL_0( sys_conf.host.len,  NULL);

	p_host = (SYS_Host_info_t*) mmalloc( sizeof(SYS_Host_info_t) + sys_conf.host.len + 1);
	_RETURN_EQUAL_0(p_host,  NULL);
	mcpy( (u8*)p_host, (u8*)&sys_conf.host, (int)sizeof( SYS_Host_info_t));

	if(p_host->len){
		sys_eeprom_read( EEPROM_CONF_ADDRESS + sizeof(EEPROM_conf_T),  p_host->cname, p_host->len);
	}
	if(p_id){
		mcpy((u8*)p_id, (u8*)sys_conf.host.s_id.id, ID_LEN);
	}

	m2m_log_debug( "host cname is %s",  p_host->cname);
	
	return p_host;
}
int sys_ota_update(const u8 *p_url){

	if(p_url && strstr( (const char*)p_url, "http:") ){
		while(1){
			t_httpUpdate_return  ret = ESPhttpUpdate.update((const char*)p_url);
			sys_cnn_status_set(SYS_CNN_OTAING);
				switch(ret) {
					case HTTP_UPDATE_FAILED:
						m2m_printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
						ESP.restart();
						break;
				
					case HTTP_UPDATE_NO_UPDATES:
						m2m_printf("HTTP_UPDATE_NO_UPDATES");
						break;
				
					case HTTP_UPDATE_OK:
						m2m_printf("HTTP_UPDATE_OK");
						ESP.restart();
						break;
				}
			}
	}
	
}

int sys_cmd_handle(size_t net,u8 cmd,u8*p_data,int recv_len, M2M_packet_T **pp_ack_data){
	int ret = 0;
	Lm2m_ota_data putota;
	Lm2m_ota_data getota;

	switch (cmd){	
		case WIFI_CMD_SYS_VERSION_RQ:
			   {
				   u8 *p_v = m2m_version();
		
				   //M2M_packet_T *p_ack  = ( M2M_packet_T*)mmalloc( sizeof(M2M_packet_T) );
				   M2M_packet_T *p_ack	= (M2M_packet_T*)malloc( sizeof(M2M_packet_T) );
				   if(p_v && p_ack){
					   memset(p_ack, 0, sizeof( M2M_packet_T) );
					   p_ack->len = strlen( (const char*)p_v );
					   m2m_log("version %d", p_ack->len);
					   p_ack->p_data = (u8*)malloc( p_ack->len + 1 );
					   if( p_ack->p_data ){
						   memset( p_ack->p_data, 0, p_ack->len + 1);
						   mcpy(p_ack->p_data,	p_v, p_ack->len);
					   }
					   m2m_log_debug("---> version %s", p_v );
					   *pp_ack_data = p_ack;
				   }
				   break;
			   }

		case WIFI_CMD_SYS_RELAYHOST_SET_RQ:
		{
			if( p_data && recv_len)
				ret = sys_host_config(net,p_data, recv_len);
		}
			break;
		
		case WIFI_CMD_SYS_REBOOT_RQ:
			// todo ack 
			ESP.restart();	
			break;
		case WIFI_CMD_ROUTER_CONF:
			if(p_data  && recv_len > sizeof(LM2M_router_conf)){
				ret = sys_ssid_pw_reset( (LM2M_router_conf*)p_data );
			}
			break;			
		case WIFI_CMD_SYS_OTA_UPDATE_RQ:
			m2m_printf("----->>WIFI_CMD_SYS_OTA_UPDATE_RQ  %s \n", p_data);
			if(p_data && recv_len > 0){
				m2m_bytes_dump((u8*)"ota ", p_data, recv_len);
				p_g_ota_url = (u8*)mmalloc(strlen((char*)p_data) +1);
				if(p_g_ota_url)
					mcpy(p_g_ota_url, p_data, strlen((char*)p_data));
			}		
			break;
		// case WIFI_CMD_SYS_BROADCASTSETTING_RQ:
		// 	if(p_data && recv_len > 0){
		// 		if(p_data[0]){
		// 			//m2m_broadcast_enable(net);
		// 		}else{					
		// 			m2m_broadcast_disable(net);
		// 		}
		//	}
		}

	return ret;
}
void system_loop(void){
  static u32 currtim=m2m_current_time_get()+MAXWAITTIME;
	if(p_g_ota_url){
		
		m2m_printf("-222---->>url %s \n", p_g_ota_url);
		sys_ota_update(p_g_ota_url);
		mfree(p_g_ota_url);
		p_g_ota_url = NULL;
	}
if(	g_sys_cnn==SYS_CNN_CONFIGING_STA && (WiFi.status() == WL_CONNECTED || currtim<m2m_current_time_get() )) {      //wait app to connect
  	WiFi.setAutoConnect(true);
	 g_sys_cnn=SYS_CNN_LOST_CONNECT;
		sys_conf.wifi_mod = WIFI_MODE_STA_SMT;
		sys_eeprom_write( EEPROM_CONF_ADDRESS, (u8*)&sys_conf, sizeof(EEPROM_conf_T));
		local_ip_save();
		m2m_setup();
	}
	// 指示灯控制.
	sys_connect_status_hanle( );
	//  恢复出厂按键探测.
	sys_factory_reset();
}
