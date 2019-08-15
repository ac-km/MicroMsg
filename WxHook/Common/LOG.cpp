#include<tchar.h>
#include <string>
#include "LOG.h"
#include"DebugLog.h"

CLOG::CLOG(void)
{
	ZeroMemory(this->GetPath(),MAX_PATH);
	ZeroMemory(this->GetTemp(),MAX_PATH);

	//::GetModuleFileNameA(NULL,this->GetPath(),MAX_PATH);
	//*(strrchr(this->GetPath(),'\\')+1)=0x00;

	memcpy(this->GetPath(), "C:\\Users\\Administrator\\Desktop\\", strlen("C:\\Users\\Administrator\\Desktop\\"));

	memcpy(this->GetTemp(),this->GetPath(),MAX_PATH);
	strcat_s(this->GetTemp(),MAX_PATH,"LOG");
	if(GetFileAttributesA(this->GetTemp())!=FILE_ATTRIBUTE_DIRECTORY)
	{
		OutputDebugStringA("创建日志目录");
		if(!CreateDirectoryA(this->GetTemp(),NULL))
		{
			OutputDebugStringA("创建日志目录失败");
			return;
		}
	}
}

bool CLOG::LogControl(char *pMsg)
{
	char buffer[MAX_PATH]={0};

	ZeroMemory(this->GetTemp(),MAX_PATH);
	memcpy_s(this->GetTemp(),MAX_PATH,this->GetPath(),MAX_PATH);
	strcat_s(this->GetTemp(),MAX_PATH,"LOG\\MainLog.txt");
	//生成当前的时间
	SetLastError(ERROR_SUCCESS);
	HANDLE hFile=CreateFileA(this->GetTemp(),GENERIC_WRITE|GENERIC_READ,NULL,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	SYSTEMTIME st;
	GetLocalTime(&st);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		OutputDebugStringA("日志文件创建失败");
		return FALSE;
	}

	ZeroMemory(buffer,sizeof(buffer));

	if(GetLastError()==ERROR_SUCCESS)
	{
		//sprintf(buffer,"%c%d-%02d-%02d %02d:%02d:%02d  %s",0xfeff,st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,pMsg);
		sprintf_s(buffer,"%c%s",0xfeff,pMsg);
	}else
	{
		//sprintf(buffer,"\r\n%d-%02d-%02d %02d:%02d:%02d  %s",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,pMsg);
		sprintf_s(buffer,"\r\n%s",pMsg);
	}
	
	DWORD WriteNum=0;
	if(SetFilePointer(hFile,0,NULL,FILE_END)==0xFFFFFFFF)
	{
		OutputDebugStringA("文件错误");
		return FALSE;
	}

	if(!WriteFile(hFile,buffer,strlen(buffer),&WriteNum,NULL))
	{
		OutputDebugStringA("日志文件写失败");
		return FALSE;
	}
	CloseHandle(hFile);
	return TRUE;
}

CLOG::~CLOG(void)
{
}
