#pragma once

#include <windows.h>
#include <stdio.h>
#include "key_press.h"
#include "error_codes.h"
#include "ErrorHandler.h"
#include "DisplayService.h"

/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		SerialCommController.h -	A controller class that controls all operations in the physical
--												layer in the OSI architecture.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					VOID drawToWindow(char input)
--					DWORD handleRead(LPVOID input)
--					VOID handleWrite(WPARAM * input)
--					VOID closePort(void)
--					LPCWSTR getComPortName(void) const
--					VOID handleParam(UINT Msg, WPARAM* wParam)
--					VOID initializeConnection(void)
--					VOID resetCommConfig(void)
--					VOID setComPort(LPCWSTR commPortName)
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
-- This controller class should be instantiated at the application session level in order to control physical layer
-- functions. This controller class can open ports, close open ports, reset COM port configurations, and handle
-- messages in connection mode.
----------------------------------------------------------------------------------------------------------------------*/
class SerialCommController {
private:
	char OUTPUT_BUFFER[1] = { 0 };
	
	COMMCONFIG commConfig;
	HANDLE commHandle;
	COMMPROP commProp;
	LPCWSTR commPortName;

	DisplayService * displayService;
	BOOL isComActive = false;
	VOID drawToWindow(char input);
	DWORD handleRead(LPVOID input);
	VOID handleWrite(WPARAM * input);

public:
	static DWORD WINAPI readFunc(LPVOID param) {
		return ((SerialCommController*)param)->handleRead(0);
	};
	
	SerialCommController() {};
	SerialCommController(DisplayService * disp) : displayService(disp) {
		commConfig.dwSize = sizeof(COMMCONFIG);
		commConfig.wVersion = 0x100;
		commPortName = TEXT("COM1");
	};
	VOID closePort();
	VOID handleParam(WPARAM* wParam); 
	VOID initializeConnection(LPCWSTR portName);
	VOID setCommConfig(LPCWSTR portName);
};