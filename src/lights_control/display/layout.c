/*************************************************************
  *
  * FileName    layout.c
  * Brief       lay out information
  *
**************************************************************/
#include "display.h"
#include "layout.h"

//gloable paramters
LayerBrief_T		layer_brief;
LayerData_T			vertical_layer[50];
LayerData_T			triangle_layer[50];
LayerData_T			fan_layer[50];



/*
 * FUNCTION NAME	Display_Layout_Enter
 * BRIEF			Enter the layout, set all led to color white
 */
void Display_Layout_Enter(void)
{
	uint16_t i = 0;

	if (display_data.init == true)
	{
		display_data.init = false;

		LayerTest = 0;
		for (i = 0; i < LED_TOTAL; i++)
		{
			LedData[i].DutyR = 0xff;
			LedData[i].DutyG = 0xff;
			LedData[i].DutyB = 0xff;
		}

		//set the head and the tail to 0xffff
		for (i = 0; i < LAYER_MAX; i++)
		{
			LayerTemp[i].Head = 0xffff;
			LayerTemp[i].Tail = 0xffff;
		}
	}
}


/*
 * FUNCTION NAME	Display_Layout_Cancel
 * BRIEF			Exit the layout mode
 */
void Display_Layout_Cancel(void)
{
	if (display_data.init == true)
	{
		display_data.init = false;

		display_data.mode= display_data.mode_buf;
		display_data.init = true;
	}
}


/*
 * FUNCTION NAME 	Display_Layout_Test
 * BRIEF			Layout test mode
 */
void Display_Layout_Test(void)
{
	uint16_t i = 0;
	uint16_t j = 0;
	
	if (display_data.init == true)
	{
		display_data.init = false;

		//
		if (LayerTemp[LayerTest].Head > LayerTemp[LayerTest].Tail)
		{
			j = LayerTemp[LayerTest].Head;
			LayerTemp[LayerTest].Tail = LayerTemp[LayerTest].Head;
			LayerTemp[LayerTest].Head = j;
		}

		//supplement the next layer automatically
		if (LayerTest < LAYER_MAX - 1 && LayerTemp[LayerTest].Tail < (LED_TOTAL - 1))
		{
			LayerTemp[LayerTest+1].Head = LayerTemp[LayerTest].Tail + 1;
			LayerTemp[LayerTest+1].Tail = LED_TOTAL - 1;
		}

		printf("=layout=head:0x%x",LayerTemp[LayerTest].Head);
		printf("=layout=tail:0x%x",LayerTemp[LayerTest].Tail);

		//change the layer color
		j = (uint16_t)(LayerTest % 3);
		for (i = 0; i < LED_TOTAL; i++)
		{
			if (i < LayerTemp[LayerTest].Head)	continue;

			if (i >= LayerTemp[LayerTest].Head && i <= LayerTemp[LayerTest].Tail)
			{
				LedData[i].DutyR = 0;
				LedData[i].DutyG = 0;
				LedData[i].DutyB = 0;
				if (j == 0)
				{
					LedData[i].DutyR = 0xff;
				}
				else if (j == 1)
				{
					LedData[i].DutyG = 0xff;
				}
				else
				{
					LedData[i].DutyB = 0xff;
				}
			}
			else
			{
				LedData[i].DutyR = 0xff;
				LedData[i].DutyG = 0xff;
				LedData[i].DutyB = 0xff;
			}
		}
	}
}


/*
 * FUNCTION NAME	Display_Layout_Save
 * BRIEF			Save the layout data
 */
void Display_Layout_Save(void)
{
	uint16_t i = 0;
	
	if (display_data.init == true)
	{
		display_data.init = false;

		for (i = 0; i < LayerTest; i++)
		{
			Layer[i].Head = LayerTemp[i].Head;
			Layer[i].Tail = LayerTemp[i].Tail;
		}

		LayerMax = LayerTest;
		
		display_data.mode= GREEN_FLASH;
		display_data.init = true;
	}
}


void Display_Layout_Photo_Ctrl(void)
{

}


/**
  * FunctionName  Display_Layout_None_Init
  */
void Display_Layout_None_Init(void)
{
	#if 1
	for (uint16_t i = 0; i < STR_LAYER_MAX; i++)
	{
		Layer[i].Head = i * STR_LAYER_SEC;
		Layer[i].Tail = (i+1) * STR_LAYER_SEC - 1;
	}

	Layer[STR_LAYER_MAX - 1].Tail = LED_TOTAL - 1;

	LayerMax = STR_LAYER_MAX;

	#else
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
	#endif
}






/**
  * FunctionName  Display_Layout_Triangle_Left
  */
void Display_Layout_Triangle_Left(void)
{

}


/**
  * FunctionName  Display_Layout_Triangle_Right
  */
void Display_Layout_Triangle_Right(void)
{

}




