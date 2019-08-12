#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>
#include <stdio.h>

#include "./Dispatch_NTDLL_NtSetInformationFile.h"
#include"../../Common/DebugLog.h"
#include"../HookPort.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "./Dispatch_NTDLL_NtClose.h"
#include "../NativeAPI_NTDLL.h"



//
//Global
//
__pfnNtSetInformationFile pfnNtSetInformationFile = NULL;



//
//Dispatch_NTDLL_NtSetInformationFile Functions
//
NTSTATUS
NTAPI
OnNtSetInformationFile(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass)
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
		nRet = pfnNtSetInformationFile(
			FileHandle,
			IoStatusBlock,
			FileInformation,
			Length,
			FileInformationClass
			);

		return nRet;
	}
	nRet = pfnNtSetInformationFile(
		FileHandle,
		IoStatusBlock,
		FileInformation,
		Length,
		FileInformationClass
		);
	return nRet;
}

NTSTATUS
NTAPI
NtSetInformationFile(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtSetInformationFile addFun = (__pfnNtSetInformationFile)GetProcAddress(hDll,"NtSetInformationFile");
	if(addFun)
	{
		ret = addFun(FileHandle,IoStatusBlock,FileInformation,Length,FileInformationClass);
	}
	return ret;
}