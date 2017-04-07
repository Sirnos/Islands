#pragma once

#include <SFML/Graphics.hpp>


class IslandApp
{
	sf::RenderWindow IslandWindow;
	sf::Event IslandWindowEvent;
	sf::View Camera;

public:
	void init(sf::VideoMode WindowMode = sf::VideoMode(1280,1024,32),unsigned FPS = 62,bool vertSync = false)
	{
		IslandWindow.create(WindowMode, "Islands", sf::Style::Fullscreen);
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

	template<class ShapeType>
	void draw(ShapeType & object)
	{
		IslandWindow.draw(object);
	}

	template<class ShapePointersVector>
	void drawShapePointersVector(ShapePointersVector & Vector)
	{
		for (size_t i = 0; i < Vector.size(); i++)
		{
			IslandWindow.draw(*Vector[i]);
		}
	}
	template<class ShapeVector>
	void drawShapeVector(ShapeVector &Vector)
	{
		for (size_t i = 0; i < Vector.size(); i++)
		{
			IslandWindow.draw(Vector[i]);
		}
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