#pragma once
#include<string>
#include<Windows.h>


//messsage
#define WM_WND_MSG	WM_USER+1

//���ذ�����β'\0'���ֽ��������0x2000
DWORD GetWBufferLen(WCHAR *buffer);

std::string MyWideCharToMultiByte(BYTE *pbDest, int nLen);

DWORD htonlx(DWORD x);