#pragma once
#include "button.hpp"
#include "textbox.hpp"

struct basicmenu
{
	button exitButton;

	virtual unsigned menuLoop(sf::Event &WindowEvent) = 0;
};

struct startMenu :basicmenu
{

};

struct loadgameMenu :basicmenu
{

};

struct creategameMenu :basicmenu
{

};

struct creditsMenu :basicmenu
{

};