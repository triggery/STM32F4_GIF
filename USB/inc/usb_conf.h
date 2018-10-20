//--------------------------------------------------------------
// File     : usb_conf.h
//--------------------------------------------------------------

#ifndef __USB_CONF__H__
#define __USB_CONF__H__

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"





/****************** USB OTG FS CONFIGURATION **********************************/

 #define RX_FIFO_FS_SIZE                          128
 #define TXH_NP_FS_FIFOSIZ                         96
 #define TXH_P_FS_FIFOSIZ                          96




#define __ALIGN_BEGIN
#define __ALIGN_END


#define __packed    __attribute__ ((__packed__))





#endif //__USB_CONF__H__


