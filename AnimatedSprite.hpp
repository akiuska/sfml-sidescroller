#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "map.hpp"
#include <map>
#include <vector>
#include <cstdlib>

extern const int WIDTH;
extern const int HEIGHT;

//Player is an animated sprite


namespace entity
{

	enum Player_States	{ STANDING, RUNNING, JUMPING, ATTACKING };
	enum xDirection		{ RIGHT, LEFT, NONE };
	enum yDirection		{ UP, DOWN, STRAIGHT };


	class AnimatedSprite : public sf::Sprite
	{
	public:
		AnimatedSprite(int x, int y)
		{
			setPosition(x, y);
		}

		virtual void loadTexture(std::string sheet, std::string fname)
		{
			if(!textures[sheet].loadFromFile(fname)){
				std::cout << "failed to load texture from " << fname << "\n";
			}
		}
		
		virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const
		{
			window.draw(*this, states);
		}

		virtual void setAnimationFrame(std::string sheet, sf::IntRect frame)
		{
			setTexture(textures[sheet]);
			setTextureRect(frame);
		}

	protected:
		std::map<std::string, sf::Texture> textures;
	};


	class Player : public AnimatedSprite
	{

	};



}








