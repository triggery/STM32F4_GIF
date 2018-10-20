/*
 * dispatcher.h
 *
 *  Created on: 2 дек. 2016 г.
 *      Author: dmitry
 */

#ifndef DISPATCHER_DISPATCHER_H_
#define DISPATCHER_DISPATCHER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdbool.h"

#define PROCESS_MAX		6

typedef enum {
	PROCESS_OK = 0,
	PROCESS_ERROOR,
	PROCESS_END
}PROCESS_RESULT;

typedef enum {
	PROCESS_ACTIVE = 0, 	// показывается, события обрабатываются
	PROCESS_HIDDEN,			// не показывается и события не обрабатываются
	PROCESS_DEMON			// не показывается, всегда выполняется
}PROCESS_STATE;

typedef struct _PROCESS_TYPE {
	PROCESS_RESULT (*entryFunction)(void);
	PROCESS_STATE PROC_STATE;
	char PID;

	struct _PROCESS_TYPE *parentProc;
}PROCESS_TYPE;

#define NULL_PROCESS (PROCESS_TYPE*)(NULL)

void setRefreshScreen(bool state);
bool getRefreshScreen(void);

int addProcess( PROCESS_TYPE* process );
int remProcess( int num );
int ProcessHandler(void);	// должна вызываться в бесконечном цикле
void initProcessHandler(void);
int ClearDispatcher(void);
int getCountProc(void);
int setProcessFunc(PROCESS_TYPE *proc, PROCESS_RESULT (*entryFunction)(void), PROCESS_STATE state, PROCESS_TYPE *parent);

#ifdef __cplusplus
}
#endif


#endif /* DISPATCHER_DISPATCHER_H_ */
