/*
 * Light_meter.c
 *
 *  Created on: Feb 5, 2024
 *      Author: 30691
 */
#include "main.h"
#include "u8g2_iic.h"
#include "veml7700.h"
#include <Math.h>
#include <stdio.h>
#include <stdlib.h>
//光圈，快门速度
const uint16_t isos[] = {6,12,25,50,64,100,125,160,200,250,320,400,500,800,1600,3200,6400};
const float shutters[] = {0.00025,0.0005,0.001, 0.002, 0.004, 0.008, 0.0167, 0.0333, 0.0667, 0.125, 0.25, 0.5, 1, 2};
const char *shutters_str[] = {"4000","2000","1000", "500", "250", "125", "60", "30", "15", "8", "4", "2", "1\"", "2\""};
const float N[] = {0.9 ,1 ,1.1 ,1.2 ,1.4 ,1.7 ,1.8 ,2 ,2.4 ,2.8 ,3 ,3.2 ,3.5 ,4 ,4.5 ,5.6 ,6 ,6.3 ,7.1 ,8 ,9 ,10 ,11 ,13 ,16 ,18 ,20 ,22 ,26 ,28 ,32 ,36 ,40 ,45 ,52 ,52 ,56 ,64};
const float Ev[] = {-2 , -1.7 , -1.3, -1, -0.7, -0.3, 0, 0.3, 0.7, 1, 1.3, 1.7 ,2};
const char *Ev_[] = {"-2" , "-1.7" , "-1.3", "-1", "-0.7", "-0.3", "0", "0.3", "0.7", "1", "1.3", "1.7" ,"2"};
double EV = 0;
double Lux = 0;

//矫正参数
double a = 6.0135e-13;
double b = -9.3924e-9;
double c = 8.1488e-5;
double d = 1.0023;

//快门选择
uint8_t Shutter = 8;
//光圈选择
uint8_t Aperture = 6;
//模式选择
uint8_t Mode = 0;
//是否按下按键
uint8_t isKey = 0;
uint8_t isKey1 = 0;
//菜单选择
uint8_t Mueu = 0;
//睡眠定时
uint32_t time = 0;
uint32_t Sleep = 0;
//位置 x轴，y轴，将要移动到的y轴
uint8_t x = 59;
uint8_t y = 0;
uint8_t toy = 0;

int Select = 0;
uint8_t SelectValue[4] ={255,5,6,30};
u8g2_t u8g2;
extern ADC_HandleTypeDef hadc1;

void LM_Init(){
	u8g2_Init(&u8g2);
	veml7700_init();
}

//进入睡眠模式
void OLED_Off(){
	if(HAL_GetTick()-Sleep>SelectValue[3]*1000){
		u8g2_SetPowerSave(&u8g2,1);
		HAL_SuspendTick();
		HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);
		HAL_ResumeTick();
		u8g2_SetPowerSave(&u8g2,0);
		Sleep = HAL_GetTick();
		HAL_Delay(100);
	}
}

//快门优先模式下光圈的计算与选择
uint8_t T(){
	uint8_t n = 0;
	float distant = 100.0f;
	float TMode_N = sqrt(pow(2,EV)*shutters[Shutter]);
	for(int i = 0;i<38;i++){
		float temp_distant = fabs(TMode_N-N[i]);
		if(temp_distant<distant){
			distant = temp_distant;
			n = i;
		}
	}
	return n;
}


//光圈优先模式下光圈的计算与选择
uint8_t A(){
	uint8_t s = 0;
	float distant = 100.00000f;
	float AMode_S = (N[Aperture]*N[Aperture])/pow(2,EV);
	for(int i = 0;i<14;i++){
		float temp_distant = fabs(AMode_S-shutters[i]);
		if(temp_distant<distant){
			distant = temp_distant;
			s = i;
		}
	}
	return s;
}

//模式选择方框的动画
void Aim()
{
	if(Mode ==1){
	    if(x!=69){
	      x=x+2;
	    }
	    u8g2_SetDrawColor(&u8g2,2);
	    u8g2_DrawBox(&u8g2,x,12,6,8);
	    u8g2_SetDrawColor(&u8g2,1);
	  }
	  if(Mode ==0){
	    if(x!=59){
	      x=x-2;
	    }
	    u8g2_SetDrawColor(&u8g2,2);
	    u8g2_DrawBox(&u8g2,x,12,7,8);
	    u8g2_SetDrawColor(&u8g2,1);
	  }
}

