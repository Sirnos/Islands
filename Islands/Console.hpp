#pragma once

#include "textbox.hpp"
#include <vector>
#include <regex>

class Console
{
	sf::RectangleShape ConsoleWindow;
	textbox ConsoleInput;

	std::vector<std::string> commands;

	sf::Font ConsoleFont;
public:
	Console()
	{
		ConsoleWindow.setSize(sf::Vector2f(400, 600));
		ConsoleWindow.setFillColor(sf::Color(36, 10, 92, 120));
		
		ConsoleInput.changeVars(sf::Color::White, 16, sf::Text::Style::Regular);
		ConsoleFont.loadFromFile("Data/Fonts/ariali.ttf");
		ConsoleInput.create(ConsoleFont, false, "Type you command here! ", 64);
	}
	~Console()
	{
		commands.clear();
	}

	std::string operator()(sf::Event &event,sf::Vector2f mousePos,bool isMouseClick)
	{
		if (ConsoleInput.isClick(mousePos, isMouseClick))
		{
			ConsoleInput.switchEnable();
		}
		return ConsoleInput.operator()(event);
	}

	void clearConsole()
	{
		commands.clear();
		ConsoleInput.setString("");
	}
	size_t getCommandsSize()
	{
		return commands.size();
	}

	std::string getCurrentText()
	{
		return ConsoleInput.getString();
	}
	void pushText(std::string &text)
	{
		commands.push_back(text);
	}
	void pushCommand()
	{
		commands.push_back(ConsoleInput.getString());
		ConsoleInput.setString("");
	}
	std::string getText(unsigned index)
	{
		return commands[index];
	}
	std::string getLastText()
	{
		return commands.back();
	}

	void setPosition(sf::Vector2f position)
	{
		ConsoleWindow.setPosition(position);
		ConsoleInput.setPosition(position + sf::Vector2f(20, 560));
	}

	sf::RectangleShape getWindow() { return ConsoleWindow; }
	sf::Text getInputText() { return ConsoleInput.getText(); }

	//function for /giveItem command
	void giveItemCheck(std::string command, ItemDefContainer &Items, playerCharacter &player)
	{
		bool firstParam = true;
		std::string idStr;
		std::string amountStr;
		size_t begPos = command.find("m") + 2;
		size_t i = begPos;

		while (i < command.size())
		{
			if (command[i] != ' ')
			{
				if (firstParam)
				{
					idStr += command[i];
				}
				else
				{
					amountStr += command[i];
				}
			}
			else
			{
				firstParam = false;
			}
			i++;
		}

		int id = std::stoi(idStr);
		if (id <= 0) { commands.push_back("id is equal 0 or smaller"); return; }
		if (id > Items.getContainer().size()) { commands.push_back("item with this id does not exist"); return; }
		int amount = std::stoi(amountStr);
		if (amount <= 0) { commands.push_back("amount is equal 0 or smaller"); return; }
		if (amount > (int)(MAXIMUM_STACK_SIZE)) { commands.push_back("amount is higher than MAXIMUM_STACK_SIZE"); return; }
		if (amount > (int)Items.getItemDef(id)->getMaxStack()) { commands.push_back("amount is higher than Item::MaxStack"); return; }

		player.pushItemToPlayer(ItemField(id, amount), Items);

		return;
	}
};