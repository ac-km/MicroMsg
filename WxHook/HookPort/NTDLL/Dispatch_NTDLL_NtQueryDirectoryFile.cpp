#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>
#include <stdio.h>

#include "./Dispatch_NTDLL_NtQueryDirectoryFile.h"
#include"../../Common/DebugLog.h"
#include"../HookPort.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"



//
//Global
//
__pfnNtQueryDirectoryFile pfnNtQueryDirectoryFile = NULL;



//
//Dispatch_NTDLL_NtQueryDirectoryFile Functions
//
NTSTATUS
NTAPI
OnNtQueryDirectoryFile(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	OUT PVOID               FileInformation,
	IN ULONG                Length,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN BOOLEAN              ReturnSingleEntry,
	IN PUNICODE_STRING      FileMask OPTIONAL,
	IN BOOLEAN              RestartScan)
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
		nRet = pfnNtQueryDirectoryFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			FileInformation,
			Length,
			FileInformationClass,
			ReturnSingleEntry,
			FileMask,
			RestartScan
			);

		return nRet;
	}

	nRet = pfnNtQueryDirectoryFile(
		FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		FileInformation,
		Length,
		FileInformationClass,
		ReturnSingleEntry,
		FileMask,
		RestartScan
		);
	return nRet;
}

NTSTATUS
NTAPI
NtQueryDirectoryFile(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	OUT PVOID               FileInformation,
	IN ULONG                Length,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN BOOLEAN              ReturnSingleEntry,
	IN PUNICODE_STRING      FileMask OPTIONAL,
	IN BOOLEAN              RestartScan)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryDirectoryFile addFun = (__pfnNtQueryDirectoryFile)GetProcAddress(hDll,"NtQueryDirectoryFile");
	if(addFun)
	{
		ret = addFun(FileHandle,Event,ApcRoutine,ApcContext,IoStatusBlock,FileInformation,Length,FileInformationClass,ReturnSingleEntry,FileMask,RestartScan);
	}
	return ret;
}