#include <stdio.h>

void sayHello(char* name);
int add(int x, int y);
int sub(int x, int y);
int mul(int x, int y);

int main()
{
    char* name = "Linh";
    void (*Welcome) (char*) = &sayHello;

    typedef int (*math) (int, int);
    math cal[3] = {add, sub, mul};

    int a = 10, b = 7;

    Welcome(name);
    printf("Your 2 number are %d and %d\n", a, b);
    printf("what operator do you want? \n 0: Add \n 1: Sub \n 2: Multiple \n");
    int choie;
    printf("Enter your choice: ");
    scanf("%d", &choie);
    if (choie < 3) printf("%d\n", cal[choie](a, b));
}

void sayHello(char* name)
{
    printf("Welcome to my programe %s \n", name);
}

int add(int x, int y)
{
    return x + y;
}

int sub(int x, int y)
{
    return x - y;
}

int mul(int x, int y)
{
    return x * y;
}