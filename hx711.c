#include "hx711.h"
#include "pico/stdlib.h"

void InitHX711(unsigned int pd_sdk, unsigned int dout)
{
    PD_SDK = pd_sdk;
    DOUT = dout;

    gpio_init(PD_SDK);
    gpio_init(DOUT);

    gpio_set_dir(PD_SDK, GPIO_OUT);
    gpio_set_dir(DOUT, GPIO_IN);

    gpio_put(PD_SDK, 0);
}

unsigned long ReadHX711(void)
{
    unsigned long ulReturn=0;
    int i;
    
    while (gpio_get(DOUT));
       
    for (i=0; i<24; i++)
    {
        gpio_put(PD_SDK, 1);
        sleep_us(5);
        ulReturn=ulReturn<<1;
        

        gpio_put(PD_SDK, 0);
        sleep_us(5);
        if (gpio_get(DOUT))
            ulReturn++;
    }

    // CLock 25

    gpio_put(PD_SDK, 1);
    sleep_us(5);

    gpio_put(PD_SDK, 0);
    sleep_us(5);

 /*   // CLock 26

    gpio_put(PD_SDK, 1);
    sleep_us(5);

    gpio_put(PD_SDK, 0);
    sleep_us(5);

    // CLock 27

    gpio_put(PD_SDK, 1);
    sleep_us(5);

    gpio_put(PD_SDK, 0);
    sleep_us(5);
*/
    return ulReturn;
}