
#include "gd32vf103.h"
#include "delay.h"
#include "lcd.h"
#include "drivers.h"


int main(void){
    Lcd_SetType(LCD_INVERTED);

    Lcd_Init();
    LCD_Clear(BLACK);

    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOA);

    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 );           

    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8);

    // Behöver lägga till "PINs" för alla dagar, aka 8st GPIOB samt 7st GPIOA pins ovan....

    int openAll=0, week[7]={1,0,1,0,0,0,0}, open[7]={0,0,0,0,0,0,0} , time[7]={}, time1[7]={}, time2[7]={}, days=-1;

    while(1){                                                                   // Loop forever
            
    // *********** koden för RTC ****************


    // *********** Kod för ihopkoppling med applikationen ***************************
       
        /* 
            Få information från applikation 
                ex: mån 8:00
                    --> time1[0] = 08;
                    --> time2[0] = 00;
                    --> week[0] = 1;
                    --> time[0] = 8:00 "fixa i samma format som RTC"
                ex: tis 10:30
                    --> time1[1] = 10;
                    --> time2[1] = 30;
                    --> week[1] = 1; 
                    --> time[1] = 10:30 "fixa i samma format som RTC"
                ex: öppna alla luckor
                    --> openAll = 1;
                ex: stänga alla luckor
                    --> openAll = 0; 
                ex: öppna måndag knappen
                    --> open[0] = 1; 
        */

    // ******************** kod för öppnandet av luckor samt larm och sensorer *********************************    

        if(openAll == 1){                                                 // Öppnar alla luckor
            gpio_bit_write(GPIOB, GPIO_PIN_0, 0);
            gpio_bit_write(GPIOB, GPIO_PIN_1, 0);
            // Fortsätta med resten av dagarna 
        }else if(openAll == 0){                                           // Stänger alla luckor
            gpio_bit_write(GPIOB, GPIO_PIN_0, 1);
            gpio_bit_write(GPIOB, GPIO_PIN_1, 1);
            // Fortsätta med resten av dagarna
        }   

    // Kod för att få ett speciellt ljud åt högtalarna?!?!?! 

        
    // Måndag:
        if(/* rtc == time[0] */){
            open[0] = 1;
            gpio_bit_write(GPIOB, GPIO_PIN_0, 0);                               // Öppnar luckan för måndag
            gpio_bit_write(GPIOB, GPIO_PIN_2, 1);                               // Sätter PÅ alarm (speaker/led)
            if(gpio_input_bit_get(GPIOA, GPIO_PIN_8) == 1){                     // Read pin A8 (sensor 1)
                // Skicka till applikation
                gpio_bit_write(GPIOB, GPIO_PIN_2, 0);                           // Stänger AV alarm (speaker/led)
                if(gpio_input_bit_get(GPIOA, GPIO_PIN_8) == 0){
                    gpio_bit_write(GPIOB, GPIO_PIN_0, 1);                       // Låser luckan
                    open[0] = 0;
                    
                    for(i=1, i<7, i++){                 // kollar när nästa inlaggda tid finns och ändrar LCD skärmen
                        if(week[i]==1){
                            days=i;
                        }
                    } 
                }    
            }                               
        }else if(open[0] == 1){
            gpio_bit_write(GPIOB, GPIO_PIN_0, 0);                               // Öppnar luckan för måndag
        }else if(open[0] == 0){
            gpio_bit_write(GPIOB, GPIO_PIN_0, 1);                               // toggle B0 (låser luckan)
        }

    // Tisdag: 
        if(/* rtc klockan tisdag == time[1] */){
            open[1] = 1;
            gpio_bit_write(GPIOB, GPIO_PIN_1, 0);                               // Öppnar luckan för tisdag
            gpio_bit_write(GPIOB, GPIO_PIN_2, 1);                               // Sätter PÅ alarm (speaker/led)
            if(gpio_input_bit_get(GPIOA, GPIO_PIN_7) == 1){                     // Read pin A7 (sensor 2)
                // Skicka till applikation
                gpio_bit_write(GPIOB, GPIO_PIN_2, 0);                           // Stänger AV alarm (speaker/led)      
                if(gpio_input_bit_get(GPIOA, GPIO_PIN_7) == 0){
                    gpio_bit_write(GPIOB, GPIO_PIN_1, 1);                       // Låser luckan
                    open[1] = 0;
                    
                    for(i=2, i<7, i++){                   // kollar när nästa inlaggda tid finns och ändrar LCD skärmen
                        if(week[i]==1){
                            days=i;
                        }
                    } 
                }                        
            }
        }else if(open[1] == 1){
            gpio_bit_write(GPIOB, GPIO_PIN_1, 0);                               // Öppnar luckan för tisdag
        }else if(open[1] == 0){
            gpio_bit_write(GPIOB, GPIO_PIN_1, 1);                               // toggle B1 (låser luckan)
        }
    
    // Fortsätta med resten av  dagarna nedan .......

    /* 
        switch(key){
            case 1: {
                gpio_bit_write(GPIOB, GPIO_PIN_0, 0);           // måndag öppnas
                break;
            }

            case 2: {
                gpio_bit_write(GPIOB, GPIO_PIN_1, 0);           // tisdag öppnas
                break;
            }

            case 3: {
                gpio_bit_write(GPIOB, GPIO_PIN_0, 0);           // alla öppnas
                gpio_bit_write(GPIOB, GPIO_PIN_1, 0);
                break;
            }

            case 4: {
                gpio_bit_write(GPIOB, GPIO_PIN_0, 1);           // luckor stängs
                gpio_bit_write(GPIOB, GPIO_PIN_1, 1);
                break;
            }
        }
    */

    // **************** LCD kod *********************

        switch(days){
            case 0: {
                LCD_Clear_Days(BLACK);
                LCD_ShowStr(10, 10, "MON", WHITE, TRANSPARENT);
                
                LCD_ShowNum(10 + 66, 10, time1[days], 2, WHITE); 
                LCD_ShowChar(10 + 90, 10, ':', 0, WHITE);
                LCD_ShowNum(10 + 100, 10, time2[days], 2, WHITE); 
                break;
            } 
            case 1: {
                LCD_Clear_Days(BLACK);
                LCD_ShowStr(10, 10, "TUE", WHITE, TRANSPARENT);

                LCD_ShowNum(10 + 66, 10, time1[days], 2, WHITE); 
                LCD_ShowChar(10 + 90, 10, ':', 0, WHITE);
                LCD_ShowNum(10 + 100, 10, time2[days], 2, WHITE);
                break;
            } 

            case 2: {
                LCD_Clear_Days(BLACK);
                LCD_ShowStr(10, 10, "WED", WHITE, TRANSPARENT);

                LCD_ShowNum(10 + 66, 10, time1[days], 2, WHITE); 
                LCD_ShowChar(10 + 90, 10, ':', 0, WHITE);
                LCD_ShowNum(10 + 100, 10, time2[days], 2, WHITE);
                break;
            }
            case 3: {
                LCD_Clear_Days(BLACK);
                LCD_ShowStr(10, 10, "THU", WHITE, TRANSPARENT);

                LCD_ShowNum(10 + 66, 10, time1[days], 2, WHITE); 
                LCD_ShowChar(10 + 90, 10, ':', 0, WHITE);
                LCD_ShowNum(10 + 100, 10, time2[days], 2, WHITE);
                break;
            }
            case 4: {
                LCD_Clear_Days(BLACK);
                LCD_ShowStr(10, 10, "FRI", WHITE, TRANSPARENT);

                LCD_ShowNum(10 + 66, 10, time1[days], 2, WHITE); 
                LCD_ShowChar(10 + 90, 10, ':', 0, WHITE);
                LCD_ShowNum(10 + 100, 10, time2[days], 2, WHITE);
                break;
            }
            case 5: {
                LCD_Clear_Days(BLACK);
                LCD_ShowStr(10, 10, "SAT", WHITE, TRANSPARENT);

                LCD_ShowNum(10 + 66, 10, time1[days], 2, WHITE); 
                LCD_ShowChar(10 + 90, 10, ':', 0, WHITE);
                LCD_ShowNum(10 + 100, 10, time2[days], 2, WHITE);
                break;
            }
            case 6: {
                LCD_Clear_Days(BLACK);
                LCD_ShowStr(10, 10, "SUN", WHITE, TRANSPARENT);

                LCD_ShowNum(10 + 66, 10, time1[days], 2, WHITE); 
                LCD_ShowChar(10 + 90, 10, ':', 0, WHITE);
                LCD_ShowNum(10 + 100, 10, time2[days], 2, WHITE);
                break;
            }
            default: {
                break;
            }
        }

        delay_1ms(500);                                                         // Wait for half a second
    
    }
}           