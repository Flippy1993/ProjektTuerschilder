#include <Arduino.h>

void goToDeepSleep(int timeToSleep, int type){

    int factorMicroSecondsToSeconds = 1000000;

    int unit = 0;
    // seconds
    if(type == 1){
        unit = factorMicroSecondsToSeconds;
    // minutes
    }else if(type == 2){
        unit = factorMicroSecondsToSeconds * 60;
    // hours
    }else if(type == 3){
        unit = (factorMicroSecondsToSeconds * 60) *60;
    }
    esp_sleep_enable_timer_wakeup(timeToSleep * unit);
    esp_deep_sleep_start();
}