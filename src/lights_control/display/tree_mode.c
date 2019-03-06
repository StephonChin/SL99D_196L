/********************************************************************
  *
  * FileName    tree_mode.c
  * Brief       the mode when the product have been lay out
  *
*********************************************************************/
#include "display.h"

/**
  * FunctionName    Display_str_steady
  */
void Display_Tree_Steady(void)
{
	uint16_t temp = 0;
	
	if(Display.Init == TRUE){	
		Display.Init = FALSE;

		Para_Err_Check(&ParaData[STEADY]);

		BrightLevel = PARA_BRIGHT_MAX - ParaData[STEADY].Bright + 1;

		SpeedCtrl = 0;
		OtherCtrl = 0;
		TempColor = 0;

		for (temp = 0; temp < LED_TOTAL; temp++){
			TempColor = temp % ParaData[STEADY].ColorNum;
			LedData[temp].DutyR = ParaData[STEADY].Color[TempColor].BufR;
			LedData[temp].DutyG = ParaData[STEADY].Color[TempColor].BufG;
			LedData[temp].DutyB = ParaData[STEADY].Color[TempColor].BufB;
		}
	}
}



void Display_Tree_Rainbow(void)
{
	uint16_t temp = 0;  
	uint16_t temp1 = 0;
	uint16_t temp2 = 0;
	uint16_t	i = 0;
	uint16_t	j = 0;

	if(Display.Init == true)
	{	
		Display.Init = false;

		Para_Err_Check(&ParaData[RAINBOW]);

		//BrightLevel = PARA_BRIGHT_MAX - ParaData[RAINBOW].Bright + 1;
		BrightLevel=1;

		SpeedCtrl = 0;
		OtherCtrl = 0;
		TempColor = 0;
		TempStep  = 0;

		TempR = 0;
		TempG = 0;
		TempB = 0;
		TopR = 240;
		TopG = 240;
		TopB = 240;

		switch (ParaData[RAINBOW].Other)
		{
			default:    FadeR = 80;      break;
			case 9:		FadeR = 80;		 break;
			case 8:		FadeR = 60;		 break;
			case 7:		FadeR = 40;		 break;
			case 6:		FadeR = 30;		 break;
			case 5:		FadeR = 24;		 break;
			case 4:		FadeR = 20;		 break;
			case 3:     FadeR = 15;      break;
			case 2:     FadeR = 10;      break;
			case 1:     FadeR = 6;      break;
			case 0:     FadeR = 4;      break;
		}

		FadeG = FadeR;
		FadeB = FadeR;

		Display_All_Set(0,0,0);

		ModeFirstFlag = true;
		return;
	}
  
  
  
  SpeedCtrl++;
  if (SpeedCtrl > ((PARA_SPEED_MAX - ParaData[RAINBOW].Speed) * (uint8_t)(1 - ModeFirstFlag)))
  {
    SpeedCtrl = 0;

	if (ModeFirstFlag)
	{
		ModeFirstFlag = false;
    	j = LayerMax;
    }
    else
    {
		j = 1;
    }

	for (i = 0; i < j; i++)
	{
	    switch (TempStep){
	      case 0:{
	        if (TempR < TopR)    TempR += FadeR;
	        else              TempStep++;
	      } break;
	      
	      case 1:{
	        if (TempG < TopG)    TempG += FadeG;
	        else              TempStep++;
	      } break;
	      
	      case 2:{
	        if (TempR > 0)    TempR -= FadeR;
	        else              TempStep++;
	      } break;
	      
	      case 3:{
	        if (TempB < TopB)    TempB += FadeB;
	        else              TempStep++;
	      } break;
	      
	      case 4:{
	        if (TempG > 0)    TempG -= FadeG;
	        else              TempStep++;
	      } break;
	      
	      case 5:{
	        if (TempR < TopR)    TempR += FadeR;
	        else              TempStep++;
	      } break;
	      
	      case 6:{
	        if (TempB > 0)    TempB -= FadeR;
	        else              TempStep = 1;
	      } break;
	    }
	    
	    
	    for (temp = 0; temp < LayerMax - 1; temp++){
	      temp2 = Layer[temp + 1].Head;
	      for (temp1 = Layer[temp].Head; temp1 <= Layer[temp].Tail; temp1++){
	        LedData[temp1].DutyR = LedData[temp2].DutyR;
	        LedData[temp1].DutyG = LedData[temp2].DutyG;
	        LedData[temp1].DutyB = LedData[temp2].DutyB;
	      }
	    }
	    
	    temp = LayerMax - 1;
	    for (temp1 = Layer[temp].Head; temp1 <= Layer[temp].Tail; temp1++){
	      LedData[temp1].DutyR = TempR;
	      LedData[temp1].DutyG = TempG;
	      LedData[temp1].DutyB = TempB;
	    }
	}
  }
}

void Display_Tree_Fade(void)
{
	uint16_t	i = 0;

	if (Display.Init == TRUE)
	{
		Display.Init = FALSE;

		Para_Err_Check(&ParaData[FADE]);

		//BrightLevel = PARA_BRIGHT_MAX - ParaData[FADE].Bright + 1;
		BrightLevel=1;

		TempColor = 0;
		TempStep = 0;
		SpeedCtrl = 0;
		HoldTime = 0;
		TempR = ParaData[FADE].Color[TempColor].BufR;
		TempG = ParaData[FADE].Color[TempColor].BufG;
		TempB = ParaData[FADE].Color[TempColor].BufB;
		Display_All_Set(TempR, TempG, TempB);

		if (ParaData[FADE].ColorNum == 1)
		{
			TopR = 0;
			TopG = 0;
			TopB = 0;
		}
		else 
		{
			TempColor = 1;
			TopR = ParaData[FADE].Color[TempColor].BufR;
			TopG = ParaData[FADE].Color[TempColor].BufG;
			TopB = ParaData[FADE].Color[TempColor].BufB;
		}

		//Get the fade level
		if (TopR >= TempR)    FadeR = (TopR - TempR) / FADE_LEVEL;
		else                  FadeR = (TempR - TopR) / FADE_LEVEL;

		if (TopG >= TempG)    FadeG = (TopG - TempG) / FADE_LEVEL;
		else                  FadeG = (TempG - TopG) / FADE_LEVEL;

		if (TopB >= TempB)    FadeB = (TopB - TempB) / FADE_LEVEL;
		else                  FadeB = (TempB - TopB) / FADE_LEVEL;

		return;
	}



	SpeedCtrl++;
	if (SpeedCtrl > (PARA_SPEED_MAX - ParaData[FADE].Speed))
	{
		SpeedCtrl = 0;
		switch (TempStep)
		{
			//fade
			case 0:
			{
				if (TempR > TopR)       TempR -= FadeR;
				else if (TopR > TempR)  TempR += FadeR;

				if (TempG > TopG)       TempG -= FadeG;
				else if (TopG > TempG)  TempG += FadeG;

				if (TempB > TopB)       TempB -= FadeB;
				else if (TopB > TempB)  TempB += FadeB;

				if (TempR == TopR && TempG == TopG && TempB == TopB)
				{
					TempStep++;
				}
			} break;

			//hold
			case 1:
			{
				if (ParaData[FADE].ColorNum == 1)
				{
					TempStep++;
				}
				else
				{
					HoldTime++;
					if (HoldTime >= 10)
					{
						HoldTime = 0;
						TempStep++;
					}
				}
			}break;

			default:
			{
				if (ParaData[FADE].ColorNum == 1)
				{
					TopR = ParaData[FADE].Color[0].BufR - TempR;
					TopG = ParaData[FADE].Color[0].BufG - TempG;
					TopB = ParaData[FADE].Color[0].BufB - TempB;
				}
				else
				{
					TempColor++;
					if (TempColor >= ParaData[FADE].ColorNum)   TempColor = 0;
					TopR = ParaData[FADE].Color[TempColor].BufR;
					TopG = ParaData[FADE].Color[TempColor].BufG;
					TopB = ParaData[FADE].Color[TempColor].BufB;
				}

				//Get the fade value again
				if (TopR >= TempR)    FadeR = (TopR - TempR) / FADE_LEVEL;
				else                  FadeR = (TempR - TopR) / FADE_LEVEL;

				if (TopG >= TempG)    FadeG = (TopG - TempG) / FADE_LEVEL;
				else                  FadeG = (TempG - TopG) / FADE_LEVEL;

				if (TopB >= TempB)    FadeB = (TopB - TempB) / FADE_LEVEL;
				else                  FadeB = (TempB - TopB) / FADE_LEVEL;


				TempStep = 0;
			} break;
		}
		Display_All_Set(TempR, TempG, TempB);
	}
}



