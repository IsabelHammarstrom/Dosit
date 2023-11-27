#include "gd32vf103.h"
#include "drivers.h"
#include "lcd.h"
#include "rtc.h"


void rtcInit(void){
   // enable power managemenet unit - perhaps enabled by default
   rcu_periph_clock_enable(RCU_PMU);
   // enable write access to the registers in the backup domain
   pmu_backup_write_enable();
   // enable backup domain
   rcu_periph_clock_enable(RCU_BKPI);
   // reset backup domain registers
   bkp_deinit();
   // set the results of a previous calibration procedure
   // bkp_rtc_calibration_value_set(x);

   // setup RTC
   // enable external low speed XO
   //rcu_osci_on(RCU_LXTAL);
   if (rcu_osci_stab_wait(RCU_HXTAL)) {
     // use external low speed oscillaotr, i.e. 32.768 kHz
     rcu_rtc_clock_config(RCU_RTCSRC_HXTAL_DIV_128);
     rcu_periph_clock_enable(RCU_RTC);
     // wait until shadow registers are synced from the backup domain
     // over the APB bus
     rtc_register_sync_wait();
     // wait until shadow register changes are synced over APB
     // to the backup doamin
     rtc_lwoff_wait();
     // prescale to 1 second
     rtc_prescaler_set(62500 - 1);
     rtc_lwoff_wait();
     rtc_flag_clear(RTC_INT_FLAG_SECOND);
     //rtc_interrupt_enable(RTC_INT_SECOND);
     rtc_lwoff_wait();
   }
}


typedef enum {
    INIT_STATE,
    SENSOR_ON_STATE,
    SENSOR_OFF_STATE,
    MANUAL_CONTROL_STATE
} SystemState;

SystemState currentState = INIT_STATE;
/**
void handleSensorState(void) {
    static int previousSensorState = -1;
    int currentSensorState = gpio_input_bit_get(GPIOB, GPIO_PIN_8);

    if (currentSensorState != previousSensorState) {
        LCD_Fill(10, 60, 160, 80, BLACK);
       
        if (currentSensorState == 0) {
            //gpio_bit_write(GPIOA, GPIO_PIN_4, 1);                       // högtalaren på
            gpio_bit_write(GPIOA, GPIO_PIN_3, 1);                       // Lysdioden sätts på
            LCD_ShowStr(10, 60, "Medicin is left", RED, TRANSPARENT);
        } else {
            //gpio_bit_write(GPIOA, GPIO_PIN_4, 0);                         // högtalaren av

            gpio_bit_write(GPIOA, GPIO_PIN_3, 0);                           // Lysdioden sätts på
            LCD_ShowStr(10, 60, "Medicin is taken", RED, TRANSPARENT);
        }
        previousSensorState = currentSensorState;
    }
}
*/

int isSensorActivationTime(void) {
    uint32_t rtc_time = rtc_counter_get();
    uint8_t hour = (rtc_time / 3600) % 24;

    uint8_t activationHour = 8;                                                                 // Ställ in förutbestämda aktiveringstid här (exempel: 8 AM)

    return (hour == activationHour);                                                        // Check if the current hour matches the activation hour
}

void handleSensorStateAtActivationTime(void) {
    static int previousSensorState = -1;
    int currentSensorState = gpio_input_bit_get(GPIOB, GPIO_PIN_8);

    if (currentSensorState != previousSensorState) {
        LCD_Fill(10, 60, 160, 80, BLACK);

        if (currentSensorState == 0) {
            // gpio_bit_write(GPIOA, GPIO_PIN_4, 1);                                  // Speaker on
            gpio_bit_write(GPIOA, GPIO_PIN_3, 1);                                     // LED on
            LCD_ShowStr(10, 60, "Medicine is left", RED, TRANSPARENT);
        } else {
            // gpio_bit_write(GPIOA, GPIO_PIN_4, 0);                                     // Speaker off
            gpio_bit_write(GPIOA, GPIO_PIN_3, 0);                                       // LED off
            LCD_ShowStr(10, 60, "Medicine is taken", RED, TRANSPARENT);
        }

        previousSensorState = currentSensorState;
    }
}

