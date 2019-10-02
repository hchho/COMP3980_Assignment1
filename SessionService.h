#pragma once

#include <windows.h>
#include "modes.h"
#include "SerialCommController.h"

/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		SessionService.h -	A class that handles all session level events according to the OSI network 
--										architecture.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:		
--					VOID createReadThread(void)
--					VOID handlePortConfig(LPCWSTR portName)
--					VOID handleCommandeMode(UINT Message, WPARAM wParam)
--					VOID handleConnectMode(UINT Message, WPARAM wParam)
--					VOID handleProcess(UINT Message, WPARAM wParam);
--
--
-- DATE:			Sept 28, 2019
--
-- REVISIONS:		(N/A)
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho
--
-- NOTES:
-- The service class handles messages from the system. All actions are mapped to the menu items defined in WINMENU
-- resource file.
----------------------------------------------------------------------------------------------------------------------*/
class SessionService {
private:
	SerialCommController * commController;
	VOID createReadThread();
	INT currentMode;

	VOID handleCommandMode(UINT Message, WPARAM wParam);
	VOID handleConnectMode(UINT Message, WPARAM wParam);
public:
	SessionService() {};
	SessionService(SerialCommController * controller) : commController(controller) {
		currentMode = COMMAND_MODE;
	};
	VOID handleProcess(UINT Message, WPARAM wParam);
};