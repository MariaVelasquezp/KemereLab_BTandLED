/*******************************************************************************
* File Name: BOOST.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_BOOST_H) /* Pins BOOST_H */
#define CY_PINS_BOOST_H

#include "cytypes.h"
#include "cyfitter.h"
#include "BOOST_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} BOOST_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   BOOST_Read(void);
void    BOOST_Write(uint8 value);
uint8   BOOST_ReadDataReg(void);
#if defined(BOOST__PC) || (CY_PSOC4_4200L) 
    void    BOOST_SetDriveMode(uint8 mode);
#endif
void    BOOST_SetInterruptMode(uint16 position, uint16 mode);
uint8   BOOST_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void BOOST_Sleep(void); 
void BOOST_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(BOOST__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define BOOST_DRIVE_MODE_BITS        (3)
    #define BOOST_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - BOOST_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the BOOST_SetDriveMode() function.
         *  @{
         */
        #define BOOST_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define BOOST_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define BOOST_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define BOOST_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define BOOST_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define BOOST_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define BOOST_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define BOOST_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define BOOST_MASK               BOOST__MASK
#define BOOST_SHIFT              BOOST__SHIFT
#define BOOST_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BOOST_SetInterruptMode() function.
     *  @{
     */
        #define BOOST_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define BOOST_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define BOOST_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define BOOST_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(BOOST__SIO)
    #define BOOST_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(BOOST__PC) && (CY_PSOC4_4200L)
    #define BOOST_USBIO_ENABLE               ((uint32)0x80000000u)
    #define BOOST_USBIO_DISABLE              ((uint32)(~BOOST_USBIO_ENABLE))
    #define BOOST_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define BOOST_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define BOOST_USBIO_ENTER_SLEEP          ((uint32)((1u << BOOST_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << BOOST_USBIO_SUSPEND_DEL_SHIFT)))
    #define BOOST_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << BOOST_USBIO_SUSPEND_SHIFT)))
    #define BOOST_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << BOOST_USBIO_SUSPEND_DEL_SHIFT)))
    #define BOOST_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(BOOST__PC)
    /* Port Configuration */
    #define BOOST_PC                 (* (reg32 *) BOOST__PC)
#endif
/* Pin State */
#define BOOST_PS                     (* (reg32 *) BOOST__PS)
/* Data Register */
#define BOOST_DR                     (* (reg32 *) BOOST__DR)
/* Input Buffer Disable Override */
#define BOOST_INP_DIS                (* (reg32 *) BOOST__PC2)

/* Interrupt configuration Registers */
#define BOOST_INTCFG                 (* (reg32 *) BOOST__INTCFG)
#define BOOST_INTSTAT                (* (reg32 *) BOOST__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define BOOST_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(BOOST__SIO)
    #define BOOST_SIO_REG            (* (reg32 *) BOOST__SIO)
#endif /* (BOOST__SIO_CFG) */

/* USBIO registers */
#if !defined(BOOST__PC) && (CY_PSOC4_4200L)
    #define BOOST_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define BOOST_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define BOOST_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define BOOST_DRIVE_MODE_SHIFT       (0x00u)
#define BOOST_DRIVE_MODE_MASK        (0x07u << BOOST_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins BOOST_H */


/* [] END OF FILE */
