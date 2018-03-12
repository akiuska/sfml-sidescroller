#include <iostream>
#include <SFML/Graphics.hpp>
#include "mapBuilder.hpp"

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
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "mapbuilder");
	window.setVerticalSyncEnabled(true);
	Map map;
	sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	view.setCenter(sf::Vector2f(map.getHighlightPosition().x*VOXEL_WIDTH, map.getHighlightPosition().y*VOXEL_HEIGHT));
	view.zoom(0.6);
	bool moved = false;
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
				else if(event.key.code == sf::Keyboard::S)
				{
					map.saveToFile(fileName);
				}
				else if(event.key.code == sf::Keyboard::J)
				{
					map.movePointer(LEFT);	
					moved = true;
				}
				else if(event.key.code == sf::Keyboard::K)
				{
					map.movePointer(DOWN);	
					moved = true;
				}
				else if(event.key.code == sf::Keyboard::L)
				{
					map.movePointer(UP);	
					moved = true;
				}
				else if(event.key.code == sf::Keyboard::SemiColon)
				{
					map.movePointer(RIGHT);	
					moved = true;
				}
				else if(event.key.code ==sf::Keyboard::F)
				{
					map.highlitAsGrass();
				}
				else if(event.key.code ==sf::Keyboard::D)
				{
					map.highlitAsDirt();
				}
				else if(event.key.code == sf::Keyboard::G)
				{
					map.extendHighlightWidth();
				}
				else if(event.key.code == sf::Keyboard::H)
				{
					map.extendHighlightHeight();
				}
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{
				if(event.key.code == sf::Keyboard::G)
				{
					map.shrinkHighlightWidth();
				}
				if(event.key.code == sf::Keyboard::H)
				{
					map.shrinkHighlightHeight();
				}
			}
		}

		if(moved == true)	
		{
			view.setCenter(sf::Vector2f(map.getHighlightPosition().x*VOXEL_WIDTH, map.getHighlightPosition().y*VOXEL_HEIGHT));
			moved = false;
		}
		window.setView(view);

        window.clear(sf::Color(101,125,93));
		window.draw(map);
        window.display();
    }

    return 0;
}
