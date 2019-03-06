/****************************************************************** 
  *
  * FileName    type_define.h
  * Date        19 JAN 2019
  * Author      Desheng.Chin
  * Brief       the entrance file for led
  *
*******************************************************************/
#ifndef _TYPE_DEFINE_H_
#define _TYPE_DEFINE_H_


#define AL99D			1
//#define AL99E			1

#if ((AL99D==1) && (AL99E==1))
	#error "./src/lights_control/type_define.h  --  Device type is not defined!!!"
#elif ((AL99D==0) && (AL99E==0))
	#error "./src/lights_control/type_define.h  --  Device type is redefined!!!"
#endif


#endif
