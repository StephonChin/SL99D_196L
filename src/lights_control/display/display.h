/**************************************************************
  * 
  * FileName  display.h
  * Date      26 NOV 2018
  * Author    DS.Chin
  *
**************************************************************/
#ifndef _DISPLAY_H_
#define _DISPLAY_H_


//include files
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../leddrv/ws2812_i2s.h"
#include "../../include/m2m_log.h"


//const value define
#define     LAYER_MAX             50

#define     COLOR_VAL_MAX         (uint8_t)0x0f  	/* the single colors count  number */
#define     THEME_VAL_MAX         (uint8_t)0x0d   	/* the themes count number */
#define     FADE_LEVEL            (uint8_t)15

/*
 * Parameter value
 */
#define     PARA_INVALID          	(uint8_t)0
#define     PARA_BRIGHT_MAX       	(uint8_t)4
#define     PARA_SPEED_MAX        	(uint8_t)4
#define     PARA_OTHER_MAX        	(uint8_t)10
#define     PARA_COLORNUM_MAX     	(uint8_t)16   /* each mode allow to own MODE_COLOR_MAX color value */


#define     POWER_OFF             	(uint8_t)0xf0
#define     POWER_ON              	(uint8_t)0xf1
#define     RED_FLASH             	(uint8_t)0xf2
#define     GREEN_FLASH           	(uint8_t)0xf3
#define     BLUE_FLASH            	(uint8_t)0xf4
#define     COMMON_MODE_LIMIT     	POWER_OFF


#define     STEADY                	(uint8_t)0x00
#define     TWINKLE               	(uint8_t)0x01
#define     SPARKLE               	(uint8_t)0x02
#define 	INSTEAD					(uint8_t)0x03
#define     FADE                  	(uint8_t)0x04
#define     ROLLING            		(uint8_t)0x05
#define     WAVES                 	(uint8_t)0x06
#define     FIREWORKS             	(uint8_t)0x07
#define     RAINBOW               	(uint8_t)0x08
#define     COLOR_RAND            	(uint8_t)0x09
#define     UPDWN                 	(uint8_t)0x0a
#define 	SNOW					(uint8_t)0x0b
#define		GLOW					(uint8_t)0x0c
#define		CARNIVAL				(uint8_t)0x0d
#define		ALTERNATE				(uint8_t)0x0e


#define     MODE_MAX              ALTERNATE

#define		RAND()				RndSeed += 199;srand(RndSeed);



#define	LAYOUT_NONE				0
#define	LAYOUT_2D				1
#define	LAYOUT_3D				2

//type redefine
typedef struct{
  uint8 Mode;
  uint8 ModeBuf;
  uint8 Init;
  uint8 LayoutNum;
}Display_t;

#define PARA_PACK_HEADRE_BYTE	8
typedef struct{
	uint8_t 	Mode;
	uint8_t   	Speed;
	uint8_t   	Bright;
	uint8_t   	Other;
	uint8_t   	ColorVal;
	uint8_t   	Chksum;
	uint8_t   	Reserve1;
	uint8_t   	ColorNum;
	struct COLOR_TYPE{
		uint8_t   BufR;
		uint8_t   BufG;
		uint8_t   BufB;
	}Color[PARA_COLORNUM_MAX + 1];
}ModePara_t;

typedef struct _LAYER_TYPE{
  uint16_t    Head;
  uint16_t    Tail;
}Layer_t;




//exported functions
void Display_Control(void);

//file functions
//common functions
uint8_t  Para_Err_Check(ModePara_t * para);
void Display_All_Set(uint8_t r, uint8_t g, uint8_t b);
void Display_Power_Off(void);
void Display_Power_On(void);
void Display_All_Flash(uint8_t r, uint8_t g, uint8_t b);


//tree functions
void Display_Tree_Steady(void);
void Display_Tree_Sparkle(void);
void Display_Tree_Rainbow(void);
void Display_Tree_Fade(void);
void Display_Tree_Snow(void);
void Display_Tree_Twinkle(void);
void Display_Tree_Fireworks(void);
void Display_Tree_Rolling(void);
void Display_Tree_Waves(void);
void Display_Tree_Updwn(void);
void Display_Tree_Color_Rand(void);
void Display_Tree_Instead(void);
void Display_Tree_Glow(void);
void Display_Tree_Carnival(void);
void Display_Tree_Alternate(void);




//layout functions
void Display_Layout_None_Init(void);


//exported parameters
extern Display_t    Display;
extern ModePara_t   ParaData[];
extern Layer_t      Layer[];
extern Layer_t 		LayerTemp[];
extern uint8_t      LayerMax;
extern uint8_t		LayerTest;





extern uint8_t           SpeedCtrl;
extern uint8_t           OtherCtrl;
extern uint8_t           TempR;
extern uint8_t           TempG;
extern uint8_t           TempB;
extern uint8_t           TempR1;
extern uint8_t           TempG1;
extern uint8_t           TempB1;
extern uint8_t           TopR;
extern uint8_t           TopG;
extern uint8_t           TopB;
extern uint8_t           FadeR;
extern uint8_t           FadeG;
extern uint8_t           FadeB;
extern uint8_t           TempColor;
extern uint8_t           FadeR1;
extern uint8_t           FadeG1;
extern uint8_t           FadeB1;
extern uint8_t           TempColor1;
extern uint8_t           TempStep;
extern uint8_t           FadeLevel;
extern uint16_t          RptCtrl;
extern uint16_t          RptTotal;
extern sint16_t          RndSeed;
extern uint16_t           HoldTime;
extern uint8_t           LayerStep;
extern uint16_t          LedPickAll;
extern uint8_t           ModeTime[LED_TOTAL];
extern uint8_t           ModeStep[LED_TOTAL];
extern uint8_t           LedPick[LED_TOTAL];
extern bool			  	ModeFirstFlag;

#endif
