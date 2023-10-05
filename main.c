/* #include "gd32vf103.h"
#include "drivers.h"
#include "lcd.h"

int main(void){
    int ms=0, s=0, key, pKey=-1, c=0, idle=0;
    int lookUpTbl[16]={1,4,7,14,2,5,8,0,3,6,9,15,10,11,12,13};
    int dac=0, speed=-100;
    int adcr, tmpr;
    char digits[10][10]={"Zero ","One  ","Two  ","Three","Four ","Five ","Six  ","Seven","Eight","Nine "};

    t5omsi();                               // Initialize timer5 1kHz
    colinit();                              // Initialize column toolbox
    l88init();                              // Initialize 8*8 led toolbox
    keyinit();                              // Initialize keyboard toolbox
    Lcd_SetType(LCD_NORMAL);                // or use LCD_INVERTED!
    Lcd_Init();
    LCD_Clear(RED);
    LCD_ShowStr(10, 10, "POLL VERSION", WHITE, TRANSPARENT);

    while (1) {
        idle++;                             // Manage Async events
        LCD_WR_Queue();                   // Manage LCD com queue!

        if (t5expq()) {                     // Manage periodic tasks
            l88row(colset());               // ...8*8LED and Keyboard
            ms++;                           // ...One second heart beat
            if (ms==1000){
              ms=0;
              l88mem(0,s++);
              LCD_ShowStr(10, 30, digits[s%10], WHITE, OPAQUE);
            }
            if ((key=keyscan())>=0) {       // ...Any key pressed?
              if (pKey==key) c++; else {c=0; pKey=key;}
              l88mem(1,lookUpTbl[key]+(c<<4));
            }
            l88mem(2,idle>>8);              // ...Performance monitor
            l88mem(3,idle); idle=0;
        }
    }
} */

#include "gd32vf103.h"
#include "drivers.h"
#include "lcd.h"


