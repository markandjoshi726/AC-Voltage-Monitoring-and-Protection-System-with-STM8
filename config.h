#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
/* Example only: verify exact STM8 part and board pinout. */
#define REG8(a) (*(volatile uint8_t *)(a))
typedef struct { volatile uint8_t ODR,IDR,DDR,CR1,CR2; } GPIO_TypeDef;
#define GPIOA ((GPIO_TypeDef *)0x5000)
#define GPIOB ((GPIO_TypeDef *)0x5005)
#define GPIOC ((GPIO_TypeDef *)0x500A)
#define GPIOD ((GPIO_TypeDef *)0x500F)
#define PIN1 (1U<<1)
#define PIN2 (1U<<2)
#define PIN3 (1U<<3)
#define PIN4 (1U<<4)
#define PIN5 (1U<<5)
#define PIN6 (1U<<6)
#define PIN7 (1U<<7)
#define CLK_CKDIVR REG8(0x50C6)
#define ADC_CSR REG8(0x5400)
#define ADC_CR1 REG8(0x5401)
#define ADC_CR2 REG8(0x5402)
#define ADC_CR3 REG8(0x5403)
#define ADC_DRH REG8(0x5404)
#define ADC_DRL REG8(0x5405)
#define ADC_TDRL REG8(0x5407)
#define ADC_EOC 0x80U
#define ADC_ADON 0x01U
#define ADC_CHANNEL 3U /* PD2 / AIN3, assumed */
#define ADC_VREF_MV 5000UL
#define ADC_TIMEOUT 60000UL
#define ADC_AVERAGE_SAMPLES 32U
#define DIVIDER_NUM 105UL /* measured ratio reported as 0.0105 */
#define DIVIDER_DEN 10000UL
#define SQRT2_X1000 1414UL
#define RELAY1_PIN PIN1 /* PA1 */
#define RELAY2_PIN PIN2 /* PA2 */
#define RELAY_LOW_LIMIT 240U
#define RELAY_OVERVOLT_LIMIT 245U
#define DIGIT1_PIN PIN4 /* PD4 */
#define DIGIT2_PIN PIN5 /* PD5 */
#define DIGIT3_PIN PIN6 /* PD6 */
#define DISPLAY_REFRESH_CYCLES 20U
#endif
