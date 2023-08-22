#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"
#include "esp_timer.h"

//==========================Macro==========================
#define MASK(x) ((uint8_t)1U<<(x))

#define UART_BAUD_RATE 4800
#define BIT_INTERVAL ((unsigned short) (1000000/UART_BAUD_RATE)) // in microsecond
#define DIVI 4
#define TICK_INTERVAL ((unsigned short) (BIT_INTERVAL/DIVI)) // = 1/DIVI Tbaud
#define TX_PIN GPIO_NUM_18
#define RX_PIN GPIO_NUM_19
#define TX_MODE 0
#define RX_MODE 1

#define C_INTERVAL 1000000
#define SEC_MIN_LIMIT 59
#define HOUR_LIMIT 23
//===========================================================


//=====================Parameter declare=====================
static bool sending, send_req;
static unsigned short bit_index_tx;

static bool receiving, recv_req, IO_prev, IO;
static unsigned short bit_index_rx, tick_index;

union {
    struct {
        unsigned short start:1;
        unsigned short data:8;
        unsigned short parity:1;    //odd parity
        unsigned short stop:1;
    } bitfield;
    unsigned short buffer;
} tx_buff, rx_buff;

struct {
    unsigned int sec, min, hour;
}digiclk;
//=========================================================


//========================ISR Handle========================
SemaphoreHandle_t tx_mutex, c_mutex, rx_mutex;

// Timer handler function
static void IRAM_ATTR tx_timer_cb(void* arg)
{
    BaseType_t high_task_awoken = pdFALSE;
    xSemaphoreGiveFromISR(tx_mutex, &high_task_awoken);
}

static void IRAM_ATTR rx_timer_cb(void* arg)
{
    BaseType_t high_task_awoken = pdFALSE;
    xSemaphoreGiveFromISR(rx_mutex, &high_task_awoken);
}

static void IRAM_ATTR c_timer_cb(void* arg)
{
    BaseType_t high_task_awoken = pdFALSE;
    xSemaphoreGiveFromISR(c_mutex, &high_task_awoken);
}
//==========================================================


