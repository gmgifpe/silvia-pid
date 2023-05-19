#include <Arduino.h>
#include <shared.h>
#include <heater.h>
#include <oled.h>
#include <timer.h>

volatile int gEvent = 0;

// Timer variables
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

int gTimer1sBrewing;

int gTimer100ms = 0;
int gTimer1s = 0;

void IRAM_ATTR onTimer () {
    portENTER_CRITICAL_ISR(&timerMux);

    gEvent |= EVENT_100MS;
    gTimer100ms++;
    if ((gTimer100ms % 10) == 0)
    {
        gTimer1s++;
        gEvent |= EVENT_1S;
    }
    portEXIT_CRITICAL_ISR(&timerMux);
}

void setup_timer() {
    // Set up the timer interrupt
    timer = timerBegin(0, 80, true); // Timer 0, Prescaler 80 (1MHz)
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 100000, true); // 100ms interval
    timerAlarmEnable(timer);
}

void background_event() {
    if (gEvent & EVENT_100MS)
    {
        gEvent &= ~EVENT_100MS;

        if (gLogo_show_time == 0) ssd1306_display(currentTemp, currentPres, gTargetTemp, heaterState);
    }

    if (gEvent & EVENT_1S)
    {
        gEvent &= ~EVENT_1S;

        if (gLogo_show_time) gLogo_show_time--;
    }

    if (gEvent & EVENT_BOOT)
    {
        gEvent &= ~EVENT_BOOT;

        gLogo_show_time = 3;
    }
}