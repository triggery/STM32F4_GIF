/*
 * ListView.c
 *
 *  Created on: 19 марта 2017 г.
 *      Author: dmitry
 */
#include "ListView.h"
#include "SSD1289.h"
#include "GesturesHandler.h"
#include "stdlib.h"
#include "icons_menu.h"

ICON_ITEM_STRUCT *newIconItem(void)
{
	ICON_ITEM_STRUCT *item = (ICON_ITEM_STRUCT*)malloc(sizeof(ICON_ITEM_STRUCT));
	//item->icon.pIconBuff = getFolder16Icon();
#if ICON_SIZE==16
	item->icon.pIconBuff = getFolder16Icon();
    item->icon.IconSize = 16;
#else
    item->icon.pIconBuff = getFolder1Icon(); //getFileman1Icon();
    item->icon.IconSize = 60;
#endif
	return item;
}

ICON_ITEM_STRUCT *getCurrItem(LIST_ITEMS_STRUCT *listing)
{
    return listing->list[listing->currItem];
}

ICON_ITEM_STRUCT *getNumItem(LIST_ITEMS_STRUCT *listing, int num)
{
    return listing->list[num];
}

void setCoordItems(LIST_ITEMS_STRUCT *listing)
{
    int c;
    int xt = X_OFFSET_ITEMS,
        yt = Y_OFFSET_ITEMS,
        xline = 0, yline = 0;

        for( c = 0; c < listing->countItems; c++)
        {
            if( listing->mtype == isIconMenu )
            {
                xt = X_OFFSET_ITEMS + xline*(ICON_SIZE + SPACE_BW_ITEMS_X);
                xline++;

                if( xt > (DISP_W - ICON_SIZE) )
                {
                    xline = 1;
                    yline++;
                    xt = X_OFFSET_ITEMS;
                    yt = Y_OFFSET_ITEMS + yline*(ICON_SIZE + SPACE_BW_ITEMS_Y);
                }
            }
            else
            {
                yt = Y_OFFSET_ITEMS + yline*(ICON_SIZE + /*SPACE_BW_ITEMS_Y*/ 7);
                yline++;
            }

            if ( yt > DISP_H )
                return;

            listing->list[c]->itemCoord.X = xt;
            listing->list[c]->itemCoord.Y = yt;
        }
}

void ListingConstructor(LIST_ITEMS_STRUCT *listing)
{
    listing->mtype = isTextMenu; // isIconMenu; //
    listing->countItems = 0;
    listing->currItem = 0;
    listing->sizeIconsItems = 0;
}

void setListingType(LIST_ITEMS_STRUCT *listing, mMenuType mtype)
{
	listing->mtype = mtype;
}

mMenuType getListingType(LIST_ITEMS_STRUCT *listing)
{
	return listing->mtype;
}

LIST_ERROR_ENUM addItem(LIST_ITEMS_STRUCT *listing, ICON_ITEM_STRUCT *item)
{
    int pos = listing->countItems;
    if( pos < MAX_ITEMS_LISTING )
    {
        listing->list[pos] = item;
        listing->countItems++;
        return ITEM_ADD_OK;
    }
    else
        return LIST_IS_FULL;
}

LIST_ERROR_ENUM remItem(LIST_ITEMS_STRUCT *listing, int num)
{
    int count = num;
    if( ( num >= 0 ) && ( num < listing->countItems ) )
    {
        while ( count < listing->countItems-1 )
        {
            listing->list[count] = listing->list[count+1];
            count+=1;
        }
        listing->countItems-=1;
        return ITEM_ADD_OK;
    }
    else
        return ITEM_ADD_ERROR;
}

