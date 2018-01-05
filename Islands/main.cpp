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
	sf::Font menuFont;
	menuFont.loadFromFile(Gui::DEFAULT_MENU_GUI_FONT_LOCATION);
	Gui::Menu menu(menuFont);

	while (app.getIslandWindow()->isOpen())
	{
		while (app.getIslandWindow()->pollEvent(*app.getIslandWindowEvent()))
		{
			if (app.getIslandWindowEvent()->type == sf::Event::Closed)
			{
				return Gui::MenuStates::Exit;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				Gui::MenuStates state;
				menu.isClick(state, sf::Mouse::getPosition(*app.getIslandWindow()));
				if (state != Gui::MenuStates::null)
				{
					return state;
				}
			}


		}

		app.clearContext(sf::Color::Black);
		app.draw(menu.getStartGameButton());
		app.draw(menu.getModsSettingsButton());
		app.draw(menu.getSettingsButton());
		app.draw(menu.getExitButton());
		app.displayContext();
	}

	return Gui::MenuStates::Exit;
}

Gui::StartGameMenuStates run_start_game_menu(IslandApp &app)
{
	sf::Font menuFont;
	menuFont.loadFromFile(Gui::DEFAULT_GAME_GUI_FONT_LOCATION);
	Gui::StartGameMenu startMenu(menuFont);

	while (app.getIslandWindow()->isOpen())
	{
		while (app.getIslandWindow()->pollEvent(*app.getIslandWindowEvent()))
		{
			if (app.getIslandWindowEvent()->type == sf::Event::Closed)
			{
				return Gui::StartGameMenuStates::Close;
			}

			std::string gameNameStr = startMenu.getGameName(*app.getIslandWindowEvent());
			if (!gameNameStr.empty())
			{
				return Gui::StartGameMenuStates::Start;
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (startMenu.isStartClick(sf::Mouse::getPosition(*app.getIslandWindow())))
				{
					if (!startMenu.getGameNameText().getString().isEmpty())
					{
						return Gui::StartGameMenuStates::Start;
					}
				}
				if (startMenu.isExitClick(sf::Mouse::getPosition(*app.getIslandWindow())))
				{
					return Gui::StartGameMenuStates::Exit;
				}
			}
		}


		app.clearContext(sf::Color::Black);
		app.draw(startMenu.getStartButton());
		app.draw(startMenu.getGameNameText());
		app.draw(startMenu.getExitButton());
		app.displayContext();
	}

	return Gui::StartGameMenuStates::null;
}

int main()
{
	EngineVars Vars;
	IslandApp app{ Vars.Video };

	while (true)
	{
		app.getIslandWindow()->setView(app.getIslandWindow()->getDefaultView());
		Gui::MenuStates state = run_menu(app);

		if (state == Gui::MenuStates::null)
		{
			ErrorHandler::logToFile("Ooops! Game menu return null value");
			return -1;
		}
		else if(state == Gui::MenuStates::StartGame)
		{
			Gui::StartGameMenuStates startState = run_start_game_menu(app);
			if (startState == Gui::StartGameMenuStates::Close)
			{
				break;
			}
			else if(startState == Gui::StartGameMenuStates::Start)
			{
				run_game(app, Vars);
			}
			
		}
		else if(state == Gui::MenuStates::Exit)
		{
			break;
		}
	}

	

	return 0;
}