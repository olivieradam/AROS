/*
    Copyright (C) 1995-2021, The AROS Development Team. All rights reserved.
*/

#include <sys/types.h>

#include <aros/x86_64/cpucontext.h>

#ifdef __AROS_EXEC_LIBRARY__

/* regs_t is a black box here */
struct ucontext;
typedef struct ucontext *regs_t;

#else

#if defined(_WANT_UCONTEXT) && defined(_HAVE_CONFIG_H)
#include <aros/host-conf.h>
#endif

#ifndef _XOPEN_SOURCE
/* On Darwin this definition is required by ucontext.h (which is marked as deprecated) 
 * At least on Debian 8, _XOPEN_SOURCE is already defined in /usr/include/features.h 
 */
#define _XOPEN_SOURCE
#endif

#if defined(HAVE_UCONTEXT_H)
#define _HAVE_UCONTEXT
#include <ucontext.h>
#else
#if defined(HAVE_SYS_UCONTEXT_H)
#define _HAVE_UCONTEXT
#include <sys/ucontext.h>
#endif
#endif

#if defined(_WANT_UCONTEXT) && !defined(_HAVE_UCONTEXT)
#warning "missing support for ucontext_t on this platform"
#endif

#define SIGCORE_NEED_SA_SIGINFO

typedef ucontext_t regs_t;

#define SIGHANDLER	bsd_sighandler
typedef void (*SIGHANDLER_T)(int);

#define SC_DISABLE(sc)   sc->uc_sigmask = KernelBase->kb_PlatformData->sig_int_mask
#define SC_ENABLE(sc)    pd->iface->SigEmptySet(&(sc)->uc_sigmask)

/* work around silly renaming of struct members in OS X 10.5 */
#if __DARWIN_UNIX03 && defined(_STRUCT_X86_EXCEPTION_STATE32)

#define R0(context)     ((context)->uc_mcontext->__ss.__rax)
#define R1(context)     ((context)->uc_mcontext->__ss.__rbx)
#define R2(context)     ((context)->uc_mcontext->__ss.__rcx)
#define R3(context)     ((context)->uc_mcontext->__ss.__rdx)
#define R4(context)     ((context)->uc_mcontext->__ss.__rdi)
#define R5(context)     ((context)->uc_mcontext->__ss.__rsi)
#define R6(context)     ((context)->uc_mcontext->__ss.__rflags)
#define R8(context)	((context)->uc_mcontext->__ss.__r8)
#define R9(context)	((context)->uc_mcontext->__ss.__r9)
#define R10(context)	((context)->uc_mcontext->__ss.__r10)
#define R11(context)	((context)->uc_mcontext->__ss.__r11)
#define R12(context)	((context)->uc_mcontext->__ss.__r12)
#define R13(context)	((context)->uc_mcontext->__ss.__r13)
#define R14(context)	((context)->uc_mcontext->__ss.__r14)
#define R15(context)	((context)->uc_mcontext->__ss.__r15)

#define FP(context)     ((context)->uc_mcontext->__ss.__rbp)
#define PC(context)     ((context)->uc_mcontext->__ss.__rip)
#define SP(context)     ((context)->uc_mcontext->__ss.__rsp)

#define CS(context)	((context)->uc_mcontext->__ss.__cs)
#define FS(context)	((context)->uc_mcontext->__ss.__fs)
#define GS(context)	((context)->uc_mcontext->__ss.__gs)

#define FPSTATE(context) ((context)->uc_mcontext->__fs.__fpu_fcw)

#else

#define R0(context)     ((context)->uc_mcontext->ss.eax)
#define R1(context)     ((context)->uc_mcontext->ss.ebx)
#define R2(context)     ((context)->uc_mcontext->ss.ecx)
#define R3(context)     ((context)->uc_mcontext->ss.edx)
#define R4(context)     ((context)->uc_mcontext->ss.edi)
#define R5(context)     ((context)->uc_mcontext->ss.esi)
#define R6(context)     ((context)->uc_mcontext->ss.eflags)
#define R8(context)	((context)->uc_mcontext->ss.r8)
#define R9(context)	((context)->uc_mcontext->ss.r9)
#define R10(context)	((context)->uc_mcontext->ss.r10)
#define R11(context)	((context)->uc_mcontext->ss.r11)
#define R12(context)	((context)->uc_mcontext->ss.r12)
#define R13(context)	((context)->uc_mcontext->ss.r13)
#define R14(context)	((context)->uc_mcontext->ss.r14)
#define R15(context)	((context)->uc_mcontext->ss.r15)

#define FP(context)     ((context)->uc_mcontext->ss.ebp)
#define PC(context)     ((context)->uc_mcontext->ss.eip)
#define SP(context)     ((context)->uc_mcontext->ss.esp)

