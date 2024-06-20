#pragma once
#include <Windows.h>


class Window
{
public:
	Window();

	//Initialize a Window
	bool init();
	bool broadcast();
	//Release a Window 
	bool release();
	bool isRun();

	//Event 
	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy();


	~Window();

protected:
	HWND m_hwnd;
	bool m_is_run;
};