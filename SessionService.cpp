#include <stdlib.h>
#include <windows.h>
#include "error_codes.h"
#include "key_press.h"
#include "modes.h"
#include "ErrorHandler.h"
#include "SessionService.h"
#include "idm.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		SessionService.cpp -A class that handles all session level events according to the OSI network
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	createReadThread
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID createReadThread(void)
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to create a thread to enable the application to read inputs from the COM port.
----------------------------------------------------------------------------------------------------------------------*/
VOID SessionService::createReadThread() {
	DWORD readThreadId;
	HANDLE readThread;
	readThread = CreateThread(NULL, 0, SerialCommController::readFunc, (LPVOID)commController, 0, &readThreadId);
	if (!readThread) {
		ErrorHandler::handleError(ERROR_RD_THREAD);
	}
	else {
		CloseHandle(readThread);
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	handleCommandMode
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID handleCommandMode(UINT Message, WPARAM wParam)
--					UINT Message:	the message dispatched to handle
--					WPARAM wParam:	the parameter attached to the event
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function during command mode to handle incoming messages and parameters. If an event is to
-- exit command mode, it configures the current mode to command mode.
----------------------------------------------------------------------------------------------------------------------*/
VOID SessionService::handleCommandMode(UINT Message, WPARAM wParam) {
	switch (Message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_COM1:
			commController->setCommConfig(TEXT("COM1"));
			break;
		case IDM_COM2:
			commController->setCommConfig(TEXT("COM1"));
			break;
		case IDM_Connect_COM1:
			commController->initializeConnection(TEXT("COM1"));
			createReadThread();
			currentMode = CONNECT_MODE;
			break;
		case IDM_Connect_COM2:
			commController->initializeConnection(TEXT("COM2"));
			createReadThread();
			currentMode = CONNECT_MODE;
			break;
		case IDM_Exit:
			commController->closePort();
			PostQuitMessage(0);
			break;
		case IDM_HELP:
			DisplayService::displayMessageBox("Press <ESC> to disconnect.");
			break;
		default:
			break;
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	handleConnectMode
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID handleConnectMode(UINT Message, WPARAM wParam)
--					UINT Message:	the message dispatched to handle
--					WPARAM wParam:	the parameter attached to the event
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function during connect mode to handle incoming messages and parameters.
----------------------------------------------------------------------------------------------------------------------*/
VOID SessionService::handleConnectMode(UINT Message, WPARAM wParam) {
	switch (Message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_Exit:
			commController->closePort();
			PostQuitMessage(0);
			currentMode = COMMAND_MODE;
			break;
		case IDM_HELP:
			DisplayService::displayMessageBox("Press <ESC> to disconnect.");
			break;
		default:
			DisplayService::displayMessageBox("In connect mode. Press <ESC> to disconnect.");
			break;
		}
	case WM_CHAR:
		switch (wParam) {
		case ESC_KEY:
			commController->closePort();
			currentMode = COMMAND_MODE;
			break;
		default:
			commController->handleParam(&wParam);
			break;
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	handleProcess
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID handleProcess(UINT Message, WPARAM wParam)
--					UINT Message:	the message dispatched to handle
--					WPARAM wParam:	the parameter attached to the event
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function in the main window processing function to handle event messages. This function passes
-- the messages to different handlers based on the application's mode.
----------------------------------------------------------------------------------------------------------------------*/
VOID SessionService::handleProcess(UINT Message, WPARAM wParam) {
	switch (currentMode) {
	case COMMAND_MODE:
		handleCommandMode(Message, wParam);
		break;
	case CONNECT_MODE:
		handleConnectMode(Message, wParam);
		break;
	default:
		break;
	}
}