#pragma once
#include<string>

//�ֽ�Ϊ��λ,������β'\0'
DWORD GetWBufferLen(WCHAR *buffer);
std::string MyWideCharToMultiByte(BYTE *pbDest, int nLen);

void pipe_start_thread(LPVOID lpParam = 0);