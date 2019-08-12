#include"HookInit.h"

//
//Program entry
//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//
	//Check arguments
	//
	//(Launcher.exe exePath [exe Name])
	CHAR szCommandLine[256] = { 0 };
	lstrcpy(szCommandLine, lpCmdLine);
	////lstrcpy(szCommandLine, "C:\\Program Files(x86)\\Tencent\\WeChat\\WeChat.exe");
	//lstrcpy(szCommandLine, "E:\\�������\\httpdebug.exe");

	CHookInit mHook;
	if (mHook.Initalization())
	{
		if (mHook.AdjustPrivileges())
		{
			if (mHook.BeginHookPortRoutine())
			{
				if (!mHook.BeginLauncherRunProgram(szCommandLine))
				{
					MessageBox(0,"����΢��ʧ��","",MB_OK);
				}
			}
			else
				MessageBox(0, "hook dll����ʧ��", "", MB_OK);
		}
		else
			MessageBox(0, "��Ȩʧ��", "", MB_OK);
	}
	else
		MessageBox(0, "hook ������ʼ��ʧ��!", "", MB_OK);

	//Sleep(INFINITE);
	return 0;
}