void setRtcTime(int button) {
    uint32_t current_time = rtc_counter_get();
    uint32_t new_time = current_time;

    switch (button) {
        case 15:                                                                    // Button for increasing hours
            new_time += 3600;                                                       // Add one hour
            break;
        case 14:                                                                    // Button for increasing minutes
            new_time += 60;                                                             // Add one minute
            break;
        case 13:                                                                    // Button for decreasing seconds
            new_time -= 1;                                                           // minus one second
            break;
        // Add cases for decreasing hours, minutes if needed
    }
    rtc_counter_set(new_time);
}


void handleRTC(void) {
    uint32_t rtc_time = rtc_counter_get();
    uint8_t hour = (rtc_time / 3600) % 24;  
    uint8_t minute = (rtc_time % 3600) / 60;  
    uint8_t second = rtc_time % 60;  

    LCD_ShowNum(10, 30, hour, 2, RED); 
    LCD_ShowChar(10 + 2 * 12, 30, ':', 16, RED);  
    LCD_ShowNum(10 + 3 * 12, 30, minute, 2, RED);  
    LCD_ShowChar(10 + 5 * 12, 30, ':', 16, RED);  
    LCD_ShowNum(10 + 6 * 12, 30, second, 2, RED);  
}

void handleManualControl(int key) {
    switch (key) {
        case 0: {
            gpio_bit_write(GPIOA, GPIO_PIN_0, 0);
            gpio_bit_write(GPIOA, GPIO_PIN_1, 1);
            gpio_bit_write(GPIOA, GPIO_PIN_3, 1);                                   //Lysdioden sätts på
            LCD_Fill(10, 10, 160, 25, BLACK);
            LCD_ShowStr(10, 10, "MON open", RED, TRANSPARENT);
            break;
        }
        case 1: {
            gpio_bit_write(GPIOA, GPIO_PIN_1, 0);
            gpio_bit_write(GPIOA, GPIO_PIN_0, 1);
            gpio_bit_write(GPIOA, GPIO_PIN_3, 1);
            LCD_Fill(10, 10, 160, 25, BLACK);
            LCD_ShowStr(10, 10, "TUE open", RED, TRANSPARENT);
            break;
        }
        case 2: {
            gpio_bit_write(GPIOA, GPIO_PIN_0, 0);
            gpio_bit_write(GPIOA, GPIO_PIN_1, 0);
            gpio_bit_write(GPIOA, GPIO_PIN_3, 1);
            LCD_Fill(10, 10, 160, 25, BLACK);
            LCD_ShowStr(10, 10, "Open all days", RED, TRANSPARENT);
            break;
        }
        case 3: {
            gpio_bit_write(GPIOA, GPIO_PIN_0, 1);
            gpio_bit_write(GPIOA, GPIO_PIN_1, 1);
            gpio_bit_write(GPIOA, GPIO_PIN_3, 0);
            LCD_Fill(10, 10, 160, 25, BLACK);
            LCD_ShowStr(10, 10, "Close all days", RED, TRANSPARENT);
            break;
        }
    }
}

/**
void updateLCD(void) {
    LCD_WR_Queue();
    colset();
}
*/

int main(void) {
    int key;
    colinit();
    keyinit();
    Lcd_SetType(LCD_NORMAL);
    Lcd_Init();
    LCD_Clear(BLACK);
    rtcInit();

    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOA);

    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_2);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8);

    while (1) {
        //updateLCD();
        LCD_WR_Queue();
        colset();
        
        key = keyscan();

        if (key != -1) {
            setRtcTime(key);                                                                // Set RTC time based on button input
        }

        switch (currentState) {
            case INIT_STATE:
                // Ytterligare initialiseringskod kan placeras här
                currentState = SENSOR_ON_STATE;                                     
                break;

            case SENSOR_ON_STATE:
                if (isSensorActivationTime()) {
                    handleSensorStateAtActivationTime();
                }
                currentState = SENSOR_OFF_STATE;
                break;
            /**
                handleSensorState();
                currentState = SENSOR_OFF_STATE;
                break;
            */
            case SENSOR_OFF_STATE:
                handleRTC();
                currentState = MANUAL_CONTROL_STATE;
                break;

            case MANUAL_CONTROL_STATE:
                handleManualControl(key);
                currentState = SENSOR_ON_STATE;
                break;
        }
    }
}
