#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>

#include"../HookPort.h"
#include "./Dispatch_NTDLL_NtWriteFile.h"
#include"../../Common/DebugLog.h"
#include "../NativeAPI_NTDLL.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "./Dispatch_NTDLL_NtClose.h"



//
//Global
//
__pfnNtWriteFile pfnNtWriteFile = NULL;



//
//Dispatch_NTDLL_NtWriteFile Functions
//
NTSTATUS
NTAPI
OnNtWriteFile(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	IN PVOID                Buffer,
	IN ULONG                Length,
	IN PLARGE_INTEGER       ByteOffset OPTIONAL,
	IN PULONG               Key OPTIONAL)
{
	NTSTATUS nRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		nRet = pfnNtWriteFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			Buffer,
			Length,
			ByteOffset,
			Key
			);

		return nRet;
	}
	nRet = pfnNtWriteFile(
		FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		Buffer,
		Length,
		ByteOffset,
		Key
		);

	return nRet;
}

NTSTATUS
NTAPI
NtWriteFile(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	IN PVOID                Buffer,
	IN ULONG                Length,
	IN PLARGE_INTEGER       ByteOffset OPTIONAL,
	IN PULONG               Key OPTIONAL)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtWriteFile addFun = (__pfnNtWriteFile)GetProcAddress(hDll,"NtWriteFile");
	if(addFun)
	{
		ret = addFun(FileHandle,Event,ApcRoutine,ApcContext,IoStatusBlock,Buffer,Length,ByteOffset,Key);
	}
	return ret;
}