#pragma once

#include <SFML/Graphics.hpp>

#include "EngineVars.hpp"

enum class mouseWheel
{
	Up,
	Down,
	Stop
};

class IslandApp
{
	const std::string Title{ "Islands" };

	sf::RenderWindow IslandWindow;
	sf::Event IslandWindowEvent;
	sf::View Camera;

public:
	IslandApp(VideoVars &v1)
	{
		sf::VideoMode appVideoMode{ v1.WindowSize.x,v1.WindowSize.y,v1.BitsPerPixel };

		if (!appVideoMode.isValid())
		{
			IslandWindow.create(sf::VideoMode::getFullscreenModes().front(), Title, sf::Style::Fullscreen);
		}
		else
		{
			IslandWindow.create(appVideoMode, Title,sf::Style::Fullscreen);
			if (v1.Windowed)
			{
				IslandWindow.create(appVideoMode, Title, sf::Style::Default);
			}
		}
		IslandWindow.setFramerateLimit(v1.FrameRate);
		IslandWindow.setVerticalSyncEnabled(v1.VerticalSync);
		Camera.setSize(static_cast<sf::Vector2f>(IslandWindow.getSize()));
	}
	IslandApp(const IslandApp& other) = delete;
	~IslandApp() = default;

	void clearContext() { IslandWindow.clear(); }
	void clearContext(sf::Color BufferColor) { IslandWindow.clear(BufferColor); }
	void displayContext() { IslandWindow.display(); }

	template<class ShapeType>
	void draw(ShapeType & object) { IslandWindow.draw(object); }

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

	sf::Vector2f getCameraPos() { return Camera.getCenter(); }
	void moveCamera(sf::Vector2f &offset) { Camera.setCenter(Camera.getCenter() + offset); }
	void setDefaultCameraPos() { Camera = IslandWindow.getDefaultView(); }

	sf::Event *getIslandWindowEvent() { return &IslandWindowEvent; }
	sf::RenderWindow *getIslandWindow() { return &IslandWindow; }
	sf::View *getIslandView() { return &Camera; }

	bool getEvents(sf::Event::EventType EventType)
	{
		while (IslandWindow.pollEvent(IslandWindowEvent))
		{
			if (IslandWindowEvent.type == EventType)
			{
				return true;
			}
		}
		return false;
	}

	sf::Vector2f getMousePosInWorld()
	{
		return IslandWindow.mapPixelToCoords(sf::Mouse::getPosition());
	}
};