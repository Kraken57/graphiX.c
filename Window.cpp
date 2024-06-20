#include "Window.h"


Window* window = nullptr;

Window::Window()
    : m_hwnd(NULL), m_is_run(false)
{
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        //this event gets fired when the window is created
        window->onCreate();

        break;
    }
    case WM_DESTROY:
    {
        //this event gets fired when the window get destroy
        window->onDestroy();

        ::PostQuitMessage(0);
        break;

    }

    default :
        return ::DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return NULL;

}

bool Window::init()
{
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = L"MyWindowClass";
    wc.lpszMenuName = NULL;
    wc.style = NULL;
    wc.lpfnWndProc = &WndProc;

    if (!::RegisterClassEx(&wc))
        return false;

    //call this pointer before the creation of the window so that it does not show null
    if (!window)
        window = this;


    //Creation of the window
    m_hwnd=::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"GraphiX", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
        NULL, NULL, NULL, NULL);


    //if the creation of the window fails 
    if (!m_hwnd)
        return false;

    //show the window 
    ::ShowWindow(m_hwnd, SW_SHOW);
    ::UpdateWindow(m_hwnd);

    

    //setting this flag as true to indicate that the window is initialized and running 
    m_is_run = true;

    return true;
}

bool Window::broadcast()
{
    MSG msg;

    while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    window->onUpdate();

    Sleep(0);

    return true;
}

bool Window::release()
{

    //Destroy window 
    if (!::DestroyWindow(m_hwnd))
        return false;

    return true;
}

bool Window::isRun()
{
    return m_is_run;
}

void Window::onDestroy()
{
    m_is_run = false;
}

Window::~Window()
{


}
