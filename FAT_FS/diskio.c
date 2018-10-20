/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/
/* Angepasst von UB                                                      */
/* V:1.0 / 26.02.2013                                                    */
/*-----------------------------------------------------------------------*/



#include "diskio.h"		// FatFs lower layer API
#include "stm32_ub_usbdisk.h"	// UB: USB drive control

/* Definitions of physical drive number for each media */
#define USB		0

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	if(pdrv != 0)
		return STA_NOINIT;

	result = USB_disk_initialize();
	if(result==0) {
		stat=0;
	}
	else {
		stat=STA_NOINIT;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	if(pdrv != 0)
		return STA_NOINIT;

	result = USB_disk_status();

	// translate the reslut code here
	if(result==0) {
		stat=0;
	}
	else {
		stat=STA_NODISK | STA_NOINIT;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;
	int result;

	if(pdrv != 0)
		return STA_NOINIT;

	result = USB_disk_read(buff, sector, count);

	// translate the reslut code here
	if(result==0) {
		res=RES_OK;
	}
	else {
		res=RES_ERROR;
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;
	int result;

	if(pdrv != 0)
		return STA_NOINIT;

	result = USB_disk_write(buff, sector, count);

	// translate the reslut code here
	if(result==0) {
		res=RES_OK;
	}
	else {
		res=RES_ERROR;
	}
	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DWORD get_fattime (void)
{
	return	((2006UL-1980) << 25)	      // Year = 2006
			| (2UL << 21)	      // Month = Feb
			| (9UL << 16)	      // Day = 9
			| (22U << 11)	      // Hour = 22
			| (30U << 5)	      // Min = 30
			| (0U >> 1)	      // Sec = 0
			;
}


#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	if(pdrv != 0)
		return STA_NOINIT;

	result = USB_disk_ioctl(cmd, buff);

	// post-process here
	if(result==0) {
		res=RES_OK;
	}
	else {
		res=RES_ERROR;
	}
	return res;
}
#endif
