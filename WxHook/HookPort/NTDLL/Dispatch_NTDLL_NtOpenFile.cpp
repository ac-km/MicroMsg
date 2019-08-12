#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>

#include "./Dispatch_NTDLL_NtOpenFile.h"
#include "../../Common/DebugLog.h"
#include"../HookPort.h"
#include "../NativeAPI_NTDLL.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtQueryAttributesFile.h"
#include "./Dispatch_NTDLL_NtClose.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "./Dispatch_NTDLL_NtDeleteFile.h"



//
//Global
//
__pfnNtOpenFile pfnNtOpenFile = NULL;



//
//Dispatch_NTDLL_NtOpenFile Functions
//
NTSTATUS
NTAPI
OnNtOpenFile(
	OUT PHANDLE  FileHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN ULONG  ShareAccess,
	IN ULONG  OpenOptions)
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
		nRet = pfnNtOpenFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			ShareAccess,
			OpenOptions
			);

		return nRet;
	}

	NTSTATUS nStatusQuery;
	FILE_BASIC_INFORMATION fbiFileAttributes;
	nStatusQuery = NtQueryAttributesFile(ObjectAttributes,&fbiFileAttributes);
	if( nStatusQuery != STATUS_OBJECT_NAME_NOT_FOUND &&
		nStatusQuery != STATUS_SUCCESS
		)
	{
		nRet = pfnNtOpenFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			ShareAccess,
			OpenOptions
			);

		return nRet;
	}
	nRet = pfnNtOpenFile(
		FileHandle,
		DesiredAccess,
		ObjectAttributes,
		IoStatusBlock,
		ShareAccess,
		OpenOptions
		);

	return nRet;
}

NTSTATUS
NTAPI
NtOpenFile(
	OUT PHANDLE  FileHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN ULONG  ShareAccess,
	IN ULONG  OpenOptions)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtOpenFile addFun = (__pfnNtOpenFile)GetProcAddress(hDll,"NtOpenFile");
	if(addFun)
	{
		ret = addFun(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,ShareAccess,OpenOptions);
	}
	return ret;
}