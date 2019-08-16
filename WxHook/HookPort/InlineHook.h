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
//Define:仅仅支持函数api hook
//

BOOL
InlineHook(
	IN	void *OrgProc,		/* 需要Hook的函数地址 */
	IN	void *NewProc,		/* 代替被Hook函数的地址 */
	OUT	void **RealProc		/* 返回原始函数的入口地址 */
	);

void UnInlineHook(
	void *OrgProc,  /* 需要恢复Hook的函数地址 */
	void *RealProc  /* 原始函数的入口地址 */
	);


//
//Define:函数任意位置hook
//

BOOL
InlineHookFunc(
IN	void *OrgProc,		/* 需要Hook的函数地址 */
IN	void *NewProc,		/* 代替被Hook函数的地址 */
OUT	void **RealProc		/* 返回原始函数的入口地址 */
);
//BUG：注意不能hook call xxxx，jmp xxxx等带有地址的函数，因为改变此指令本身地址，没有对其含有的地址（xxxx）进行修正

void UnInlineHookFunc(
	void *OrgProc,  /* 需要恢复Hook的函数地址 */
	void *RealProc  /* 原始函数的入口地址 */
	);