/**
  * FunctionName  Display_str_sparkle
  */
void Display_Tree_Sparkle(void)
{
	uint16_t  i = 0;

	if (Display.Init == true)
	{
		Display.Init = false;

		Para_Err_Check(&ParaData[SPARKLE]);

		//BrightLevel = PARA_BRIGHT_MAX - ParaData[SPARKLE].Bright + 1;
		BrightLevel=1;
		
		SpeedCtrl = 0;
		OtherCtrl = 0;
		TempColor = 0;

		for (i = 0; i < LED_TOTAL; i++)
		{
			TempColor = i % ParaData[SPARKLE].ColorNum;
			LedData[i].DutyR = ParaData[SPARKLE].Color[TempColor].BufR / 3;
			LedData[i].DutyG = ParaData[SPARKLE].Color[TempColor].BufG / 3;
			LedData[i].DutyB = ParaData[SPARKLE].Color[TempColor].BufB / 3;

			ModeTime[i] = 0;
			LedPick[i] = 0;
		}

		return;
	}

	SpeedCtrl++;
	if (SpeedCtrl > (PARA_SPEED_MAX - ParaData[SPARKLE].Speed))
	{
		SpeedCtrl = 0;

		for (i = 0; i < LED_TOTAL; i++)
		{
			if (LedPick[i] == 1)
			{
				LedPick[i] = 0;
				LedData[i].DutyR /= 3;
				LedData[i].DutyG /= 3;
				LedData[i].DutyB /= 3;
			}
		}

		uint16_t  j = 0;
		uint16_t timeout = LED_TOTAL;
		for (i = 0; i < (ParaData[SPARKLE].Other + 1) * 2; i++)
		{
			timeout = LED_TOTAL * 5;
			do
			{
				timeout--;
				RAND();
				j = (uint16_t)(rand() % LED_TOTAL);
			} while((LedPick[j] == 1) && timeout);

			//if get valid led number
			if (timeout != 0)
			{
				LedPick[j] = 1;
				LedData[j].DutyR *= 3;
				LedData[j].DutyG *= 3;
				LedData[j].DutyB *= 3;
			}
		}
	}
}


/**
  * FunctionName  Display_Tree_Snow
  */
void Display_Tree_Snow(void)
{
	if(Display.Init == true){	
	    Display.Init = false;

	    Para_Err_Check(&ParaData[SNOW]);
	    
	    //BrightLevel = PARA_BRIGHT_MAX - ParaData[SNOW].Bright + 1;
		BrightLevel=1;
		
		SpeedCtrl = 0;
	    OtherCtrl = 0;
	    TempColor = 0;
	    TempStep  = 0;
	    LayerStep = 0;
	    
		TempR = ParaData[SNOW].Color[TempColor].BufR;
	    TempG = ParaData[SNOW].Color[TempColor].BufG;
	    TempB = ParaData[SNOW].Color[TempColor].BufB;
	    
	    TopR = TempR;
	    TopG = TempG;
	    TopB = TempB;
	    
	    FadeR = TopR / FADE_LEVEL;
	    FadeG = TopG / FADE_LEVEL;
	    FadeB = TopB / FADE_LEVEL;
	    
	    
	    Display_All_Set(0,0,0);
	    return;
	}
  
  
	SpeedCtrl++;
	if (SpeedCtrl > (PARA_SPEED_MAX - ParaData[SNOW].Speed))
	{
		SpeedCtrl = 0;

	    switch (TempStep)
	    {
			case 0:
			{
				if (LayerStep < LayerMax)
				{
					TempR = TopR;
					TempG = TopG;
					TempB = TopB;
					LayerStep++;
				}
				else
				{
					TempStep++;
					LayerStep = 0;
				}
			}break;

			case 1:
			{
				if (TempR > 0 || TempG > 0 || TempB > 0)
				{
					TempR -= FadeR;
					TempG -= FadeG;
					TempB -= FadeB;
				}
				else
				{
					TempStep++;
				}
			}break;

			case 2:
			{
				if (LayerStep < LayerMax)
				{
					LayerStep++;
				}
				else
				{
					TempStep++;
				}
			}break;

			default:
			{
				TempStep = 0;
				LayerStep = 0;
				TempColor++;
				if (TempColor >= ParaData[SNOW].ColorNum)
				{
					TempColor = 0;
				}

				TopR = ParaData[SNOW].Color[TempColor].BufR;
				TopG = ParaData[SNOW].Color[TempColor].BufG;
				TopB = ParaData[SNOW].Color[TempColor].BufB;
				FadeR = TopR / FADE_LEVEL;
				FadeG = TopG / FADE_LEVEL;
				FadeB = TopB / FADE_LEVEL;
			}break;
	    }

		uint16_t temp = 0;
		uint16_t temp1 = 0;
		uint16_t temp2 = 0;
		for (temp = 0; temp < LayerMax - 1; temp++){
	      temp2 = Layer[temp + 1].Head;
	      for (temp1 = Layer[temp].Head; temp1 <= Layer[temp].Tail; temp1++){
	        LedData[temp1].DutyR = LedData[temp2].DutyR;
	        LedData[temp1].DutyG = LedData[temp2].DutyG;
	        LedData[temp1].DutyB = LedData[temp2].DutyB;
	      }
	    }
	    
	    temp = LayerMax - 1;
	    for (temp1 = Layer[temp].Head; temp1 <= Layer[temp].Tail; temp1++){
	      LedData[temp1].DutyR = TempR;
	      LedData[temp1].DutyG = TempG;
	      LedData[temp1].DutyB = TempB;
	    }
    }


}