//=====================Init function========================
void io_init() {
    // Configure IO for UART
    gpio_reset_pin(TX_PIN);
    gpio_set_direction(TX_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(TX_PIN, 1); // Default: TX pin = 1 (if not active)
    gpio_reset_pin(RX_PIN);
    gpio_set_direction(RX_PIN, GPIO_MODE_INPUT);
}

void tx_param_init() {
    sending = 0;
    send_req = 0;
    bit_index_tx = 0;
}

void rx_param_init() {
    receiving = 0;
    recv_req = 1;
    IO_prev = 1;
    IO = 1;
    bit_index_rx = 0;
    tick_index = 0;
}

esp_timer_handle_t tx_timer, rx_timer;
void tx_timer_init() {
    // Configure and start the timer to count sec (by timer interrupt)
    esp_timer_create_args_t timer_config = {
        .callback = tx_timer_cb, // Function to call when timer expires
        .arg = NULL,                // Argument to pass to the callback
        .dispatch_method = ESP_TIMER_TASK,  // Call the callback from task or from ISR
                                // ESP_TIMER_TASK = from timer task; ESP_TIMER_ISR = from timer ISR
        .name = "1 bit interval" // Timer name, used in esp_timer_dump function
    };
    ESP_ERROR_CHECK(esp_timer_create(&timer_config, &tx_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(tx_timer, BIT_INTERVAL));
}

void rx_timer_init() {
    // // Configure and start the timer to count sec (by timer interrupt)
    esp_timer_create_args_t timer_config = {
        .callback = rx_timer_cb, // Function to call when timer expires
        .arg = NULL,                // Argument to pass to the callback
        .dispatch_method = ESP_TIMER_TASK,  // Call the callback from task or from ISR
                                // ESP_TIMER_TASK = from timer task; ESP_TIMER_ISR = from timer ISR
        .name = "1 tick interval" // Timer name, used in esp_timer_dump function
    };
    ESP_ERROR_CHECK(esp_timer_create(&timer_config, &rx_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(rx_timer, TICK_INTERVAL));
}


void c_timer_init() {
    // Configure and start the timer to count sec (by timer interrupt)
    esp_timer_handle_t c_timer;
    esp_timer_create_args_t timer_config = {
        .callback = c_timer_cb, // Function to call when timer expires
        .arg = NULL,                // Argument to pass to the callback
        .dispatch_method = ESP_TIMER_TASK,  // Call the callback from task or from ISR
                                // ESP_TIMER_TASK = from timer task; ESP_TIMER_ISR = from timer ISR
        .name = "1s timer cbs" // Timer name, used in esp_timer_dump function
    };
    ESP_ERROR_CHECK(esp_timer_create(&timer_config, &c_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(c_timer, C_INTERVAL));
}

unsigned short find_parity(uint8_t data){
    unsigned short parity = 0;
    for(int j = 0; j < 8; j++) parity ^= ((data & MASK(j))>>j);
    return parity;
}

void init_tx_buf(char* message, unsigned short char_index){
    tx_buff.bitfield.start = 0;
    tx_buff.bitfield.data = *(message + char_index);
    tx_buff.bitfield.parity = find_parity((uint8_t)tx_buff.bitfield.data);
    tx_buff.bitfield.stop = 1;
}
//==========================================================

//======================Support Function======================
bool is_start_bit(){
    bool res = (IO != IO_prev)&&(!IO);
    return res;
}

void timer_off(esp_timer_handle_t timer) {
    ESP_ERROR_CHECK(esp_timer_stop(timer));
    ESP_ERROR_CHECK(esp_timer_delete(timer));  
}

void c_update();

void uart_send_word (char* message, unsigned short len) {
    tx_param_init();
    tx_timer_init();

    for (unsigned short char_index = 0; char_index < len; ) {

        if (xSemaphoreTake(tx_mutex, portMAX_DELAY)==pdPASS) {
            if (!send_req) {
                if(char_index < len) {
                        init_tx_buf(message, char_index);
                        send_req = 1;
                        char_index ++;
                }
            }

            if (!sending){
                if(send_req){
                    sending = 1;
                }
                else gpio_set_level(TX_PIN, 1);
            }
            else {
                if(bit_index_tx < 11) {
                    gpio_set_level(TX_PIN, (tx_buff.buffer & MASK(bit_index_tx))>>bit_index_tx);
                    bit_index_tx++;
                }
                else {
                    tx_param_init();
                }
            }
        }
    }
    timer_off(tx_timer);           
}

char* uart_recv_word() {
    char * msg;
    msg = malloc(sizeof(char)*15);
    rx_param_init();
    rx_timer_init();
    
    int cnt = 0;
    
    for (unsigned short char_index = 0; char_index < 15; ) {
        if (xSemaphoreTake(rx_mutex, portMAX_DELAY)==pdPASS) {
            if (cnt==(UART_BAUD_RATE*DIVI)) {
                cnt = 0;
                c_update();
            }
            else cnt++;

            IO = gpio_get_level(RX_PIN);

            if(!recv_req) {
                recv_req = 1;
                msg[char_index] = rx_buff.bitfield.data;
                if (msg[char_index] == '!') {
                    msg[char_index++] = '\0';
                    timer_off(rx_timer);
                    return msg;
                    break;
                }
                else char_index++;
            }

            if (!receiving){
                if(is_start_bit()) {
                    receiving = 1;
                    bit_index_rx = 0;
                    tick_index = 1;
                }
            }
            else {
                if (tick_index != ((unsigned short) (DIVI/2) -1)) {
                    if (tick_index < (DIVI -1)) {
                        tick_index++;
                    }
                    else {
                        tick_index = 0;
                        if (bit_index_rx < 10) bit_index_rx++;
                        else {
                            bit_index_rx = 0;
                            receiving = 0;
                            recv_req = 0;
                        }
                    }
                }
                else {
                    //Sampling
                    tick_index++;
                    rx_buff.buffer = (rx_buff.buffer & (~MASK(bit_index_rx))) | (IO<<(bit_index_rx));
                }
            }           
            IO_prev = IO;
        }
    }
    msg = "Run to end loop";
    return msg;
}

void reset_digiclk() {
    digiclk.sec = 0;
    digiclk.min = 0;
    digiclk.hour = 0;
};

void new_hour() {
    digiclk.min = 0;
    if (digiclk.hour == HOUR_LIMIT) digiclk.hour = 0;
    else digiclk.hour ++;
}
void new_min() {
    digiclk.sec = 0;
    if (digiclk.min == SEC_MIN_LIMIT) new_hour();
    else digiclk.min ++;
}
void c_update() {
    if (digiclk.sec == SEC_MIN_LIMIT) new_min();
    else digiclk.sec ++;
}

void c_display() {
    char* msg = malloc(sizeof(char)*50);
    sprintf(msg, "Time: %02d:%02d:%02d\r\n", digiclk.hour, digiclk.min, digiclk.sec);
    uart_send_word(msg, strlen(msg)+1);
}

inline bool c_valid(int hour, int min, int sec) {
    return ((hour <= HOUR_LIMIT)&&(min <= SEC_MIN_LIMIT) && (sec <= SEC_MIN_LIMIT));
}

void c_set(char* data) {
    char* sec = malloc(sizeof(char)*5);
    char* min = malloc(sizeof(char)*5);
    char* hour = malloc(sizeof(char)*5);
    strncpy(hour,data+5,2);
    strncpy(min,data+8,2);
    strncpy(sec,data+11,2);
    if (c_valid(atoi(hour), atoi(min), atoi(sec))) {
        digiclk.sec = atoi(sec);
        digiclk.min = atoi(min);
        digiclk.hour = atoi(hour);
        c_display();
    }
    else {
        uart_send_word("Invalid time\r\n",strlen("Invalid time\r\n")+1);
    }
}
//==========================================================


void app_main(void)
{
    tx_mutex = xSemaphoreCreateBinary();
    if (tx_mutex == NULL) printf("Fail to create tx_mutex");
    rx_mutex = xSemaphoreCreateBinary();
    if (rx_mutex == NULL) printf("Fail to create rx_mutex");
    c_mutex = xSemaphoreCreateBinary();
    if (c_mutex == NULL) printf("Fail to create c_mutex");
    io_init();
    c_timer_init();
    reset_digiclk();
    uart_send_word("Welcome to clock programe\r\n", strlen("Welcome to clock programe\r\n")+1);

    bool uart_mode = RX_MODE;
    char* current_msg = malloc(sizeof(char)*50);

    while (1) {
        if (xSemaphoreTake(c_mutex, portMAX_DELAY)==pdPASS) //Semaphore take from c_isr 
        {
            c_update();
        }

        if (uart_mode==RX_MODE)
        {
            uart_send_word(">>> ", strlen(">>> ")+1);
            current_msg = uart_recv_word();
            uart_mode = TX_MODE;
        }
        else {
            if(!strcmp(current_msg, "AT+R")){
                c_display();
                uart_mode = RX_MODE;
            }
            else if((strstr(current_msg, "AT+W") != NULL)&&(strlen(current_msg)==13)){
                c_set(current_msg);
                uart_mode = RX_MODE;
            }
            else if (!strcmp(current_msg, "quit")){
                uart_send_word("End programe\r\n", strlen("End programe\r\n")+1);
                break;
            }

            else{
                uart_send_word("Invalid command\r\n", strlen("Invalid command\r\n")+1);
                uart_mode = RX_MODE;
            }
        }
    }
}