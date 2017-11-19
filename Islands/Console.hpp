#pragma once

#include "textbox.hpp"
#include <vector>

class Console
{
	sf::RectangleShape ConsoleWindow;
	textbox ConsoleInput;

	std::vector<std::string> commands;
	std::vector<std::string> history;

	unsigned lastHistoryCmd;

	sf::Font ConsoleFont;

	bool enable;
public:
	Console(const sf::Vector2f &size, const sf::Color &color, unsigned fontSize)
		:lastHistoryCmd(0), enable(true), ConsoleWindow(size)
	{
		ConsoleWindow.setFillColor(color);
		ConsoleInput.changeVars(sf::Color::White, fontSize, sf::Text::Style::Regular);
		ConsoleFont.loadFromFile("Data/Fonts/ariali.ttf");
		ConsoleInput.create(ConsoleFont, false, "Type you command here! ", 64);
	}
	~Console()
	{
		commands.clear();
		history.clear();
	}

	bool getEnable() const { return enable; }
	void setEnable(bool newVal) { enable = newVal; }

	bool getTextboxEnable() { return ConsoleInput.getIsEnable(); }
	void setTexboxEnable(bool newVal) { ConsoleInput.setEnable(newVal); }

	void pushCommandToHistory(const std::string &command)
	{
		history.push_back(command);
		lastHistoryCmd = history.size() - 1;
	}
	std::string getCmdFromHistory(unsigned cmdNumber) { return history[cmdNumber]; }
	std::string getLastCmdFromHistory() { return history.back(); }
	unsigned getHistorySize() const { return history.size(); }

	unsigned &getLastHistoryCmdNumber() { return lastHistoryCmd; }

	std::string operator()(const sf::Event &event, const sf::Vector2f &mousePos, bool isMouseClick)
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
	void clearHistory() { history.clear(); }

	std::string getCurrentText() { return ConsoleInput.getString(); }
	void setCurrentText(const std::string &text) { ConsoleInput.setString(text); }

	size_t getCommandsSize() const { return commands.size(); }
	void pushText(const std::string &text) { commands.push_back(text); }
	std::string getText(unsigned index) const { return commands[index]; }
	std::string getLastText() const { return commands.back(); }

	void setPosition(const sf::Vector2f &position)
	{
		ConsoleWindow.setPosition(position);
		ConsoleInput.setPosition(position + sf::Vector2f(20, 560));
	}

	sf::RectangleShape getWindow() { return ConsoleWindow; }
	sf::Text getInputText() { return ConsoleInput.getText(); }

	//function for /giveItem command
	void giveItemCheck(const std::string &command, ItemDefContainer &Items, PlayerInventory &Inv)
	{
		bool firstParam = true;
		std::string idStr;
		std::string amountStr;
		size_t begPos = command.find('m') + 2;
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
		if (static_cast<unsigned>(id) > Items.getContainer().size() - 1) { commands.push_back("item with this id does not exist"); return; }
		int amount = std::stoi(amountStr);
		if (amount <= 0) { commands.push_back("amount is equal 0 or smaller"); return; }
		if (static_cast<unsigned>(amount) > MAXIMUM_STACK_SIZE) { commands.push_back("amount is higher than MAXIMUM_STACK_SIZE"); return; }
		if (static_cast<unsigned>(amount) > Items.getDefinition(id)->getMaxStack()) { commands.push_back("amount is higher than Item::MaxStack"); return; }

		ItemField item(id, amount);
		Inv.pushItem(item, Items.getDefinition(id)->getMaxStack());
		pushCommandToHistory(command);

		return;
	}
};