void Display_Tree_Twinkle(void)
{
	uint16_t  i = 0;
	uint16_t  j = 0;
	uint16_t	l = 0;
	uint16_t	m = 0;

	if (Display.Init == true)
	{
		Display.Init = false;
		Para_Err_Check(&ParaData[TWINKLE]);

		//BrightLevel = PARA_BRIGHT_MAX - ParaData[TWINKLE].Bright + 1;
		BrightLevel=1;

		SpeedCtrl = 0;
		OtherCtrl = 0;
		TempColor = 0;

		for (i = 0; i < LED_TOTAL; i++)
		{
			TempColor = i % ParaData[TWINKLE].ColorNum;
			FadeR = ParaData[TWINKLE].Color[TempColor].BufR / FADE_LEVEL;
			FadeG = ParaData[TWINKLE].Color[TempColor].BufG / FADE_LEVEL;
			FadeB = ParaData[TWINKLE].Color[TempColor].BufB / FADE_LEVEL;
			RAND();
			j = (uint8_t)rand() % FADE_LEVEL;
			LedData[i].DutyR = FadeR * j;
			LedData[i].DutyG = FadeG * j;
			LedData[i].DutyB = FadeB * j;
			ModeStep[i] = 0;
			ModeTime[i] = 0;
			LedPick[i] = 0;

			ModeFirstFlag = true;
		}

		return;
	}

	
	if (ModeFirstFlag)
	{
		ModeFirstFlag = false;
		m = 100;
	}
	else
	{
		m = 1;
	}

	for (l = 0; l < m; l++)
	{
		for (i = 0; i < LED_TOTAL; i++)
		{
			TempColor = i % ParaData[TWINKLE].ColorNum;
			FadeR = ParaData[TWINKLE].Color[TempColor].BufR / FADE_LEVEL;
			FadeG = ParaData[TWINKLE].Color[TempColor].BufG / FADE_LEVEL;
			FadeB = ParaData[TWINKLE].Color[TempColor].BufB / FADE_LEVEL;

			//Fade in and fade out
			if (LedPick[i] == 0)
			{
				ModeTime[i]++;
				if (ModeTime[i] >= (i % 3) + 3)
				{
					ModeTime[i] = 0;
					switch (ModeStep[i])
					{
						case 0:
						{
							if (LedData[i].DutyR > 0 || LedData[i].DutyG > 0 || LedData[i].DutyB > 0)
							{
								LedData[i].DutyR -= FadeR;
								LedData[i].DutyG -= FadeG;
								LedData[i].DutyB -= FadeB;
							}
							else
							{
								ModeStep[i]++;
							}
						} break;

						default:
							{
							if (LedData[i].DutyR < (FadeR * FADE_LEVEL) || 
								LedData[i].DutyG < (FadeG * FADE_LEVEL) || 
								LedData[i].DutyB < (FadeB * FADE_LEVEL))
							{
								LedData[i].DutyR += FadeR;
								LedData[i].DutyG += FadeG;
								LedData[i].DutyB += FadeB;
							}
							else
							{
								ModeStep[i] = 0;
							}
						} break;
					}
				}
			}

			//flash
			else
			{
				ModeTime[i]++;
				if (ModeTime[i] >= (i % 5))
				{
					ModeTime[i] = 0;
					switch (ModeStep[i])
					{
						case 0:
						{
							LedData[i].DutyR = FadeR * FADE_LEVEL;
							LedData[i].DutyG = FadeG * FADE_LEVEL;
							LedData[i].DutyB = FadeB * FADE_LEVEL;
							ModeStep[i]++;
						} break;

						default:
						{
							LedData[i].DutyR = 0;
							LedData[i].DutyG = 0;
							LedData[i].DutyB = 0;
							ModeStep[i] = 0;
							LedPick[i] = 0;
						}break;
					}
				}
			}
		}
	}

	//get random leds to flash
	OtherCtrl++;
	if (OtherCtrl >= 20)
	{
		OtherCtrl = 0;

		uint16_t	timeout = LED_TOTAL;
		for (i = 0; i < (ParaData[SPARKLE].Other + 1); i++)
		{
			timeout = LED_TOTAL * 5;
			do
			{
				timeout--;
				RAND();
				j = (uint16_t)(rand() % LED_TOTAL);
			} while((LedPick[j] == 1) && timeout);

			if (timeout != 0)
			{
				LedPick[j] = 1;
				ModeStep[j] = 0;
				ModeTime[j] = 0;
				TempColor = j % ParaData[TWINKLE].ColorNum;
				LedData[j].DutyR = ParaData[TWINKLE].Color[TempColor].BufR;
				LedData[j].DutyG = ParaData[TWINKLE].Color[TempColor].BufG;
				LedData[j].DutyB = ParaData[TWINKLE].Color[TempColor].BufB;
			}
		}
	}
}


void Display_Tree_Fireworks(void)
{
  uint16_t i = 0, j = 0;
  
  if (Display.Init == true){
    Display.Init = false;
    Para_Err_Check(&ParaData[FIREWORKS]);
  
    //BrightLevel = PARA_BRIGHT_MAX - ParaData[FIREWORKS].Bright + 1;
    BrightLevel=1;

    SpeedCtrl = 0;
    TempStep = 0;
    TempColor = 0;
    LayerStep = 0;
    RptCtrl = 0;
    FadeLevel = 0;
    Display_All_Set(0, 0, 0);
    return;
  }

  
  switch (TempStep){
    //turn on from bottom to top layer
    case 0:{
      SpeedCtrl++;
      if (SpeedCtrl > PARA_SPEED_MAX - ParaData[FIREWORKS].Speed){
        SpeedCtrl = 0;
        for (i = Layer[LayerStep].Head; i <= Layer[LayerStep].Tail; i++){
          LedData[i].DutyR = ParaData[FIREWORKS].Color[TempColor].BufR;
          LedData[i].DutyG = ParaData[FIREWORKS].Color[TempColor].BufG;
          LedData[i].DutyB = ParaData[FIREWORKS].Color[TempColor].BufB;
        }

        LayerStep++;
        if (LayerStep >= LayerMax){
          TempStep++;
          FadeR = ParaData[FIREWORKS].Color[TempColor].BufR / FADE_LEVEL;
          FadeG = ParaData[FIREWORKS].Color[TempColor].BufG / FADE_LEVEL;
          FadeB = ParaData[FIREWORKS].Color[TempColor].BufB / FADE_LEVEL;
          FadeLevel = FADE_LEVEL;
        }
      }
    }break;

    //fade out to 1/2
    case 1:{
      if (FadeLevel > FADE_LEVEL / 2){
        FadeLevel--;
        for (i = 0; i < LED_TOTAL; i++){
          LedData[i].DutyR = FadeR * FadeLevel;
          LedData[i].DutyG = FadeG * FadeLevel;
          LedData[i].DutyB = FadeB * FadeLevel;
        }
      }
      else    TempStep++;
    } break;

    //to maximum birght and all color 
    case 2:
	{
      for (i = 0; i < LED_TOTAL; i++)
	  {
        j = i % ParaData[FIREWORKS].ColorNum;
        LedData[i].DutyR = ParaData[FIREWORKS].Color[j].BufR;
        LedData[i].DutyG = ParaData[FIREWORKS].Color[j].BufG;
        LedData[i].DutyB = ParaData[FIREWORKS].Color[j].BufB;
        LedPick[i] = 1;
        ModeTime[i] = 0;
      }
      
      TempStep++;
      HoldTime = 0;
    }break;

    //hold for 2 seconds
    case 3:
	{
      HoldTime++;
      if (HoldTime >= 40)
	  {
        HoldTime = 0;
        TempStep++;
        LedPickAll = 0;
      }
    } break;

    //fade out and flash
    case 4:
	{
      //fade out
      for (i = 0; i < LED_TOTAL; i++)
	  {
        if (LedPick[i] == 0)    continue;     //if the led has been fade off, exit this circle
        
        RAND();
        ModeTime[i]++;
        if (ModeTime[i] >= ((uint8_t)rand() % 6))
		{
          ModeTime[i] = 0;

          j = i % ParaData[FIREWORKS].ColorNum;
          TempR = ParaData[FIREWORKS].Color[j].BufR;
          TempG = ParaData[FIREWORKS].Color[j].BufG;
          TempB = ParaData[FIREWORKS].Color[j].BufB;
          FadeR = TempR / FADE_LEVEL;
          FadeG = TempG / FADE_LEVEL;
          FadeB = TempB / FADE_LEVEL;
          if (LedData[i].DutyR > 0 || LedData[i].DutyG > 0 || LedData[i].DutyB > 0){
            LedData[i].DutyR -= FadeR;
            LedData[i].DutyG -= FadeG;
            LedData[i].DutyB -= FadeB;
          }
          else{
            LedPick[i] = 0;
            LedPickAll++;
            if (LedPickAll >= LED_TOTAL)   TempStep++;      //all leds are off
          }
        }
      }
    }break;

    //change color and repeat
    default:{
      TempColor++;
      if (TempColor >= ParaData[FIREWORKS].ColorNum)    TempColor = 0;
      TempStep = 0;
      LayerStep = 0;
    }break;
  }
}


