/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : PWMR.h
**     Project     : TEAM_Robot
**     Processor   : MK22FX512VLK12
**     Component   : PWM
**     Version     : Component 02.241, Driver 01.01, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-09-29, 15:25, # CodeGen: 0
**     Abstract    :
**         This component implements a pulse-width modulation generator
**         that generates signal with variable duty and fixed cycle. 
**     Settings    :
**          Component name                                 : PWMR
**          PWM or PPG device                              : FTM0_C3V
**          Duty compare                                   : 
**          Output pin                                     : PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/CMP1_OUT
**          Output pin signal                              : PWM_LEFT_J10_01
**          Counter                                        : FTM0_CNT
**          Interrupt service/event                        : Disabled
**          Period                                         : 20 kHz
**          Starting pulse width                           : 0 �s
**          Initial polarity                               : low
**          Same period in modes                           : no
**          Component uses entire timer                    : no
**          Initialization                                 : 
**            Enabled in init. code                        : no
**            Events enabled in init.                      : yes
**          CPU clock/speed selection                      : 
**            High speed mode                              : This component enabled
**            Low speed mode                               : This component disabled
**            Slow speed mode                              : This component disabled
**          Referenced components                          : 
**            PWM_LDD                                      : PWM_LDD
**     Contents    :
**         Enable     - byte PWMR_Enable(void);
**         Disable    - byte PWMR_Disable(void);
**         SetRatio16 - byte PWMR_SetRatio16(word Ratio);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file PWMR.h
** @version 01.01
** @brief
**         This component implements a pulse-width modulation generator
**         that generates signal with variable duty and fixed cycle. 
*/         
/*!
**  @addtogroup PWMR_module PWMR module documentation
**  @{
*/         

#ifndef __PWMR_H
#define __PWMR_H

/* MODULE PWMR. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "PwmLdd2.h"

#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif 


#define PWMR_PERIOD_VALUE  PwmLdd2_PERIOD_VALUE /* Initial period value in ticks of the timer. It is available only if the bean is enabled in high speed mode. */
#define PWMR_PERIOD_VALUE_HIGH PwmLdd2_PERIOD_VALUE_0 /* Period value in ticks of the timer in high speed mode. It is available only if the bean is enabled in high speed mode. */


/*
** ===================================================================
**     Method      :  PWMR_Enable (component PWM)
**     Description :
**         This method enables the component - it starts the signal
**         generation. Events may be generated (<DisableEvent>
**         /<EnableEvent>).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
#define PWMR_Enable() (PwmLdd2_Enable(PwmLdd2_DeviceData))

/*
** ===================================================================
**     Method      :  PWMR_Disable (component PWM)
**     Description :
**         This method disables the component - it stops the signal
**         generation and events calling. When the timer is disabled,
**         it is possible to call <ClrValue> and <SetValue> methods.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
#define PWMR_Disable() (PwmLdd2_Disable(PwmLdd2_DeviceData))

/*
** ===================================================================
**     Method      :  PWMR_SetRatio16 (component PWM)
**     Description :
**         This method sets a new duty-cycle ratio. Ratio is expressed
**         as a 16-bit unsigned integer number. 0 - FFFF value is
**         proportional to ratio 0 - 100%. The method is available
**         only if it is not selected list of predefined values in
**         <Starting pulse width> property. 
**         Note: Calculated duty depends on the timer possibilities and
**         on the selected period.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Ratio           - Ratio to set. 0 - 65535 value is
**                           proportional to ratio 0 - 100%
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
#define PWMR_SetRatio16(Ratio) (PwmLdd2_SetRatio16(PwmLdd2_DeviceData, Ratio))

/* END PWMR. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __PWMR_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
