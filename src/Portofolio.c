/*
 ============================================================================
 Name        : Portofolio.c
 Author      : 
 Version     :
 Copyright   : DDM
 Description : Portofolio Project in C, Ansi-style
 ============================================================================
 */

#include "defines.h"
#include "os.h"
#include "ForOthers.h"

static uint16 PORT_l_u16GenericTimeout;

static PORT_tenAPPReturns PORT_l_fenControlHandlers( PORT_tenHandlers enHandler, PORT_tenHandlerState enState )
{
  PORT_tenAPPReturns enReturnCode = hPORT_nenUndefinedReturn;

  if(enHandler < hPMG_nenTotalHandler)
  {
    /* Valid Handler */
    if(enState < hPORT_nenTotalHandlerStates)
    {
      /* Valid Handler State */
      PORT_stModuleCallBacksHook[enHandler].state = enState;
      enReturnCode = hPORT_nenSuccess;
    }
    else
    {
      /* Invalid Handler State */
      enReturnCode = hPORT_nenInvalidParameter;
    }
  }
  else
  {
    /* Invalid handler */
    enReturnCode = hPORT_nenInvalidParameter;
  }

  return enReturnCode;
}

static void PORT_l_fu8InitTheAppState(void)
{
  uint8 u8indexHandlers;

  for(u8indexHandlers = 0U; u8indexHandlers < (sizeof(PORT_stModuleCallBacksHook) / sizeof(PORT_stModuleCallBacksHook[hPORT_nenHandlerCANStuffing])); u8indexHandlers ++)
  {
    if( hPORT_nenHandlerEnabled == PORT_stModuleCallBacksHook[u8indexHandlers].state )
    {
      /* Handler enabled */
      if(NULL != PORT_stModuleCallBacksHook[u8indexHandlers].pfStartFunction)
      {
        PORT_stModuleCallBacksHook[u8indexHandlers].pfStartFunction();
      }
      else
      {
        /* NULL pointer*/
      }
    }
    else
    {
      /* Handler disabled! */
    }
  }

  PORT_l_enApplicationStateMachine = hPORT_nenAPPHandlig;
}

static void PORT_l_fu8EndTheAppState(void)
{
  uint8 u8indexHandlers;

  for(u8indexHandlers = 0U; u8indexHandlers < (sizeof(PORT_stModuleCallBacksHook) / sizeof(PORT_stModuleCallBacksHook[hPORT_nenHandlerCANStuffing])); u8indexHandlers ++)
  {
    if( hPORT_nenHandlerEnabled == PORT_stModuleCallBacksHook[u8indexHandlers].state )
    {
      /* Handler enabled */
      if(NULL != PORT_stModuleCallBacksHook[u8indexHandlers].pfStopFunction)
      {
        PORT_stModuleCallBacksHook[u8indexHandlers].pfStopFunction();
        PORT_stModuleCallBacksHook[u8indexHandlers].state = hPORT_nenHandlerDisabled;
      }
      else
      {
        /* NULL pointer*/
      }
    }
    else
    {
      /* Handler disabled! */
    }
  }

  PORT_l_enApplicationStateMachine = hPORT_nenAPPEnableHandlers;
}

void PORT_g_fvDecideTheReturn(PORT_tenAPPReturns enRet)
{
  switch(enRet)
  {
    case hPORT_nenSuccess:
    {
      DDM_DBG("\r\n -> Success Operation.");
      break;
    }

    case hPORT_nenFailure:
    {
      DDM_DBG("\r\n -> Failure Operation.");
      break;
    }

    case hPORT_nenBusy:
    {
      DDM_DBG("\r\n -> Busy Operation.");

      break;
    }

    case hPORT_nenNULLPointer:
    {
      DDM_DBG("\r\n -> NULL Operation.");

      break;
    }

    case hPORT_nenInvalidParameter:
    {
      DDM_DBG("\r\n -> Invalid Parameter(s) Operation.");

      break;
    }

    case hPORT_nenUndefinedReturn:
    {
      DDM_DBG("\r\n -> Undefined Operation.");

      break;
    }

    case hPORT_nenFullReturn:
    {
      DDM_DBG("\r\n -> Buffer Full Operation.");

      break;
    }

    default:
    {
      DDM_DBG("\r\n -> Default Operation.");

      break;
    }
  }
}