void Display_Tree_Rolling(void)
{
  uint16_t temp = 0;  
  uint16_t temp1 = 0;
  uint16_t temp2 = 0;
  uint16_t i = 0;
  uint16_t j = 0;
  
	if(Display.Init == true){	
    Display.Init = false;
    Para_Err_Check(&ParaData[ROLLING]);
    //BrightLevel = PARA_BRIGHT_MAX - ParaData[ROLLING].Bright + 1;
    BrightLevel=1;
    
	SpeedCtrl = 0;
    OtherCtrl = 0;
    TempColor = 0;
    TempStep  = 0;
    RptCtrl   = 0;
    
	TempR = ParaData[ROLLING].Color[TempColor].BufR;
    TempG = ParaData[ROLLING].Color[TempColor].BufG;
    TempB = ParaData[ROLLING].Color[TempColor].BufB;

    RptTotal = PARA_OTHER_MAX - ParaData[ROLLING].Other + 3; 
    
    Display_All_Set(0,0,0);

    ModeFirstFlag = true;
    
    return;
	}
  
  
  SpeedCtrl++;
  if (SpeedCtrl > (PARA_SPEED_MAX - ParaData[ROLLING].Speed) * (uint8_t)(1 - ModeFirstFlag))
  {
    SpeedCtrl = 0;

    if (ModeFirstFlag)
    {
		ModeFirstFlag = false;
		j = LayerMax;
    }
    else
    {
		j = 1;
    }

    for (i = 0; i < j; i++)
    {
	    switch (TempStep){
	      case 0:{
	        RptCtrl++;
	        if (RptCtrl >= RptTotal - 1){
	          RptCtrl = 0;
	          TempStep++;
	        }
	      }break;
	      
	      //Check the count of color, if the mode has only one color, fill it with dark
	      case 1:{
	        if (ParaData[ROLLING].ColorNum == 1)   TempStep = 2;
	        else                                      TempStep = 3;
	      } break;
	      
	      //dark
	      case 2:{
	        TempR = ParaData[ROLLING].Color[0].BufR / 4;
          	TempG = ParaData[ROLLING].Color[0].BufG / 4;
          	TempB = ParaData[ROLLING].Color[0].BufB / 4;
	        RptCtrl++;
	        if (RptCtrl > RptTotal){
	          RptCtrl = 0;
	          TempStep = 0;
	          TempR = ParaData[ROLLING].Color[0].BufR;
	          TempG = ParaData[ROLLING].Color[0].BufG;
	          TempB = ParaData[ROLLING].Color[0].BufB;
	        }
	      } break;
	      
	      //change color
	      case 3:{
	        TempColor++;
	        if (TempColor >= ParaData[ROLLING].ColorNum)   TempColor = 0;
	        TempR = ParaData[ROLLING].Color[TempColor].BufR;
	        TempG = ParaData[ROLLING].Color[TempColor].BufG;
	        TempB = ParaData[ROLLING].Color[TempColor].BufB;
	        TempStep = 0;
	      } break;
	    }
	    
	    
	    for (temp = 0; temp < LayerMax - 1; temp++){
	      temp2 = Layer[temp + 1].Head;
	      for (temp1 = Layer[temp].Head; temp1 <= Layer[temp].Tail; temp1++){
	        LedData[temp1].DutyR = LedData[temp2].DutyR;
	        LedData[temp1].DutyG = LedData[temp2].DutyG;
	        LedData[temp1].DutyB = LedData[temp2].DutyB;
	      }
	    }
	    
	    temp = LayerMax - 1;
	    for (temp1 = Layer[temp].Head; temp1 <= Layer[temp].Tail; temp1++){
	      LedData[temp1].DutyR = TempR;
	      LedData[temp1].DutyG = TempG;
	      LedData[temp1].DutyB = TempB;
	    }
	}
  }
}

