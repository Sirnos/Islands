#include <SFML/Graphics.hpp>

#include "IslandApp.hpp"
#include "Engine.hpp"

void main()
{
	IslandApp app;
	Engine IslandEngine;
	app.initiate();
	
	sf::RenderWindow *appWindow = app.getIslandWindow();
	sf::Event *appEvent = app.getIslandWindowEvent();

	IslandEngine.init();

	while (appWindow->isOpen())
	{
		while (appWindow->pollEvent(*appEvent))
		{
			if (appEvent->type == sf::Event::Closed)
			{
				appWindow->close();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			return;
		}

		IslandEngine();

		app.clearContext();

		IslandEngine.DrawAll(app.getIslandWindow());

		app.displayContext();
	}

	return;
}