/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "Mc32DriverLcd.h"
#include "Mc32DriverAdc.h"
#include "bsp.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {       
            // Initialisation du LCD
            lcd_init();
            lcd_bl_on();
            printf_lcd("Tp0 Led+AD 2023-24");
            lcd_gotoxy(1,2);    // Aller à la deuxième ligne
            printf_lcd("Affolter Jeremy");
            
            // Initialisation de l'adc
            BSP_InitADC10();  
            
            OnLeds();    // Allume toutes les leds
            
            // Start le timer
            DRV_TMR0_Start();
            
            APP_UpdateState(APP_STATE_WAIT);
            break;
        }
        
        case APP_STATE_WAIT:
        {
           // Ne rien faire 
            break;
        }
        
        case APP_STATE_SERVICE_TASKS:
        {
            static uint8_t i = 0;
            // Lecture des ADC
            appData.AdcRes = BSP_ReadAllADC(); 
            
            // Aller à la troisième ligne et afficher les valeurs d'ADC
            lcd_gotoxy(1,3);
            // Afficher les valeurs ADC
            printf_lcd("Ch0 %4d Ch1 %4d", appData.AdcRes.Chan0, appData.AdcRes.Chan1);
            
            if(i == 0)
            {
                // Éteindre toutes les leds la première fois
                ClearLeds();
                i++;
            }
            
            Chenillard();   // Fonction du chenillard
            
            // Passer à l'état wait
            APP_UpdateState(APP_STATE_WAIT);
            break;
        }

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

void APP_UpdateState(APP_STATES NewState)
{
    // Passe à l'état indiqué en entrée
    appData.state = NewState;
}
 
void Chenillard (void)
{
    static uint8_t i = 0;
    
    // Allume la led qui correspond à l'état
    switch(i)
    {
        case 0:
        {
            BSP_LEDOff(BSP_LED_7);
            BSP_LEDOn(BSP_LED_0);
            break;
        }
        case 1:
        {
            BSP_LEDOff(BSP_LED_0);
            BSP_LEDOn(BSP_LED_1);
            break;
        }
        case 2:
        {
            BSP_LEDOff(BSP_LED_1);
            BSP_LEDOn(BSP_LED_2);
            break;
        }
        case 3:
        {
            BSP_LEDOff(BSP_LED_2);
            BSP_LEDOn(BSP_LED_3);
            break;
        }
        case 4:
        {
            BSP_LEDOff(BSP_LED_3);
            BSP_LEDOn(BSP_LED_4);
            break;
        }
        case 5:
        {
            BSP_LEDOff(BSP_LED_4);
            BSP_LEDOn(BSP_LED_5);
            break;
        }
        case 6:
        {
            BSP_LEDOff(BSP_LED_5);
            BSP_LEDOn(BSP_LED_6);
            break;
        }
        case 7:
        {
            BSP_LEDOff(BSP_LED_6);
            BSP_LEDOn(BSP_LED_7);
            break;
        }
    }
    i++;
    if(i>7)
    {
        i = 0;
    }
}

void ClearLeds(void)
{
    // Éteindre toutes les leds
    BSP_LEDOff(BSP_LED_0);
    BSP_LEDOff(BSP_LED_1);
    BSP_LEDOff(BSP_LED_2);
    BSP_LEDOff(BSP_LED_3);
    BSP_LEDOff(BSP_LED_4);
    BSP_LEDOff(BSP_LED_5);
    BSP_LEDOff(BSP_LED_6);
    BSP_LEDOff(BSP_LED_7);
}

void OnLeds(void)
{
    // Allume toutes les leds
    BSP_LEDOn(BSP_LED_0);
    BSP_LEDOn(BSP_LED_1);
    BSP_LEDOn(BSP_LED_2);
    BSP_LEDOn(BSP_LED_3);
    BSP_LEDOn(BSP_LED_4);
    BSP_LEDOn(BSP_LED_5);
    BSP_LEDOn(BSP_LED_6);
    BSP_LEDOn(BSP_LED_7);
}

/*******************************************************************************
 End of File
 */
