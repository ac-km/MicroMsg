#pragma once
#include<string>
#include<Windows.h>


//messsage
#define WM_WND_MSG	WM_USER+1

//���ذ�����β'\0'���ֽ��������0x2000
DWORD GetWBufferLen(WCHAR *buffer);

std::string MyWideCharToMultiByte(BYTE *pbDest, int nLen);

DWORD htonlx(DWORD x);


//MSG_CUSTOM�Զ�����Ϣ
#define MSG_CUSTOM		0xFF00		//������Ϣ��DWORD����+0000FF00+DWORD(MSG_00)+�ַ�����
#define MSG_CUSTOM01	0xFF01		//����ͼ��DWORD����+0000FF01+DWORD(MSG_00)+�ַ�����
#define MSG_CUSTOM02	0xFF02		//����wxid��DWORD����+0000FF02+DWORD(MSG_00)+�ַ�����
#define MSG_CUSTOM03	0xFF03		//����΢�źţ�DWORD����+0000FF03+DWORD(MSG_00)+�ַ�����
#define MSG_00			0x00		//�Զ�����Ϣר��

#define MSG_CUSTOMFF	0xFFFF		//ԭʼ����DWORD����+0000FFFF+DWORD(MSG_##)+protobuf��
//MSG_CUSTOMFF΢�ŵ���Ϣ����(MSG_TYPE)
#define MSG_01	0x01

//pip��Ϣ��װ����
bool syncpack(DWORD custom_type, DWORD msg_type, std::string &msg,unsigned char **out,unsigned int *outlen);

//pip��Ϣ���װ����
bool syncunpack(unsigned char *in,DWORD &custom_type, DWORD &msg_type,std::string &out);


typedef struct
{
	DWORD index;
	DWORD custom_type;
	DWORD msg_type;
	std::string data;
}ViewItem,*PViewItem;