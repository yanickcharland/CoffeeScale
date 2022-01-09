/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <pico/stdlib.h>
#include "hx711.h"

#define __BUTTON_1_TOUCHED     ButtonTouch[0]==0 && ButtonTouch[1]==1

char ButtonTouch[2];

int nbPour;
int actPour;
int totTime;
int totWeight;
int TimePerPour;
int PercPerPour[6];
int WtPerPour[6];
int nSeq;

int ElapseTimer;
int TotalTimer;

bool Timer_Callback(struct repeating_timer *t)
{
    if (ElapseTimer>0){
        ElapseTimer--;
        TotalTimer++;
    }
    if (ElapseTimer<=0 && nSeq==1){
        actPour++;
        
        if (actPour<5)
            ElapseTimer=TimePerPour*10;
        else
            nSeq=2;
    }
}


void InitRecipe(void){
    nbPour=4;
    totTime=180;
    totWeight=600;
    WtPerPour[0]=0;
    WtPerPour[1]=80;
    WtPerPour[2]=240;
    WtPerPour[3]=360;
    WtPerPour[4]=480;
    WtPerPour[5]=600;

    TimePerPour=totTime/nbPour;
    ElapseTimer=0;
    TotalTimer=0;
    actPour=1;
    nSeq=0;
}

int main() {

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    const uint PD_SDK = 20;
    const uint DOUT = 21;

    long ZEROVAL = 201400;

    float rawWeight;
    long LoadCellValue;
    char i;
    long WVal;
    float remainWt;
    uint WtRatio;

    struct repeating_timer timer;
    
    sleep_ms(3000);

    stdio_init_all();

    uart_init(uart0, 9600);

    InitHX711(20, 21);

    InitRecipe();

    add_repeating_timer_ms(100, Timer_Callback, NULL, &timer);

    rawWeight=0;
    LoadCellValue=0;

    ButtonTouch[0]=0;
    ButtonTouch[1]=0;
      
    while (true) {

        LoadCellValue=ReadHX711();
        rawWeight = (float)LoadCellValue;
        rawWeight = ((rawWeight - ZEROVAL) * 2000) / 485000;

        WtRatio=100*(rawWeight-WtPerPour[actPour-1])/(WtPerPour[actPour]-WtPerPour[actPour-1]);
        remainWt=WtPerPour[actPour]-rawWeight;
        if (remainWt<0)
            remainWt=0;

        if (WtRatio>=100)
            WtRatio=100;

 
        if (uart_is_readable(uart0)){
            uart_read_blocking(uart0, ButtonTouch, 2);
        }
        else{
            printf("n0.val=%.0f%c%c%cj0.val=%d%c%c%cn1.val=%.0f%c%c%c", rawWeight, 0xff, 0xff, 0xff, WtRatio, 0xff, 0xff, 0xff,remainWt , 0xff, 0xff, 0xff);
            printf("x0.val=%d%c%c%cx1.val=%d%c%c%c", ElapseTimer, 0xff, 0xff, 0xff, TotalTimer, 0xff, 0xff, 0xff);
        }
                
        if (__BUTTON_1_TOUCHED) {
            ZEROVAL = LoadCellValue;
            InitRecipe();
            nSeq=0;
            ButtonTouch[0]=0;
            ButtonTouch[1]=0;
        }

        if(nSeq==0 && WtRatio>98){
            ElapseTimer=TimePerPour*10;
            nSeq=1;
        }



        sleep_ms(100);
    }

}
