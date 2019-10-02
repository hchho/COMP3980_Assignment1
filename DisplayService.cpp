#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "DisplayService.h"
#include "WINDOW.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		DisplayService.cpp -	A service class that handles display events from the application.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					VOID displayMessageBox(const char * content)
--					VOID drawInput(char input)
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
-- This service class can be used to display any visual out put in the application. It should be used for events
-- such as dialogs, message boxes, and drawing.
----------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	drawInput
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID drawInput(char input)
--					char input:	the input to draw on the screen
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to draw a character in the screen
----------------------------------------------------------------------------------------------------------------------*/
VOID DisplayService::drawInput(char input) {
	HDC deviceContext = GetDC(*windowHandle);// get device context	
	TEXTMETRIC textMetric;
	char str[255];
	GetTextMetrics(deviceContext, &textMetric);
	sprintf(str, "%c", input); // convert char to str
	TextOut(deviceContext, textMetric.tmMaxCharWidth * xCoord, yCoord, (LPCWSTR) str, strlen(str)); // output character

	// Resets cursor
	if ((xCoord + 1) * textMetric.tmMaxCharWidth >= WINDOW_WIDTH) {
		yCoord = yCoord + textMetric.tmHeight + textMetric.tmExternalLeading;
		xCoord = 0;
	}
	else {
		xCoord++; // increment the screen x-coordinate
	}

	ReleaseDC(*windowHandle, deviceContext); // Release device context
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	getWindowHandle
--
-- DATE:		Sept 30, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	HWND *  getWindowHandle()
--
-- RETURNS:		pointer to window handle
--
-- NOTES:
-- Call this function to return the window handle
----------------------------------------------------------------------------------------------------------------------*/
HWND * DisplayService::getWindowHandle() {
	return windowHandle;
}