// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "framework.h"
#include "detours/detours.h"
#include <stdarg.h>
#include <Windows.h>
#include <iostream>


SYSTEMTIME st;

static int (WINAPI* OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
static int (WINAPI* OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;

void dllLog() {
    GetLocalTime(&st);
    printf("DLL LOGS: %d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}

extern "C" __declspec(dllexport) int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
    printf("\n\n**************************************\n");
    printf("MessageBoxA Hooked\n");
    dllLog();
    printf("****************************************\n\n");
    return OldMessageBoxA(NULL, "new MessageBoxA", "Hookes", MB_OK);
}


extern "C" __declspec(dllexport) int WINAPI NewMessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType)
{
    printf("\n\n**************************************\n");
    printf("MessageBoxW Hooked\n");
    dllLog();
    printf("****************************************\n\n");
    return OldMessageBoxW(NULL, L"new MessageBoxW", L"Hookes", MB_OK);
}


static HANDLE(WINAPI* OldCreateFile) (
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
    ) = CreateFile;


extern "C" __declspec(dllexport) HANDLE WINAPI NewCreateFile(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
) {
    printf("\n\n**************************************\n");
    printf("CreateFile Hooked\n");
    dllLog();
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, pchar, num, NULL, FALSE);

    std::cout << "lpFileName:" << pchar << std::endl;
    std::cout << "dwDesireAccess : 0x" << std::hex << dwDesiredAccess << std::endl;
    std::cout << "lpSecurityAttributes : 0x" << std::hex << lpSecurityAttributes << std::endl;
    std::cout << "dwCreationDisposition : 0x" << std::hex << dwCreationDisposition << std::endl;
    std::cout << "dwFlagsAndAttributes : 0x" << std::hex << dwFlagsAndAttributes << std::endl;
    std::cout << "hTemplateFile : 0x" << std::hex << hTemplateFile << std::endl;

    printf("****************************************\n\n");

    return OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

}



static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;

static BOOL(WINAPI* OldHeapDestroy)(HANDLE) = HeapDestroy;
extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
    HANDLE hHeap = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
    printf("\n\n********************************************\n");
    printf("HeapCreate Hooked\n");
    dllLog();
    std::cout << "fIOoptions : " << fIOoptions << std::endl;
    std::cout << "dwInitialSize : " << dwInitialSize << std::endl;
    std::cout << "dwMaximumSize : " << dwMaximumSize << std::endl;
    std::cout << "hHeap : " << hHeap << std::endl;
    printf("* *******************************************\n\n");
    return hHeap;
}



extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap)
{
    printf(" \n\n********************************************\r");
    printf("HeapDestroy Hooked\n");
    GetLocalTime(&st);
    printf("DLLÈÕÖ¾Êä³ö:%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hHeap:" << hHeap << std::endl;
    printf("********************************************\n\n");
    return OldHeapDestroy(hHeap);
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
        DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
        DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        DetourTransactionCommit();
    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
        DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
        DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        DetourTransactionCommit();
        break;
    }
    return TRUE;
}

