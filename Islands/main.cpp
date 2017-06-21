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

	bool isClick = false;
	char lastKey = 'U';
	mouseWheel lastMouseWheelAct;

	ImGui::SFML::Init(*app.getIslandWindow());

	while (app.getIslandWindow()->isOpen())
	{
		lastMouseWheelAct = mouseWheel::Stop;
		lastKey = '0';
		isClick = false;

		while (app.getIslandWindow()->pollEvent(*app.getIslandWindowEvent()))
		{
			ImGui::SFML::ProcessEvent(*app.getIslandWindowEvent());

			if (app.getIslandWindowEvent()->type == sf::Event::Closed)
			{
				app.getIslandWindow()->close();
			}
			if (app.getIslandWindowEvent()->type == sf::Event::TextEntered)
			{
				lastKey = static_cast<char>(app.getIslandWindowEvent()->text.unicode);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { isClick = true; }
		
				if (app.getIslandWindowEvent()->type == sf::Event::MouseWheelScrolled)
				{
					if (app.getIslandWindowEvent()->mouseWheelScroll.delta < 0)
					{
						lastMouseWheelAct = mouseWheel::Down;
					}
					if (app.getIslandWindowEvent()->mouseWheelScroll.delta > 0)
					{
						lastMouseWheelAct = mouseWheel::Up;
					}

				}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { return 1; }

		IslandEngine(app, lastKey, lastMouseWheelAct, isClick);

		app.clearContext(skyColor);

		IslandEngine.DrawAll(app);

		app.displayContext();
	}

	ImGui::SFML::Shutdown();

	return 0;
}