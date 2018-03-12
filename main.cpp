#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.hpp"
#include "entity.hpp"


extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;



int main(int argc, char** argv)
{
	if(argc != 2)
	{
		std::cout << "no filename specified\n" << "usage: " << argv[0] << " filename" << std::endl;
	}
	std::string fileName = argv[1];
	std::cout << "filename : " << fileName << std::endl;
    sf::RenderWindow window(sf::VideoMode(800, 640), "sidescroller");
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(true);
	window.setFramerateLimit(30);
	Map map(fileName);
	Player player(50, 50);

	sf::Clock clock;
	sf::View view( sf::Vector2f(50, 50), sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			else if(event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Q)
				{
					return EXIT_SUCCESS;	
				}
				else if(event.key.code == sf::Keyboard::Space)
				{
					player.jump();
				}
				else if(event.key.code == sf::Keyboard::F)
				{
					if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
						player.attack(map, UP);
					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
						player.attack(map, DOWN);
					else
						player.attack(map, STRAIGHT);
				}

			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::J))
			{
				player.moveLeft();
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::SemiColon))
			{
				player.moveRight();
			}
			else 
				player.doNothing();
		} 
		sf::Time time = clock.getElapsedTime();	
		view.setCenter(player.getPosition().x,player.getPosition().y);
		view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

		player.update(map, time);
		clock.restart();
		window.setView(view);	
        window.clear(sf::Color(101,125,93));
		window.draw(map);
		window.draw(player);
        window.display();
		
    }

    return 0;
}
