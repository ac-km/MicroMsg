#pragma once
#include<string>

//字节为单位,包含结尾'\0'
DWORD GetWBufferLen(WCHAR *buffer);
std::string MyWideCharToMultiByte(BYTE *pbDest, int nLen);

void pipe_start_thread(LPVOID lpParam = 0);