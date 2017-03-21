#pragma once

#include <stack>

#include <SFML/Graphics.hpp>

enum class menu_type :size_t
{
	startMenu,
	creategameMenu,
	loadgameMenu,
	creditsMenu,
};

class IslandMenu
{
public:
	std::stack <menu_type> MenuStack;
	
};