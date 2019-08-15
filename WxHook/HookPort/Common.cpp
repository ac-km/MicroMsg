#include "stdafx.h"
#include<stdio.h>
#include "Common.h"
#include"pipe.h"

DWORD GetWBufferLen(WCHAR *buffer)
{
	DWORD len = 0;
	DWORD MAX_LEN = 0x1000;
	do
	{
		if (buffer[len++] == 0) break;
	} while (len < MAX_LEN);
	return len*sizeof(WCHAR);
}

std::string MyWideCharToMultiByte(BYTE *pbDest, int nLen)
{
	//计算需要多少个字节才能表示对应的多字节字符串
	DWORD num = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)pbDest, nLen, NULL, 0, NULL, 0);
	//开辟空间
	char *pChar = NULL;
	pChar = (char*)malloc(num * sizeof(char));
	if (pChar == NULL)
	{
		free(pChar);
	}
	memset(pChar, 0, num * sizeof(char));
	//将宽字节字符串转换为多字节字符串
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)pbDest, nLen, pChar, num, NULL, 0);
	std::string retString;
	retString.append(pChar, num);
	free(pChar);
	return retString;
}

void pipe_start_thread(LPVOID lpParam)
{
	//穿件
	DWORD dwThreadId = 0;
	char *p = 0;
	if (lpParam)
	{
		int len = strlen((char*)lpParam) + 1;
		p =(char*)malloc(len);
		memcpy(p, lpParam, len);
	}
	CreateThread(
		NULL,//default security attributes
		0,//use default stack size
		pipe_client,//thread function
		p,//argument to thread function
		0,//use default creation flags
		&dwThreadId);//returns the thread identifier
	
					 
					 //Check there turn value for success.
	if (dwThreadId == NULL)
	{
		return;
	}
}