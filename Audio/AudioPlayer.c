#include "AudioPlayer.h"
#include "file_man.h"
#include "audio_dac.h"
#include "dispatcher.h"
#include "SSD1289.h"
#include "icons_menu.h"

int flagend = 0;
void WavePlayaer(void)
{
	/*	До первого вызова этой функции должны быть вызваны эти:
	 * 	fileman_init();
		open_folder("/");
	 */
	if( getStatusPlay() )	// если файл проигрывается, то выходим
		return;
	if( flagend ) {
		ClearDispatcher();
		LCD_PutString(5, 300, "All files played " );
		return;
	}

	LCD_Clear(LCD_BackColor());
	//LCD_WriteBMP_16( 180, 65, 60, 60, getFileman1Icon() );

	FileMan_Show();

	ICON_ITEM_STRUCT *item = FileMan_GetCurrIconItem();
	char *fileitem = item->ItemName;
	Point itemCoord = item->itemCoord;

	if( strstr( fileitem, ".wav" ) )
	{
		if( getMenuListType() == isTextMenu )
			LCD_WriteBMP_16( 2, itemCoord.Y+16, 16, 16, getMedia16Icon() );
		else
			LCD_WriteBMP_16( itemCoord.X, itemCoord.Y+16, 16, 16, getMedia16Icon() );

		audio_play_start(fileitem);
		//initAudioProc();
	}

	FILE_MAN_ERROR ferr;
		setNextItem(&ferr);
	if( ferr == FILE_MAN_LAST_FILE )  // если двигаться дальше нельзя т.к. конец списка
	{
		if( !getStatusPlay() ) {	  // если воспроизведение окончилось ( и конец списка )
			ClearDispatcher();
			LCD_PutString(5, 300, "All files played " );
		}
		if ( getStatusPlay() ) {	 // проигрывается последний файл
			flagend = 1;
		}
	}
}