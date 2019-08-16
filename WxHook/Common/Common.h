#pragma once
#include<string>
#include<Windows.h>


//messsage
#define WM_WND_MSG	WM_USER+1

//返回包含结尾'\0'的字节数，最大0x2000
DWORD GetWBufferLen(WCHAR *buffer);

std::string MyWideCharToMultiByte(BYTE *pbDest, int nLen);

DWORD htonlx(DWORD x);