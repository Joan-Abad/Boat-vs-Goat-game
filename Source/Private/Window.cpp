#include "Window.h"

Window::Window(const char* windowName, sf::Vector2f windowSize)
{
	m_windowName = windowName;
	m_WindowSize = windowSize; 
	m_windowName = windowName;
	m_window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), m_windowName);
}

Window::~Window()
{
	delete m_window; 
}

void Window::Update()
{
	Display();
}

void Window::ClearWindow()
{
	m_window->clear();
}

void Window::Display()
{
	m_window->display();
}

void Window::HandleInput()
{
}
