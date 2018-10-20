//--------------------------------------------------------------
// File     : stm32_ub_usb_msc_host.c
// Datum    : 10.04.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO, MISC, STM32_UB_FATFS
// Funktion : USB-MSC-HOST per USB-OTG-Port am Discovery
//
//            Diese Lib erzeugt nur den USB-HOST
//            Für die Dateiverwaltung wird die Library
//            "stm32_ub_fatfs" benötigt (die USB-Version)
//
// Hinweis  : Einige USB-Sticks werden nicht unterstützt
//            Ich habe einen Intenso 4GB-Stick der geht
//            ein Sandisk 8GB funktioniert nicht !!
//
//            Beim Discovery-Modul funktioniert nur der
//            USB-FullSpeed-Mode (USB-High-Speed geht nicht)

//--------------------------------------------------------------
//              PA9   -> USB_OTG_VBUS
//              PA10  -> USB_OTG_ID
//              PA11  -> USB_OTG_DM
//              PA12  -> USB_OTG_DP
//              PC0   -> USB_VBUS_Enable
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_usb_msc_host.h"



//--------------------------------------------------------------
// Globale Variabeln
//--------------------------------------------------------------
USB_OTG_CORE_HANDLE          USB_OTG_Core;
USBH_HOST                    USB_Host;




//--------------------------------------------------------------
// Init vom USB-OTG-Port als MSC-HOST
// (Host für Wechseldatenträger z.B. USB-Stick)
//--------------------------------------------------------------
void UB_USB_MSC_HOST_Init(void)
{
  USB_MSC_HOST_STATUS=USB_MSC_DEV_DETACHED;
  USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_Callbacks);
}



//--------------------------------------------------------------
// USB_HOST-Funktion
// diese Funktion muss zyklisch aufgerufen werden
// Return_wert :
//  USB_MSC_DEV_CONNECTED     , Device angeschlossen und bereit
//  != USB_MSC_DEV_CONNECTED  , Device nicht bereit
//
// Hinweis : falls "USB_MSC_DEV_NOT_SUPPORTED" muss die
//           Init Procedure wiederholt werden
//           (bzw ein anderer USB-Stick benutzt werden)
//--------------------------------------------------------------
USB_MSC_HOST_STATUS_t UB_USB_MSC_HOST_Do(void)
{

  if(USB_MSC_HOST_STATUS!=USB_MSC_DEV_NOT_SUPPORTED) {
    // wenn der USB-Stick nicht unterstützt wird,
    // wird die Funktion nicht mehr aufgerufen
    // damit die Software nicht hängenbleibt
    USBH_Process(&USB_OTG_Core, &USB_Host);
  }

  return(USB_MSC_HOST_STATUS);
}




