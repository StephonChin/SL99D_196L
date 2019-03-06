/*************************************************************
  *
  * FileName    layout.c
  * Brief       lay out information
  *
**************************************************************/
#include "display.h"

/**
  * FunctionName  Display_Layout_None_Init
  */
void Display_Layout_None_Init(void)
{
	#if AL99D
	LayerMax = 17;

	Layer[0].Head = 0;
	Layer[0].Tail = 18;

	Layer[1].Head = 19;
	Layer[1].Tail = 37;

	Layer[2].Head = 38;
	Layer[2].Tail = 53;

	Layer[3].Head = 54;
	Layer[3].Tail = 69;

	Layer[4].Head = 70;
	Layer[4].Tail = 81;

	Layer[5].Head = 82;
	Layer[5].Tail = 93;

	Layer[6].Head = 94;
	Layer[6].Tail = 105;

	Layer[7].Head = 106;
	Layer[7].Tail = 117;

	Layer[8].Head = 118;
	Layer[8].Tail = 128;

	Layer[9].Head = 129;
	Layer[9].Tail = 139;

	Layer[10].Head = 140;
	Layer[10].Tail = 150;

	Layer[11].Head = 151;
	Layer[11].Tail = 160;

	Layer[12].Head = 161;
	Layer[12].Tail = 170;

	Layer[13].Head = 171;
	Layer[13].Tail = 179;

	Layer[14].Head = 180;
	Layer[14].Tail = 187;

	Layer[15].Head = 188;
	Layer[15].Tail = 191;

	Layer[16].Head = 192;
	Layer[16].Tail = 195;

	#elif AL99E
	LayerMax = 17;

	Layer[0].Head = 0;
	Layer[0].Tail = 15;

	Layer[1].Head = 16;
	Layer[1].Tail = 30;

	Layer[2].Head = 58;
	Layer[2].Tail = 45;

	Layer[3].Head = 46;
	Layer[3].Tail = 60;

	Layer[4].Head = 61;
	Layer[4].Tail = 75;

	Layer[5].Head = 76;
	Layer[5].Tail = 90;

	Layer[6].Head = 91;
	Layer[6].Tail = 105;

	Layer[7].Head = 106;
	Layer[7].Tail = 120;

	Layer[8].Head = 121;
	Layer[8].Tail = 135;

	Layer[9].Head = 136;
	Layer[9].Tail = 150;

	Layer[10].Head = 151;
	Layer[10].Tail = 165;

	Layer[11].Head = 166;
	Layer[11].Tail = 180;

	Layer[12].Head = 181;
	Layer[12].Tail = 195;

	Layer[13].Head = 196;
	Layer[13].Tail = 210;

	Layer[14].Head = 211;
	Layer[14].Tail = 225;

	Layer[15].Head = 226;
	Layer[15].Tail = 240;

	Layer[16].Head = 241;
	Layer[16].Tail = 244;
	#endif
}




