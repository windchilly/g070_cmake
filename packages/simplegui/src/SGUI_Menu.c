#include "SGUI_Menu.h"

#define SGUI_MENU_ICON_DECLARE(NAME)                extern const SGUI_BMP_RES NAME
#define SGUI_MENU_ICON_DEFINE(NAME, W, H, ...)      const SGUI_BYTE NAME##DATA[] = {__VA_ARGS__};\
                                                    const SGUI_BMP_RES NAME = {W, H, NAME##DATA}
static SGUI_MENU_ICON_DEFINE(SGUI_MENU_ICON_MOVEUP, 5, 3,
0x04,0x06,0x07,0x06,0x04);
static SGUI_MENU_ICON_DEFINE(SGUI_MENU_ICON_MOVEDOWN, 5, 3,
0x01,0x03,0x07,0x03,0x01);

void SGUI_Menu_Initialize(SGUI_MENU* pstObj, const SGUI_RECT* cpstLayout, const SGUI_FONT_RES* pstFontRes, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount)
{
    SGUI_RECT               sItemstLayout;

    if((NULL != pstObj) && (NULL != pstFontRes))
    {
        // Save layout.
        SGUI_SystemIF_MemoryCopy(&(pstObj->stLayout), cpstLayout, sizeof(SGUI_RECT));
        // Initialize member object pointer.
        pstObj->pstFontRes = pstFontRes;
        pstObj->stItems.pstFirstItem = NULL;
        sItemstLayout.iX = pstObj->stLayout.iX+1;
        sItemstLayout.iY = pstObj->stLayout.iY+SGUI_MENU_ICON_MOVEUP.iHeight+1;
        sItemstLayout.iWidth = pstObj->stLayout.iWidth-2;
        sItemstLayout.iHeight = pstObj->stLayout.iHeight-(SGUI_MENU_ICON_MOVEUP.iHeight+SGUI_MENU_ICON_MOVEDOWN.iHeight+2);
        SGUI_ItemsBase_Initialize(&(pstObj->stItems), &sItemstLayout, pstObj->pstFontRes, pstItemsData, iItemsCount);
    }
}

void SGUI_Menu_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_MENU* pstObj)
{
    SGUI_RECT               stIconArea;
    SGUI_POINT              stIconInnerPos;

    stIconInnerPos.iX =     0;
    stIconInnerPos.iY =     0;

    if(NULL != pstObj)
    {
        /* Clear list item display area. */
        SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stLayout.iX, pstObj->stLayout.iY, pstObj->stLayout.iWidth, pstObj->stLayout.iHeight, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
        // Paint items.
        //SGUI_ItemsBase_Resize(&(pstObj->stItems));
        SGUI_ItemsBase_Repaint(pstDeviceIF, &(pstObj->stItems));
        /* Paint arrow icon. */
        stIconArea.iWidth = 5;
        stIconArea.iHeight = 3;
        stIconArea.iX = pstObj->stLayout.iX+((pstObj->stLayout.iWidth-stIconArea.iWidth)/2);
        if(SGUI_Menu_CanScrollUp(pstObj))
        {
            stIconArea.iY = pstObj->stLayout.iY+1;
            SGUI_Basic_DrawBitMap(pstDeviceIF, &stIconArea, &stIconInnerPos, &SGUI_MENU_ICON_MOVEUP, SGUI_DRAW_NORMAL);
        }
        if(SGUI_Menu_CanScrollDown(pstObj))
        {
            stIconArea.iY = SGUI_RECT_Y_END(pstObj->stItems.stLayout)+1;
            SGUI_Basic_DrawBitMap(pstDeviceIF, &stIconArea, &stIconInnerPos, &SGUI_MENU_ICON_MOVEDOWN, SGUI_DRAW_NORMAL);
        }
    }
}

void SGUI_Menu_Resize(SGUI_MENU* pstObj, const SGUI_RECT* cpstNewLayout)
{
    SGUI_RECT               stItemsLayout;

    if((NULL != pstObj) && (NULL != cpstNewLayout))
    {
        pstObj->stLayout.iX = cpstNewLayout->iX;
        pstObj->stLayout.iY = cpstNewLayout->iY;
        pstObj->stLayout.iWidth = cpstNewLayout->iWidth;
        pstObj->stLayout.iHeight = cpstNewLayout->iHeight;
        stItemsLayout.iX = cpstNewLayout->iX+1;
        stItemsLayout.iY = cpstNewLayout->iY+SGUI_MENU_ICON_MOVEUP.iHeight+1;
        stItemsLayout.iWidth = cpstNewLayout->iWidth-2;
        stItemsLayout.iHeight = cpstNewLayout->iHeight-(SGUI_MENU_ICON_MOVEUP.iHeight+SGUI_MENU_ICON_MOVEDOWN.iHeight+2);
        SGUI_ItemsBase_Resize(&(pstObj->stItems), &stItemsLayout);
    }
}

void SGUI_Menu_PopupSubMenu(SGUI_SCR_DEV* pstDeviceIF, SGUI_MENU* pstObj, const SGUI_RECT* cpstParentItemLayout)
{
    SGUI_INT                iFullHeight;
    SGUI_INT                iVisibleHeight;
    SGUI_RECT               stLayout;

    if((NULL != pstObj) && (pstObj->stItems.iCount >0))
    {
        /* Calculate a virtual height when display all items. */
        iFullHeight = pstObj->stItems.iCount*ITEM_HEIGHT(pstObj->pstFontRes);
        iFullHeight = iFullHeight+SGUI_MENU_ICON_MOVEUP.iHeight+SGUI_MENU_ICON_MOVEDOWN.iHeight+2;

        stLayout.iX = SGUI_RECT_X_END(*cpstParentItemLayout)+1;
        iVisibleHeight = pstDeviceIF->stSize.iHeight-cpstParentItemLayout->iY-1;
        if((iVisibleHeight < iFullHeight) && (cpstParentItemLayout->iY > (pstDeviceIF->stSize.iHeight/2)))
        {
            iVisibleHeight = cpstParentItemLayout->iY+cpstParentItemLayout->iHeight+1;
            stLayout.iY = iVisibleHeight-iFullHeight-1;
            if(stLayout.iY<0)
            {
                stLayout.iY = 0;
            }
        }
        else
        {
            stLayout.iY = cpstParentItemLayout->iY;
        }
        stLayout.iWidth = cpstParentItemLayout->iWidth+2;
        if(iFullHeight > iVisibleHeight)
        {
            stLayout.iHeight = iVisibleHeight;
        }
        else
        {
            stLayout.iHeight = iFullHeight;
        }
        SGUI_Menu_Resize(pstObj, &stLayout);
    }
}