void Display_Tree_Waves(void)
{
	#if 0
	uint16 i = 0, j = 0, k = 0;
	uint16_t l = 0;
	uint16_t m = 0;

	if(Display.Init == true){	
		Display.Init = false;

		Para_Err_Check(&ParaData[WAVES]);

		//BrightLevel = PARA_BRIGHT_MAX - ParaData[WAVES].Bright + 1;
		BrightLevel=1;

		SpeedCtrl 	= 0;
		OtherCtrl 	= 0;
		TempColor 	= 0;
		TempStep  	= 0;
		RptCtrl 	= 0;
		TempColor 	= 0;

		TopR = ParaData[WAVES].Color[TempColor].BufR;
		TopG = ParaData[WAVES].Color[TempColor].BufG;
		TopB = ParaData[WAVES].Color[TempColor].BufB;
		#if 0
		TempR = 0;
		TempG = 0;
		TempB = 0;
		#else
		TempR = TopR;
		TempG = TopG;
		TempB = TopB;
		#endif

		if (TopR >= TopG && TopR >= TopB){
			FadeR = 0;
			FadeG = TopG / (ParaData[WAVES].Other + 1);
			FadeB = TopB / (ParaData[WAVES].Other + 1);
			TempR = TopR;
		}
		else if (TopG >= TopB && TopG >= TopR){
			FadeG = 0;
			FadeR = TopR / (ParaData[WAVES].Other + 1);
			FadeB = TopB / (ParaData[WAVES].Other + 1);
			TempG = TopG;
		}
		else if (TopB >= TopR && TopB >= TopG){
			FadeB = 0;
			FadeR = TopR / (ParaData[WAVES].Other + 1);
			FadeG = TopG / (ParaData[WAVES].Other + 1);
			TempB = TopB;
		}
		

		Display_All_Set(0,0,0);

		ModeFirstFlag = true;

		return;
	}
  

	SpeedCtrl++;
	if (SpeedCtrl > (PARA_SPEED_MAX - ParaData[WAVES].Speed) * (uint8_t)(1 - ModeFirstFlag))
	{
		SpeedCtrl = 0;

		if (ModeFirstFlag)
		{
			ModeFirstFlag = false;
			m = LayerMax;
		}
		else
		{
			m = 1;
		}

		for (l = 0; l < m; l++)
		{
			switch (TempStep){
				#if 0
				case 0:{
					if (FadeR != 0){
						if (TempR < TopR - FadeR){
							TempR += FadeR;
						}
						else{
							TempR = TopR;
							TempStep++;
						}
					}
					else	TempStep++;
				}break;

				case 1:{
					if (FadeG != 0){
						if (TempG < TopG - FadeG){
							TempG += FadeG;
						}
						else{
							TempG = TopG;
							TempStep++;
						}
					}
					else	TempStep++;
				}break;

				case 2:{
					if (FadeB != 0){
						if (TempB < TopB - FadeB){
							TempB += FadeB;
						}
						else{
							TempB = TopB;
							TempStep++;
						}
					}
					else	TempStep++;
				}break;

				case 3:{
					if (FadeR == 0)		TempStep++;
					else{
						if (TempR > FadeR)		TempR -= FadeR;
						else{
							TempR = 0;
							TempStep++;
						}
					}
				} break;

				case 4:{
					if (FadeG == 0)		TempStep++;
					else{
						if (TempG > FadeG)		TempG -= FadeG;
						else{
							TempG = 0;
							TempStep++;
						}
					}
				}break;

				case 5:{
					if (FadeB == 0)	TempStep++;
					else{
						if (TempB > FadeB)		TempB -= FadeB;
						else{
							TempB = 0;
							TempStep++;
						}
					}
				}break;

				case 6:{
					FadeR = TempR / FADE_LEVEL;
					FadeG = TempG / FADE_LEVEL;
					FadeB = TempB / FADE_LEVEL;
					TempStep++;
				}break;

				case 7:{
					for (uint8_t m = 0; m <= (PARA_OTHER_MAX - ParaData[WAVES].Other + 2); m++){
						if (TempR > 0 || TempG > 0 || TempB > 0){
							TempR -= FadeR;
							TempG -= FadeG;
							TempB -= FadeB;
						}
						else	TempStep++;
					}
				}break;

				default:{
					TempColor++;
					if (TempColor >= ParaData[WAVES].ColorNum)	TempColor = 0;
					TopR = ParaData[WAVES].Color[TempColor].BufR;
					TopG = ParaData[WAVES].Color[TempColor].BufG;
					TopB = ParaData[WAVES].Color[TempColor].BufB;
					TempR = 0;
					TempG = 0;
					TempB = 0;

					if (TopR >= TopG && TopR >= TopB){
						FadeR = 0;
						FadeG = TopG / (ParaData[WAVES].Other + 1);
						FadeB = TopB / (ParaData[WAVES].Other + 1);
						TempR = TopR;
					}
					else if (TopG >= TopB && TopG >= TopR){
						FadeG = 0;
						FadeR = TopR / (ParaData[WAVES].Other + 1);
						FadeB = TopB / (ParaData[WAVES].Other + 1);
						TempG = TopG;
					}
					else if (TopB >= TopR && TopB >= TopG){
						FadeB = 0;
						FadeR = TopR / (ParaData[WAVES].Other + 1);
						FadeG = TopG / (ParaData[WAVES].Other + 1);
						TempB = TopB;
					}
					TempStep = 0;
				}break;
				#else

				case 0:{
					if (FadeR == 0)		TempStep++;
					else{
						if (TempR > FadeR)		TempR -= FadeR;
						else{
							TempR = 0;
							TempStep++;
						}
					}
				} break;

				case 1:{
					if (FadeG == 0)		TempStep++;
					else{
						if (TempG > FadeG)		TempG -= FadeG;
						else{
							TempG = 0;
							TempStep++;
						}
					}
				}break;

				case 2:{
					if (FadeB == 0)	TempStep++;
					else{
						if (TempB > FadeB)		TempB -= FadeB;
						else{
							TempB = 0;
							TempStep++;
						}
					}
				}break;

				case 3:{
					FadeR = TempR / FADE_LEVEL;
					FadeG = TempG / FADE_LEVEL;
					FadeB = TempB / FADE_LEVEL;
					TempStep++;
				}break;

				case 4:{
					for (uint8_t m = 0; m <= (PARA_OTHER_MAX - ParaData[WAVES].Other + 2); m++){
						if (TempR > 0 || TempG > 0 || TempB > 0){
							TempR -= FadeR;
							TempG -= FadeG;
							TempB -= FadeB;
						}
						else	TempStep++;
					}
				}break;

				default:
				{
					TempColor++;
					if (TempColor >= ParaData[WAVES].ColorNum)	TempColor = 0;
					TopR = ParaData[WAVES].Color[TempColor].BufR;
					TopG = ParaData[WAVES].Color[TempColor].BufG;
					TopB = ParaData[WAVES].Color[TempColor].BufB;
					TempR = 0;
					TempG = 0;
					TempB = 0;

					if (TopR >= TopG && TopR >= TopB){
						FadeR = 0;
						FadeG = TopG / (ParaData[WAVES].Other + 1);
						FadeB = TopB / (ParaData[WAVES].Other + 1);
						TempR = TopR;
					}
					else if (TopG >= TopB && TopG >= TopR){
						FadeG = 0;
						FadeR = TopR / (ParaData[WAVES].Other + 1);
						FadeB = TopB / (ParaData[WAVES].Other + 1);
						TempG = TopG;
					}
					else if (TopB >= TopR && TopB >= TopG){
						FadeB = 0;
						FadeR = TopR / (ParaData[WAVES].Other + 1);
						FadeG = TopG / (ParaData[WAVES].Other + 1);
						TempB = TopB;
					}
					TempStep = 0;
				}break;

				#endif
			}
			

			for (i = 0; i < LayerMax - 1; i++){
				k = Layer[i + 1].Head;
				for (j = Layer[i].Head; j <= Layer[i].Tail; j++){
					LedData[j].DutyR = LedData[k].DutyR;
					LedData[j].DutyG = LedData[k].DutyG;
					LedData[j].DutyB = LedData[k].DutyB;
				}
			}

			i = LayerMax - 1;
			for (j = Layer[i].Head; j <= Layer[i].Tail; j++){
				LedData[j].DutyR = TempR;
				LedData[j].DutyG = TempG;
				LedData[j].DutyB = TempB;
			}
		}
	}
	#else
	uint16_t	i = 0;
	uint16_t	j = 0;
	uint16_t	k = 0;
	uint16_t	l = 0;
	uint16_t	m = 0;

	if (Display.Init == TRUE)
	{
		Display.Init = FALSE;

		Para_Err_Check(&ParaData[WAVES]);

		//BrightLevel = PARA_BRIGHT_MAX - ParaData[WAVES].Bright + 1;
		BrightLevel=1;

		TempColor = 0;
		TempStep = 0;
		SpeedCtrl = 0;
		HoldTime = 0;
		TempR = ParaData[WAVES].Color[TempColor].BufR;
		TempG = ParaData[WAVES].Color[TempColor].BufG;
		TempB = ParaData[WAVES].Color[TempColor].BufB;
		Display_All_Set(TempR, TempG, TempB);

		if (ParaData[WAVES].ColorNum == 1)
		{
			TopR = 0;
			TopG = 0;
			TopB = 0;
		}
		else 
		{
			TempColor = 1;
			TopR = ParaData[WAVES].Color[TempColor].BufR;
			TopG = ParaData[WAVES].Color[TempColor].BufG;
			TopB = ParaData[WAVES].Color[TempColor].BufB;
		}

		//Get the fade level
		if (TopR >= TempR)    FadeR = (TopR - TempR) / FADE_LEVEL;
		else                  FadeR = (TempR - TopR) / FADE_LEVEL;

		if (TopG >= TempG)    FadeG = (TopG - TempG) / FADE_LEVEL;
		else                  FadeG = (TempG - TopG) / FADE_LEVEL;

		if (TopB >= TempB)    FadeB = (TopB - TempB) / FADE_LEVEL;
		else                  FadeB = (TempB - TopB) / FADE_LEVEL;

		ModeFirstFlag = true;

		return;
	}



	SpeedCtrl++;
	if (SpeedCtrl > (PARA_SPEED_MAX - ParaData[WAVES].Speed) * (uint8_t)(1 - ModeFirstFlag))
	{
		SpeedCtrl = 0;

		if (ModeFirstFlag)
		{
			ModeFirstFlag = false;
			m = LayerMax;
		}
		else
		{
			m = 1;
		}

		for (l = 0; l < m; l++)
		{
			switch (TempStep)
			{
				//fade
				case 0:
				{
					if (TempR > TopR)       TempR -= FadeR;
					else if (TopR > TempR)  TempR += FadeR;

					if (TempG > TopG)       TempG -= FadeG;
					else if (TopG > TempG)  TempG += FadeG;

					if (TempB > TopB)       TempB -= FadeB;
					else if (TopB > TempB)  TempB += FadeB;

					if (TempR == TopR && TempG == TopG && TempB == TopB)
					{
						TempStep++;
					}
				} break;


				default:
				{
					if (ParaData[WAVES].ColorNum == 1)
					{
						TopR = ParaData[WAVES].Color[0].BufR - TempR;
						TopG = ParaData[WAVES].Color[0].BufG - TempG;
						TopB = ParaData[WAVES].Color[0].BufB - TempB;
					}
					else
					{
						TempColor++;
						if (TempColor >= ParaData[WAVES].ColorNum)   TempColor = 0;
						TopR = ParaData[WAVES].Color[TempColor].BufR;
						TopG = ParaData[WAVES].Color[TempColor].BufG;
						TopB = ParaData[WAVES].Color[TempColor].BufB;
					}

					//Get the fade value again
					if (TopR >= TempR)    FadeR = (TopR - TempR) / FADE_LEVEL;
					else                  FadeR = (TempR - TopR) / FADE_LEVEL;

					if (TopG >= TempG)    FadeG = (TopG - TempG) / FADE_LEVEL;
					else                  FadeG = (TempG - TopG) / FADE_LEVEL;

					if (TopB >= TempB)    FadeB = (TopB - TempB) / FADE_LEVEL;
					else                  FadeB = (TempB - TopB) / FADE_LEVEL;


					TempStep = 0;
				} break;
			}

			for (i = 0; i < LayerMax - 1; i++)
			{
				k = Layer[i + 1].Head;
				for (j = Layer[i].Head; j <= Layer[i].Tail; j++){
					LedData[j].DutyR = LedData[k].DutyR;
					LedData[j].DutyG = LedData[k].DutyG;
					LedData[j].DutyB = LedData[k].DutyB;
				}
			}

			i = LayerMax - 1;
			for (j = Layer[i].Head; j <= Layer[i].Tail; j++){
				LedData[j].DutyR = TempR;
				LedData[j].DutyG = TempG;
				LedData[j].DutyB = TempB;
			}
		}
	}

	#endif
}

