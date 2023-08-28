#include <project.h>
#include <stdbool.h>
bool state = true;

#define LIGHT_OFF                       (0u)
#define LIGHT_ON                        (1u)

/* Interrupt prototypes */
CY_ISR_PROTO(WDTIsrHandler);

#define SLEEP_INTERVAL_2S           10                       /* millisecond */
#define ILO_FREQ                    32000                       /* Hz */
#define LOG_ROW_INDEX               (CY_FLASH_NUMBER_ROWS - 1)  /* last row */

typedef enum {
    ACTIVE,
    WAKEUP_SLEEP,
    WAKEUP_DEEPSLEEP,
    SLEEP,
    DEEPSLEEP
} ApplicationPower;

ApplicationPower applicationPower;

int apiResult;

void AppCallBack(uint32 event, void* eventParam) 
{ 
    CYBLE_BLESS_CLK_CFG_PARAMS_T clockConfig; 
     
    switch(event) 
    { 
      /* Handle stack events */ 
      case CYBLE_EVT_STACK_ON: 
 
          /* C8. Get the configured clock parameters for BLE subsystem */ 
          CyBle_GetBleClockCfgParam(&clockConfig);     
             
          /* C8. Set the device sleep-clock accuracy (SCA) based on the tuned ppm  
          of the WCO */ 
          clockConfig.bleLlSca = CYBLE_LL_SCA_000_TO_020_PPM; 
             
          /* C8. Set the clock parameter of BLESS with updated values */ 
          CyBle_SetBleClockCfgParam(&clockConfig); 
 
          /* Put the device into discoverable mode so that a Central device can   
          connect to it. */ 
          apiResult = CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST); 
           
          /* Application-specific event handling here */ 
        
        /*Probably we receive a RX Buff, with the transmiter data from GATT*/
        
        break; 
    
      /* Other application-specific event handling here */ 
      case CYBLE_EVT_GAP_DEVICE_CONNECTED: 
       // LED_Write(1);
        break;
    
      case CYBLE_EVT_GAP_DEVICE_DISCONNECTED: 
        apiResult = CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST); 
        //LED_Write(0);
        break;
    } 
} 

int enable_deepsleep = 1;
inline void RunApplication() 
{ 
    /* if you are done with everything and ready to go to deepsleep,  
    then set it up to go to deepsleep. Update the code inside if() specific 
    to your application*/   
    if ( enable_deepsleep == 1 ) 
    { 
        applicationPower = DEEPSLEEP; 
    } 
} 

void ManageApplicationPower() 
{ 
    switch(applicationPower) 
    { 
        case ACTIVE: // don’t need to do anything 
        break; 
         
        case WAKEUP_SLEEP: // do whatever wakeup needs to be done  
         
            applicationPower = ACTIVE; 
        break; 
         
        case WAKEUP_DEEPSLEEP: // do whatever wakeup needs to be done. 
         
            applicationPower = ACTIVE; 
        break; 
 
        case SLEEP:  
        /*********************************************************************** 
         * Place code to place the application components to sleep here  
        ************************************************************************/         
        break; 
         
        case DEEPSLEEP: 
        /*********************************************************************** 
        *  Place code to place the application components to deepsleep here  
        ************************************************************************/      
         
        break; 
    } 
}   

