/*******************************************************************************

  File Name   : ccp_can_interface.c
  Date        : 28.09.2001
  Version     : 1.0.1
  Desciption  : additional functions for CCP


*******************************************************************************/


// -----------------------------------------------------------------------------
// INCLUDE FILES
// -----------------------------------------------------------------------------
#include "ccp_can_interface.h"   // additional functions for CCP usage
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Empty Interrupt Service Routine
// -----------------------------------------------------------------------------
@interrupt void irq_dummy(void) {       // dummy function,
}                                       // interrupt vect.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// SENDING an CRM-DTO when receiving an CRO
// -----------------------------------------------------------------------------
BYTE ccpSend( BYTEPTR msg )
{
  ccpBootTransmitCrm(msg);              // callup CAN-send
  return 1;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CONVERT pointer
// -----------------------------------------------------------------------------
MTABYTEPTR ccpGetPointer( BYTE addr_ext, DWORD addr )        // get Pointer into
{                                                            // normal C
  return (MTABYTEPTR) addr;
}
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// CALCULATE new measuerments
// -----------------------------------------------------------------------------
void ccpUserBackground( void )
{
}
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Define
// -----------------------------------------------------------------------------
BYTE ccpDisableNormalOperation( MTABYTEPTR a, WORD s )
{
  disable_interrupt();                  // disables all interrupts, e.g. TIMER_0
  return 1;
}
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Disable/Enable Interrupt Functions
// -----------------------------------------------------------------------------
BYTE disable_interrupt_count = 0;       // init counter

void disable_interrupt() {

  _asm("SEI");                          // set I-Bit
  disable_interrupt_count++;            // incr. counter
}

void enable_interrupt() {

  if (--disable_interrupt_count==0) {   // if interrupts only one time disabled
    _asm("CLI");                        // enable them
  }
}
// -----------------------------------------------------------------------------
