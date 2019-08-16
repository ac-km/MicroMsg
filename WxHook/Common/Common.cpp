#include<stdio.h>
#include "Common.h"
#include"../HookPort/pipe.h"

//返回字节数
DWORD GetWBufferLen(WCHAR *buffer)
{
	DWORD len = 0;
	DWORD MAX_LEN = 0x2000;
	do
	{
		if (buffer[len++] == 0) break;
	} while (len < MAX_LEN);
	return len*sizeof(WCHAR);
}

DWORD htonlx(DWORD x)
{
	char *s = (char *)&x;
	return (DWORD)(s[0] << 24 | s[1] << 16 | s[2] << 8 | s[3]);
}

std::string MyWideCharToMultiByte(BYTE *pbDest, int nLen)
{
	std::wstring wstr = L"";
	wstr.append((WCHAR*)pbDest, nLen);
	// 预算-缓冲区中多字节的长度    
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// 给指向缓冲区的指针变量分配内存    
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
	// 开始向缓冲区转换字节    
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}
