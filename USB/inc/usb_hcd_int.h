//--------------------------------------------------------------
// File     : usb_hcd_int.h
//--------------------------------------------------------------



#ifndef __HCD_INT_H__
#define __HCD_INT_H__


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usb_hcd.h"




#define CLEAR_HC_INT(HC_REGS, intr) \
  {\
  USB_OTG_HCINTn_TypeDef  hcint_clear; \
  hcint_clear.d32 = 0; \
  hcint_clear.b.intr = 1; \
  USB_OTG_WRITE_REG32(&((HC_REGS)->HCINT), hcint_clear.d32);\
  }\

#define MASK_HOST_INT_CHH(hc_num) { USB_OTG_HCGINTMSK_TypeDef  GINTMSK; \
    GINTMSK.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[hc_num]->HCGINTMSK); \
    GINTMSK.b.chhltd = 0; \
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCGINTMSK, GINTMSK.d32);}

#define UNMASK_HOST_INT_CHH(hc_num) { USB_OTG_HCGINTMSK_TypeDef  GINTMSK; \
    GINTMSK.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[hc_num]->HCGINTMSK); \
    GINTMSK.b.chhltd = 1; \
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCGINTMSK, GINTMSK.d32);}

#define MASK_HOST_INT_ACK(hc_num) { USB_OTG_HCGINTMSK_TypeDef  GINTMSK; \
    GINTMSK.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[hc_num]->HCGINTMSK); \
    GINTMSK.b.ack = 0; \
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCGINTMSK, GINTMSK.d32);}

#define UNMASK_HOST_INT_ACK(hc_num) { USB_OTG_HCGINTMSK_TypeDef  GINTMSK; \
    GINTMSK.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[hc_num]->HCGINTMSK); \
    GINTMSK.b.ack = 1; \
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCGINTMSK, GINTMSK.d32);}


/* Callbacks handler */
void ConnectCallback_Handler(USB_OTG_CORE_HANDLE *pdev);
void Disconnect_Callback_Handler(USB_OTG_CORE_HANDLE *pdev);
void Overcurrent_Callback_Handler(USB_OTG_CORE_HANDLE *pdev);
uint32_t USBH_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);




#endif //__HCD_INT_H__



