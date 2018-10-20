/*
 * ListView.h
 *
 *  Created on: 19 марта 2017 г.
 *      Author: dmitry
 */

#ifndef MY_LIB_LISTVIEW_H_
#define MY_LIB_LISTVIEW_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "SSD1289.h"

#define SPACE_BW_ITEMS_X  	12
#define SPACE_BW_ITEMS_Y 	22

#define X_OFFSET_ITEMS  	18
#define Y_OFFSET_ITEMS  	5

#define MAX_ITEMS_LISTING   20
#define MAX_CHAR_NAME_ITEM  20
#define ICON_SIZE 			60    //16

 typedef enum {
     isTextMenu = 0,
     isIconMenu = 1
 } mMenuType;

 typedef enum {
     ITEM_ADD_OK = 0,
     ITEM_ADD_ERROR,
     LIST_IS_EMPTY,
     LIST_IS_FULL
 } LIST_ERROR_ENUM;

 typedef struct _ICON_STRUCT {
	  const uint16_t *pIconBuff;
      uint8_t IconSize;
  } ICON_STRUCT;

 typedef struct _ICON_ITEM_STRUCT {
     Point itemCoord;
     ICON_STRUCT icon;
     char ItemName[MAX_CHAR_NAME_ITEM];
  } ICON_ITEM_STRUCT, *pICON_ITEM_STRUCT;

 typedef struct _LIST_ITEMS_STRUCT {
     ICON_ITEM_STRUCT *list[MAX_ITEMS_LISTING];
     mMenuType mtype;
     uint16_t countItems;
     uint16_t currItem;
     uint8_t sizeIconsItems;
 } LIST_ITEMS_STRUCT;

 ICON_ITEM_STRUCT *newIconItem(void);
 ICON_ITEM_STRUCT *getCurrItem(LIST_ITEMS_STRUCT *listing);
 ICON_ITEM_STRUCT *getNumItem(LIST_ITEMS_STRUCT *listing, int num);
 void ListingConstructor(LIST_ITEMS_STRUCT *listing);
 void setListingType(LIST_ITEMS_STRUCT *listing, mMenuType mtype);
 mMenuType getListingType(LIST_ITEMS_STRUCT *listing);
 void setCoordItems(LIST_ITEMS_STRUCT *listing);
 LIST_ERROR_ENUM addItem(LIST_ITEMS_STRUCT *listing, ICON_ITEM_STRUCT *item);
 LIST_ERROR_ENUM remItem(LIST_ITEMS_STRUCT *listing, int num);

#ifdef __cplusplus
}
#endif

#endif /* MY_LIB_LISTVIEW_H_ */
