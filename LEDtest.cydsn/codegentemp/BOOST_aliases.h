/*******************************************************************************
* File Name: BOOST.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_BOOST_ALIASES_H) /* Pins BOOST_ALIASES_H */
#define CY_PINS_BOOST_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define BOOST_0			(BOOST__0__PC)
#define BOOST_0_PS		(BOOST__0__PS)
#define BOOST_0_PC		(BOOST__0__PC)
#define BOOST_0_DR		(BOOST__0__DR)
#define BOOST_0_SHIFT	(BOOST__0__SHIFT)
#define BOOST_0_INTR	((uint16)((uint16)0x0003u << (BOOST__0__SHIFT*2u)))

#define BOOST_INTR_ALL	 ((uint16)(BOOST_0_INTR))


#endif /* End Pins BOOST_ALIASES_H */


/* [] END OF FILE */
