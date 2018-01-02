#pragma once

#include "textbox.hpp"
#include "button.hpp"

namespace Gui
{

	enum class MenuStates
	{
		Menu,
		NewGame,
		LoadGame,
		ModSettings,
		Settings

	};

	const static std::string DEFAULT_START_NEW_GAME_TEXT = "Start new game";
	const static std::string DEFAULT_LOAD_GAME_TEXT = "Load game";
	const static std::string DEFAULT_MODS_SETTINGS_TEXT = "Mods Settings";
	const static std::string DEFAULT_SETTINGS_TEXT = "Settings";
	const static std::string DEFAULT_EXIT_TEXT = "Exit";

	const static sf::Vector2i DEFAULT_MENU_BUTTON_SIZE = sf::Vector2i(160, 80);

	const static sf::Vector2i DEFAULT_START_NEW_GAME_POS = sf::Vector2i(200, 200);
	const static sf::Vector2i DEFAULT_LOAD_GAME_POS = sf::Vector2i(200, 400);
	const static sf::Vector2i DEFAULT_MODS_SETTINGS_POS = sf::Vector2i(200, 600);
	const static sf::Vector2i DEFAULT_SETTINGS_POS = sf::Vector2i(200, 800);
	const static sf::Vector2i DEFAULT_EXIT_POS = sf::Vector2i(200, 1000);

	class Menu
	{
		button startNewGame;
		button loadGame;
		button modsSettings;
		button settings;
		button exit;


		MenuStates currentState;


	public:
		Menu() = delete;
		Menu(const Menu &other) = delete;
		Menu(sf::Font &menuFont)
			:currentState(MenuStates::Menu),
			startNewGame(menuFont, DEFAULT_START_NEW_GAME_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_START_NEW_GAME_TEXT),
			loadGame(menuFont, DEFAULT_LOAD_GAME_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_LOAD_GAME_TEXT),
			modsSettings(menuFont, DEFAULT_MODS_SETTINGS_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_MODS_SETTINGS_TEXT),
			settings(menuFont, DEFAULT_SETTINGS_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_SETTINGS_TEXT),
			exit(menuFont, DEFAULT_EXIT_POS, DEFAULT_MENU_BUTTON_SIZE, DEFAULT_EXIT_TEXT)
		{}
		~Menu() = default;


		const sf::Text &getStartGameButton() const
		{
			return startNewGame.getText();
		}
		const sf::Text &getLoadGameButton() const
		{
			return loadGame.getText();
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


	};


}