//按键扫描
uint8_t KeyScan(uint8_t Key_Value,uint8_t Length)
{
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) && isKey1 ==0){
		Key_Value++;
		if(Key_Value>Length){
			Key_Value--;
		}
		isKey1 = 1;
		time = HAL_GetTick();
	}
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) && isKey1 ==0){
		if(Key_Value-1<0){
			Key_Value=0;
		}
		else{
			Key_Value--;
		}
		isKey1 = 1;
		time = HAL_GetTick();
	}
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) && isKey == 0){
		isKey = 1;
		time = HAL_GetTick();
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) && isKey== 1){
		if(HAL_GetTick()-time<1000){
			Mode = !Mode;
		}
		else{
			Mueu = 1;
		}
		isKey = 0;
		Sleep = HAL_GetTick();
	}
	if((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) || !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)) && isKey1 == 1){
		if(HAL_GetTick()-time>150){
			isKey1 = 0;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) && isKey1 == 1){
		isKey1 =0;
		Sleep = HAL_GetTick();
	}
	return Key_Value;
}


void KeyScan1(){
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)&& isKey1 ==0){
		Select++;
		if(Select==3){
			Select =2;
		}
		isKey1 =1;
		time = HAL_GetTick();
	}
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)&& isKey1 ==0){
		Select--;
		if(Select<0){
			Select = 0;
		}
		isKey1 =1;
		time = HAL_GetTick();
	}
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) && isKey == 0){
		isKey = 1;
		time = HAL_GetTick();
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) && isKey== 1){
		if(HAL_GetTick()-time<1000){
			if(Mueu ==1){
				Mueu =2;
			}
			else{
				Mueu =1;
			}
		}
		else{
			Mueu = 0;
		}
		isKey = 0;
		Sleep = HAL_GetTick();
	}
	if((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) || !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)) && isKey1 == 1){
		if(HAL_GetTick()-time>150){
			isKey1 = 0;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) && isKey1 == 1){
		isKey1 =0;
		Sleep = HAL_GetTick();
	}
}


void KeyScan2(){
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) && isKey1 ==0){
		SelectValue[Select+1]++;
		isKey1 =1;
		time = HAL_GetTick();
	}
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) && isKey1 ==0){
		SelectValue[Select+1]--;
		isKey1 =1;
		time = HAL_GetTick();
	}
	if(Select == 0){
		if(SelectValue[1]<0)SelectValue[1]=0;
		if(SelectValue[1]>16)SelectValue[1]=16;
	}
	if(Select == 1){
		if(SelectValue[2]<0)SelectValue[2]=0;
		if(SelectValue[2]>12)SelectValue[2]=12;
	}
	if(Select == 2){
		if(SelectValue[3]<30)SelectValue[3]=30;
		if(SelectValue[3]>180)SelectValue[3]=180;
	}
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) && isKey == 0){
		isKey = 1;
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) && isKey== 1){
		Mueu =1;
		isKey = 0;
		Sleep = HAL_GetTick();
	}
	if((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) || !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)) && isKey1 == 1){
		if(HAL_GetTick()-time>150){
			isKey1 = 0;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) && isKey1 == 1){
		isKey1 =0;
		Sleep = HAL_GetTick();
	}
}


//菜单选择框的动画
void frame(){
  if(Select<3 && Select>=0){
	toy = 2+Select*15;
  }
  if(toy>y){
	  y=y+3;
  }
  if(toy<y){
	  y=y-3;
  }
	u8g2_SetDrawColor(&u8g2,2);
	u8g2_DrawBox(&u8g2,0,3+y,129,14);
	u8g2_SetDrawColor(&u8g2,1);
}


