#include <iostream>
#include <cstdio>
#include <windows.h>
#include <detours/detours.h>

#pragma comment(lib,"detours.lib")

int main()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	WCHAR DirPath[MAX_PATH + 1];
	wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\17697\\source\\repos\\mydllmain\\Debug");//Dll的文件夹
	char DLLPath[MAX_PATH + 1] = "C:\\Users\\17697\\source\\repos\\mydllmain\\Debug\\mydllmain.dll";//Dll的地址
	WCHAR EXE[MAX_PATH + 1] = { 0 };
	wcscpy_s(EXE, MAX_PATH,L"C:\\Users\\17697\\source\\repos\\mydllmain\\Debug\\MSGbox.exe" /*L"C:\\WINDOWS\\system32\\notepad.exe"*/);//需要注入程序的地址
	if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi, DLLPath, NULL))
	{
		ResumeThread(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
	}
	else
	{
		char error[100];
		sprintf_s(error, "%d", GetLastError());
	}
	return 0;
}