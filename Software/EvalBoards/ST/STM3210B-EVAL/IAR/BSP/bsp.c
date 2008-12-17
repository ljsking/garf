/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_MODULE
#include <bsp.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
                                                                /* -------------------- GPIOA PINS -------------------- */
#define  BSP_GPIOA_PB_WAKEUP                      DEF_BIT_00
#define  BSP_GPIOA_UART1_TX                       DEF_BIT_09
#define  BSP_GPIOA_UART1_RX                       DEF_BIT_10
#define  BSP_GPIOA_LCD_BL                         DEF_BIT_14
#define  BSP_GPIOA_SW_LED_BUTTON_YELLOW           DEF_BIT_06
#define  BSP_GPIOA_SW_LED_BUTTON_GREEN            DEF_BIT_05
#define  BSP_GPIOA_SW_LED_GREEN                   DEF_BIT_04

#define  BSP_GPIOA_7_Seg_C_2                      DEF_BIT_07

                                                                /* -------------------- GPIOB PINS -------------------- */
#define  BSP_GPIOB_LCD_CS                         DEF_BIT_02
#define  BSP_GPIOB_TEMP_INT                       DEF_BIT_05
#define  BSP_GPIOB_TEMP_SCL                       DEF_BIT_06

#define  BSP_GPIOB_AUDIO_PWM                      DEF_BIT_08
#define  BSP_GPIOB_PB_KEY                         DEF_BIT_09
#define  BSP_GPIOB_LCD_SCL                        DEF_BIT_13
#define  BSP_GPIOB_LCD_SDO                        DEF_BIT_14
#define  BSP_GPIOB_LCD_SDI                        DEF_BIT_15

#define  BSP_GPIOB_7_Seg_B_1                      DEF_BIT_13
#define  BSP_GPIOB_7_Seg_A_1                      DEF_BIT_12
#define  BSP_GPIOB_7_Seg_G_1                      DEF_BIT_08
#define  BSP_GPIOB_7_Seg_D_1                      DEF_BIT_10
#define  BSP_GPIOB_7_Seg_E_1                      DEF_BIT_02
#define  BSP_GPIOB_7_Seg_C_1                      DEF_BIT_01

#define  BSP_GPIOB_SW_LED_RED                     DEF_BIT_14
#define  BSP_GPIOB_SW_LED_BUTTON_RED              DEF_BIT_11
#define  BSP_GPIOB_SW_LED_YELLOW                  DEF_BIT_00
                                                                /* -------------------- GPIOC PINS -------------------- */
#define  BSP_GPIOC_SW_1                           DEF_BIT_07
#define  BSP_GPIOC_LED3                           DEF_BIT_08
#define  BSP_GPIOC_LED4                           DEF_BIT_09
#define  BSP_GPIOC_SW_LED_BLUE                    DEF_BIT_02
#define  BSP_GPIOC_SW_LED_BUTTON_BLUE             DEF_BIT_03
#define  BSP_GPIOC_7_Seg_F_1                      DEF_BIT_13

#define  BSP_GPIOC_7_Seg_A_2                      DEF_BIT_15
#define  BSP_GPIOC_7_Seg_B_2                      DEF_BIT_14
#define  BSP_GPIOC_7_Seg_E_2                      DEF_BIT_05
#define  BSP_GPIOC_7_Seg_D_2                      DEF_BIT_04
#define  BSP_GPIOC_7_Seg_F_2                      DEF_BIT_00
#define  BSP_GPIOC_7_Seg_G_2                      DEF_BIT_01
                                                                /* -------------------- GPIOD PINS -------------------- */
