// SteamBind.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <detours.h>

int main()
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    CHAR szCommand[2048];
    CHAR szExe[1024] = { 0x47, 0x61, 0x6D, 0x65, 0x2E, 0x65, 0x78, 0x65, 0x00 };
    CHAR szFullExe[1024] = "\0";
    PCHAR pszFileExe = NULL;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    szCommand[0] = L'\0';

    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
    SearchPathA(NULL, szExe, ".exe", ARRAYSIZE(szFullExe), szFullExe, &pszFileExe);

    // 콘솔을 감춥니다
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, 0);

    // 특정 DLL을 주입한 상태로 프로세스를 실행합니다.
    if (!DetourCreateProcessWithDllA(szFullExe[0] ? szFullExe : NULL, szCommand,
        NULL, NULL, TRUE, dwFlags, NULL, NULL,
        &si, &pi, "SteamClient.dll", NULL)) {
        DWORD dwError = GetLastError();

        ExitProcess(9009);
    }

    ResumeThread(pi.hThread);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD dwResult = 0;

    if (!GetExitCodeProcess(pi.hProcess, &dwResult)) {
        printf_s("GetExitCodeProcess failed: %d\n", GetLastError());
        return 9010;
    }

    return dwResult;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
