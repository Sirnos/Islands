#pragma once

#include "textbox.hpp"
#include "button.hpp"

namespace Gui
{

	enum class MenuStates
	{
		null,
		StartGame,
		ModsSettings,
		Settings,
		Exit
	};

	const static std::string DEFAULT_START_GAME_TEXT = "Start Game";
	const static std::string DEFAULT_MODS_SETTINGS_TEXT = "Mods Settings";
	const static std::string DEFAULT_SETTINGS_TEXT = "Settings";
	const static std::string DEFAULT_EXIT_TEXT = "Exit";

	const static sf::Vector2i DEFAULT_MENU_BUTTON_SIZE = sf::Vector2i(160, 80);

	const static sf::Vector2i DEFAULT_START_GAME_POS = sf::Vector2i(200, 200);
	const static sf::Vector2i DEFAULT_MODS_SETTINGS_POS = sf::Vector2i(200, 400);
	const static sf::Vector2i DEFAULT_SETTINGS_POS = sf::Vector2i(200, 600);
	const static sf::Vector2i DEFAULT_EXIT_POS = sf::Vector2i(200, 800);

	const static sf::Vector2i STARTGAME_MENU_DEFAULT_START_GAME_POS = sf::Vector2i(600, 200);
	const static sf::Vector2i STARTGAME_MENU_DEFAULT_EXIT_POS = sf::Vector2i(600, 600);
	const static sf::Vector2f STARTGAME_MENU_DEFAULT_GAME_NAME_POS = sf::Vector2f(600, 400);

	const static std::string DEFAULT_MENU_GUI_FONT_LOCATION = "Data/Fonts/ariali.ttf";


	class Menu
	{
		button startGame;
		button modsSettings;
		button settings;
		button exit;

	public:
		Menu() = delete;
		Menu(const Menu &other) = delete;
		Menu(sf::Font &menuFont)
			: startGame(menuFont, DEFAULT_START_GAME_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_START_GAME_TEXT),
			modsSettings(menuFont, DEFAULT_MODS_SETTINGS_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_MODS_SETTINGS_TEXT),
			settings(menuFont, DEFAULT_SETTINGS_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_SETTINGS_TEXT),
			exit(menuFont, DEFAULT_EXIT_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_EXIT_TEXT)
		{
			startGame.setButtonPosition(static_cast<sf::Vector2f>(DEFAULT_START_GAME_POS));
			modsSettings.setButtonPosition(static_cast<sf::Vector2f>(DEFAULT_MODS_SETTINGS_POS));
			settings.setButtonPosition(static_cast<sf::Vector2f>(DEFAULT_SETTINGS_POS));
			exit.setButtonPosition(static_cast<sf::Vector2f>(DEFAULT_EXIT_POS));
		}
		~Menu() = default;


		const sf::Text &getStartGameButton() const
		{
			return startGame.getText();
		}
		const sf::Text &getModsSettingsButton() const
		{
			return modsSettings.getText();
		}
		const sf::Text &getSettingsButton() const
		{
			return settings.getText();
		}
		const sf::Text &getExitButton() const
		{
			return exit.getText();
		}


		void isClick(MenuStates &whichButton, const sf::Vector2i &mousePos)
		{
			whichButton = MenuStates::null;

			if (startGame.isClick(mousePos))
			{
				whichButton = MenuStates::StartGame;
			}
			else if (modsSettings.isClick(mousePos))
			{
				whichButton = MenuStates::ModsSettings;
			}
			else if (settings.isClick(mousePos))
			{
				whichButton = MenuStates::Settings;
			}
			else if(exit.isClick(mousePos))
			{
				whichButton = MenuStates::Exit;
			}
		}
	};

	enum class StartGameMenuStates
	{
		null,
		Start,
		Exit,
		Close
	};

	class StartGameMenu
	{
		button start;
		button exit;

		textbox gameName;

	public:
		StartGameMenu() = delete;
		StartGameMenu(const StartGameMenu &other) = delete;
		StartGameMenu(sf::Font &menuFont)
			: start(menuFont, STARTGAME_MENU_DEFAULT_START_GAME_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_START_GAME_TEXT),
			exit(menuFont, STARTGAME_MENU_DEFAULT_EXIT_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_EXIT_TEXT),
			gameName(30, "default", menuFont, false)
		{
			start.setButtonPosition(static_cast<sf::Vector2f>(STARTGAME_MENU_DEFAULT_START_GAME_POS));
			exit.setButtonPosition(static_cast<sf::Vector2f>(STARTGAME_MENU_DEFAULT_EXIT_POS));
			gameName.setPosition(STARTGAME_MENU_DEFAULT_GAME_NAME_POS);
			gameName.setEnable(true);
		}


		const sf::Text &getStartButton() const
		{
			return start.getText();
		}
		const sf::Text &getExitButton() const
		{
			return exit.getText();
		}
		const sf::Text &getGameNameText() const
		{
			return gameName.getText();
		}


		bool isStartClick(const sf::Vector2i &mousePos)
		{
			return start.isClick(mousePos);
		}
		bool isExitClick(const sf::Vector2i &mousePos)
		{
			return exit.isClick(mousePos);
		}

		std::string getGameName(sf::Event &events)
		{
			return gameName(events);
		}
	};
}

