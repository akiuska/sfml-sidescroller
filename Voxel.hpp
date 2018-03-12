#pragma once

#include <SFML/Graphics.hpp>
#include "map.hpp"

const int VOXEL_WIDTH = 8;
const int VOXEL_HEIGHT = 8;

enum Voxel_Type  { PASSABLE, IMPASSABLE, AIR };
enum Block_Type  { GRASS, DIRT, NONE };

class Voxel : public sf::Drawable 
{
public:
	Voxel(int x, int y, Block_Type type);
	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const;
	void breakVoxel();
	void setTexture(sf::Texture texture);
	Voxel_Type getVoxelType();
	sf::Vector2f getPosition();
	float getFriction();

private:
	Block_Type block_type;
	Voxel_Type voxel_type;
	sf::RectangleShape rect;
	float friction;
	static std::map<std::string, sf::Texture> textures;
	int health;
};

std::map<std::string, sf::Texture> Voxel::textures;

Voxel::Voxel(int x, int y, Block_Type type = NONE)
	{
		if(textures.size() == 0)	
		{
			textures["grass_1"].loadFromFile("images/grass1.png");
			textures["dirt_1"].loadFromFile("images/dirt1.png");
		}
			
		rect.setOrigin(sf::Vector2f(VOXEL_WIDTH/2, VOXEL_HEIGHT/2));
		rect.setSize(sf::Vector2f(VOXEL_WIDTH, VOXEL_HEIGHT));	
		this->block_type = type;
		if(block_type == GRASS)
		{
			rect.setTexture( &textures["grass_1"] );
			voxel_type = IMPASSABLE;
			health = 3;
			friction = 0.7;
		}
		else if(type == DIRT)
		{
			rect.setTexture(&textures["dirt_1"]);
			voxel_type = IMPASSABLE;
			health = 3;
			friction = 0.7;
		}
		else
		{
			block_type = NONE;
			voxel_type = AIR;
			health = 0;
			rect.setFillColor(sf::Color(101,125,93));
		}
		block_type = type;
		rect.setPosition(sf::Vector2f(x, y));
	}

	void Voxel::draw(sf::RenderTarget& window, sf::RenderStates states) const
	{
		window.draw(rect, states);
	}

	void Voxel::breakVoxel()
	{
		if(this->voxel_type == IMPASSABLE)
		{
			std::cout << "punched voxel! health = " << health << std::endl;
			health--;
			if(health == 0)
			{
				voxel_type = AIR;
				block_type = NONE;
				rect.setFillColor(sf::Color(101,125,93));	
			}
		}
	}

	Voxel_Type Voxel::getVoxelType()
	{
		return voxel_type;
	}	

	sf::Vector2f Voxel::getPosition(){
		return rect.getPosition();
	}

	float Voxel::getFriction()
	{
		return friction;
	}

	void Voxel::setTexture(sf::Texture texture)
	{
		rect.setTexture(&texture);
	}
