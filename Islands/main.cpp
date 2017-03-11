#include <SFML/Graphics.hpp>

#include "IslandApp.hpp"
void main()
{
	IslandApp app;
	app.initiate();
	
	sf::RenderWindow *appWindow = app.getIslandWindow();
	sf::Event *appEvent = app.getIslandWindowEvent();

	while (appWindow->isOpen())
	{
		while (appWindow->pollEvent(*appEvent))
		{
			if (appEvent->type == sf::Event::Closed)
			{
				appWindow->close();
			}
		}
		app.clearContext();
		app.displayContext();
	}

	return;
}