#define CS(context)	((context)->uc_mcontext->ss.cs)
#define FS(context)	((context)->uc_mcontext->ss.fs)
#define GS(context)	((context)->uc_mcontext->ss.gs)

#define FPSTATE(context) ((context)->uc_mcontext->fs.fpu_fcw)

#endif

#define GLOBAL_SIGNAL_INIT(sighandler) \
	static void sighandler ## _gate (int sig, siginfo_t *info, void *sc) \
	{						     \
	    sighandler(sig, sc);		             \
	}

/*
 * Save CPU registers.
 * Unfortunately Darwin doesn't provide us with values of all segment registers.
 * We can't do anything with it.
 */
#define SAVE_CPU(cc, sc)	\
    cc.Flags = ECF_SEGMENTS;	\
    cc.rax    = R0(sc);		\
    cc.rbx    = R1(sc);		\
    cc.rcx    = R2(sc);		\
    cc.rdx    = R3(sc);		\
    cc.rdi    = R4(sc);		\
    cc.rsi    = R5(sc);		\
    cc.rflags = R6(sc);		\
    cc.r8     = R8(sc);		\
    cc.r9     = R9(sc);		\
    cc.r10    = R10(sc);	\
    cc.r11    = R11(sc);	\
    cc.r12    = R12(sc);	\
    cc.r13    = R13(sc);	\
    cc.r14    = R14(sc);	\
    cc.r15    = R15(sc);	\
    cc.rbp    = FP(sc);		\
    cc.rip    = PC(sc);		\
    cc.rsp    = SP(sc);		\
    cc.cs     = CS(sc);		\
    cc.fs     = FS(sc);		\
    cc.gs     = GS(sc);

/*
 * Restore CPU registers.
 * Note that we do not restore segment registers because they
 * are of own use by Darwin.
 */
#define RESTORE_CPU(cc, sc) \
    R0(sc)  = cc.rax;       \
    R1(sc)  = cc.rbx;       \
    R2(sc)  = cc.rcx;       \
    R3(sc)  = cc.rdx;       \
    R4(sc)  = cc.rdi;       \
    R5(sc)  = cc.rsi;       \
    R6(sc)  = cc.rflags;    \
    R8(sc)  = cc.r8;	    \
    R9(sc)  = cc.r9;	    \
    R10(sc) = cc.r10;	    \
    R11(sc) = cc.r11;	    \
    R12(sc) = cc.r12;	    \
    R13(sc) = cc.r13;	    \
    R14(sc) = cc.r14;	    \
    R15(sc) = cc.r15;	    \
    FP(sc)  = cc.rbp;       \
    PC(sc)  = cc.rip;       \
    SP(sc)  = cc.rsp;

/*
 * Save all registers from UNIX signal context to AROS context.
 * Save also SSE state if the context has buffer. ECF_FPFXS will be set
 * if SSE state was copied.
 */
#define SAVEREGS(cc, sc)                                       				\
    SAVE_CPU((cc)->regs, sc);								\
    if ((cc)->regs.FXSData)								\
    {											\
    	(cc)->regs.Flags |= ECF_FPFXS;							\
        CopyMemQuick(&FPSTATE(sc), (cc)->regs.FXSData, sizeof(struct FPXSContext));	\
    }

/*
 * Restore all registers from AROS context to UNIX signal context.
 * Check context flags to decide whether to restore SSE or not.
 */
#define RESTOREREGS(cc, sc)                                    				\
    RESTORE_CPU((cc)->regs, sc);							\
    if ((cc)->regs.Flags & ECF_FPFXS) {							\
        CopyMemQuick((cc)->regs.FXSData, &FPSTATE(sc), sizeof(struct FPXSContext)); \
    }

/* Print signal context. Used in crash handler. */
#define PRINT_SC(sc)						\
    bug ("    RSP=%016lx  RBP=%016lx  RIP=%016lx\n"		\
	 "    RAX=%016lx  RBX=%016lx  RCX=%016lx  RDX=%016lx\n" \
	 "    RDI=%016lx  RSI=%016lx  RFLAGS=%016lx\n"		\
	 "    R8 =%016lx  R9 =%016lx  R10=%016lx  R11=%016lx\n" \
	 "    R12=%016lx  R13=%016lx  R14=%016lx  R15=%016lx\n" \
	 , SP(sc), FP(sc), PC(sc)				\
	 , R0(sc), R1(sc), R2(sc), R3(sc)			\
	 , R4(sc), R5(sc), R6(sc), R8(sc), R9(sc)		\
	 , R10(sc), R11(sc), R12(sc), R13(sc), R14(sc), R15(sc) \
	)

#endif /* __AROS_EXEC_LIBRARY__ */

#define EXCEPTIONS_COUNT 17

struct AROSCPUContext
{
    struct ExceptionContext regs;
    int errno_backup;
};
