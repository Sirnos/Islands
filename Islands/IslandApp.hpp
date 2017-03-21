#pragma once

#include <SFML/Graphics.hpp>

class IslandApp
{
	sf::RenderWindow IslandWindow;
	sf::Event IslandWindowEvent;
	sf::View Camera;

public:
	void initiate(sf::VideoMode WindowMode = sf::VideoMode(1280,1024,32),unsigned FPS = 62,bool vertSync = false)
	{
		IslandWindow.create(WindowMode, "Islands" /*sf::Style::Fullscreen*/);
		IslandWindow.setFramerateLimit(FPS);
		IslandWindow.setVerticalSyncEnabled(vertSync);
	}
	void clearContext()
	{
		IslandWindow.clear();
	}
	void clearContext(sf::Color BufferColor)
	{
		IslandWindow.clear(BufferColor);
	}
	void displayContext()
	{
		IslandWindow.display();
	}

	void drawShapeVector(std::vector <sf::RectangleShape*> &vect)
	{
		for (size_t i = 0; i < vect.size(); i++)
		{
			IslandWindow.draw(*vect[i]);
		}
	}
	void drawShapeVector(std::vector <sf::CircleShape*> &vect)
	{
		for (size_t i = 0; i < vect.size(); i++)
		{
			IslandWindow.draw(*vect[i]);
		}
	}
	void drawShapeVector(std::vector <sf::ConvexShape*> &vect)
	{
		for (size_t i = 0; i < vect.size(); i++)
		{
			IslandWindow.draw(*vect[i]);
		}
	}
	
	void drawShape(sf::RectangleShape &shape)
	{
		IslandWindow.draw(shape);
	}
	void drawShape(sf::CircleShape &shape)
	{
		IslandWindow.draw(shape);
	}
	void drawShape(sf::ConvexShape &shape)
	{
		IslandWindow.draw(shape);
	}

	void drawTextVector(std::vector <sf::Text*> &vect)
	{
		for (size_t i = 0; i < vect.size(); i++)
		{
			IslandWindow.draw(*vect[i]);
		}
	}
	void drawText(sf::Text &txt)
	{
		IslandWindow.draw(txt);
	}

	void drawSpriteVector(std::vector <sf::Sprite*> &vect)
	{
		for (size_t i = 0; i < vect.size(); i++)
		{
			IslandWindow.draw(*vect[i]);
		}
	}
	void drawSprite(sf::Sprite &sprite)
	{
		IslandWindow.draw(sprite);
	}

	sf::Vector2f getCameraPos()
	{
		return Camera.getCenter();
	}
	void moveCamera(sf::Vector2f &offset)
	{
		Camera.setCenter(Camera.getCenter() + offset);
	}
	void setDefaultCameraPos()
	{
		Camera = IslandWindow.getDefaultView();
	}

	sf::Event *getIslandWindowEvent()
	{
		return &IslandWindowEvent;
	}
	bool getEvents(sf::Event::EventType EventType)
	{
		while (IslandWindow.pollEvent(IslandWindowEvent))
		{
			if (IslandWindowEvent.type == EventType)
			{
				return true;
			}
		}
	}

	sf::RenderWindow *getIslandWindow()
	{
		return &IslandWindow;
	}
};