#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>

#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "../../Common/DebugLog.h"
#include"../HookPort.h"
#include "../NativeAPI_NTDLL.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtQueryAttributesFile.h"
#include "./Dispatch_NTDLL_NtClose.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "./Dispatch_NTDLL_NtDeleteFile.h"
#include "./Dispatch_NTDLL_NtSetInformationFile.h"



//
//Global
//
__pfnNtCreateFile pfnNtCreateFile = NULL;



//
//Dispatch_NTDLL_NtCreateFile Functions
//
NTSTATUS
NTAPI
OnNtCreateFile(
    OUT PHANDLE  FileHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN PLARGE_INTEGER  AllocationSize  OPTIONAL,
    IN ULONG  FileAttributes,
    IN ULONG  ShareAccess,
    IN ULONG  CreateDisposition,
    IN ULONG  CreateOptions,
    IN PVOID  EaBuffer  OPTIONAL,
    IN ULONG  EaLength)
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
		nRet = pfnNtCreateFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			AllocationSize,
			FileAttributes,
			ShareAccess,
			CreateDisposition,
			CreateOptions,
			EaBuffer,
			EaLength
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
		nRet = pfnNtCreateFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			AllocationSize,
			FileAttributes,
			ShareAccess,
			CreateDisposition,
			CreateOptions,
			EaBuffer,
			EaLength
			);

		return nRet;
	}
	nRet = pfnNtCreateFile(
		FileHandle,
		DesiredAccess,
		ObjectAttributes,
		IoStatusBlock,
		AllocationSize,
		FileAttributes,
		ShareAccess,
		CreateDisposition,
		CreateOptions,
		EaBuffer,
		EaLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreateFile(
    OUT PHANDLE  FileHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN PLARGE_INTEGER  AllocationSize  OPTIONAL,
    IN ULONG  FileAttributes,
    IN ULONG  ShareAccess,
    IN ULONG  CreateDisposition,
    IN ULONG  CreateOptions,
    IN PVOID  EaBuffer  OPTIONAL,
    IN ULONG  EaLength)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateFile addFun = (__pfnNtCreateFile)GetProcAddress(hDll,"NtCreateFile");
	if(addFun)
	{
		ret = addFun(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,AllocationSize,FileAttributes,ShareAccess,CreateDisposition,CreateOptions,EaBuffer,EaLength);
	}
	return ret;
}