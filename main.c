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
    int check=0, counter=0, tmp, theTime;

    t5omsi();                               // Initialize timer5 1kHz
    colinit();                              // Initialize column toolbox
    l88init();                              // Initialize 8*8 led toolbox
    keyinit();                              // Initialize keyboard toolbox
    Lcd_SetType(LCD_NORMAL);                // or use LCD_INVERTED!

    Lcd_Init();
    LCD_Clear(BLACK);

    //LCD_ShowChar(10,60,126,0,YELLOW);    // ENLIGT storleken //ny  // bitar mönster står i oledfont.h


    while (1) {
        idle++;                             // Manage Async events
        LCD_WR_Queue();                   // Manage LCD com queue!

        if (t5expq()) {                     // Manage periodic tasks
          l88row(colset());               // ...8*8LED and Keyboard

          key=keyscan();
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

          ms++;                           // ...One second heart beat
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
            
            


          if ((key=keyscan())) {       // ...Any key pressed?
              if ((key==12) ||(key==8) ||(key==4)){
                if (check==0){
                  check=1;
                  tmp=key;
                }  
              }
              if (key==1){
                if (check==1){
                  check=0;
                  LCD_ShowStr(20, 60, "TEST", RED, TRANSPARENT);
                }
              }
          }

            
            
            switch (dayCounter) {
            case 1:{LCD_Clear_Days(BLACK); LCD_ShowStr(10, 40, "MON", WHITE, TRANSPARENT);break;}
            case 2:{LCD_ShowStr(10, 40, "TUE", WHITE, TRANSPARENT);break;}
            case 3:{LCD_ShowStr(10, 40, "WED", WHITE, TRANSPARENT);break;}
            case 4:{LCD_ShowStr(10, 40, "THU", WHITE, TRANSPARENT);break;}
            case 5:{LCD_ShowStr(10, 40, "FRI", WHITE, TRANSPARENT);break;}
            case 6:{LCD_ShowStr(10, 40, "SAT", WHITE, TRANSPARENT);break;}
            case 7:{LCD_ShowStr(10, 40, "SUN", WHITE, TRANSPARENT);break;}
            default:
              break;
            }
            
            


    
              
              
              

              //if (pKey==key) c++; else {c=0; pKey=key;}

              l88mem(1,lookUpTbl[key]+(c<<4));
            
            
            l88mem(2,idle>>8);              // ...Performance monitor
            l88mem(3,idle); idle=0;
        }
    }
}