//主循环，包括主界面的绘制
void Loop(){
	u8g2_ClearBuffer(&u8g2);
	OLED_Off();
	if(Mueu == 0){
	Lux = get_lux_data();
	if(Lux>1000){
		Lux = a * pow(Lux, 4) + b * pow(Lux, 3) + c * pow(Lux, 2) + d * Lux; //大于1000lux的需要矫正
	}
//	EV值计算
	EV = log2((Lux/2.5f))+logf(isos[SelectValue[1]]/100.0f)/M_LN2+Ev[SelectValue[2]]-1.00f;

//	快门优先
	if(Mode == 1){
		Aperture = KeyScan(Aperture,37);
		char buf[32];
		u8g2_SetFont(&u8g2,u8g2_font_spleen12x24_mf);
		sprintf(buf,"%.1f",N[Aperture]);
		Shutter = A();
		u8g2_DrawStr(&u8g2,8,20,shutters_str[Shutter]);
		u8g2_SetFont(&u8g2,u8g2_font_spleen16x32_mf);
		u8g2_DrawStr(&u8g2,44,54,"F");
		u8g2_DrawStr(&u8g2,60,54,buf);
		u8g2_SetFont(&u8g2,u8g2_font_spleen5x8_mf);
		sprintf(buf,"%d",isos[SelectValue[1]]);
		u8g2_DrawStr(&u8g2,0,40,"ISO");
		u8g2_DrawStr(&u8g2,15,40,buf);
		u8g2_DrawStr(&u8g2,60,19,"T");
		u8g2_DrawStr(&u8g2,70,19,"A");
		if(Shutter<12){
			u8g2_DrawStr(&u8g2,3,10,"1");
		}
		Aim();
		u8g2_SetFont(&u8g2,u8g2_font_spleen6x12_mf);
		sprintf(buf,"%.3f",EV);
		u8g2_DrawStr(&u8g2,80,20,"EV");
		u8g2_DrawStr(&u8g2,92,20,buf);
//		sprintf(buf,"%.3f",Lux);
		HAL_ADC_Start(&hadc1);
		itoa(HAL_ADC_GetValue(&hadc1),buf,10);
		u8g2_DrawStr(&u8g2,1,53,buf);
	}

//	光圈优先
	if(Mode == 0){
		Shutter = KeyScan(Shutter,13);
		char buf[32];
		u8g2_SetFont(&u8g2,u8g2_font_spleen12x24_mf);
		Aperture = T();
		sprintf(buf,"%.1f",N[Aperture]);
		u8g2_DrawStr(&u8g2,8,20,shutters_str[Shutter]);
		u8g2_SetFont(&u8g2,u8g2_font_spleen16x32_mf);
		u8g2_DrawStr(&u8g2,44,54,"F");
		u8g2_DrawStr(&u8g2,60,54,buf);
		u8g2_SetFont(&u8g2,u8g2_font_spleen5x8_mf);
		sprintf(buf,"%d",isos[SelectValue[1]]);
		u8g2_DrawStr(&u8g2,0,40,"ISO");
		u8g2_DrawStr(&u8g2,15,40,buf);
		u8g2_DrawStr(&u8g2,60,19,"T");
		u8g2_DrawStr(&u8g2,70,19,"A");
		if(Shutter<12){
			u8g2_DrawStr(&u8g2,3,10,"1");
		}
		Aim();
		u8g2_SetFont(&u8g2,u8g2_font_spleen6x12_mf);
		sprintf(buf,"%.3f",EV);
		u8g2_DrawStr(&u8g2,80,20,"EV");
		u8g2_DrawStr(&u8g2,92,20,buf);
//		sprintf(buf,"%.3f",Lux);
		HAL_ADC_Start(&hadc1);
		itoa(HAL_ADC_GetValue(&hadc1),buf,10);
		u8g2_DrawStr(&u8g2,1,53,buf);
	  }
		u8g2_SendBuffer(&u8g2);
		return;
	}
//	菜单页面
	if(Mueu == 1 || Mueu ==2 ){
		if(Mueu ==1){
			KeyScan1();
		}
		if(Mueu ==2){
			KeyScan2();
		}
		char buf[32];
		u8g2_SetFont(&u8g2,u8g2_font_spleen6x12_mf);
		sprintf(buf,"%d",isos[SelectValue[1]]);
		u8g2_DrawStr(&u8g2,0,14,".ISO");
		u8g2_DrawStr(&u8g2,100,14,buf);
		u8g2_DrawStr(&u8g2,0,29,".EV Compensate");
		u8g2_DrawStr(&u8g2,100,29,Ev_[SelectValue[2]]);
		sprintf(buf,"%d",SelectValue[3]);
		u8g2_DrawStr(&u8g2,0,45,".Sleep Time");
		u8g2_DrawStr(&u8g2,100,45,buf);
		frame();
		u8g2_SendBuffer(&u8g2);
		return;
	}
}
