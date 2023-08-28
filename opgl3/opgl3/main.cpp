// �������� ����������� ����������:
// OpelGL
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

// ��� ������� �����:
#include "main.h"

// ����������� �����������:
HWND  g_hWnd;
RECT  g_rRect;
HDC   g_hDC;
HGLRC g_hRC;
HINSTANCE g_hInstance;

///////////////////////////////////////////////////////////////
//
//			������� ���������� ������ ���� � �������� �����
//
///////////////////////////////////////////////////////////////

void RenderScene()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// ������ �������� ������ ������ �����, � ���������� �����
	// ��� ����������

	SwapBuffers(g_hDC);
}

///////////////////////////////////////////////////////////////
//
//			�������������� ������� ����
//
///////////////////////////////////////////////////////////////

void Init(HWND hWnd)
{
	g_hWnd = hWnd;
	GetClientRect(g_hWnd, &g_rRect);
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);
}

///////////////////////////////////////////////////////////////
//
//			������� ����
//
///////////////////////////////////////////////////////////////

WPARAM MainLoop()
{
	MSG msg;

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			RenderScene();
		}
	}

	DeInit();

	return(msg.wParam);
}

///////////////////////////////////////////////////////////////
//
//			��������� ��������� windows
//
///////////////////////////////////////////////////////////////

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG    lRet = 0;
	PAINTSTRUCT    ps;

	switch (uMsg)
	{
	case WM_SIZE:		// ���� ������ ������ ����

		SizeOpenGLScreen(LOWORD(lParam), HIWORD(lParam));// LoWord=Width, HiWord=Height
		GetClientRect(hWnd, &g_rRect);		// �������� window rectangle
		break;

	case WM_PAINT:			// ���� ����� ������������ �����
		BeginPaint(hWnd, &ps);	// ����. paint struct
		EndPaint(hWnd, &ps);	// EndPaint, ���������
		break;

	case WM_KEYDOWN:	// ��� ��������� ��������, ��� ������ ������� �� ����������.
		// ���� ������� ��������� � ��������� wParam
		switch (wParam)
		{
		case VK_ESCAPE:			// ���� ����� ESCAPE
			PostQuitMessage(0);	// �������
			break;
		}
		break;

	case WM_CLOSE:		// ���� ���� ���� �������
		PostQuitMessage(0);	// �������
		break;

	default:		// Return �� ���������
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}
