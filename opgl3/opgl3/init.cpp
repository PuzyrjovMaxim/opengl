// �������� ��� �����
#include "main.h"
#include <stdlib.h>
#include <string>


///////////////////////////////////////////////////////////////
//
//		�������, ��������� ���� ������
//
///////////////////////////////////////////////////////////////
const char* Name = "Wingman`s 3dLessons";
size_t newsize = strlen(Name) + 1;

size_t convertedChars = 0;
wchar_t* classname = new wchar_t[newsize];
const wchar_t classname[] = L"Wingman`s 3dLessons";

HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hWnd;		// ���������� ����
	WNDCLASS wndclass;	// ����� ����
	memset(&wndclass, 0, sizeof(WNDCLASS));		// ����������� ������ ��� ����� ����
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	// ����������� ���������
	wndclass.lpfnWndProc = WinProc;			// ������� ��������� �� ������� ��������� ���������
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// ������
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		// ������
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// ���� ����� �����
	wndclass.lpszClassName = classname;


								// ��� ������
	RegisterClass(&wndclass);				//������������ �����

	dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN; // ����� ����

	g_hInstance = hInstance;

	// ������������� �������� ������� ����.
	RECT rWindow;
	rWindow.left = 0;			// ����� ������� -  0
	rWindow.right = width;		// ������ ������� - 800
	rWindow.top = 0;		// ���� - 0
	rWindow.bottom = height;		// ��� - 800

	AdjustWindowRect(&rWindow, dwStyle, false);	// ��������� �������� �������

	// ������ ����
	hWnd = CreateWindow(classname[], strWindowName, dwStyle, 0, 0,
		rWindow.right - rWindow.left, rWindow.bottom - rWindow.top,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) return NULL;			// ���� �� ���������� - �������...
	ShowWindow(hWnd, SW_SHOWNORMAL);	// �������� ����
	UpdateWindow(hWnd);			// � ���������� ���
	SetFocus(hWnd);				// ���������� ���������� �� ���� ����

	return hWnd;
}


///////////////////////////////////////////////////////////////
//
//		�������, ��������������� ������ �������
//
///////////////////////////////////////////////////////////////
bool bSetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;	// ���������� ������� �������
	int pixelformat;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// ������������� ������ ���������
	pfd.nVersion = 1;				// ������ ������ = 1
	// ������� ������ ����� OpenGL
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = PFD_MAIN_PLANE;		// ����������� ����� (���� ���� ������������)
	pfd.iPixelType = PFD_TYPE_RGBA;		// ��� ����� RGB and Alpha ���� ��������
	pfd.cColorBits = SCREEN_DEPTH;		// ���������� ���� #define ��� �������� �������
	pfd.cDepthBits = SCREEN_DEPTH;	// ��� ������������ ��� RGBA, �� ��� ����� ���������
	pfd.cAccumBits = 0;
	pfd.cStencilBits = 0;

	// �-� ���� ������ �������, �������� ���������� �������� �����������, ����� ��� �������
	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE)
	{
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	// ������������� ��������� ������ �������
	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
	{
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////
//
//		�-� ������������� ������ ���� OpenGL
//
///////////////////////////////////////////////////////////////
void SizeOpenGLScreen(int width, int height)
{
	if (height == 0)		// ������������ ������� �� 0
		height = 1;

	// ������� ���� ���� OpenGL �������� � ������� ���� ���������. ��� ������� ����� �������
	// ������� ������ ����.
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);	// ������� ������� ��������
	glLoadIdentity();		// � ������� �

	// �������� ����������� ������ ����
	// ���������:
	// (���� �������, ��������� ������ � ������,
	// ��������� ���������� ������� �� ������, ��� ������� �� �����,
	// � ���������� ����������, ��� �������� ���������� ���������
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, .5f, 150.0f);

	// �����: ������� ��������� ������ ���� ������ �������, ���� �� �� ������ ��������
	// ���������� ���������, ������� � ����������. ���� ��� ����� �� ����, �� ���
	// ������������� ����� � ������� ���������� ������ ������ 1, �� ��������� �����
	// ����� ���������� ����� � �����.

	glMatrixMode(GL_MODELVIEW);  // ������� ������� �������
	glLoadIdentity();            // � ������� �
}


///////////////////////////////////////////////////////////////
//
//			�-� �������������� OpenGL
//
///////////////////////////////////////////////////////////////

void InitializeOpenGL(int width, int height)
{
	g_hDC = GetDC(g_hWnd);	// ������������� ���������� ���������� ����

	if (!bSetupPixelFormat(g_hDC))		// ������������� ������ �������
		PostQuitMessage(0);			// � ������� ��� ������

	g_hRC = wglCreateContext(g_hDC);		// �������� ���������� ��� hdc
	wglMakeCurrent(g_hDC, g_hRC);		// ������ �������� �������
	glEnable(GL_TEXTURE_2D);			// �������� ��������
	glEnable(GL_DEPTH_TEST);			// � ���� �������

	// � ������������� ������ ��������:
	SizeOpenGLScreen(width, height);
}

///////////////////////////////////////////////////////////////
//
//			�-� ��-�������������� OpenGL
//
///////////////////////////////////////////////////////////////
void DeInit()
{
	if (g_hRC)
	{
		wglMakeCurrent(NULL, NULL);	// ����������� ������, ������� ��� �������
		wglDeleteContext(g_hRC);	// ������� �������� ���������� OpenGL
	}

	if (g_hDC)
		ReleaseDC(g_hWnd, g_hDC);	// ������� HDC �� ������

	UnregisterClass("Wingman`s 3dLessons", g_hInstance);	// ����������� ����� ����
	PostQuitMessage(0);					// �������
}

///////////////////////////////////////////////////////////////
//
//			������� ������������ � ������� ����
//
///////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{
	HWND hWnd;

	// ������� ���� � ������� ����� �������, � ������� �������:
	// ���, ������, ������, ����� ����� ��� ����, ����� �� �� ��������, � hInstance
	hWnd = CreateMyWindow("Wingman`s 3dLessons", SCREEN_WIDTH, SCREEN_HEIGHT, 0, false, hInstance);

	// ������� ��� ������
	if (hWnd == NULL) return TRUE;

	// �������������� OpenGL
	Init(hWnd);

	// ��������� ������� ����
	return MainLoop();
}
