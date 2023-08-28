// Включаем необходимые библиотеки:
// OpelGL
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

// Наш главный хидер:
#include "main.h"

// Необходимые дескрипторы:
HWND  g_hWnd;
RECT  g_rRect;
HDC   g_hDC;
HGLRC g_hRC;
HINSTANCE g_hInstance;

///////////////////////////////////////////////////////////////
//
//			Функция вызывается каждый кадр и рендерит сцену
//
///////////////////////////////////////////////////////////////

void RenderScene()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Сейчас рисуется просто черный экран, в дальнейшем будет
	// код рендеринга

	SwapBuffers(g_hDC);
}

///////////////////////////////////////////////////////////////
//
//			Инициализирует игровое окно
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
//			Главный цикл
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
//			Обработка сообщений windows
//
///////////////////////////////////////////////////////////////

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG    lRet = 0;
	PAINTSTRUCT    ps;

	switch (uMsg)
	{
	case WM_SIZE:		// Если изменён размер окна

		SizeOpenGLScreen(LOWORD(lParam), HIWORD(lParam));// LoWord=Width, HiWord=Height
		GetClientRect(hWnd, &g_rRect);		// получаем window rectangle
		break;

	case WM_PAINT:			// Если нужно перерисовать сцену
		BeginPaint(hWnd, &ps);	// Иниц. paint struct
		EndPaint(hWnd, &ps);	// EndPaint, подчищаем
		break;

	case WM_KEYDOWN:	// Это сообщение означает, что нажата клавиша на клавиатуре.
		// Сама клавиша передаётся в параметре wParam
		switch (wParam)
		{
		case VK_ESCAPE:			// Если нажат ESCAPE
			PostQuitMessage(0);	// Выходим
			break;
		}
		break;

	case WM_CLOSE:		// Если окно было закрыто
		PostQuitMessage(0);	// Выходим
		break;

	default:		// Return по умолчанию
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}
