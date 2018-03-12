#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Voxel.hpp"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 960;

const int WIDTH = 300;
const int HEIGHT = 120;

extern const int VOXEL_HEIGHT;
extern const int VOXEL_WIDTH;


class Map : public sf::Drawable
{
public:
	Map(){};
	Map(std::string fname);
	bool loadFromFile(std::string fname);
	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const;
	void damageVoxel( sf::Vector2f pos);
	int getVoxelSurface(sf::Vector2f pos) const;
	Voxel_Type getVoxelType(sf::Vector2f pos) const;
	float getVoxelFriction(sf::Vector2f pos);

private:
	friend Voxel;
	Voxel* voxels[WIDTH][HEIGHT];	
	sf::Texture grass_1;
};

Map::Map(std::string fname)
{
	loadFromFile(fname);
}


bool Map::loadFromFile(std::string fname)
{
	std::cout << "loading map data from : " << fname << std::endl;
	std::ifstream fin(fname);

	if(fin.is_open())
	{
		fin.seekg (0, fin.end);
		int length = fin.tellg();
		fin.seekg (0, fin.beg);

		if(length == (WIDTH*HEIGHT))
		{
			char * buffer =new char[length];
			fin.read(buffer, length);
			fin.close();	
			int bufIndex = 0;//keep track of location in buffer
			for(int j = 0; j < HEIGHT; j++)	
			{
				for(int i = 0; i < WIDTH; i++)
				{
					char symbol = buffer[bufIndex];
					int x =  ((VOXEL_WIDTH*i)+(VOXEL_WIDTH/2));
					int y =  ((VOXEL_HEIGHT*j)+(VOXEL_HEIGHT/2));

					std::cout << symbol; 

					if(i == 0)
						std::cout << std::endl;

					if(symbol == 'G')
					{
						voxels[i][j] = new Voxel(x, y, GRASS);
					}
					else if(symbol == 'D')
					{
						voxels[i][j] = new Voxel(x, y, DIRT);
					}
					else
					{
						voxels[i][j] = new Voxel(x, y, NONE );
					}
					bufIndex++;
				}	
			}
		}
		else
		{
			std::cout << "map file is incorrect size. dimensions must be " << WIDTH << "x" << HEIGHT << " voxels." << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "map file : " << fname << " failed to open" << std::endl;
		return false;
	}
	return true;	
}

int Map::getVoxelSurface(sf::Vector2f pos) const
{
	int tile_x = int(pos.x/VOXEL_WIDTH);
	int tile_y = int(pos.y/VOXEL_HEIGHT);
	if(voxels[tile_x][tile_y]->getVoxelType() == IMPASSABLE)	
	{
		return voxels[tile_x][tile_y]->getPosition().y - VOXEL_HEIGHT/2;
	}
	else 
		return pos.y;
}

Voxel_Type Map::getVoxelType(sf::Vector2f pos) const
{
	int tile_x = int(pos.x/VOXEL_WIDTH);
	int tile_y = int(pos.y/VOXEL_HEIGHT);
	if(((pos.x >= 0) && (pos.x <= voxels[WIDTH-1][0]->getPosition().x)) && ((pos.y >= 0) && (pos.y <= WINDOW_HEIGHT)))
	{
		return voxels[tile_x][tile_y]->getVoxelType();
	}
	else
	{
		return AIR;
	}	
}	

float Map::getVoxelFriction(sf::Vector2f pos)
{
	int tile_x = int(pos.x/VOXEL_WIDTH);
	int tile_y = int(pos.y/VOXEL_HEIGHT);
	if(((pos.x >= 0) && (pos.x <= voxels[WIDTH-1][0]->getPosition().x)) && ((pos.y >= 0) && (pos.y <= WINDOW_HEIGHT)))
	{
		return voxels[tile_x][tile_y]->getFriction();
	}
	else
	{
		return AIR;
	}
}

void Map::damageVoxel( sf::Vector2f pos)
{
	int tile_x = int(pos.x/VOXEL_WIDTH);
	int tile_y = int(pos.y/VOXEL_HEIGHT);
	if(((pos.x >= 0) && (pos.x <= voxels[WIDTH-1][0]->getPosition().x)) && ((pos.y >= 0) && (pos.y <= WINDOW_HEIGHT)))
	{
		voxels[tile_x][tile_y]->breakVoxel();
	}
}

//void loadMapData(const std::string& dataFile);
void Map::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
	for(int j = 0; j < HEIGHT; j++)
	{
		for(int i = 0; i < WIDTH; i++)
		{
			if(voxels[i][j]->getVoxelType() != AIR)
				window.draw(*voxels[i][j], states);
		}
	}
}


