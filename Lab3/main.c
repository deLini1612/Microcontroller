#include <stdio.h>
#include "include/soc/soc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_timer.h"

#define TIMER_INTERVAL 1000000
#define SEC_MIN_LIMIT 59
#define HOUR_LIMIT 23

typedef struct digiclk {
    unsigned int sec, min, hour;
}digiclk;

static digiclk c;

//Create mutex (binary semaphore)
SemaphoreHandle_t mutex;

// Timer handler function
static void IRAM_ATTR timer_cb(void* arg)
{
    BaseType_t high_task_awoken = pdFALSE;
    xSemaphoreGiveFromISR(mutex, &high_task_awoken);
}


void reset_digiclk(digiclk* c) {
    c->sec = 0;
    c->min = 0;
    c->hour = 0;
};

void display_digiclk(digiclk* c) {
    printf("Time: %02d:%02d:%02d\n", c-> hour, c->min, c->sec);
}

void app_main()
{
    //Create mutex
    mutex = xSemaphoreCreateBinary();
    if (mutex == NULL) printf("Fail to create mutex");

    // Configure and start the timer to count sec (by timer interrupt)
    esp_timer_handle_t timer;
    esp_timer_create_args_t timer_config = {
        .callback = timer_cb, // Function to call when timer expires
        .arg = NULL,                // Argument to pass to the callback
        .dispatch_method = ESP_TIMER_TASK,  // Call the callback from task or from ISR
                                // ESP_TIMER_TASK = from timer task; ESP_TIMER_ISR = from timer ISR
        .name = "1s timer cbs" // Timer name, used in esp_timer_dump function
    };
    ESP_ERROR_CHECK(esp_timer_create(&timer_config, &timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer, TIMER_INTERVAL));

    // Restart digital clock
    reset_digiclk(&c);

    // Keep the task alive
    while (1) {
        if (xSemaphoreTake(mutex, portMAX_DELAY)==pdPASS) //Semaphore take from isr 
        {
            display_digiclk(&c);
            if (c.sec == SEC_MIN_LIMIT) {
                c.sec = 0;
                if (c.min == SEC_MIN_LIMIT) {
                    c.min = 0;
                    if (c.hour == HOUR_LIMIT) {
                        c.hour = 0;
                    }
                    else c.hour ++;
                }
                else c.min ++;
            }
            else c.sec ++;
        }
    }
}

