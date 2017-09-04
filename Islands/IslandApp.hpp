#pragma once

#include <SFML/Graphics.hpp>

enum class mouseWheel
{
	Up,
	Down,
	Stop
};

class IslandApp
{
	sf::RenderWindow IslandWindow;
	sf::Event IslandWindowEvent;
	sf::View Camera;

public:
	IslandApp(sf::VideoMode AppVideoMode,unsigned FPS,bool VerticalSync,bool Windowed)
	{
		const std::string AppTitle("Islands");
		if (!AppVideoMode.isValid())
		{
			IslandWindow.create(sf::VideoMode::getFullscreenModes()[0], AppTitle, sf::Style::Fullscreen);
		}
		else
		{
			IslandWindow.create(AppVideoMode, AppTitle,sf::Style::Fullscreen);
			if (Windowed)
			{
				IslandWindow.create(AppVideoMode, AppTitle, sf::Style::Default);
			}
		}
		IslandWindow.setFramerateLimit(FPS);
		IslandWindow.setVerticalSyncEnabled(VerticalSync);
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