static void PORT_l_fvEnableHandlersState( PORT_tenHandlers enHND )
{
  PORT_tenAPPReturns enReturnCallStuff, enReturnCallDestuff, enReturnCallKlineInitTester, enReturnCallKlineInitECU;

  PORT_tenAPPReturns enReturnCallCANTP, enReturnCallCANMultiClient, enReturnCallCANMultiServer, enReturnCallLINFrame;

  switch(enHND)
  {
    case hPORT_nenHandlerCANStuffing:
    {
      DDM_DBG("\r\n Enable CAN Stuffing.");

      enReturnCallStuff = PORT_l_fenControlHandlers(hPORT_nenHandlerCANStuffing, hPORT_nenHandlerEnabled);
      PORT_g_fvDecideTheReturn(enReturnCallStuff);

      DDM_DBG("\r\n Enable CAN Destuffing.");

      enReturnCallDestuff = PORT_l_fenControlHandlers(hPORT_nenHandlerCANDestuffing, hPORT_nenHandlerEnabled);
      PORT_g_fvDecideTheReturn(enReturnCallDestuff);

      if( (hPORT_nenSuccess == enReturnCallStuff) && (hPORT_nenSuccess == enReturnCallDestuff) )
      {
        /* Go into next SM */
        DDM_DBG("\r\n Enabling SUCCESS.");

        PORT_l_enApplicationStateMachine = hPORT_nenAPPInit;
      }
      else
      {
        /* Stay here. We did something wrong. */
      }
     break;
    }

    case hPORT_nenHandlerKlineFastInitTester:
    {
      DDM_DBG("\r\n Enable Kline FastInit Tester.");

      enReturnCallKlineInitTester = PORT_l_fenControlHandlers(hPORT_nenHandlerKlineFastInitTester, hPORT_nenHandlerEnabled);
      PORT_g_fvDecideTheReturn(enReturnCallKlineInitTester);

      DDM_DBG("\r\n Enable Kline FastInit ECU.");

      enReturnCallKlineInitECU = PORT_l_fenControlHandlers(hPORT_nenHandlerKlineFastInitECU, hPORT_nenHandlerEnabled);
      PORT_g_fvDecideTheReturn(enReturnCallKlineInitTester);

      if( (hPORT_nenSuccess == enReturnCallKlineInitTester) && (hPORT_nenSuccess == enReturnCallKlineInitECU) )
      {
        /* Go into next SM */
        DDM_DBG("\r\n Enabling SUCCESS.");

        PORT_l_enApplicationStateMachine = hPORT_nenAPPInit;
      }
      else
      {
        /* Stay here. We did something wrong. */
      }
      break;
    }

    case hPORT_nenHandlerCANTP:
    {
      DDM_DBG("\r\n Enable CAN Transport Protocol.");

      enReturnCallCANTP = PORT_l_fenControlHandlers(hPORT_nenHandlerCANTP, hPORT_nenHandlerEnabled);
      PORT_g_fvDecideTheReturn(enReturnCallCANTP);

      DDM_DBG("\r\n Enable CAN MultiFrame Client.");

      enReturnCallCANMultiClient = PORT_l_fenControlHandlers(hPORT_nenHandlerCANMultiFrameClient, hPORT_nenHandlerEnabled);
      PORT_g_fvDecideTheReturn(enReturnCallCANMultiClient);

      DDM_DBG("\r\n Enable CAN MultiFrame Server.");

      enReturnCallCANMultiServer = PORT_l_fenControlHandlers(hPORT_nenHandlerCANMultiFrameServer, hPORT_nenHandlerEnabled);
      PORT_g_fvDecideTheReturn(enReturnCallCANMultiServer);

      if( (hPORT_nenSuccess == enReturnCallCANTP) && (hPORT_nenSuccess == enReturnCallCANMultiClient) && (hPORT_nenSuccess == enReturnCallCANMultiServer) )
      {
        /* Go into next SM */
        DDM_DBG("\r\n Enabling SUCCESS.");

        PORT_l_enApplicationStateMachine = hPORT_nenAPPInit;
      }
      else
      {
        /* Stay here. We did something wrong. */
      }
      break;
    }

    case hPORT_nenHandlerLINFrame:
    {

      DDM_DBG("\r\n Enable LIN Frame.");

      enReturnCallLINFrame = PORT_l_fenControlHandlers(hPORT_nenHandlerLINFrame, hPORT_nenHandlerEnabled);
      PORT_g_fvDecideTheReturn(enReturnCallLINFrame);

      if( hPORT_nenSuccess == enReturnCallLINFrame )
      {
        /* Go into next SM */
        DDM_DBG("\r\n Enabling SUCCESS.");

        PORT_l_enApplicationStateMachine = hPORT_nenAPPInit;
      }
      else
      {
        /* Stay here. We did something wrong. */
      }
      break;
    }


    default: break;
  }
}