//if the layermax > FADE_LEVEL, this mode need to fixed
void Display_Tree_Updwn(void)
{
	uint16 i = 0, j = 0, k = 0;

	if(Display.Init == true){	
		Display.Init = false;

		Para_Err_Check(&ParaData[UPDWN]);

		//BrightLevel = PARA_BRIGHT_MAX - ParaData[UPDWN].Bright + 1;
		BrightLevel=1;

		SpeedCtrl 	= 0;
		OtherCtrl 	= 0;
		TempColor 	= 0;
		TempStep  	= 0;
		LayerStep 	= 0;
		RptCtrl 	= 0;
		TempColor 	= 0;
		TempColor1 = 1;
		if (TempColor1 >= ParaData[UPDWN].ColorNum)	TempColor1 = 0;
		
		FadeR = ParaData[UPDWN].Color[TempColor].BufR / FADE_LEVEL;
		FadeG = ParaData[UPDWN].Color[TempColor].BufG / FADE_LEVEL;
		FadeB = ParaData[UPDWN].Color[TempColor].BufB / FADE_LEVEL;
		FadeR1 = ParaData[UPDWN].Color[TempColor1].BufR / FADE_LEVEL;
		FadeG1 = ParaData[UPDWN].Color[TempColor1].BufG / FADE_LEVEL;
		FadeB1 = ParaData[UPDWN].Color[TempColor1].BufB / FADE_LEVEL;

		TempR = 0;
		TempG = 0;
		TempB = 0;

		TempR1 = 0;
		TempG1 = 0;
		TempB1 = 0;

		Display_All_Set(0,0,0);

		return;
	}

	SpeedCtrl++;
	if (SpeedCtrl >= 2){
		SpeedCtrl = 0;

		switch (TempStep){
			case 0:{
				LayerStep++;
				if (LayerStep > LayerMax + FADE_LEVEL)	TempStep++;

				for (i = 0; i < LayerMax; i++)
				{
					if (LayerStep < FADE_LEVEL)
					{
						//from bottom to top
						if (i > LayerStep)
						{
							TempR = 0;
							TempG = 0;
							TempB = 0;
						}
						else
						{
							j = FADE_LEVEL - LayerStep + i;
							TempR = FadeR * j;
							TempG = FadeG * j;
							TempB = FadeB * j;
						}

						//from top to bottom
						if ((LayerMax - 1 - i) > LayerStep)
						{
							TempR1 = 0;
							TempG1 = 0;
							TempB1 = 0;
						}
						else
						{
							j = FADE_LEVEL - LayerStep + LayerMax - 1 - i;
							TempR1 = FadeR1 * j;
							TempG1 = FadeG1 * j;
							TempB1 = FadeB1 * j;
						}
					}
					else{

						if (LayerStep < LayerMax)
						{
							//from bottom to top
							if ((i < (LayerStep - FADE_LEVEL)) || (i > LayerStep)){
								TempR = 0;
								TempG = 0;
								TempB = 0;
							}
							else{
								j = i - (LayerStep - FADE_LEVEL);
								TempR = FadeR * j;
								TempG = FadeG * j;
								TempB = FadeB * j;
							}

							//from top to bottom
							if (((LayerMax - 1 - i) < (LayerStep - FADE_LEVEL)) || (LayerMax - 1 - i > LayerStep)){
								TempR1 = 0;
								TempG1 = 0;
								TempB1 = 0;
							}
							else{
								j = LayerMax - 1 - i - (LayerStep - FADE_LEVEL);
								TempR1 = FadeR1 * j;
								TempG1 = FadeG1 * j;
								TempB1 = FadeB1 * j;
							}
						}

						else
						{
							//from bottom to top
							if (i < (LayerStep - FADE_LEVEL)){
								TempR = 0;
								TempG = 0;
								TempB = 0;
							}
							else{
								j = i - (LayerStep - FADE_LEVEL);
								TempR = FadeR * j;
								TempG = FadeG * j;
								TempB = FadeB * j;
							}

							//from top to bottom
							if ((LayerMax - 1 - i) < (LayerStep - FADE_LEVEL)){
								TempR1 = 0;
								TempG1 = 0;
								TempB1 = 0;
							}
							else{
								j = LayerMax - 1 - i - (LayerStep - FADE_LEVEL);
								TempR1 = FadeR1 * j;
								TempG1 = FadeG1 * j;
								TempB1 = FadeB1 * j;
							}
						}
					}

					//get the layer color value
					k = (uint16_t)TempR + (uint16_t)TempR1;
					if (k > 255)	k = 255;
					for (j = Layer[i].Head; j <= Layer[i].Tail; j++)	LedData[j].DutyR = (uint8_t)k;
					k = (uint16_t)TempG + (uint16_t)TempG1;
					if (k > 255)	k = 255;
					for (j = Layer[i].Head; j <= Layer[i].Tail; j++)	LedData[j].DutyG = (uint8_t)k;
					k = (uint16_t)TempB + (uint16_t)TempB1;
					if (k > 255)	k = 255;
					for (j = Layer[i].Head; j <= Layer[i].Tail; j++)	LedData[j].DutyB = (uint8_t)k;
				}
			}break;

			//change the color
			default:{
				TempColor++;
				if (TempColor >= ParaData[UPDWN].ColorNum)	TempColor = 0;
				TempColor1++;
				if (TempColor1 >= ParaData[UPDWN].ColorNum) TempColor1 = 0;
				FadeR = ParaData[UPDWN].Color[TempColor].BufR / FADE_LEVEL;
				FadeG = ParaData[UPDWN].Color[TempColor].BufG / FADE_LEVEL;
				FadeB = ParaData[UPDWN].Color[TempColor].BufB / FADE_LEVEL;
				FadeR1 = ParaData[UPDWN].Color[TempColor1].BufR / FADE_LEVEL;
				FadeG1 = ParaData[UPDWN].Color[TempColor1].BufG / FADE_LEVEL;
				FadeB1 = ParaData[UPDWN].Color[TempColor1].BufB / FADE_LEVEL;
				TempStep = 0;
				LayerStep = 0;
				TempR = 0;
				TempG = 0;
				TempB = 0;
				TempR1 = 0;
				TempG1 = 0;
				TempB1 = 0;
			}break;
		}
	}
}

