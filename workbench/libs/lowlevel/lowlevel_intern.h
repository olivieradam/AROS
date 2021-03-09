/*
    Copyright (C) 1995-2020, The AROS Development Team. All rights reserved.

    Desc: Internal header file for lowlevel library
*/
#ifndef __LOWLEVEL_INTERN_H__
#define __LOWLEVEL_INTERN_H__

#include <proto/exec.h>

#include <exec/types.h>
#include <exec/libraries.h>
#include <exec/execbase.h>
#include <exec/interrupts.h>
#include <exec/semaphores.h>

#include <aros/debug.h>

#if defined(LOWLEVEL_ARCHDATA)
#include "lowlevel_arch.h"
#else
struct llArchData
{
    
};
#endif

struct llKBInterrupt
{
    struct Interrupt        	llkbi_Interrupt;
    ULONG                       llkbi_KeyData;
    APTR                        llkbi_Data;
    VOID                        (* llkbi_Code)();
};

/*
    This is the LowLevelBase structure. It is documented here because 
    it is completely private. Applications should treat it as a struct 
    Library, and use the workbench.library functions to get information.
*/
struct LowLevelBase
{
    struct Library              ll_Lib;

    struct SignalSemaphore      ll_Lock;
    struct Interrupt            ll_VBlank;
    BPTR                        ll_SegList;

    /* Input event handling ... */
    struct Library              *ll_InputBase;
    struct Library              *ll_UtilityBase;
    struct Interrupt        	*ll_InputHandler;
    struct MsgPort          	*ll_InputMP;
    struct IOStdReq         	*ll_InputIO;

    ULONG                       ll_LastKey;
    struct List                 ll_KBInterrupts;

    /* Timer handling ... */
    struct Library              *ll_TimerBase;
    struct MsgPort          	*ll_TimerMP;
    struct IOStdReq         	*ll_TimerIO;

    struct llArchData           ll_Arch;
};

/*
 * Defintion of internal structures.
 */

#endif /* __LOWLEVEL_INTERN_H__  */

