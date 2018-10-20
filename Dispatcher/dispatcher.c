/*
 * dispatcher.c
 *
 *  Created on: 2 дек. 2016 г.
 *      Author: dmitry
 */
#include "dispatcher.h"
#include "stddef.h"

//#include "SSD1289.h"

/*	How to use:
 * 	1) #include "dispatcher.h"
 * 	2) define PROCESS_TYPE audioProc;
 * 	3) set process and point to function use setProcessFunc( &audioProc, audio_play_loop );
 * 	4) add process to task table addProcess( &audioProc );
 */

int countProc = 0;
int currentProc = 0;
PROCESS_TYPE tab_process[PROCESS_MAX];

int addProcess( PROCESS_TYPE* process ) // возвращает индекс по которому записан элемент в таблицу процессов
{
	int cnt = 0;
	while( tab_process[cnt].entryFunction != NULL )
	{
		// пробегаем не по нулевым ячейкам
		cnt++;
		if( cnt == PROCESS_MAX ) {
			return -1;	// not added
		}
	}

	int i;
	for( i=0; i < PROCESS_MAX; 	i++ )
	{
		/*tab_process[i].entryFunction != NULL && tab_process[i].PROC_STATE == PROCESS_ACTIVE*/
		if( tab_process[i].entryFunction == process->parentProc->entryFunction )
		{
			if( tab_process[i].PROC_STATE == PROCESS_ACTIVE )
			{
				tab_process[i].PROC_STATE = PROCESS_HIDDEN;
				break;
			}
		}
	}

	tab_process[cnt] = *process;
	countProc++;

	return cnt;
}

int remProcess( int num )
{
	int i;
	if( tab_process[num].parentProc != NULL_PROCESS )
	{
		for( i=0; i < PROCESS_MAX; 	i++ )
		{
			if ( tab_process[i].entryFunction == tab_process[num].parentProc->entryFunction )
			{
				if( tab_process[i].PROC_STATE == PROCESS_HIDDEN )
				{
					tab_process[i].PROC_STATE = PROCESS_ACTIVE;
					break;
				}
			}
		}
	}

	if( ( num >= 0 ) && ( num < PROCESS_MAX ) ) {
		tab_process[num].entryFunction = NULL;
		if( countProc > 0 )
			countProc--;

		/*LCD_PutValue( "removed proc = ", num, 5, nextLinePos() );
		Delay(200);*/
	}
	return num;
}

int ProcessHandler(void)	// должна вызываться в бесконечном цикле
{
	PROCESS_RESULT (*entryFunction)(void);
	PROCESS_RESULT res;

	if( tab_process[currentProc].entryFunction == NULL ) {
		currentProc++;
		if( currentProc == PROCESS_MAX ) {
			currentProc = 0;
			return -1;	// no process in table
		}
	}
	else {

		if( tab_process[currentProc].PROC_STATE != PROCESS_HIDDEN )
		{
			entryFunction = tab_process[currentProc].entryFunction;
			res = entryFunction();
			switch(res) {
				case PROCESS_OK:
					break;
				case PROCESS_ERROOR:
					break;
				case PROCESS_END:
					remProcess(currentProc);
					setRefreshScreen(1);
					/*if(tab_process[currentProc].parentProc != NULL_PROCESS)
						addProcess(tab_process[currentProc].parentProc);*/
					break;
			}
		}

		currentProc++;
		if( currentProc == PROCESS_MAX ) {
			currentProc = 0;
		}
	}

	return currentProc;
}

void initProcessHandler(void)
{
	int cnt = 0;
	for( ; cnt < PROCESS_MAX; cnt++ )
	{
		tab_process[cnt].entryFunction = NULL;
		/*LCD_PutValue( "init erased = ", cnt, 5, nextLinePos() );
		Delay(200);*/
	}
}

int ClearDispatcher(void)
{
	int t = 0;
	while( t < PROCESS_MAX ) {
		remProcess(t);
		t++;
	}

	countProc = 0;
	currentProc = 0;

	return t;
}

int getCountProc(void)
{
	return countProc;
}

int setProcessFunc(PROCESS_TYPE *proc, PROCESS_RESULT (*entryFunction)(void), PROCESS_STATE state, PROCESS_TYPE *parent)
{
	proc->entryFunction = entryFunction;
	proc->PROC_STATE = state;
	proc->parentProc = parent; //NULL_PROCESS; // родительский процесс

	// извлечем временно или деактивируем родительский процесс

	return 0;
}

bool refresh = 0;
void setRefreshScreen(bool state)
{
	refresh = state;
}

bool getRefreshScreen(void)
{
	return refresh;
}
