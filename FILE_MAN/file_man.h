/*
 * file_man.h
 *
 *  Created on: 30 нояб. 2016 г.
 *      Author: dmitry
 */

#ifndef FILE_MAN_FILE_MAN_H_
#define FILE_MAN_FILE_MAN_H_

#ifdef __cplusplus
 extern "C" {
#endif

//#define SIZE_OF_LFN_BUF 20

#include "ListView.h"
#include "GesturesHandler.h"
#include "dispatcher.h"

typedef struct _file_struct_type {
	char file_name[MAX_CHAR_NAME_ITEM];	// SIZE_OF_LFN_BUF
	unsigned char file_type;
} FILE_STRUCT_TYPE;

typedef enum _file_man_error {
	FILE_MAN_OK = 0,
	FILE_MAN_EMPTY,
	FILE_MAN_ERROOR,
	FILE_MAN_LAST_FILE,
	FILE_MAN_FIRST_FILE,
	FILE_MAN_NOT_SUPPORT
} FILE_MAN_ERROR;


void FileMan_Init(void);
int FileMan_OpenFolder( const char *path );
int FileMan_Show(void);
int FileMen_ShowFilesList(void);
ICON_ITEM_STRUCT* FileMan_GetCurrIconItem(void);
ICON_ITEM_STRUCT* FileMan_GetIconItemIndex(int index);

int open_folder( const char *path );
void setCoortItemsFM(void);
char *getCurrFileName(void);

int getCurrIndexFile(void);
Point getCoordCurrIcon(void);
Point getCoordIconIndex(int findex);
mMenuType getMenuListType(void);
bool fileList_IsEmpty(void);
int setNextItem(FILE_MAN_ERROR *ferr);
int setPrevItem(FILE_MAN_ERROR *ferr);

int FindTouchedItem(TOUCH_EVENT *touchEvent);
PROCESS_RESULT FileMan_TouchHandler(void);

FILE_MAN_ERROR openCurrentFile(void);

#ifdef __cplusplus
}
#endif


#endif /* FILE_MAN_FILE_MAN_H_ */
