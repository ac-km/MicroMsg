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
	unsigned char *s = (unsigned char *)&x;
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

//pip消息封装函数
bool syncpack(DWORD custom_type, DWORD msg_type, std::string &msg, unsigned char **out, unsigned int *outlen)
{
	DWORD len = 0;
	if (msg.size())
	{
		len = msg.size() + 12;
		//从后往前插入
		//插入MSG_CUSTOMFF微信定义的消息类型
		char c1 = (msg_type & 0xFF000000) >> 24;
		char c2 = (msg_type & 0x00FF0000) >> 16;
		char c3 = (msg_type & 0x0000FF00) >> 8;
		char c4 = (msg_type & 0x000000FF);
		msg.insert(msg.begin(), c4);
		msg.insert(msg.begin(), c3);
		msg.insert(msg.begin(), c2);
		msg.insert(msg.begin(), c1);
		//插入custom_type
		c1 = (custom_type & 0xFF000000) >> 24;
		c2 = (custom_type & 0x00FF0000) >> 16;
		c3 = (custom_type & 0x0000FF00) >> 8;
		c4 = (custom_type & 0x000000FF);
		msg.insert(msg.begin(), c4);
		msg.insert(msg.begin(), c3);
		msg.insert(msg.begin(), c2);
		msg.insert(msg.begin(), c1);
		//插入头部长度
		c1 = (len & 0xFF000000) >> 24;
		c2 = (len & 0x00FF0000) >> 16;
		c3 = (len & 0x0000FF00) >> 8;
		c4 = (len & 0x000000FF);
		msg.insert(msg.begin(), c4);
		msg.insert(msg.begin(), c3);
		msg.insert(msg.begin(), c2);
		msg.insert(msg.begin(), c1);
		*out = (unsigned char*)malloc(len);
		memcpy(*out, msg.c_str(), len);
		return true;
	}
	return false;
}

//pip消息解封装函数
bool syncunpack(unsigned char *in, DWORD &custom_type, DWORD &msg_type, std::string &out)
{
	unsigned int len = htonlx(*(unsigned int*)in);
	custom_type = htonlx(*(unsigned int*)(in + 4));
	msg_type= htonlx(*(unsigned int*)(in + 8));
	out.append((char*)in + 12, len - 12);
	return true;
}
