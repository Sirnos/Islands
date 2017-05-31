#include <SFML/Graphics/RenderWindow.hpp>
#include "IslandApp.hpp"
#include "Engine.hpp"

const sf::Color skyColor = { 54,128,235,60 };

int main()
{
	IslandApp app;
	Engine IslandEngine;

	app.init(sf::VideoMode(1280,1024),62,true);
	IslandEngine.init();
	char lastKey = 'U';

	while (app.getIslandWindow()->isOpen())
	{
		lastKey = '0';
		while (app.getIslandWindow()->pollEvent(*app.getIslandWindowEvent()))
		{
			if (app.getIslandWindowEvent()->type == sf::Event::Closed)
			{
				app.getIslandWindow()->close();
			}
			if (app.getIslandWindowEvent()->type == sf::Event::TextEntered)
			{
				lastKey = static_cast<char>(app.getIslandWindowEvent()->text.unicode);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { return 1; }

		IslandEngine(app,lastKey);

		app.clearContext(skyColor);

		IslandEngine.DrawAll(app);

		app.displayContext();
	}

	return 0;
}