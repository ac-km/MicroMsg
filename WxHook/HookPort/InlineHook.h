#pragma once


BOOL
WriteReadOnlyMemory(
	LPBYTE	lpDest,
	LPBYTE	lpSource,
	ULONG	Length
	);

BOOL 
GetPatchSize(
	IN	void *Proc,
	IN	DWORD dwNeedSize,
	OUT LPDWORD lpPatchSize
	);

//
//Define:����֧�ֺ���api hook
//

BOOL
InlineHook(
	IN	void *OrgProc,		/* ��ҪHook�ĺ�����ַ */
	IN	void *NewProc,		/* ���汻Hook�����ĵ�ַ */
	OUT	void **RealProc		/* ����ԭʼ��������ڵ�ַ */
	);

void UnInlineHook(
	void *OrgProc,  /* ��Ҫ�ָ�Hook�ĺ�����ַ */
	void *RealProc  /* ԭʼ��������ڵ�ַ */
	);


//
//Define:��������λ��hook
//

BOOL
InlineHookFunc(
IN	void *OrgProc,		/* ��ҪHook�ĺ�����ַ */
IN	void *NewProc,		/* ���汻Hook�����ĵ�ַ */
OUT	void **RealProc		/* ����ԭʼ��������ڵ�ַ */
);
//BUG��ע�ⲻ��hook call xxxx��jmp xxxx�ȴ��е�ַ�ĺ�������Ϊ�ı��ָ����ַ��û�ж��京�еĵ�ַ��xxxx����������

void UnInlineHookFunc(
	void *OrgProc,  /* ��Ҫ�ָ�Hook�ĺ�����ַ */
	void *RealProc  /* ԭʼ��������ڵ�ַ */
	);