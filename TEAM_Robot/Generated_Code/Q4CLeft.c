/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : Q4CLeft.c
**     Project     : TEAM_Robot
**     Processor   : MK22FX512VLK12
**     Component   : QuadCounter
**     Version     : Component 01.031, Driver 01.00, CPU db: 3.00.000
**     Repository  : My Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-09-29, 15:25, # CodeGen: 0
**     Abstract    :
**
This driver implements a quadrature encoder using two signals (C1 and C2) to generate position information.
**     Settings    :
**          Component name                                 : Q4CLeft
**          C1 and C2 swapped                              : no
**          Counter Type                                   : 32bit
**          Method                                         : 
**            Sampling                                     : Enabled
**              Error Correction                           : no
**              C1                                         : Cx
**              C2                                         : Cx
**            Input Capture                                : Disabled
**          Shell                                          : Enabled
**            Shell                                        : CLS1
**            Utility                                      : UTIL1
**          SDK                                            : MCUC1
**     Contents    :
**         GetPos       - Q4CLeft_QuadCntrType Q4CLeft_GetPos(void);
**         SetPos       - void Q4CLeft_SetPos(Q4CLeft_QuadCntrType pos);
**         GetVal       - uint8_t Q4CLeft_GetVal(void);
**         Sample       - void Q4CLeft_Sample(void);
**         NofErrors    - uint16_t Q4CLeft_NofErrors(void);
**         SwapPins     - uint8_t Q4CLeft_SwapPins(bool swap);
**         Deinit       - void Q4CLeft_Deinit(void);
**         Init         - void Q4CLeft_Init(void);
**         ParseCommand - uint8_t Q4CLeft_ParseCommand(const unsigned char *cmd, bool *handled, const...
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2014, all rights reserved.
**     This an open source software using Processor Expert.
**     This is a free software and is opened for education,  research  and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file Q4CLeft.c
** @version 01.00
** @brief
**
This driver implements a quadrature encoder using two signals (C1 and C2) to generate position information.
*/         
/*!
**  @addtogroup Q4CLeft_module Q4CLeft module documentation
**  @{
*/         

/* MODULE Q4CLeft. */

#include "Q4CLeft.h"

#if Q4CLeft_SWAP_PINS_AT_RUNTIME
  static bool Q4CLeft_swappedPins = FALSE;
#endif

/* The decoder has 4 different states, together with the previous state the table has 16 entries.
   The value in the table (0,1,-1) indicates the steps taken since previous sample. */
#define QUAD_ERROR  3 /*!< Value to indicate an error in impulse detection. Has to be different from 0,1,-1 */

static const signed char Q4CLeft_Quad_Table[4][4] =
  {               /* prev   new    */
    {             /* c1 c2  c1 c2  */
     0,           /* 0  0   0  0  no change or missed a step? */
     1,           /* 0  0   0  1   */
     -1,          /* 0  0   1  0   */
     QUAD_ERROR   /* 0  0   1  1  error, lost impulse */
     },
    {             /* c1 c2  c1 c2  */
     -1,          /* 0  1   0  0   */
     0,           /* 0  1   0  1   no change or missed a step? */
     QUAD_ERROR,  /* 0  1   1  0   error, lost impulse */
     1            /* 0  1   1  1   */
     },
    {             /* c1 c2  c1 c2  */
     1,           /* 1  0   0  0   */
     QUAD_ERROR,  /* 1  0   0  1   error, lost impulse */
     0,           /* 1  0   1  0   no change or missed a step? */
     -1           /* 1  0   1  1   */
     },
    {             /* c1 c2  c1 c2  */
     QUAD_ERROR,  /* 1  1   0  0   error, lost impulse */
     -1,          /* 1  1   0  1   */
     1,           /* 1  1   1  0   */
     0            /* 1  1   1  1   no change or missed a step? */
     }
  };
static uint8_t Q4CLeft_last_quadrature_value; /*! Value of C1&C2 during last round. */

static Q4CLeft_QuadCntrType Q4CLeft_currPos = 0; /*!< Current position */
static uint16_t Q4CLeft_nofErrors = 0;

/*
** ===================================================================
**     Method      :  Q4CLeft_SetPos (component QuadCounter)
**     Description :
**         Sets the position information. Can be used as well to reset
**         the position information.
**     Parameters  :
**         NAME            - DESCRIPTION
**         pos             - Position value to be set.
**     Returns     : Nothing
** ===================================================================
*/
void Q4CLeft_SetPos(Q4CLeft_QuadCntrType pos)
{
  Q4CLeft_currPos = pos;
}

/*
** ===================================================================
**     Method      :  Q4CLeft_GetPos (component QuadCounter)
**     Description :
**         Returns the current position based on the encoder tracking.
**     Parameters  : None
**     Returns     :
**         ---             - position
** ===================================================================
*/
Q4CLeft_QuadCntrType Q4CLeft_GetPos(void)
{
  return Q4CLeft_currPos;
}

/*
** ===================================================================
**     Method      :  Q4CLeft_GetVal (component QuadCounter)
**     Description :
**         Returns the quadrature value (0, 1, 2 or 3)
**     Parameters  : None
**     Returns     :
**         ---             - Quadrature value (0-3)
** ===================================================================
*/
uint8_t Q4CLeft_GetVal(void)
{
#if Q4CLeft_SWAP_PINS_AT_RUNTIME
  if (Q4CLeft_swappedPins) {
    return Q4CLeft_GET_C1_C2_PINS();
  } else {
    return Q4CLeft_GET_C1_C2_PINS_SWAPPED();
  }
#else
  return Q4CLeft_GET_C1_C2_PINS();
#endif
}

/*
** ===================================================================
**     Method      :  Q4CLeft_Sample (component QuadCounter)
**     Description :
**         Call this method to periodically sample the signals.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Q4CLeft_Sample(void)
{
  signed char new_step;
  uint8_t c12; /* value of the two sensor input */

  c12 = Q4CLeft_GetVal();
  new_step = Q4CLeft_Quad_Table[Q4CLeft_last_quadrature_value][c12];
  Q4CLeft_last_quadrature_value = c12;
  if (new_step == QUAD_ERROR) {
    Q4CLeft_nofErrors++;
  } else if (new_step != 0) {
    Q4CLeft_currPos += new_step;
  }
}