void Display_Tree_Glow(void)
{
	uint16_t	i = 0;
  	uint16_t	j = 0;
  	uint16_t	l = 0;
  	uint16_t	m = 0;
  
	if(Display.Init == true)
	{	
	    Display.Init = false;

	    Para_Err_Check(&ParaData[GLOW]);
	    
	    //BrightLevel = PARA_BRIGHT_MAX - ParaData[GLOW].Bright + 1;
	    BrightLevel=1;
	    
		SpeedCtrl = 0;
	    OtherCtrl = 0;
	    TempColor = 0;
	    TempStep  = 0;
	    
		TempR = 0;
	    TempG = 0;
	    TempB = 0;

	    for ( i = 0; i < LED_TOTAL; i++)
	    {
			ModeStep[i] = 0;
			RAND();
			ModeTime[i] = (uint8_t)(rand() % 50);
	    }

	    for (i = 0; i < LED_TOTAL; i++)
	    {
			j = i % ParaData[GLOW].ColorNum;
			LedData[i].DutyR = ParaData[GLOW].Color[j].BufR;
			LedData[i].DutyG = ParaData[GLOW].Color[j].BufG;
			LedData[i].DutyB = ParaData[GLOW].Color[j].BufB;
	    }

	    ModeFirstFlag = true;
	    return;
	}
  
  	SpeedCtrl++;
  	if (SpeedCtrl > (PARA_SPEED_MAX - ParaData[GLOW].Speed))
  	{
		if (ModeFirstFlag)
		{
			ModeFirstFlag = false;
			m = 1;
		}
		else
		{
			m = 1;
		}
			

		for (l = 0; l < m; l++)
		{
			for (i = 0; i < LED_TOTAL; i++)
			{
				if (ModeTime[i])
				{
					ModeTime[i]--;
					continue;
				}

				j = i % ParaData[GLOW].ColorNum;
				TopR = ParaData[GLOW].Color[j].BufR;
				TopG = ParaData[GLOW].Color[j].BufG;
				TopB = ParaData[GLOW].Color[j].BufB;
				FadeR = TopR / FADE_LEVEL;
				FadeG = TopG / FADE_LEVEL;
				FadeB = TopB / FADE_LEVEL;
				
				switch (ModeStep[i])
				{
			      	case 0:
			      	{
				        if (LedData[i].DutyR < TopR || LedData[i].DutyG < TopG || LedData[i].DutyB < TopB)
				        {
				        	LedData[i].DutyR += FadeR;
							LedData[i].DutyG += FadeG;
							LedData[i].DutyB += FadeB;
				        }
				        else
				        {
				        	ModeStep[i]++;
				        }
				     } break;
				      
				      default:
				      {
				        if (LedData[i].DutyR > 0 || LedData[i].DutyG > 0 || LedData[i].DutyB > 0)
				        {
							LedData[i].DutyR -= FadeR;
							LedData[i].DutyG -= FadeG;
							LedData[i].DutyB -= FadeB;
				        }
				        else
				        {
				        	ModeStep[i] = 0;
				        }
				      } break;
			    }
			}
		}
	}
}
void Display_Tree_Color_Rand(void)
{
  uint16_t    i = 0, j = 0, k = 0;
  uint16_t	l = 0;
  uint16_t	m = 0;
  
  if (Display.Init == true){
    Display.Init  = false;
    Para_Err_Check(&ParaData[COLOR_RAND]);
    //BrightLevel = PARA_BRIGHT_MAX - ParaData[COLOR_RAND].Bright + 1;
    BrightLevel=1;

    SpeedCtrl = 0;
    OtherCtrl = ParaData[COLOR_RAND].Other;

    TempR = 0;
    TempG = 0;
    TempB = 0;

    HoldTime = 0;
    Display_All_Set(0,0,0); 

	ModeFirstFlag = true;
    
    return;
	}
  
  HoldTime++;
  if (HoldTime > OtherCtrl * 20){
    HoldTime = 0;
    TempStep++;
    if (TempStep > 5)   TempStep = 0;
  }
    
  SpeedCtrl++;
  if (SpeedCtrl > (PARA_SPEED_MAX - ParaData[COLOR_RAND].Speed) * (uint8_t)(1 - ModeFirstFlag))
  {
    SpeedCtrl = 0;

    if (ModeFirstFlag)
    {
		ModeFirstFlag = false;
		m = LayerMax;
    }
    else
    {
		m = 1;
    }

    for (l = 0; l < m; l++)
    {
	    switch (TempStep){
	      case 0:        TempB = 0; TempR += 12; TempG -= 7;	break;
	      case 1:        TempG = 0; TempB -= 15; TempR += 9;	break;
	      case 2:        TempR = 0; TempG += 7; TempB -= 16;	break;
	      case 3:        TempB = 0; TempR -= 13; TempG += 8;	break;
	      case 4:        TempG = 0; TempB += 10; TempR -= 7;	break;
	      default:       TempR = 0; TempG -= 8; TempB += 11;	break;
	    }
	    
	    
	    for (i = 0; i < LayerMax - 1; i++){
	      j = Layer[i + 1].Head;
	      for (k = Layer[i].Head; k <= Layer[i].Tail; k++){
	        LedData[k].DutyR = LedData[j].DutyR;
	        LedData[k].DutyG = LedData[j].DutyG;
	        LedData[k].DutyB = LedData[j].DutyB;
	      }
	    }
	    
	    i = LayerMax - 1;
	    for (k = Layer[i].Head; k <= Layer[i].Tail; k++){
	      LedData[k].DutyR = TempR;
	      LedData[k].DutyG = TempG;
	      LedData[k].DutyB = TempB;
	    }
	}
  }
}


