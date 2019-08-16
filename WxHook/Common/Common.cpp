#include<stdio.h>
#include "Common.h"
#include"../HookPort/pipe.h"

//�����ֽ���
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
	// Ԥ��-�������ж��ֽڵĳ���    
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// ��ָ�򻺳�����ָ����������ڴ�    
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
	// ��ʼ�򻺳���ת���ֽ�    
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}
