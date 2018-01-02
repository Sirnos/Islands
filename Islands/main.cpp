#include <SFML/Graphics/RenderWindow.hpp>
#include "IslandApp.hpp"
#include "Engine.hpp"
#include "EngineVars.hpp"
#include "MenuGui.hpp"

const static sf::Color DEFAULT_SKY_COLOR = { 54, 128, 235, 60 };

void run_game(IslandApp &app, const EngineVars &vars)
{
	Engine IslandEngine{ vars.Game, vars.Render };

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

		app.clearContext(DEFAULT_SKY_COLOR);

		IslandEngine.DrawAll(app);

		app.displayContext();
	}
}

Gui::MenuStates run_menu(IslandApp &app)
{
	while (app.getIslandWindow()->isOpen())
	{
		while (app.getIslandWindow()->pollEvent(*app.getIslandWindowEvent()))
		{
			if (app.getIslandWindowEvent()->type == sf::Event::Closed)
			{
				return Gui::MenuStates::Exit;
			}

		}
	}

	return Gui::MenuStates::Exit;
}


int main()
{
	EngineVars Vars;
	IslandApp app{ Vars.Video };
	run_game(app, Vars);

	return 0;
}