static void PORT_l_fvHandlingTheAppState( void )
{
  uint8 u8indexHandlers;

  /* Let's enter in the OS loop. */
  for(u8indexHandlers = 0U; u8indexHandlers < (sizeof(PORT_stModuleCallBacksHook) / sizeof(PORT_stModuleCallBacksHook[hPORT_nenHandlerCANStuffing])); u8indexHandlers ++)
  {
    if( hPORT_nenHandlerEnabled == PORT_stModuleCallBacksHook[u8indexHandlers].state )
    {
      /* Handler enabled */
      if(NULL != PORT_stModuleCallBacksHook[u8indexHandlers].pfHandler)
      {
        PORT_stModuleCallBacksHook[u8indexHandlers].pfHandler();
      }
      else
      {
        /* NULL pointer*/
      }
    }
    else
    {
      /* Handler disabled! */
    }
  }
}

void PORT_l_fvResetTimeout(void)
{
  PORT_l_u16GenericTimeout = 0x0000u;
}

PORT_tenHandlers PORT_l_fvInputFromUser(void)
{
  PORT_tenHandlers enHandlerSelect;
  uint8 u8Selection = 0xFF;

  DDM_DBG("\r\n Select an example to run.");
  DDM_DBG("\r\n\t 1. CAN Stuffing/Destuffing.");
  DDM_DBG("\r\n\t 2. Kline Fast Init.");
  DDM_DBG("\r\n\t 3. CAN Multiframe.");
  DDM_DBG("\r\n\t 4. LIN Frame.");

  DDM_DBG("\r\n\t 5. Clear the screen.");

  DDM_DBG("\r\n\n DDM ->");

  scanf("%d", &u8Selection);

  switch(u8Selection)
  {
    case 0x01:
    {
      enHandlerSelect = hPORT_nenHandlerCANStuffing;
      break;
    }

    case 0x02:
    {
      enHandlerSelect = hPORT_nenHandlerKlineFastInitTester;
      break;
    }

    case 0x03:
    {
      enHandlerSelect = hPORT_nenHandlerCANTP;
      break;
    }

    case 0x04:
    {
      enHandlerSelect = hPORT_nenHandlerLINFrame;
      break;
    }

    case 0x05:
    {
      enHandlerSelect = hPMG_nenTotalHandler;
      clrscr();
      break;
    }

    default:
    {
      enHandlerSelect = hPMG_nenTotalHandler;
    }
  }

  return enHandlerSelect;
}

int main(void)
{
  PORT_tenHandlers enHandlerRet = hPMG_nenTotalHandler;

  while(1)
  {
    switch(PORT_l_enApplicationStateMachine)
    {
      case hPORT_nenAPPEnableHandlers:
      {
	enHandlerRet = PORT_l_fvInputFromUser();
        PORT_l_fvEnableHandlersState(enHandlerRet);
        PORT_l_fvResetTimeout();

        break;
      }

      case hPORT_nenAPPInit:
      {
        PORT_l_fu8InitTheAppState();

        break;
      }

      case hPORT_nenAPPHandlig:
      {
	if(MAX_ALLOWED_CALLS > PORT_l_u16GenericTimeout)
	{
	  PORT_l_fvHandlingTheAppState();
	  PORT_l_u16GenericTimeout++;
	}
	else
	{
	  PORT_l_enApplicationStateMachine = hPORT_nenAPPEnd;
	}

        break;
      }

      case hPORT_nenAPPEnd:
      {
	PORT_l_fu8EndTheAppState();
	break;
      }

      default: break;
    }
  }

  return EXIT_SUCCESS;
}



