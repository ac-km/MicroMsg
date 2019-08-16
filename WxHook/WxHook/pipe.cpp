#include "stdafx.h"  
#include "pipe.h"
#include"../Common/DebugLog.h"
#include"../Common/Common.h"


#define BUFSIZE 4096 

BOOL fConnected;
DWORD dwThreadId;
HANDLE hPipe, hThread;
LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\LiudadaNemaedPipe");
HWND mhandle;

DWORD WINAPI InstanceThread(LPVOID);
VOID GetAnswerToRequest(LPSTR, LPSTR, LPDWORD);

DWORD WINAPI InstanceThread(LPVOID lpvParam)
{
	TCHAR chRequest[BUFSIZE] = {0};
	TCHAR chReply[BUFSIZE] = {0};
	DWORD cbBytesRead, cbReplyBytes, cbWritten;
	BOOL fSuccess;
	HANDLE mPipe;

	// The thread's parameter is a handle to a pipe instance.   

	mPipe = (HANDLE)lpvParam;

	while (1)
	{
		// Read client requests from the pipe.   
		fSuccess = ReadFile(
			mPipe,        // handle to pipe   
			chRequest,    // buffer to receive data   
			BUFSIZE, // size of buffer   
			&cbBytesRead, // number of bytes read   
			NULL);        // not overlapped I/O   

		if (!fSuccess || cbBytesRead == 0)
			break;

		GetAnswerToRequest(chRequest, chReply, &cbReplyBytes);

		// Write the reply to the pipe.   
		fSuccess = WriteFile(
			mPipe,        // handle to pipe   
			chReply,      // buffer to write from   
			cbReplyBytes, // number of bytes to write   
			&cbWritten,   // number of bytes written   
			NULL);        // not overlapped I/O   

		if (!fSuccess || cbReplyBytes != cbWritten) break;
	}

	// Flush the pipe to allow the client to read the pipe's contents   
	// before disconnecting. Then disconnect the pipe, and close the   
	// handle to this pipe instance.   

	FlushFileBuffers(mPipe);
	DisconnectNamedPipe(mPipe);
	CloseHandle(mPipe);

	return 1;
}

VOID GetAnswerToRequest(LPSTR chRequest,
	LPSTR chReply, LPDWORD pchBytes)
{
	if (mhandle)
	{
		::SendMessageA(mhandle, WM_WND_MSG, (WPARAM)1, (LPARAM)chRequest);
	}
	if (strcmp(chRequest, "CLIENT") == 0)
	{
		StringCchCopyA(chReply, BUFSIZE, "HANDLE");
		*pchBytes = (strlen(chReply) + 1);

	}
	else
	{
		StringCchCopyA(chReply, BUFSIZE, "ERROR");
		*pchBytes = (strlen(chReply) + 1);
	}
}


DWORD WINAPI pipe_server(LPVOID lpParam)
{
	mhandle = *(HWND*)lpParam;
	for (;;)
	{
		hPipe = CreateNamedPipe(
			lpszPipename,             // pipe name            指向管道名称的指针
			PIPE_ACCESS_DUPLEX,       // read/write access    管道打开模式
			PIPE_TYPE_MESSAGE |       // message type pipe    管道模式
			PIPE_READMODE_MESSAGE |   // message-read mode   
			PIPE_WAIT,                // blocking mode   
			PIPE_UNLIMITED_INSTANCES, // max. instances       最大实例数 
			BUFSIZE,                  // output buffer size   输出缓存大小
			BUFSIZE,                  // input buffer size    输入缓存大小
			0,                        // client time-out      超时设置
			NULL);                    // default security attribute   

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			printf("CreatePipe failed");
			return 0;
		}

		// Wait for the client to connect; if it succeeds,   
		// the function returns a nonzero value. If the function  
		// returns zero, GetLastError returns ERROR_PIPE_CONNECTED.   

		fConnected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (fConnected)
		{
			// Create a thread for this client.   
			hThread = CreateThread(
				NULL,              // no security attribute   
				0,                 // default stack size   
				InstanceThread,    // thread proc  
				(LPVOID)hPipe,    // thread parameter   
				0,                 // not suspended   
				&dwThreadId);      // returns thread ID   

			if (hThread == NULL)
			{
				printf("CreateThread failed");
				return 0;
			}
			else CloseHandle(hThread);
		}
		else
		{
			// The client could not connect, so close the pipe.   
			CloseHandle(hPipe);
		}
	}
}