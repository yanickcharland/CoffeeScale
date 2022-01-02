#ifndef HX711_DEF
#define HX711_DEF

unsigned int PD_SDK;
unsigned int DOUT;

void InitHX711(unsigned int pd_sdk, unsigned int dout);
unsigned long ReadHX711(void);

#endif