void Display_Tree_Instead(void)
{
	uint16_t i = 0;
	uint16_t j = 0;
	uint16_t k = 0;

	if (Display.Init == true)
	{
		Display.Init = false;
		Para_Err_Check(&ParaData[INSTEAD]);

		//BrightLevel = PARA_BRIGHT_MAX - ParaData[INSTEAD].Bright + 1;
		BrightLevel=1;

		ModeStep[0] = 0;
		LedPickAll	= 0;
		TempColor	= 0;

		for (i = 0; i < LED_TOTAL; i++)
		{
			LedPick[i] = 0;
		}


		for (i = 0; i < LED_TOTAL; i++)
		{
			LedData[i].DutyR = 0;
			LedData[i].DutyG = 0;
			LedData[i].DutyB = 0;
		}

		SpeedCtrl = 0;


		FadeR = ParaData[INSTEAD].Color[0].BufR / FADE_LEVEL;
		FadeG = ParaData[INSTEAD].Color[0].BufG / FADE_LEVEL;
		FadeB = ParaData[INSTEAD].Color[0].BufB / FADE_LEVEL;
		return;
	}


	/* Mode Control */
	for (k = 0; k < ParaData[INSTEAD].Speed + 1; k++)
	{
		switch (ParaData[INSTEAD].ColorNum)
		{
			case 1:
			{
				switch (ModeStep[0])
				{
					case 0:
					{
						if (LedPickAll < LED_TOTAL)
						{
							do
							{
								RAND();
								j = (uint16_t)rand() % LED_TOTAL;
							}while(	LedPick[j] == 1);
							LedPick[j] = 1;
							LedData[j].DutyR = ParaData[INSTEAD].Color[TempColor].BufR;
							LedData[j].DutyG = ParaData[INSTEAD].Color[TempColor].BufG;
							LedData[j].DutyB = ParaData[INSTEAD].Color[TempColor].BufB;
							LedPickAll++;
						}
						else
						{
							LedPickAll = 0;
							ModeStep[0]++;

							for (i = 0; i < LED_TOTAL; i++)
							{
								LedPick[i] = 0;
							}
						}
					} break;

					default:
					{
						if (LedData[0].DutyR > 0 || LedData[0].DutyG > 0 || LedData[0].DutyB > 0)
						{
							for (i = 0; i < LED_TOTAL; i++)
							{
								LedData[i].DutyR -= FadeR;
								LedData[i].DutyG -= FadeG;
								LedData[i].DutyB -= FadeB;
							}
						}
						else
						{
							ModeStep[0] = 0;
						}
					} break;
				}
			} break;


			default:
			{
				if (LedPickAll < LED_TOTAL)
				{
					do
					{
						RAND();
						j = (uint16_t)rand() % LED_TOTAL;
					}while(	LedPick[j] == 1);
					LedPick[j] = 1;
					LedData[j].DutyR = ParaData[INSTEAD].Color[TempColor].BufR;
					LedData[j].DutyG = ParaData[INSTEAD].Color[TempColor].BufG;
					LedData[j].DutyB = ParaData[INSTEAD].Color[TempColor].BufB;
					LedPickAll++;
				}
				else
				{
					LedPickAll = 0;
					ModeStep[0]++;

					for (i = 0; i < LED_TOTAL; i++)
					{
						LedPick[i] = 0;
					}
					
					TempColor++;
					if (TempColor >= ParaData[INSTEAD].ColorNum)
					{
						TempColor = 0x0;
					}
				}
			} break;
		}
	}
}

void Display_Tree_Carnival(void)
{
	uint16_t    i = 0;
	uint16_t	l = 0;
	uint16_t	m = 0;

	if (Display.Init == true)
	{
		Display.Init  = false;
		Para_Err_Check(&ParaData[CARNIVAL]);
		//BrightLevel = PARA_BRIGHT_MAX - ParaData[CARNIVAL].Bright + 1;
		BrightLevel=1;

		SpeedCtrl = 0;

		TempR = 0;
		TempG = 0;
		TempB = 0;

		HoldTime = 0;
		Display_All_Set(0,0,0); 

		TempStep = 1;

		ModeFirstFlag = true;
		return;
	}

	SpeedCtrl++;
	if (SpeedCtrl > (PARA_SPEED_MAX - ParaData[CARNIVAL].Speed) * (uint8_t)( 1 - ModeFirstFlag))
	{
		SpeedCtrl = 0;

		if (ModeFirstFlag)
		{
			ModeFirstFlag = false;
			m = 50;
		}
		else
		{
			m = 1;
		}
			

		for (l = 0; l < m; l++)
			{
			for (i = 0; i < LED_TOTAL; i++)
			{
				switch (i % 6)
				{
					case 0:
					{
						LedData[i].DutyB = 0;
						RAND();
						LedData[i].DutyR += (uint8_t)rand() % 10;
						RAND();
						LedData[i].DutyG -= (uint8_t)rand() % 10;
					}break;

					case 1:
					{
						LedData[i].DutyG = 0;
						RAND();
						LedData[i].DutyB += (uint8_t)rand() % 11;
						RAND();
						LedData[i].DutyR -= (uint8_t)rand() % 9;
					}break;

					case 2:
					{
						LedData[i].DutyR = 0;
						RAND();
						LedData[i].DutyG += (uint8_t)rand() % 12;
						RAND();
						LedData[i].DutyB -= (uint8_t)rand() % 8;
					}break;

					case 3:
					{
						LedData[i].DutyB = 0;
						RAND();
						LedData[i].DutyG += (uint8_t)rand() % 13;
						RAND();
						LedData[i].DutyR -= (uint8_t)rand() % 7;
					}break;

					case 4:
					{
						LedData[i].DutyG = 0;
						RAND();
						LedData[i].DutyR += (uint8_t)rand() % 14;
						RAND();
						LedData[i].DutyB -= (uint8_t)rand() % 6;
					}break;

					default:
					{
						LedData[i].DutyR = 0;
						RAND();
						LedData[i].DutyB += (uint8_t)rand() % 15;
						RAND();
						LedData[i].DutyG -= (uint8_t)rand() % 5;
					}break;
				}
			}
		}
	}
}

void Display_Tree_Alternate(void)
{
	uint16_t	i = 0;
	uint16_t	j = 0;
	uint16_t	l = 0;
	uint16_t	m = 0;
  
	if(Display.Init == true)
	{	
	    Display.Init = false;

	    Para_Err_Check(&ParaData[ALTERNATE]);
	    
	    //BrightLevel = PARA_BRIGHT_MAX - ParaData[ALTERNATE].Bright + 1;
	    BrightLevel=1;
	    
		SpeedCtrl = 0;
	    OtherCtrl = 0;
	    TempColor = 0;
	    TempStep  = 0;
	    
		TempR = 0;
	    TempG = 0;
	    TempB = 0;
	    TopR = 240;
	    TopG = 240;
	    TopB = 240;

	    switch (ParaData[ALTERNATE].Speed)
	    {
			case 0:		FadeR = 10;	RptTotal = 60; break;
			case 1:		FadeR = 15; RptTotal = 40; break;
			case 2: 	FadeR = 20; RptTotal = 30; break;
			case 3:		FadeR = 24;	RptTotal = 25; break;
			case 4:		FadeR = 30;	RptTotal = 20; break;
			default:	FadeR = 40;	RptTotal = 15; break;
	    }
	    
	    FadeG = FadeR;
	    FadeB = FadeR;

	    for ( i = 0; i < LED_TOTAL; i++)
	    {
			ModeStep[i] = 0;
			RAND();
			ModeTime[i] = (uint8_t)(rand() % RptTotal);
	    }

	    Display_All_Set(0,0,0);

	    ModeFirstFlag = true;
	    return;
	}
  
  

	if (ModeFirstFlag)
	{
		ModeFirstFlag = false;
		m = RptTotal;
	}
	else
	{
		m = 1;
	}
		

	for (l = 0; l < m; l++)
	{
		for (i = 0; i < LED_TOTAL; i++)
		{
			if (ModeTime[i])
			{
				ModeTime[i]--;
				continue;
			}
			
			switch (ModeStep[i])
			{
		      	case 0:
		      	{
			        if (LedData[i].DutyR < TopR)    LedData[i].DutyR += FadeR;
			        else              ModeStep[i]++;
			     } break;
			      
			      case 1:{
			        if (LedData[i].DutyG < TopG)    LedData[i].DutyG += FadeG;
			        else              ModeStep[i]++;
			      } break;
			      
			      case 2:{
			        if (LedData[i].DutyR > 0)    LedData[i].DutyR -= FadeR;
			        else              ModeStep[i]++;
			      } break;
			      
			      case 3:{
			        if (LedData[i].DutyB < TopB)    LedData[i].DutyB += FadeB;
			        else              ModeStep[i]++;
			      } break;
			      
			      case 4:{
			        if (LedData[i].DutyG > 0)    LedData[i].DutyG -= FadeG;
			        else              ModeStep[i]++;
			      } break;
			      
			      case 5:{
			        if (LedData[i].DutyR < TopR)    LedData[i].DutyR += FadeR;
			        else              ModeStep[i]++;
			      } break;
			      
			      case 6:{
			        if (LedData[i].DutyB > 0)    LedData[i].DutyB -= FadeR;
			        else              ModeStep[i] = 1;
			      } break;
		    }
		}
	}
}