void ManageSystemPower() 
{ 
    /* Variable declarations */ 
    CYBLE_BLESS_STATE_T blePower; 
    uint8 interruptStatus ; 
     
    /* Disable global interrupts to avoid any other tasks from interrupting this 
section of code*/ 
    interruptStatus  = CyEnterCriticalSection(); 
     
    /* C9. Get current state of BLE sub system to check if it has successfully 
entered deep sleep state */ 
    blePower = CyBle_GetBleSsState(); 
     
    /* C10. System can enter Deep-Sleep only when BLESS and rest of the application 
are in DeepSleep or equivalent power modes */ 
    if((blePower == CYBLE_BLESS_STATE_DEEPSLEEP || blePower == 
CYBLE_BLESS_STATE_ECO_ON) && applicationPower == DEEPSLEEP) 
    { 
        applicationPower = WAKEUP_DEEPSLEEP; 
        /* C11. Put system into Deep-Sleep mode*/ 
        CySysPmDeepSleep(); 
    } 
    /* C12. BLESS is not in Deep Sleep mode. Check if it can enter Sleep mode */ 
    else if((blePower != CYBLE_BLESS_STATE_EVENT_CLOSE)) 
    { 
        /* C13. Application is in Deep Sleep. IMO is not required */ 
        if(applicationPower == DEEPSLEEP) 
        { 
            applicationPower = WAKEUP_DEEPSLEEP; 
             
            /* C14. change HF clock source from IMO to ECO*/ 
            CySysClkWriteHfclkDirect(CY_SYS_CLK_HFCLK_ECO);  
            /* C14. stop IMO for reducing power consumption */ 
            CySysClkImoStop();  
            /*C15. put the CPU to sleep */ 
            CySysPmSleep(); 
            /* C16. starts execution after waking up, start IMO */ 
            CySysClkImoStart(); 
            /* C16. change HF clock source back to IMO */ 
            CySysClkWriteHfclkDirect(CY_SYS_CLK_HFCLK_IMO); 
        } 
        /*C17. Application components need IMO clock */ 
        else if(applicationPower == SLEEP ) 
        { 
            /* C18. Put the system into Sleep mode*/ 
            applicationPower = WAKEUP_SLEEP; 
            CySysPmSleep(); 
        } 
    } 
     
    /* Enable interrupts */ 
    CyExitCriticalSection(interruptStatus ); 
}

void InitWatchdog(uint16 sleep_interval)
{
    /*==============================================================================*/
    /* configure counter 0 for wakeup interrupt                                     */
    /*==============================================================================*/
    /* Counter 0 of Watchdog time generates peridically interrupt to wakeup system */
    CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
    /* Set interval as desired value */
	CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, ((uint32)(sleep_interval * ILO_FREQ) / 1000));
    /* clear counter on match event */
	CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, 1u);
    
    /*==============================================================================*/
    /* enable watchdog                                                              */
    /*==============================================================================*/
    /* enable the counter 0 */
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
    /* check if counter 0 is enabled, otherwise keep looping here */
    while(!CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER0));
}

int main()
{
    /* initialize watchdog */
    InitWatchdog(SLEEP_INTERVAL_2S);
    /* connect ISR routine to Watchdog interrupt */
    isr_1_StartEx(WDTIsrHandler); // The "isr_WDT" prefix comes from our TopDesign
    /* set the highest priority to make ISR is executed in all condition */
    isr_1_SetPriority(0);
    
    LED_Write(LIGHT_ON);
        
    isr_1_StartEx(WDTIsrHandler);

    LED_Write(LIGHT_OFF);

    
     /* Variable declarations */
     CYBLE_LP_MODE_T lpMode;
     CYBLE_BLESS_STATE_T blessState;
     uint8 interruptStatus;
     /* Enable global interrupts */
     CyGlobalIntEnable;
     
     /* C1. Stop the ILO to reduce current consumption */
     CySysClkIloStop();
     /* C2. Configure the divider values for the ECO, so that a 3-MHz ECO divided 
     clock can be provided to the CPU in Sleep mode */
     CySysClkWriteEcoDiv(CY_SYS_CLK_ECO_DIV8);
     
     /* Start the BLE Component and register the generic event handler */
     apiResult = CyBle_Start(AppCallBack);
     
     /* Wait for BLE Component to initialize */
     while (CyBle_GetState() == CYBLE_STATE_INITIALIZING)
     {
     CyBle_ProcessEvents(); 
     } 
     /*Application-specific Component and other initialization code below */
     applicationPower = ACTIVE;
     
     /* main while loop of the application */
     while(1)
     {
     /* Process all pending BLE events in the stack */
     CyBle_ProcessEvents(); 
     /* C3. Call the function that manages the BLESS power modes */
     CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP); 
     /*C4. Run your application specific code here */
     if(applicationPower == ACTIVE)
     {
        RunApplication();
     }
     
     /*C6. Manage Application power mode */
     ManageApplicationPower(); 
     
     /*C7. Manage System power mode */
     ManageSystemPower();
    }
}

CY_ISR(WDTIsrHandler)
{
    state=!state;
    
    if(state){
        LED_Write(0);
    }
    else {  
        LED_Write(1);
    }
       
    /* clear watchdog counter before deep sleep */
    CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET);
        
    /* clear interrupt flag to enable next interrupt */
    CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);     
}