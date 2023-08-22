#include <stdio.h>
#include "include/soc/soc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "driver/uart.h"
#include <string.h>
#include "sdkconfig.h"

#define TIMER_INTERVAL 1000000
#define SEC_MIN_LIMIT 59
#define HOUR_LIMIT 23

// UART port number, can be UART_NUM_0 ~ (UART_NUM_MAX -1)
#define UART_PORT_NUM UART_NUM_1
#define TX_PIN 18
#define RX_PIN 19
#define RTS_PIN UART_PIN_NO_CHANGE
#define CTS_PIN UART_PIN_NO_CHANGE
#define BUF_SIZE 1024
#define UART_BAUD_RATE 9600

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

void timer_init() {
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
}

void uart_init() {
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,  //UART word length = 8bits
        .parity = UART_PARITY_DISABLE,  //Disable parity
        .stop_bits = UART_STOP_BITS_1,  //1 stop bit
        // UART hardware flow control modes
        // If wanna enable hardware flow control: set it UART_HW_FLOWCTRL_CTS_RTS
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,  // disable hardware flow control
        // .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));

    // Set UART pins(TX: IO18, RX: IO19, no RTS, CTS)
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, TX_PIN, RX_PIN, RTS_PIN, CTS_PIN));

    // Install drivers
    // UART_HW_FIFO_LEN(uart_num) = Length of the UART HW FIFO
    // Rx_buffer_size > UART_HW_FIFO_LEN(uart_num)
    // Tx_buffer_size > UART_HW_FIFO_LEN(uart_num) or = 0 (=0: sont use buffer for send phase)
    // const int uart_buffer_size = (1024 * 2);
    // Install UART driver (non event queue)
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT_NUM, BUF_SIZE*2, 0, 0, NULL, 0));
}

void uart_send (char* data) {
    int len = strlen(data);
    uart_write_bytes(UART_PORT_NUM, data, strlen(data));
    if (len) {
        data[len] = '\0';
    }
}

void app_main()
{
    //Create mutex
    mutex = xSemaphoreCreateBinary();
    if (mutex == NULL) printf("Fail to create mutex");
    timer_init();
    // Restart digital clock
    reset_digiclk(&c);
    uart_init();

    // Keep the task alive
    while (1) {
        if (xSemaphoreTake(mutex, portMAX_DELAY)==pdPASS) //Semaphore take from isr 
        {
            char time[50];
            sprintf(time, "Time: %02d:%02d:%02d\r\n", c.hour, c.min, c.sec);
            uart_send(time);
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