/*
** ===================================================================
**     Method      :  Q4CLeft_NofErrors (component QuadCounter)
**     Description :
**         Returns the number of decoding errors
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint16_t Q4CLeft_NofErrors(void)
{
  return Q4CLeft_nofErrors;
}

/*
** ===================================================================
**     Method      :  Q4CLeft_Deinit (component QuadCounter)
**     Description :
**         Module de-initialization method
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Q4CLeft_Deinit(void)
{
  /* nothing needed */
}

/*
** ===================================================================
**     Method      :  Q4CLeft_Init (component QuadCounter)
**     Description :
**         Module initialization method
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Q4CLeft_Init(void)
{
  Q4CLeft_currPos = 0;
  Q4CLeft_last_quadrature_value = Q4CLeft_GET_C1_C2_PINS();
  Q4CLeft_nofErrors = 0;
#if Q4CLeft_SWAP_PINS_AT_RUNTIME
  Q4CLeft_swappedPins = FALSE;
#endif
}

/*
** ===================================================================
**     Method      :  Q4CLeft_ParseCommand (component QuadCounter)
**     Description :
**         Handler to process shell commands
**     Parameters  :
**         NAME            - DESCRIPTION
**         cmd             - Command string to be parsed
**       * handled         - Pointer to boolean. The handler
**                           sets this variable to TRUE if command was
**                           handled, otherwise let it untouched.
**         io              - Pointer to I/O structure
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
/*!
 * \brief Parses a command
 * \param cmd Command string to be parsed
 * \param handled Sets this variable to TRUE if command was handled
 * \param io I/O stream to be used for input/output
 * \return Error code, ERR_OK if everything was fine
 */
uint8_t Q4CLeft_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
  uint8_t res=ERR_OK;

  if (UTIL1_strcmp((const char*)cmd, CLS1_CMD_HELP)==0 || UTIL1_strcmp((const char *)cmd, "Q4CLeft help")==0) {
    CLS1_SendHelpStr((const unsigned char*)"Q4CLeft", (const unsigned char*)"Q4CLeft command group\r\n", io->stdOut);
    CLS1_SendHelpStr((const unsigned char*)"  help|status", (const unsigned char*)"Print help or status information\r\n", io->stdOut);
    CLS1_SendHelpStr((const unsigned char*)"  reset", (const unsigned char*)"Reset the current position counter\r\n", io->stdOut);
    *handled = TRUE;
  } else if (UTIL1_strcmp((const char*)cmd, CLS1_CMD_STATUS)==0 || UTIL1_strcmp((const char*)cmd, "Q4CLeft status")==0) {
    CLS1_SendStr((const unsigned char*)"Q4CLeft:\r\n", io->stdOut);
    CLS1_SendStatusStr((const unsigned char*)"  pos", (const unsigned char*)"", io->stdOut);
  #if Q4CLeft_CNTR_BITS==16
    CLS1_SendNum16u(Q4CLeft_currPos, io->stdOut);
  #elif Q4CLeft_CNTR_BITS==32
    CLS1_SendNum32u(Q4CLeft_currPos, io->stdOut);
  #else
    #error "unknown counter size!"
  #endif
    CLS1_SendStr((const unsigned char*)", ", io->stdOut);
  #if Q4CLeft_CNTR_BITS==16
    CLS1_SendNum16s((int16_t)Q4CLeft_currPos, io->stdOut);
  #elif Q4CLeft_CNTR_BITS==32
    CLS1_SendNum32s((int32_t)Q4CLeft_currPos, io->stdOut);
  #else
    #error "unknown counter size!"
  #endif
    CLS1_SendStr((const unsigned char*)"\r\n", io->stdOut);
    CLS1_SendStatusStr((const unsigned char*)"  C1 C2", (const unsigned char*)"", io->stdOut);
    if (Q4CLeft_GET_C1_PIN()!=0) {
      CLS1_SendStr((const unsigned char*)"1 ", io->stdOut);
    } else {
      CLS1_SendStr((const unsigned char*)"0 ", io->stdOut);
    }
    if (Q4CLeft_GET_C2_PIN()!=0) {
      CLS1_SendStr((const unsigned char*)"1\r\n", io->stdOut);
    } else {
      CLS1_SendStr((const unsigned char*)"0\r\n", io->stdOut);
    }
    CLS1_SendStatusStr((const unsigned char*)"  errors", (const unsigned char*)"", io->stdOut);
    CLS1_SendNum16u(Q4CLeft_nofErrors, io->stdOut);
    CLS1_SendStr((const unsigned char*)"\r\n", io->stdOut);
    *handled = TRUE;
  } else if (UTIL1_strcmp((const char*)cmd, "Q4CLeft reset")==0) {
    Q4CLeft_SetPos(0);
    Q4CLeft_nofErrors = 0;
    *handled = TRUE;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  Q4CLeft_SwapPins (component QuadCounter)
**     Description :
**         Swap the two pins
**     Parameters  :
**         NAME            - DESCRIPTION
**         swap            - if C1 and C2 pins shall be swapped.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t Q4CLeft_SwapPins(bool swap)
{
  Q4CLeft_swappedPins = swap;
  return ERR_OK;
}

/* END Q4CLeft. */

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
