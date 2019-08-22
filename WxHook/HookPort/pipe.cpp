#include "stdafx.h"
#include<stdio.h>
#include "pipe.h"
#include"../Common/DebugLog.h"
#include"../Common/Common.h"

#define BUFSIZE 512 

HANDLE hPipe;
LPSTR lpvMessage = (char*)"CLIENT";
CHAR chBuf[BUFSIZE];
BOOL fSuccess;
DWORD cbRead, cbWritten, dwMode;
LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\LiudadaNemaedPipe");

DWORD WINAPI pipe_client(LPVOID lpParam)
{
	while (1)
	{
		hPipe = CreateFile(
			lpszPipename,   // pipe name   
			GENERIC_READ |  // read and write access   
			GENERIC_WRITE,
			0,              // no sharing   
			NULL,           // default security attributes  
			OPEN_EXISTING,  // opens existing pipe   
			0,              // default attributes   
			NULL);          // no template file   

							// Break if the pipe handle is valid.   

		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs.   

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
#ifdef Dbg
			DebugLog("Could not open pipe");
#endif
			return 0;
		}

		// All pipe instances are busy, so wait for 20 seconds.   

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
#ifdef Dbg
			DebugLog("Could not open pipe");
#endif
			return 0;
		}
	}
	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // pipe handle   
		&dwMode,  // new pipe mode   
		NULL,     // don't set maximum bytes   
		NULL);    // don't set maximum time   
	if (!fSuccess)
	{
#ifdef Dbg
		DebugLog("SetNamedPipeHandleState failed");
#endif
		return 0;
	}

	// Send a message to the pipe server. 
	DWORD len = htonlx(*(DWORD*)lpParam);
	fSuccess = WriteFile(
		hPipe,				// pipe handle   
		lpParam,            // message   
		len,				// message length   
		&cbWritten,         // bytes written   
		NULL);              // not overlapped  
	free(lpParam);
	lpParam = 0;

	if (!fSuccess)
	{
#ifdef Dbg
		DebugLog("WriteFile failed");
#endif
		return 0;
	}

	do
	{
		// Read from the pipe.   

		fSuccess = ReadFile(
			hPipe,    // pipe handle   
			chBuf,    // buffer to receive reply   
			BUFSIZE,  // size of buffer   
			&cbRead,  // number of bytes read   
			NULL);    // not overlapped   

		if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
			break;

	} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA   

	CloseHandle(hPipe);
	return 0;
}


void pipe_start_thread(DWORD custom_type,DWORD code,std::string &msg)
{
	//´©¼þ
	DWORD dwThreadId = 0;
	unsigned char *p = 0;
	unsigned int len = 0;
	if (syncpack(custom_type, code, msg, &p, &len))
	{
		CreateThread(
			NULL,//default security attributes
			0,//use default stack size
			pipe_client,//thread function
			p,//argument to thread function
			0,//use default creation flags
			&dwThreadId);//returns the thread identifier
	}

	//Check there turn value for success.
	//if (dwThreadId == NULL)
	//{
	//	return;
	//}
}