// CPPGP2025.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <Windows.h>
#include <iostream>
#include "GameState.h"
#include "ZVector3.h"
#include "ZMatrix.h"
#include "CPPGP2025.h"

GameState* g_currentState = nullptr;
HWND g_hWnd;

void MatrixTest();
void ChangeState(GameState* newState);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY: // 창이 닫힐 때 발생
        PostQuitMessage(0); // 메세지 루프를 종료하도록 요청
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);

    }
}

void SetupConsole()
{
    AllocConsole(); // 현재 프로세스에 새로운 콘솔 창을 할당
    FILE* pConsole;
    // stdout에 쓰는 모든 내용이 새로 할당된 콘솔 창에 출력
    freopen_s(&pConsole, "CONOUT$", "w", stdout);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevlnstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetupConsole();
    MatrixTest();

    // 윈도우 클래스 정의
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"CPPGPClass";

    //  윈도우 클래스 등록
    RegisterClass(&wc);

    // 윈도우 생성
    HWND hWnd = CreateWindowEx(
        0,                      // 확장 윈도우 스타일
        L"CPPGPClass",          // 윈도우 클래스 이름
        L"CPPGP",               // 윈도우 제목
        WS_OVERLAPPEDWINDOW,    // 윈도우 스타일

        // 크기 및 위치
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL, // 부모 윈도우
        NULL, // 메뉴
        hInstance,
        NULL // 추가 애플리케이션 데이터
    );

    if (hWnd == NULL)
    {
        std::cout << "Failed to create window!" << std::endl;
        return 0;
    }

    // 윈도우를 화면에 표시 및 업데이트
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    FreeConsole();
    return (int)msg.wParam;
}

void MatrixTest()
{
    ZVector3 v1(1, 2, 3);
    std::cout << v1 << std::endl;

    ZVector3 v2(4, -5, 6);
    std::cout << v2.Cross(v1) << std::endl;

    std::cout << ZVector3::Dot(v1, v2) << std::endl;
    std::cout << v1.radBetween(v1, v2) << std::endl;
    std::cout << v1.degBetween(v1, v2) << std::endl;

    // Matrix 테스트
    ZVector3 localPoint(1.0, 1.0, 1.0);
    std::cout << "Local Space Point : " << localPoint << std::endl;

    // Scale 모든 축으로 2배 확대
    ZMatrix matScale = ZMatrix::CreateScale(2.0, 2.0, 2.0);

    // Rotation : Y축 90도 회전
    const double PI = 3.1415926535897932;
    ZMatrix matRotation = ZMatrix::CreateRotationY(PI / 2.0);

    // Translation : 월드 공간의 (5, 6, 7) 위치로 이동
    ZMatrix matTranslation = ZMatrix::CreateTranslation(5.0, 6.0, 7.0);

    // 변환을 순차적으로 적용
    ZVector3 pointAfterScale = localPoint.Transform(matScale);
    std::cout << pointAfterScale << std::endl;

    ZVector3 pointAfterRotation = pointAfterScale.Transform(matRotation);
    std::cout << pointAfterRotation << std::endl;

    ZVector3 finalWorldPoint = pointAfterRotation.Transform(matTranslation);
    std::cout << finalWorldPoint << std::endl;

    // 한방에 월드 매트릭스
    ZMatrix worldMatrix = matScale * matRotation * matTranslation;
    ZVector3 finalWorldPointByMatrix = localPoint.Transform(worldMatrix);
    std::cout << finalWorldPointByMatrix << std::endl;
}

void ChangeState(GameState* newState)
{
    if (g_currentState)
    {
        g_currentState->Exit();
        delete g_currentState;

    }
    g_currentState = newState;
    g_currentState->Enter(g_hWnd);
}
