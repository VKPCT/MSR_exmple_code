/**********************************************************************************************************************
 *  DO NOT EDIT!!! This file is automatically generated.
 *  -------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  Copyright (c) 2013-2015 by Vector Informatik GmbH. All rights reserved.
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  Vtt_Rte.h
 *  ECU-Instance:  MyECU
 *  Generated at:  2016-02-25  13:06:02
 *
 *     Generator:  VTT (VTT.exe)
 *
 *   Description:  RTE VFB tracing definition file
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "VttCntrl.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
FUNC(void, VTTCNTRL_CODE) VttRte_OnStateChange(uint8 action, uint8 oldState, uint8 newState);
extern void Rte_VttMemCpy(void* target, const void* source, uint32 size);

/**********************************************************************************************************************
 *  GLOBAL VARIABLE PROTOTYPES
 *********************************************************************************************************************/
extern sint32 rtePortHandles[];
