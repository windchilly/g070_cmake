#ifndef __INCLUDED_SGUI_INTERFACE_H__
#define __INCLUDED_SGUI_INTERFACE_H__

#include "SGUI_Typedef.h"
#include "SGUI_Common.h"
#include <string.h>

#if   defined ( __CC_ARM )
	#define SGUI_ASM            __asm           // asm keyword for ARM Compiler(Keil MDK).
	#define SGUI_INLINE         __inline        // inline keyword for ARM Compiler.
	#pragma diag_suppress       870             // Disabled "multibyte character sequence" warning.

#elif defined ( __ICCARM__ )
	#define SGUI_ASM              __asm         // < asm keyword for IAR Compiler.
	#define SGUI_INLINE           inline        // inline keyword for IAR Compiler. Only available in High optimization mode!

#elif defined ( __GNUC__ )
	#define SGUI_ASM              __asm         // asm keyword for GNU Compiler.
	#define SGUI_INLINE           inline        // inline keyword for GNU Compiler.

#elif defined ( __TASKING__ )
	#define SGUI_ASM              __asm         // asm keyword for TASKING Compiler.
	#define SGUI_INLINE           inline        // inline keyword for TASKING Compiler.
#endif

#ifdef _SIMPLE_GUI_ENCODE_TEXT_
// Only used when running in simulated environment and text encode need convert to GB2312.
SGUI_SZSTR              SGUI_SystemIF_EncodeConvert(SGUI_CSZSTR szSourceEncode, SGUI_SZSTR szDestinationEncode, SGUI_SZSTR szSource);
  #define       SGUI_ENCODE_BUFFER_SIZE         (512)
  #define       ENCODE(S)                       (SGUI_SystemIF_EncodeConvert(_SIMPLE_GUI_ENCODE_TEXT_SRC_, _SIMPLE_GUI_ENCODE_TEXT_DEST_, (char *)(S)))
#else
 #define        ENCODE(S)                       (S)
#endif

#ifdef _SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_
SGUI_PTR                SGUI_SystemIF_Allocate(SGUI_SIZE sSize);
void                    SGUI_SystemIF_Free(SGUI_PTR pFreePointer);
#endif

#define                 SGUI_SystemIF_MemoryCopy(DEST, SOURCE, SIZE) \
                            (memcpy(DEST, SOURCE, SIZE))
#define                 SGUI_SystemIF_MemorySet(PTR, VAL, SIZE) \
                            (memset(PTR, VAL, SIZE))
#define                 SGUI_SystemIF_StringLength(/* SGUI_CSZSTR */STR) \
                            ((NULL == (STR))?0:(strlen((STR))))
#define                 SGUI_SystemIF_StringCopy(/* SGUI_SZSTR */DEST, /* SGUI_CSZSTR */SRC) \
                            (strcpy((DEST), (SRC)))
#define                 SGUI_SystemIF_StringLengthCopy(/* SGUI_SZSTR */DEST, /* SGUI_CSZSTR */SRC, /* SGUI_SIZE */SIZE) \
                            (strncpy((DEST), (SRC), (SIZE)))
#define                 SGUI_SystemIF_StringCompare(/* SGUI_SZSTR */LEFT, /* SGUI_CSZSTR */RIGHT) \
                            (strcmp((LEFT), (RIGHT)))

#endif // __INCLUDED_SGUI_INTERFACE_H__
