#include <stdio.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Timer interval in microseconds
#define TIMER_INTERVAL 1000000
#define LED_PIN GPIO_NUM_8  //LED pin = IO8

bool led_status = false;

// Timer handler function
void callback_func(void* arg)
{
    printf("Timmer interrupt!\n");
    led_status = !led_status;
    gpio_set_level(LED_PIN, led_status);
}
    
void led_config()
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
}

extern "C" void app_main()
{
    // Configure LED output
    led_config();

    // Configure and start the timer
    esp_timer_handle_t timer;
    esp_timer_create_args_t timer_config = {
        .callback = callback_func, // Function to call when timer expires
        .arg = NULL,                // Argument to pass to the callback
        .dispatch_method = ESP_TIMER_TASK,  // Call the callback from task or from ISR
                                // ESP_TIMER_TASK = from timer task; ESP_TIMER_ISR = from timer ISR
        .name = "example_timer_blinky" // Timer name, used in esp_timer_dump function
    };
    esp_timer_create(&timer_config, &timer);
    esp_timer_start_periodic(timer, TIMER_INTERVAL);

    // Keep the task alive
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}