#define  BSP_GPIOD_CAN_RX                         DEF_BIT_00
#define  BSP_GPIOD_CAN_TX                         DEF_BIT_01
#define  BSP_GPIOD_UART2_CTS                      DEF_BIT_03
#define  BSP_GPIOD_UART2_RTS                      DEF_BIT_04
#define  BSP_GPIOD_UART2_TX                       DEF_BIT_05
#define  BSP_GPIOD_UART2_RX                       DEF_BIT_06
#define  BSP_GPIOD_LCD_RS                         DEF_BIT_07
#define  BSP_GPIOD_JOY_UP                         DEF_BIT_08
#define  BSP_GPIOD_JOY_SEL                        DEF_BIT_12
#define  BSP_GPIOD_JOY_DOWN                       DEF_BIT_14
#define  BSP_GPIOD_LCD_RD                         DEF_BIT_15

                                                                /* -------------------- GPIOE PINS -------------------- */
#define  BSP_GPIOE_JOY_LEFT                       DEF_BIT_00
#define  BSP_GPIOE_JOY_RIGHT                      DEF_BIT_01

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_ADC_Init     (void);

static  void  BSP_LED_Init     (void);

static  void  BSP_7Segs_Init     (void);

static  void  BSP_PB_Init      (void);

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    RCC_WaitForHSEStartUp();


    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    FLASH_SetLatency(FLASH_Latency_2);
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);

    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
        ;
    }

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    while (RCC_GetSYSCLKSource() != 0x08) {
        ;
    }

    BSP_ADC_Init();                                             /* Initialize the I/Os for the ADC      controls.       */
    BSP_LED_Init();                                             /* Initialize the I/Os for the LED      controls.       */
    BSP_7Segs_Init();                                             /* Initialize the I/Os for the LED      controls.       */
    BSP_PB_Init();                                              /* Initialize the I/Os for the PB       control.        */
}

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                         OS CORTEX-M3 FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         OS_CPU_SysTickClkFreq()
*
* Description : Get system tick clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : Clock frequency (of system tick).
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;

    freq = BSP_CPU_ClkFreq();
    return (freq);
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                              ADC FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             BSP_ADC_Init()
*
* Description : Initialize the board's ADC
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_ADC_Init (void)
{
    ADC_InitTypeDef   adc_init;
    GPIO_InitTypeDef  gpio_init;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    gpio_init.GPIO_Pin  = GPIO_Pin_4;
    gpio_init.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &gpio_init);

    adc_init.ADC_Mode               = ADC_Mode_Independent;
    adc_init.ADC_ScanConvMode       = DISABLE;
    adc_init.ADC_ContinuousConvMode = ENABLE;
    adc_init.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
    adc_init.ADC_DataAlign          = ADC_DataAlign_Right;
    adc_init.ADC_NbrOfChannel       = 1;
    ADC_Init(ADC1, &adc_init);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_13Cycles5);
    ADC_Cmd(ADC1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


/*
*********************************************************************************************************
*                                           BSP_ADC_GetStatus()
*
* Description : This function initializes the board's ADC
*
* Argument(s) : adc             ID of the ADC to probe.  For this board, the only legitimate value is 1.
*
* Return(s)   : The numerator of the binary fraction representing the result of the latest ADC conversion.
*               This value will be a 12-bit value between 0x0000 and 0x0FFF, inclusive.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT16U  BSP_ADC_GetStatus (CPU_INT08U  adc)
{
    CPU_INT16U  result;


    result = 0;

    if (adc == 1) {
        result = ADC_GetConversionValue(ADC1);
    }

    return (result);
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               PB FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              BSP_PB_Init()
*
* Description : Initialize the board's PB.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_PB_Init (void)
{
    GPIO_InitTypeDef  gpio_init;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
      
    gpio_init.GPIO_Pin  = BSP_GPIOA_SW_LED_BUTTON_YELLOW|BSP_GPIOA_SW_LED_BUTTON_GREEN;
    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio_init);
    
    gpio_init.GPIO_Pin  = BSP_GPIOB_SW_LED_BUTTON_RED;
    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &gpio_init);
    
    gpio_init.GPIO_Pin  = BSP_GPIOC_SW_LED_BUTTON_BLUE;
    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &gpio_init);
}

/*
*********************************************************************************************************
*                                           BSP_PB_GetStatus()
*
* Description : Get the status of a push button on the board.
*
* Return(s)   : DEF_FALSE   if the push button is pressed.
*               DEF_TRUE    if the push button is not pressed.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_PB_GetStatus ()
{
    CPU_BOOLEAN  status;
    CPU_INT32U   pin;
    
    status = 0;
    
    pin    = GPIO_ReadInputDataBit(GPIOB, BSP_GPIOB_SW_LED_BUTTON_RED);
    if (pin == 0) {
        status |= BSP_PUSH_BUTTON_1;
    }
    
    pin    = GPIO_ReadInputDataBit(GPIOA, BSP_GPIOA_SW_LED_BUTTON_YELLOW);
    if (pin == 0) {
        status |= BSP_PUSH_BUTTON_2;
    }
    
    pin    = GPIO_ReadInputDataBit(GPIOA, BSP_GPIOA_SW_LED_BUTTON_GREEN);
    if (pin == 0) {
        status |= BSP_PUSH_BUTTON_3;
    }
    
    pin    = GPIO_ReadInputDataBit(GPIOC, BSP_GPIOC_SW_LED_BUTTON_BLUE);
    if (pin == 0) {
        status |= BSP_PUSH_BUTTON_4;
    }
    
    return (status);
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                              LED FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             BSP_LED_Init()
*
* Description : Initialize the I/O for the LEDs
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_LED_Init (void)
{
    GPIO_InitTypeDef  gpio_init;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

    gpio_init.GPIO_Pin   = BSP_GPIOA_SW_LED_GREEN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &gpio_init);
    
    gpio_init.GPIO_Pin   =  BSP_GPIOC_LED3 | BSP_GPIOC_LED4 | BSP_GPIOC_SW_LED_BLUE;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &gpio_init);
    
    gpio_init.GPIO_Pin   =  BSP_GPIOB_SW_LED_RED | BSP_GPIOB_SW_LED_YELLOW;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &gpio_init);
}

/*
*********************************************************************************************************
*                                             BSP_LED_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turn ON all LEDs on the board
*                       1    turn ON LED 1
*                       2    turn ON LED 2
*                       3    turn ON LED 3
*                       4    turn ON LED 4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_On (CPU_INT08U led)
{
    switch (led) {
        case 0:
             GPIO_SetBits(GPIOA, BSP_GPIOA_SW_LED_GREEN);
             GPIO_SetBits(GPIOC, BSP_GPIOC_SW_LED_BLUE);
             GPIO_ResetBits(GPIOC, BSP_GPIOC_LED3|BSP_GPIOC_LED4);
             GPIO_SetBits(GPIOB, BSP_GPIOB_SW_LED_RED|BSP_GPIOB_SW_LED_YELLOW);
             break;
             
        case 1:
             GPIO_SetBits(GPIOB, BSP_GPIOB_SW_LED_RED);
             break;
             
        case 2:
             GPIO_SetBits(GPIOB, BSP_GPIOB_SW_LED_YELLOW);
             break;
             
        case 3:
             GPIO_SetBits(GPIOA, BSP_GPIOA_SW_LED_GREEN);
             break;
             
        case 4:
             GPIO_SetBits(GPIOC, BSP_GPIOC_SW_LED_BLUE);
             break;
             
        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                              BSP_LED_Off()
*
* Description : Turn OFF any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turn OFF all LEDs on the board
*                       1    turn OFF LED 1
*                       2    turn OFF LED 2
*                       3    turn OFF LED 3
*                       4    turn OFF LED 4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Off (CPU_INT08U led)
{
    switch (led) {
        case 0:
             GPIO_ResetBits(GPIOA, BSP_GPIOA_SW_LED_GREEN);
             GPIO_ResetBits(GPIOC, BSP_GPIOC_SW_LED_BLUE);
             GPIO_SetBits(GPIOC, BSP_GPIOC_LED3|BSP_GPIOC_LED4);
             GPIO_ResetBits(GPIOB, BSP_GPIOB_SW_LED_RED|BSP_GPIOB_SW_LED_YELLOW);
             break;
             
        case 1:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_SW_LED_RED);
             break;
             
        case 2:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_SW_LED_YELLOW);
             break;
             
        case 3:
             GPIO_ResetBits(GPIOA, BSP_GPIOA_SW_LED_GREEN);
             break;
             
        case 4:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_SW_LED_BLUE);
             break;
             
        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                            BSP_LED_Toggle()
*
* Description : TOGGLE any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    TOGGLE all LEDs on the board
*                       1    TOGGLE LED 1
*                       2    TOGGLE LED 2
*                       3    TOGGLE LED 3
*                       4    TOGGLE LED 4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (CPU_INT08U led)
{
    CPU_INT32U  pins;


    pins = GPIO_ReadOutputData(GPIOC);

    switch (led) {
        case 0:
             BSP_LED_Toggle(3);
             BSP_LED_Toggle(4);
             break;


        case 3:
             if ((pins & BSP_GPIOC_LED3) == 0) {
                 GPIO_SetBits(  GPIOC, BSP_GPIOC_LED3);
             } else {
                 GPIO_ResetBits(GPIOC, BSP_GPIOC_LED3);
             }
            break;

        case 4:
             if ((pins & BSP_GPIOC_LED4) == 0) {
                 GPIO_SetBits(  GPIOC, BSP_GPIOC_LED4);
             } else {
                 GPIO_ResetBits(GPIOC, BSP_GPIOC_LED4);
             }
            break;

        default:
             break;
    }
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                              7 Segments FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             BSP_7Segs_Init()
*
* Description : Initialize the I/O for the 7 Segments
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_7Segs_Init (void)
{
    GPIO_InitTypeDef  gpio_init;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
    
    gpio_init.GPIO_Pin   =  BSP_GPIOA_7_Seg_C_2;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &gpio_init);

    gpio_init.GPIO_Pin   =  BSP_GPIOB_7_Seg_A_1 | BSP_GPIOB_7_Seg_B_1 | BSP_GPIOB_7_Seg_C_1 | 
      BSP_GPIOB_7_Seg_D_1 | BSP_GPIOB_7_Seg_E_1 | BSP_GPIOB_7_Seg_G_1;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &gpio_init);
    
    
    gpio_init.GPIO_Pin   =  BSP_GPIOC_7_Seg_F_1 | BSP_GPIOC_7_Seg_A_2 | BSP_GPIOC_7_Seg_B_2 |
      BSP_GPIOC_7_Seg_D_2 | BSP_GPIOC_7_Seg_E_2 | BSP_GPIOC_7_Seg_F_2 | BSP_GPIOC_7_Seg_G_2;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &gpio_init);
}

/*
*********************************************************************************************************
*                                             BSP_7Segs_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : seg     The ID of the seg to control:
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_7Segs (CPU_INT08U seg)
{
   GPIO_SetBits(GPIOA, BSP_GPIOA_7_Seg_C_2);
   GPIO_SetBits(GPIOB, BSP_GPIOB_7_Seg_A_1|BSP_GPIOB_7_Seg_B_1|BSP_GPIOB_7_Seg_C_1
                          |BSP_GPIOB_7_Seg_D_1|BSP_GPIOB_7_Seg_E_1|BSP_GPIOB_7_Seg_G_1);
   GPIO_SetBits(GPIOC, BSP_GPIOC_7_Seg_F_1 | BSP_GPIOC_7_Seg_A_2 | BSP_GPIOC_7_Seg_B_2 |
      BSP_GPIOC_7_Seg_D_2 | BSP_GPIOC_7_Seg_E_2 | BSP_GPIOC_7_Seg_F_2 | BSP_GPIOC_7_Seg_G_2);
   switch (seg) {
        case 0:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_7_Seg_A_1|BSP_GPIOB_7_Seg_B_1|BSP_GPIOB_7_Seg_C_1
                          |BSP_GPIOB_7_Seg_D_1|BSP_GPIOB_7_Seg_E_1);
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_F_1);
             break;
        case 1:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_7_Seg_B_1|BSP_GPIOB_7_Seg_C_1);
             break;
        case 2:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_7_Seg_A_1|BSP_GPIOB_7_Seg_B_1|BSP_GPIOB_7_Seg_D_1
                          |BSP_GPIOB_7_Seg_E_1|BSP_GPIOB_7_Seg_G_1);
             break;
        case 3:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_7_Seg_A_1|BSP_GPIOB_7_Seg_B_1|BSP_GPIOB_7_Seg_C_1
                          |BSP_GPIOB_7_Seg_D_1|BSP_GPIOB_7_Seg_G_1);
             break;
        case 4:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_7_Seg_B_1|BSP_GPIOB_7_Seg_C_1|BSP_GPIOB_7_Seg_G_1);
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_F_1);
             break;
        case 5:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_7_Seg_A_1|BSP_GPIOB_7_Seg_C_1|BSP_GPIOB_7_Seg_D_1
                          |BSP_GPIOB_7_Seg_G_1);
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_F_1);
             break;
        case 6:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_7_Seg_A_1|BSP_GPIOB_7_Seg_C_1|BSP_GPIOB_7_Seg_D_1
                          |BSP_GPIOB_7_Seg_E_1|BSP_GPIOB_7_Seg_G_1);
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_F_1);
             break;
        case 7:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_7_Seg_A_1|BSP_GPIOB_7_Seg_B_1|BSP_GPIOB_7_Seg_C_1);
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_F_1);
             break;
        case 8:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_7_Seg_A_1|BSP_GPIOB_7_Seg_B_1|BSP_GPIOB_7_Seg_C_1
                          |BSP_GPIOB_7_Seg_D_1|BSP_GPIOB_7_Seg_E_1|BSP_GPIOB_7_Seg_G_1);
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_F_1);
             break;
        case 9:
             GPIO_ResetBits(GPIOB, BSP_GPIOB_7_Seg_A_1|BSP_GPIOB_7_Seg_B_1|BSP_GPIOB_7_Seg_C_1
                          |BSP_GPIOB_7_Seg_D_1|BSP_GPIOB_7_Seg_G_1);
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_F_1);
             break;
        default:
             break;
    }
    switch (seg) {
        case 0:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_A_2|BSP_GPIOC_7_Seg_B_2|BSP_GPIOC_7_Seg_D_2
                          |BSP_GPIOC_7_Seg_E_2|BSP_GPIOC_7_Seg_F_2);
             GPIO_ResetBits(GPIOA, BSP_GPIOA_7_Seg_C_2);
             break;
        case 1:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_B_2);
             GPIO_ResetBits(GPIOA, BSP_GPIOA_7_Seg_C_2);
             break;
        case 2:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_A_2|BSP_GPIOC_7_Seg_B_2|BSP_GPIOC_7_Seg_D_2
                          |BSP_GPIOC_7_Seg_E_2|BSP_GPIOC_7_Seg_G_2);
             break;
        case 3:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_A_2|BSP_GPIOC_7_Seg_B_2|BSP_GPIOC_7_Seg_D_2
                          |BSP_GPIOC_7_Seg_G_2);
             GPIO_ResetBits(GPIOA, BSP_GPIOA_7_Seg_C_2);
             break;
        case 4:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_B_2|BSP_GPIOC_7_Seg_F_2|BSP_GPIOC_7_Seg_G_2);
             GPIO_ResetBits(GPIOA, BSP_GPIOA_7_Seg_C_2);
             break;
        case 5:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_A_2|BSP_GPIOC_7_Seg_D_2
                          |BSP_GPIOC_7_Seg_F_2|BSP_GPIOC_7_Seg_G_2);
             GPIO_ResetBits(GPIOA, BSP_GPIOA_7_Seg_C_2);
             break;
        case 6:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_A_2|BSP_GPIOC_7_Seg_D_2
                          |BSP_GPIOC_7_Seg_E_2|BSP_GPIOC_7_Seg_F_2|BSP_GPIOC_7_Seg_G_2);
             GPIO_ResetBits(GPIOA, BSP_GPIOA_7_Seg_C_2);
             break;
        case 7:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_A_2|BSP_GPIOC_7_Seg_B_2
                          |BSP_GPIOC_7_Seg_F_2);
             GPIO_ResetBits(GPIOA, BSP_GPIOA_7_Seg_C_2);
             break;
        case 8:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_A_2|BSP_GPIOC_7_Seg_B_2|BSP_GPIOC_7_Seg_D_2
                          |BSP_GPIOC_7_Seg_E_2|BSP_GPIOC_7_Seg_F_2|BSP_GPIOC_7_Seg_G_2);
             GPIO_ResetBits(GPIOA, BSP_GPIOA_7_Seg_C_2);
             break;
        case 9:
             GPIO_ResetBits(GPIOC, BSP_GPIOC_7_Seg_A_2|BSP_GPIOC_7_Seg_B_2|BSP_GPIOC_7_Seg_D_2
                          |BSP_GPIOC_7_Seg_F_2|BSP_GPIOC_7_Seg_G_2);
             GPIO_ResetBits(GPIOA, BSP_GPIOA_7_Seg_C_2);
             break;
        default:
             break;
    }
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           OS PROBE FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           OSProbe_TmrInit()
*
* Description : Select & initialize a timer for use with the uC/Probe Plug-In for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : OSProbe_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == 1))
void  OSProbe_TmrInit (void)
{
    TIM_TimeBaseInitTypeDef  tim_init;


    tim_init.TIM_Period        = 0xFFFF;
    tim_init.TIM_Prescaler     = 0x00;
    tim_init.TIM_ClockDivision = 0x0;
    tim_init.TIM_CounterMode   = TIM_CounterMode_Up;

#if (OS_PROBE_TIMER_SEL == 2)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInit(TIM2, &tim_init);
    TIM_SetCounter(TIM2, 0);
    TIM_PrescalerConfig(TIM2, 256, TIM_PSCReloadMode_Immediate);
    TIM_Cmd(TIM2, ENABLE);
#elif (OS_PROBE_TIMER_SEL == 3)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInit(TIM3, &tim_init);
    TIM_SetCounter(TIM3, 0);
    TIM_PrescalerConfig(TIM3, 256, TIM_PSCReloadMode_Immediate);
    TIM_Cmd(TIM3, ENABLE);
#elif (OS_PROBE_TIMER_SEL == 4)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseInit(TIM4, &tim_init);
    TIM_SetCounter(TIM4, 0);
    TIM_PrescalerConfig(TIM4, 256, TIM_PSCReloadMode_Immediate);
    TIM_Cmd(TIM4, ENABLE);
#endif
}
#endif

/*
*********************************************************************************************************
*                                            OSProbe_TmrRd()
*
* Description : Read the current counts of a 16-bit free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 16-bit counts of the timer in a 32-bit variable.
*
* Caller(s)   : OSProbe_TimeGetCycles().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == 1))
CPU_INT32U  OSProbe_TmrRd (void)
{
#if (OS_PROBE_TIMER_SEL == 2)

    return ((CPU_INT32U)TIM_GetCounter(TIM2));

#elif (OS_PROBE_TIMER_SEL == 3)

    return ((CPU_INT32U)TIM_GetCounter(TIM3));

#elif (OS_PROBE_TIMER_SEL == 4)

    return ((CPU_INT32U)TIM_GetCounter(TIM4));

#endif
}
#endif
