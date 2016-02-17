#include "ir_distance_hw.h"

void system_init( void );


void system_init()
{

    ADC1_Init();
    // Initialize hardware UART1 module on PORTA at 115200 bps, 8 data, no parity and 1 stop bit :

    UART1_Init_Advanced(115200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);


    ir_measure_init( ADC1_Read, 4, RESOLUTION_12 );
    UART1_Write_Text( "Here and Ready\r\n" );
}

void main()
{
    char tmp_text[20];

    system_init();

    while( 1 )
    {
        sprintf( tmp_text, "Distance is: %4.2f\r\n", ir_measure_cm() );
        UART1_Write_Text( tmp_text );
        Delay_ms( 500 );
    }
}