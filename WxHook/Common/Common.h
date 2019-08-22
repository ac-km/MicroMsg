#pragma once
#include<string>
#include<Windows.h>


//messsage
#define WM_WND_MSG	WM_USER+1

//返回包含结尾'\0'的字节数，最大0x2000
DWORD GetWBufferLen(WCHAR *buffer);

std::string MyWideCharToMultiByte(BYTE *pbDest, int nLen);

DWORD htonlx(DWORD x);


//MSG_CUSTOM自定义消息
#define MSG_CUSTOM		0xFF00		//其它消息（DWORD长度+0000FF00+DWORD(MSG_00)+字符串）
#define MSG_CUSTOM01	0xFF01		//个人图像（DWORD长度+0000FF01+DWORD(MSG_00)+字符串）
#define MSG_CUSTOM02	0xFF02		//个人wxid（DWORD长度+0000FF02+DWORD(MSG_00)+字符串）
#define MSG_CUSTOM03	0xFF03		//个人微信号（DWORD长度+0000FF03+DWORD(MSG_00)+字符串）
#define MSG_00			0x00		//自定义消息专用

#define MSG_CUSTOMFF	0xFFFF		//原始流（DWORD长度+0000FFFF+DWORD(MSG_##)+protobuf）
//MSG_CUSTOMFF微信的消息类型(MSG_TYPE)
#define MSG_01	0x01

//pip消息封装函数
bool syncpack(DWORD custom_type, DWORD msg_type, std::string &msg,unsigned char **out,unsigned int *outlen);

//pip消息解封装函数
bool syncunpack(unsigned char *in,DWORD &custom_type, DWORD &msg_type,std::string &out);


typedef struct
{
	DWORD index;
	DWORD custom_type;
	DWORD msg_type;
	std::string data;
}ViewItem,*PViewItem;