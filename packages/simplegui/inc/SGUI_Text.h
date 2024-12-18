#ifndef _INCLUDE_GUI_FONT_H_
#define _INCLUDE_GUI_FONT_H_

#include "SGUI_Common.h"
#include "SGUI_Basic.h"

#define FONT_LIB_ADDR                   0X00000
// Max font size is 64 pix, maximum of font data size is 64 * 8 = 512 Bytes.
#define TEXT_NUMBER_STR_LENGTH_MAX      (12)
#define TEXT_PLACEHOLDER_CHARACTER      ('*')
#define SGUI_IS_VISIBLE_CHAR(C)         ((C>0x1F) && (C<0x7F))
#define SGUI_T(TEXT)                    (TEXT)

#ifdef __cplusplus
extern "C"{
#endif
extern SGUI_CSZSTR      SGUI_EMPTY_STRING;
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"{
#endif
void            SGUI_Text_GetTextExtent(SGUI_CSZSTR cszTextBuffer, const SGUI_FONT_RES* pstFontRes, SGUI_AREA_SIZE* pstTextExtent);
void            SGUI_Text_DrawASCIICharacter(SGUI_SCR_DEV* pstDeviceIF, SGUI_CHAR cChar, const SGUI_FONT_RES* pstFontRes, const SGUI_RECT* pstDisplayArea, const SGUI_POINT* pstInnerPos, SGUI_DRAW_MODE eFontMode);
void            SGUI_Text_DrawText(SGUI_SCR_DEV* pstDeviceIF, SGUI_CSZSTR cszTextBuffer, const SGUI_FONT_RES* pstFontRes, const SGUI_RECT* pstDisplayArea, const SGUI_POINT* pstInnerPos, SGUI_DRAW_MODE eFontMode);
SGUI_SIZE       SGUI_Text_DrawMultipleLinesText(SGUI_SCR_DEV* pstDeviceIF, SGUI_CSZSTR szTextBuffer, const SGUI_FONT_RES* pstFontRes, SGUI_RECT* pstDisplayArea, SGUI_INT iTopOffset, SGUI_DRAW_MODE eFontMode);
SGUI_SIZE       SGUI_Text_GetMultiLineTextLines(SGUI_CSZSTR cszText, const SGUI_FONT_RES* pstFontRes, SGUI_SIZE uiDisplayAreaWidth);
SGUI_SIZE       SGUI_Text_GetCharacterData(const SGUI_FONT_RES* pstFontRes, SGUI_UINT32 uiCode, SGUI_BYTE* pDataBuffer, SGUI_SIZE sBufferSize);
#ifdef __cplusplus
}
#endif

#endif
