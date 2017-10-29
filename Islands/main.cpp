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

	bool isClickL = false;
	bool isClickR = false;
	char lastKey = 'U';
	mouseWheel lastMouseWheelAct;

	while (app.getIslandWindow()->isOpen())
	{
		lastMouseWheelAct = mouseWheel::Stop;
		lastKey = '0';
		isClickL = false;
		isClickR = false;

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
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { isClickL = true; }
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) { isClickR = true; }
		
			IslandEngine.manageConsole(*app.getIslandWindowEvent(), app.getMousePosInWorld(), isClickL);

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

		IslandEngine(app, lastKey, lastMouseWheelAct, isClickL, isClickR);

		app.clearContext(skyColor);

		IslandEngine.DrawAll(app);

		app.displayContext();
	}

	return 0;
}