int main(void){

    int ms=0, s=0, m=0, h=0, dayCounter=0, key, pKey=-1, c=0, idle=0;
    int lookUpTbl[16]={1,4,7,14,2,5,8,0,3,6,9,15,10,11,12,13};
    int dac=0, speed=-100;
    //int check=0, counter=0, tmp, theTime=0;
    int button1Pressed = 0, button3Pressed = 0, lcdTest=0, lcdDay = 0, timeToShow = 0;

    t5omsi();                               // Initialize timer5 1kHz
    colinit();                              // Initialize column toolbox
    l88init();                              // Initialize 8*8 led toolbox
    keyinit();                              // Initialize keyboard toolbox
    Lcd_SetType(LCD_NORMAL);                // or use LCD_INVERTED!

    Lcd_Init();
    LCD_Clear(BLACK);


    while (1) {
        idle++;                                  // Manage Async events
        LCD_WR_Queue();                          // Manage LCD com queue!

        if (t5expq()) {                           // Manage periodic tasks
          l88row(colset());                        // ...8*8LED and Keyboard
          key=keyscan();



          if (key == 1) {
                button1Pressed = 1; 
                button3Pressed = 0;
            }

          if (button1Pressed && key == 12) {

            if (timeToShow == 0) {                               // Kontrollera om knapp A trycks efter att knapp # har tryckts               
                LCD_ShowNum(10 + 66, 10, 8, 2, WHITE); 
                LCD_ShowChar(10 + 90, 10, ':', 0, WHITE);
                LCD_ShowNum(10 + 100, 10, 00, 2, WHITE); 
                
            }
            else if (timeToShow == 1) {                               // Kontrollera om knapp B trycks efter att knapp # har tryckts 
                LCD_ShowNum(10 + 66, 10, 12, 2, WHITE); 
                LCD_ShowChar(10 + 90, 10, ':', 0, WHITE);
                LCD_ShowNum(10 + 100, 10, 00, 2, WHITE); 
                
            }
            else if (timeToShow == 2) {                               // Kontrollera om knapp C trycks efter att knapp # har tryckts               
                LCD_ShowNum(10 + 66, 10, 20, 2, WHITE);
                LCD_ShowChar(10 + 90, 10, ':', 0, WHITE); 
                LCD_ShowNum(10 + 100, 10, 00, 2, WHITE); 
                
            }
            timeToShow++;                               // Öka tiden att visa
                  if (timeToShow > 2) {
                      timeToShow = 0;                       // Återställ till den första tiden efter att alla har visats
                  }
            lcdTest = 1;
          }
          if (lcdTest) {
                button1Pressed = 0;
                lcdTest = 0;
          }
          

 ///////////////////////////////////////////////////////////////knapp * för att visa dagar//////////////////////////////////////////////////////


          if (key == 3) {
                button3Pressed = 1;
                button1Pressed = 0;
          }

          if (button3Pressed && key == 12) {
                lcdDay++;                         // Gå framåt i veckodagar
                if (lcdDay > 6) {
                    lcdDay = 0;                   // Återgå till måndag om vi är förbi söndag
                }               
          }
          if (button3Pressed && key == 8) {
                lcdDay--;                           // Gå bakåt i veckodagar
                if (lcdDay < 0) {
                    lcdDay = 6;                     // Återgå till söndag om vi är före måndag
                }               
            }
            
            switch (lcdDay) {
                case 0: {
                  LCD_Clear_Days(BLACK);
                  LCD_ShowStr(10, 10, "MON", RED, TRANSPARENT);
                  break;
                }     
                case 1:{
                    LCD_Clear_Days(BLACK);
                    LCD_ShowStr(10, 10, "TUE", RED, TRANSPARENT);
                    break;
                }
                case 2:{
                    LCD_Clear_Days(BLACK);
                    LCD_ShowStr(10, 10, "WED", RED, TRANSPARENT);
                    break;
                }
                case 3:{
                    LCD_Clear_Days(BLACK);
                    LCD_ShowStr(10, 10, "THU", RED, TRANSPARENT);
                    break;
                }
                case 4:{
                    LCD_Clear_Days(BLACK);
                    LCD_ShowStr(10, 10, "FRI", RED, TRANSPARENT);
                    break;
                }
                case 5:{
                    LCD_Clear_Days(BLACK);
                    LCD_ShowStr(10, 10, "SAT", RED, TRANSPARENT);
                    break;
                }
                case 6:{
                    LCD_Clear_Days(BLACK);
                    LCD_ShowStr(10, 10, "SUN", RED, TRANSPARENT);
                    break;
                }
              }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
          switch (key){                                                               //knappar     //ny
            case 15:{ 
              LCD_Clear_Days(BLACK);                                                            // knapp 1
              LCD_ShowStr(10, 10, "MON", RED, TRANSPARENT); break;
              }      
            case 14:{
              LCD_Clear_Days(BLACK);                                                             // knapp 2
              LCD_ShowStr(10, 10, "TUS", RED, TRANSPARENT); break;
              }      
            case 13:{
              LCD_Clear_Days(BLACK);                                                               // knapp 3
              LCD_ShowStr(10, 10, "WED", RED, TRANSPARENT); break;
              }      
            case 11:{
              LCD_Clear_Days(BLACK);                                                              // knapp 4
              LCD_ShowStr(10, 10, "THU", RED, TRANSPARENT); break;
              }      
            case 10:{
              LCD_Clear_Days(BLACK);                                                              // knapp 5
              LCD_ShowStr(10, 10, "FRI", RED, TRANSPARENT); break;
              }      
            case 9:{
              LCD_Clear_Days(BLACK);                                                                // knapp 6
              LCD_ShowStr(10, 10, "SAT", RED, TRANSPARENT); break;
              }      
            case 7:{
              LCD_Clear_Days(BLACK);                                                                 // knapp 7
              LCD_ShowStr(10, 10, "SUN", RED, TRANSPARENT); break;
              } 
          }  
*/
          ms++;                                                                 // ...One second heart beat
          if (ms==1000){
            ms=0;
            l88mem(0,s++);
            if (s == 60) {
              s = 0;
              m++;
              if (m>0){
                LCD_ShowChar(30 + 45, 38, ':', 0, WHITE);
              }
              LCD_ShowNum(20 + 65, 40, m, 2, WHITE);
              if (m == 60) {
                m = 0;
                h++;
                LCD_ShowNum(50, 40, h, 2, WHITE);
                if (h == 23) {
                  dayCounter++;
                  h = 0;
                }
              }
            }
          }
            
          
        
  

              //if (pKey==key) c++; else {c=0; pKey=key;}
              l88mem(1,lookUpTbl[key]+(c<<4));
            
            
            l88mem(2,idle>>8);              // ...Performance monitor
            l88mem(3,idle); idle=0;
        }
    }
}