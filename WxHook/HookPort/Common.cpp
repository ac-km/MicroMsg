#include "stdafx.h"
#include "Common.h"

DWORD GetWBufferLen(WCHAR *buffer)
{
	DWORD len = 0;
	DWORD MAX_LEN = 0x400;
	do
	{
		if (buffer[len++] == 0) break;
	} while (len < MAX_LEN);
	return len*sizeof(WCHAR);
}