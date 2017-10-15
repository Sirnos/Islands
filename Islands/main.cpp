#include <SFML/Graphics/RenderWindow.hpp>
#include "IslandApp.hpp"
#include "Engine.hpp"
#include "EngineVars.hpp"

const sf::Color skyColor = { 54,128,235,60 };

int main()
{
	EngineVars Vars;

	IslandApp app{ Vars.Video };
	Engine IslandEngine{ Vars.Game, Vars.Render };

	bool isClick = false;
	char lastKey = 'U';
	mouseWheel lastMouseWheelAct;

	while (app.getIslandWindow()->isOpen())
	{
		lastMouseWheelAct = mouseWheel::Stop;
		lastKey = '0';
		isClick = false;

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
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { isClick = true; }
		
			IslandEngine.manageConsole(*app.getIslandWindowEvent(), app.getMousePosInWorld(), isClick);

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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { break; }

		IslandEngine(app, lastKey, lastMouseWheelAct, isClick);

		app.clearContext(skyColor);

		IslandEngine.DrawAll(app);

		app.displayContext();
	}

	return 0;
}