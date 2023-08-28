// Включаем наш хидер
#include "main.h"
#include <stdlib.h>
#include <string>


///////////////////////////////////////////////////////////////
//
//		Функция, создающая наше окошко
//
///////////////////////////////////////////////////////////////
const char* Name = "Wingman`s 3dLessons";
size_t newsize = strlen(Name) + 1;

size_t convertedChars = 0;
wchar_t* classname = new wchar_t[newsize];
const wchar_t classname[] = L"Wingman`s 3dLessons";

HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hWnd;		// дескриптор окна
	WNDCLASS wndclass;	// класс окна
	memset(&wndclass, 0, sizeof(WNDCLASS));		// Резервируем память под класс окна
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	// Стандартные параметра
	wndclass.lpfnWndProc = WinProc;			// Передаём указатель на функцию обработки сообщений
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// Иконка
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		// Курсор
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// Окно будет белым
	wndclass.lpszClassName = classname;


								// Имя класса
	RegisterClass(&wndclass);				//регистрируем класс

	dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN; // Стиль окна

	g_hInstance = hInstance;

	// Устанавливаем заданные размеры окна.
	RECT rWindow;
	rWindow.left = 0;			// Левая сторона -  0
	rWindow.right = width;		// Правая сторона - 800
	rWindow.top = 0;		// Верх - 0
	rWindow.bottom = height;		// Низ - 800

	AdjustWindowRect(&rWindow, dwStyle, false);	// Применяем заданные размеры

	// Создаём окно
	hWnd = CreateWindow(classname[], strWindowName, dwStyle, 0, 0,
		rWindow.right - rWindow.left, rWindow.bottom - rWindow.top,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) return NULL;			// Есди не получилось - умираем...
	ShowWindow(hWnd, SW_SHOWNORMAL);	// Показать окно
	UpdateWindow(hWnd);			// И нарисовать его
	SetFocus(hWnd);				// Фокусирует клавиатуру на наше окно

	return hWnd;
}


///////////////////////////////////////////////////////////////
//
//		Функция, устанавливающая формат пиксела
//
///////////////////////////////////////////////////////////////
bool bSetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;	// Дескриптор формата пиксела
	int pixelformat;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Устанавливаем размер структуры
	pfd.nVersion = 1;				// Всегда ставим = 1
	// Передаём нужные флаги OpenGL
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = PFD_MAIN_PLANE;		// Стандартная маска (один хрен игнорируется)
	pfd.iPixelType = PFD_TYPE_RGBA;		// Нам нужны RGB and Alpha типа пикселей
	pfd.cColorBits = SCREEN_DEPTH;		// Используем наши #define для цветовой глубины
	pfd.cDepthBits = SCREEN_DEPTH;	// Это игнорируется для RGBA, но все равно передадим
	pfd.cAccumBits = 0;
	pfd.cStencilBits = 0;

	// Ф-я ищет формат пиксела, наиболее подходящий заданным требованиям, выход при неудаче
	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE)
	{
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	// Устанавливаем указанный формат пиксела
	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
	{
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////
//
//		Ф-я устанавливает размер окна OpenGL
//
///////////////////////////////////////////////////////////////
void SizeOpenGLScreen(int width, int height)
{
	if (height == 0)		// Предотвратим деление на 0
		height = 1;

	// Сделаем наше окно OpenGL размером с главное окно программы. При желании можно сделать
	// вьюпорт меньше окна.
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);	// Выберем матрицу проекции
	glLoadIdentity();		// И сбросим её

	// Вычислим перспективу нашего окна
	// Параметры:
	// (угол взгляда, отношение ширины и высоты,
	// Ближайшее расстояние обьекта до камеры, при котором он виден,
	// и дальнейшее расстояние, при котороом происходит отрисовка
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, .5f, 150.0f);

	// Важно: дальняя дистанция должна быть больше единицы, если вы не хотите получить
	// прикольные артефакты, работая с освещением. Мало кто знает об этом, но при
	// использовании света с ближней дистанцией камеры меньше 1, на полигонах будут
	// видны неуместные блики и флеши.

	glMatrixMode(GL_MODELVIEW);  // Выберем матрицу моделей
	glLoadIdentity();            // И сбросим её
}


///////////////////////////////////////////////////////////////
//
//			Ф-я инициализирует OpenGL
//
///////////////////////////////////////////////////////////////

void InitializeOpenGL(int width, int height)
{
	g_hDC = GetDC(g_hWnd);	// Устанавливаем глобальный дескриптор окна

	if (!bSetupPixelFormat(g_hDC))		// Устанавливаем формат пиксела
		PostQuitMessage(0);			// И выходим при ошибке

	g_hRC = wglCreateContext(g_hDC);		// Контекст рендеринга для hdc
	wglMakeCurrent(g_hDC, g_hRC);		// Делаем контекст текущим
	glEnable(GL_TEXTURE_2D);			// Включаем текстуры
	glEnable(GL_DEPTH_TEST);			// И тест глубины

	// И устанавливаем размер вьюпорта:
	SizeOpenGLScreen(width, height);
}

///////////////////////////////////////////////////////////////
//
//			Ф-я де-Инициализирует OpenGL
//
///////////////////////////////////////////////////////////////
void DeInit()
{
	if (g_hRC)
	{
		wglMakeCurrent(NULL, NULL);	// Освобождает память, занятую для рендера
		wglDeleteContext(g_hRC);	// Удаляет контекст рендеринга OpenGL
	}

	if (g_hDC)
		ReleaseDC(g_hWnd, g_hDC);	// Убирает HDC из памяти

	UnregisterClass("Wingman`s 3dLessons", g_hInstance);	// Освобождаем класс окна
	PostQuitMessage(0);					// Выходим
}

///////////////////////////////////////////////////////////////
//
//			Функция регистрирует и создает окно
//
///////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{
	HWND hWnd;

	// Создает окно с помощью нашей функции, в которую передаём:
	// Имя, Ширину, Высоту, любые флаги для окна, хотим ли мы фулскрин, и hInstance
	hWnd = CreateMyWindow("Wingman`s 3dLessons", SCREEN_WIDTH, SCREEN_HEIGHT, 0, false, hInstance);

	// Выходим при ошибке
	if (hWnd == NULL) return TRUE;

	// Инициализируем OpenGL
	Init(hWnd);

	// Запускаем игровой цикл
	return MainLoop();
}
