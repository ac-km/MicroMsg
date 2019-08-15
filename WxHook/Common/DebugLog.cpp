#include <windows.h>
#include <stdio.h>

#include "./DebugLog.h"
#include"../Common/LOG.h"



//
//Usage
//
//#ifdef Dbg
//DebugLog(DbgInfo,L"XXX");
//#endif



//
//Define
//
#define DbgLine "\r\n%s,%s,%s(%d):    "


CLOG gLog;



void DebugLog(LPCSTR lpFILE, LPCSTR lpFUNCTION, int lpLINE, LPCSTR lpOutputString)
{
#ifdef Dbg
	//获取应用程序目录
	char szapipath[MAX_PATH];
	memset(szapipath, 0, MAX_PATH);
	GetModuleFileNameA(NULL, szapipath, MAX_PATH);

	//获取应用程序名称
	char szExe[MAX_PATH] = "";//（TEST.exe）
	char *pbuf = NULL;
	char* szLine = strtok_s(szapipath, "\\", &pbuf);
	while (NULL != szLine)
	{
		strcpy_s(szExe, szLine);
		szLine = strtok_s(NULL, "\\", &pbuf);
	}

	//删除.exe
	//strncpy_s(szapipath, szExe, strlen(szExe) - 4);

	char strDbgLine[0x1000] = {0};
	wsprintfA(strDbgLine,DbgLine, szExe,strrchr(lpFILE,'\\')+1,lpFUNCTION,lpLINE);
	OutputDebugStringA(strDbgLine);
	gLog.LogControl(strDbgLine);
	OutputDebugStringA(lpOutputString);
	gLog.LogControl((char*)lpOutputString);
#endif

	return;
}