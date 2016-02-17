/*******************************************************************************
* Title                 :   System Initialization
* Filename              :   sys_init.c
* Author                :   JWB
* Origin Date           :   04/23/2012
* Notes                 :   None
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials     Description
*  03/11/15       1.0.0             VM      Module Created.
*
*******************************************************************************/
/** @file ir_distance_hw.c
 *  @brief This module contains functions for calculating the distance between
 *  the IR Distance click and an object
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "ir_distance_hw.h"
#include <math.h>

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
#if defined(__MIKROC_PRO_FOR_AVR__)   || \
    defined(__MIKROC_PRO_FOR_8051__)  || \
    defined(__MIKROC_PRO_FOR_DSPIC__) || \
    defined(__MIKROC_PRO_FOR_PIC32__)
static uint16_t ( *adc_hal_read )( uint8_t channel );
#elif defined( __MIKROC_PRO_FOR_ARM__ )
static uint16_t ( *adc_hal_read )( uint16_t channel );
#endif
static uint8_t read_channel;
static ir_resolution_t read_resolution;

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
#if defined(__MIKROC_PRO_FOR_AVR__)   || \
    defined(__MIKROC_PRO_FOR_8051__)  || \
    defined(__MIKROC_PRO_FOR_DSPIC__) || \
    defined(__MIKROC_PRO_FOR_PIC32__)
void ir_measure_init( uint16_t( *read_adc )( uint8_t adc_channel ),
                      uint8_t channel,
                      ir_resolution_t resolution )
#elif defined( __MIKROC_PRO_FOR_ARM__ )
void ir_measure_init( uint16_t( *read_adc )( uint16_t adc_channel ),
                      uint8_t channel,
                      ir_resolution_t resolution )
#endif
{
    adc_hal_read = read_adc;
    read_channel = channel;
    read_resolution = resolution;
}

double ir_measure_cm()
{
    uint16_t read = adc_hal_read( read_channel );
    float vadc;
    float vref33;
    float b3;
    float a3;
    float threshold;
    double y;
    float a;
    float b;

    switch( read_resolution )
    {
        case RESOLUTION_10:
            vadc = 0.0f;
            vref33 = 0.0032226562f;
            b3 = -1.28f;
            a3 = 21.31f;
            threshold = 2.0f;
            y = 0.0;

            vadc = ( float )read * vref33;

            if ( ( vadc > threshold ) || ( vadc < 0.2f ) )
                return 0.0;

            y = pow( vadc, b3 );
            y *= a3;

            return y;
            break;
        case RESOLUTION_12:
            a = 49.96f;
            b  = -1.34f;
            vref33 = 0.0008058608f;
            vadc;
            threshold = 3.0f;
            y = 0.0;

            vadc = ( float ) read * vref33;

            if ( vadc > threshold )
                return 0.0;

            y = pow( vadc, b );
            y *= a;

            if( y <= 20.0 )
                return y;
            else if( y > 20.0 && y <= 110.0 )
                return y - 3.0f;
            break;

        default:
            return;

    }
}



