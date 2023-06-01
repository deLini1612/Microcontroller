# Bai giang tren lop ve interrupt

# Problem
Viet chuong trinh nhan nut bam tren ESP32C3 bang ngat. Nhay LED tuong bao hieu duoc nut bam. Phan tich hoat dong

# Operation Sequence
1. Dau vao (GPIO 1 = 1)
2. Xay ra ngat
3. Ham `trap_entry` duoc goi
4. Ham `handle_trap` duoc goi
5. Ham `handle_trap` goi ham xu ly ngat tuong ung `gpio1_isr_handler`
6. Ham `gpio1_isr_handler` bat led duoc noi voi GPIO3

# Cac buoc chuong trinh can thuc hien
## Khoi tao:
- Khoi tao GPIO 1: la input va co ngat duoc noi voi ngat so 1
- Khoi tao GPIO 3: la output noi PULL-UP
- Thiet lap dia chi vector ngat
- Enable ngat cua GPIO 1 va ngat cua he thong tuong ung voi ngat do
## Xu ly ngat
- Luu gia tri cac thanh ghi
- Kiem tra gia tri cua GPIO 1 = 1
- Nhay LED bang cach dat GPIO 3 = 1 va cho 1s, tat bang cach dat GPIO 3 = 0

## Xac dinh so qua ham can viet
- Can viet cac ham sau (chua quyet dinh minh can viet = C hay assembly)

```C
void gpio_init(int pin, int direction);
void gpio_init_isr (int pin, int isr_nr);
void isr_enable();
void gpio_isr_enable(int pin);
void setup_isr_vector();
void trap_entry();
void handle_trap();
void setup_handler(int isr_nr, (void*) handler);
void gpio1_isr_handler();
```

- Ma gia cua chuong trinh can thuc hien cho `main`, `gpio1_isr_handler`, `setup_handler`, `handle_trap`

```C
void main()
{
    gpio_input(1); // will define INPUT and OUTPUT later
    gpio_output(3);
    gpio_isr_setup(1);
    gpio_isr_enable(1); //enable ngat cua chan 1
    setup_isr_vector(trap_entry); //khi co ngat, dat gia tri trap_entry = mtvec
    setup_handler(1, gpio1_isr_handler); //khi co ngat so 1 xay ra, goi ham gpio1_isr_handler
    isr_enable();
    while(1);
}
```

```C
void gpio1_isr_handler()
{
    if(gpio_read(1) == 1)  // Check if input in pin 1 is 1
    {
        gpio_write(3, 1);  // Set output of pin 3 to 1
        delay_ms(1000);
        gpio_write(3, 0);
    }
}
```

```C
static (void*) isr_handlers[31]; // Mang cua user interrupt
void setup_handler(int isr_nr, (void*) handler)
{
    isr_handlers[isr_nr] = handler;
}
```

```C
int get_mcause()
{
    int mcause;
    asm volatile("csrr %0, mcause" : "=r"(mcause));
    return mcause;
}
```

```C
void handle_trap()
{
    // Lay gia tri thanh ghi mcause
    int isr_nr = get_mcause();
    isr_handlers[isr_nr](); //Lay thanh so mcause
}
```

```C
extern void trap_entry();
void setup_isr_vector()
{
    asm volatile("csrw mtvec, %0" : : "r"(trap_entry));
}
```

# Cac buoc can thuc hien
1. Copy file vector.s, interrupt.c vao thumuc esp32c3
2. Sua file build.mk (nhu cung cap)
3. Copy ham main() va gpio1_isr_handler() vao main.c trong 1 thu muc example