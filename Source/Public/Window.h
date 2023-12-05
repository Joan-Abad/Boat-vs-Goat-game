#pragma once
#include <SFML/Graphics.hpp>

class Window
{
public:
	Window(const char* windowName, sf::Vector2f windowSize);
	~Window();

	//Code that gets executed each tick 
	void Update();

	//Clears the previous graphs
	void ClearWindow();

	//Draws the content of the window
	void Draw();

	//Handles all the input used when the window is active
	void HandleInput();

	inline sf::RenderWindow& GetWindow() { return *m_window; };

private:
	//The size of the window
	sf::Vector2f m_WindowSize; 

	//The name of the window
	const char* m_windowName; 

	//The window displayed
	sf::RenderWindow* m_window; 
};