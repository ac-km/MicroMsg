#pragma once



//
//Define
//
//struct CHookPortControlInterface
//{
//	virtual BOOL InitHookPort(LPCWSTR lpSandBoxName) = FALSE;
//};



//
//DLL Export
//
//extern "C" __declspec(dllexport) BOOL GetHookPortControlInit(CHookPortControlInterface** lpParameter);

extern "C" __declspec(dllexport) BOOL HookPort_Start(LPVOID lpParameter);