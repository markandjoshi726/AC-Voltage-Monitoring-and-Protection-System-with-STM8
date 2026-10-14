/* Educational STM8S003F3P6-style reconstruction, not original project source.
 * Not validated for live mains control. See README.md before use.
 */
#include "config.h"

static const uint8_t patterns[10] = {
  0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F
};
static void relays_off(void) {
  GPIOA->ODR &= (uint8_t)~(RELAY1_PIN | RELAY2_PIN);
}
static void gpio_init(void) {
  relays_off();
  GPIOA->ODR |= PIN3;
  GPIOB->ODR |= PIN4|PIN5;
  GPIOC->ODR |= PIN3|PIN4|PIN5|PIN6|PIN7;
  GPIOD->ODR &= (uint8_t)~(DIGIT1_PIN|DIGIT2_PIN|DIGIT3_PIN);
  GPIOA->DDR |= PIN1|PIN2|PIN3;
  GPIOA->CR1 |= PIN1|PIN2|PIN3;
  GPIOB->DDR |= PIN4|PIN5;
  GPIOB->CR1 |= PIN4|PIN5;
  GPIOC->DDR |= PIN3|PIN4|PIN5|PIN6|PIN7;
  GPIOC->CR1 |= PIN3|PIN4|PIN5|PIN6|PIN7;
  GPIOD->DDR |= DIGIT1_PIN|DIGIT2_PIN|DIGIT3_PIN;
  GPIOD->CR1 |= DIGIT1_PIN|DIGIT2_PIN|DIGIT3_PIN;
  GPIOD->DDR &= (uint8_t)~PIN2; /* AIN3 input */
  GPIOD->CR1 &= (uint8_t)~PIN2;
}
static void adc_init(void) {
  ADC_CSR = ADC_CHANNEL;
  ADC_CR1 = 0x70U; /* prescaler /18 */
  ADC_CR2 = 0x08U; /* right aligned */
  ADC_CR3 = 0U;
  ADC_TDRL |= PIN3;
  ADC_CR1 |= ADC_ADON;
}
static uint8_t adc_read(uint16_t *value) {
  uint32_t wait = 0UL;
  uint8_t low, high;
  ADC_CSR &= (uint8_t)~ADC_EOC;
  ADC_CR1 |= ADC_ADON;
  while ((ADC_CSR & ADC_EOC)==0U) {
    if (++wait >= ADC_TIMEOUT) return 0U;
  }
  low = ADC_DRL;
  high = ADC_DRH;
  *value = ((uint16_t)high<<8) | low;
  ADC_CSR &= (uint8_t)~ADC_EOC;
  return (*value <= 1023U) ? 1U : 0U;
}
static uint8_t adc_average(uint16_t *value) {
  uint32_t sum=0UL;
  uint16_t sample;
  uint8_t i;
  for (i=0U;i<ADC_AVERAGE_SAMPLES;++i) {
    if (!adc_read(&sample)) return 0U;
    sum += sample;
  }
  *value=(uint16_t)(sum/ADC_AVERAGE_SAMPLES);
  return 1U;
}
static uint16_t adc_to_vrms(uint16_t sample) {
  uint32_t adc_mv=((uint32_t)sample*ADC_VREF_MV)/1023UL;
  uint32_t peak_mv=(adc_mv*DIVIDER_DEN)/DIVIDER_NUM;
  uint32_t rms_mv=(peak_mv*1000UL)/SQRT2_X1000;
  return (uint16_t)((rms_mv+500UL)/1000UL);
}
static void control_relays(uint16_t v) {
  relays_off(); /* break before make */
  if (v > RELAY_OVERVOLT_LIMIT) return;
  if (v >= RELAY_LOW_LIMIT) GPIOA->ODR |= RELAY1_PIN;
  else GPIOA->ODR |= RELAY2_PIN;
}
static void segment(GPIO_TypeDef *port,uint8_t pin,uint8_t on) {
  if(on) port->ODR &= (uint8_t)~pin;
  else port->ODR |= pin;
}
static void digits_off(void) {
  GPIOD->ODR &= (uint8_t)~(DIGIT1_PIN|DIGIT2_PIN|DIGIT3_PIN);
}
static void set_segments(uint8_t pattern) {
  segment(GPIOA,PIN3,pattern&0x01U);
  segment(GPIOB,PIN4,pattern&0x02U);
  segment(GPIOB,PIN5,pattern&0x04U);
  segment(GPIOC,PIN3,pattern&0x08U);
  segment(GPIOC,PIN4,pattern&0x10U);
  segment(GPIOC,PIN5,pattern&0x20U);
  segment(GPIOC,PIN6,pattern&0x40U);
  segment(GPIOC,PIN7,pattern&0x80U);
}
static void display_digit(uint8_t n,uint8_t value) {
  volatile uint16_t i;
  digits_off();
  set_segments(patterns[value]);
  if(n==0U) GPIOD->ODR |= DIGIT1_PIN;
  else if(n==1U) GPIOD->ODR |= DIGIT2_PIN;
  else GPIOD->ODR |= DIGIT3_PIN;
  for(i=0U;i<1600U;++i) { }
  digits_off();
}
static void display_voltage(uint16_t voltage) {
  if(voltage>999U) voltage=999U;
  display_digit(0U,(uint8_t)(voltage/100U));
  display_digit(1U,(uint8_t)((voltage/10U)%10U));
  display_digit(2U,(uint8_t)(voltage%10U));
}
int main(void) {
  uint16_t sample=0U,voltage=0U;
  uint8_t i;
  CLK_CKDIVR=0U;
  gpio_init();
  relays_off();
  adc_init();
  while(1) {
    if(!adc_average(&sample)) {
      relays_off();
      voltage=0U; /* fault placeholder, not valid measurement */
    } else {
      voltage=adc_to_vrms(sample);
      control_relays(voltage);
    }
    for(i=0U;i<DISPLAY_REFRESH_CYCLES;++i)
      display_voltage(voltage);
  }
}
