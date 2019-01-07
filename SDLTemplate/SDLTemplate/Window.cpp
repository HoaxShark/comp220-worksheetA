#include "Window.h"

Window::Window()
{
	screenWidth = 600;
	screenHeight = 400;
	windowTitle = "Main Window";
}


Window::~Window()
{
}

bool Window::GetIsFullscreen()
{
	return isFullscreen;
}

void Window::SetIsFullscreen()
{
	isFullscreen = !isFullscreen;
}
