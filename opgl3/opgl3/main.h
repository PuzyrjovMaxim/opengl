#ifndef _MAIN_H
#define _MAIN_H

// Хидеры, необходимые для работы программы
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>
#include <gl\glu.h>


// Обьявим глобальные переменные, ширину, высоту и глубину цвета экрана
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_DEPTH 16

// Глобальные параметры окна; будут доступны из других файлов:
extern HWND  g_hWnd;
extern RECT  g_rRect;
extern HDC   g_hDC;
extern HGLRC g_hRC;
extern HINSTANCE g_hInstance;




// Прототип главной функции программы - WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);

// Прототип функции обработки сообщений
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Функция - главный цикл программы
WPARAM MainLoop();

// Функция, создающая окно
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);

// Функция, устанавливающая формат пиксела
bool bSetupPixelFormat(HDC hdc);

// Прототип функции, устанавливающей размеры окна OpenGL
void SizeOpenGLScreen(int width, int height);

// Функция, инициализирующая OpenGL
void InitializeOpenGL(int width, int height);

// Общая инициализация
void Init(HWND hWnd);

// Функция, которая собственно рисует сцену
void RenderScene();

// Де-инициализация
void DeInit();

#endif
