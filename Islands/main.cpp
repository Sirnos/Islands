#include <SFML/Graphics.hpp>

void main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 1024), "Islands", sf::Style::Fullscreen);


	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		window.display();
	}

	return;
}