#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <SFML/Graphics.hpp>

enum Voxel_Type { SOLID, PASSABLE, AIR };
enum Direction {RIGHT, LEFT, UP, DOWN};

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 640;

const int VOXELS_X = 300;
const int VOXELS_Y = 120;

const int VOXEL_WIDTH = 8;
const int VOXEL_HEIGHT = 8;

enum Block_Type {GRASS, DIRT, NONE};

class Voxel : public sf::Drawable
{
public:	
	Voxel( sf::Vector2f pos, Block_Type type = NONE )
	{
		rect.setSize(sf::Vector2f(8, 8));	
		rect.setOrigin(VOXEL_WIDTH/2, VOXEL_HEIGHT/2);	

		if(textures.size() == 0)
		{
			textures["grass"].loadFromFile("../images/grass1.png");
			textures["dirt"].loadFromFile("../images/dirt1.png");
		}
	
		switch (type)
		{
			case GRASS:
				rect.setTexture(&textures["grass"]);
				break;
			case DIRT:
				rect.setTexture(&textures["dirt"]);	
				break;
			case NONE:
				rect.setFillColor(sf::Color(101,125,93));
				break;
		}
		this->type = type;
		rect.setOutlineThickness(1);
		//rect.setOutlineColor(sf::Color(sf::Color::White));
		rect.setPosition(sf::Vector2f(pos.x, pos.y));
	}
	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const
	{
		window.draw(rect, states);
	}

	void setAs(Block_Type type)
	{
		this->type = type;
		switch (type)
		{
			case GRASS:
				rect.setTexture(&textures["grass"]);
				break;
			case DIRT:
				rect.setTexture(&textures["dirt"]);	
				break;
			case NONE:
				rect.setFillColor(sf::Color(101,125,93));
				break;
		}
	}

	Block_Type getVoxelType()
	{
		return type;
	}

	void highLight(bool lit)
	{
		if(lit == true)
		{
			rect.setOutlineThickness(1.0f);
			rect.setOutlineColor(sf::Color::Red);
			highlit = true;
		}
		else
		{
			rect.setOutlineThickness(0.0f);
			highlit = false;
		}
	}

	bool getHighlit()
	{
		return highlit;
	}

private:
	sf::RectangleShape rect;
	static std::map<std::string, sf::Texture> textures;
	Block_Type type;
	bool highlit;
};

std::map<std::string, sf::Texture> Voxel::textures;

struct VoxelPointer
{
	int x; 
	int y;
};


class Map : public sf::Drawable
{
public:
	Map()		
	{
		highlight.x = 20;
		highlight.y = 20;
		height = 1; 
		width = 1;
		for(int j = 0; j < VOXELS_Y; j++)
		{
			for(int i = 0; i < VOXELS_X; i++)
			{
				int x = i * VOXEL_WIDTH + VOXEL_WIDTH/2;
				int y = j * VOXEL_HEIGHT + VOXEL_HEIGHT/2;
				voxels[j][i] = new Voxel( sf::Vector2f(x, y), NONE);
			}
		}
		voxels[highlight.y][highlight.x]->highLight(true);
	}

	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const
	{
		for(int j = 0; j < VOXELS_Y; j++)
		{
			for(int i = 0; i < VOXELS_X; i++)
			{
				if(voxels[j][i]->getVoxelType() != NONE || voxels[j][i]->getHighlit() == true)
					window.draw(*voxels[j][i], states);
			}
		}
	}

	void movePointer(Direction dir)
	{
		voxels[highlight.y][highlight.x]->highLight(false);

		if(dir == RIGHT && highlight.x+width/2+width%2 < VOXELS_X)
		{
			highlight.x++;
		}
		else if(dir == LEFT && (highlight.x-width/2-width%2 > 0))
		{
			highlight.x--;
		}
		else if(dir == UP && (highlight.y-height/2-height%2 > 0))
		{
			highlight.y--;
		}
		else if(dir == DOWN && (highlight.y + (height/2 + height%2)< VOXELS_Y))
		{
			highlight.y++;
		}

		updateHighlight();

	}	
	
	void clearHighlight()
	{
		for(int j = 0; j < VOXELS_Y; j++)
		{
			for(int i = 0; i < VOXELS_X;i++)
			{
				voxels[j][i]->highLight(false);
			}
		}
	}

	void updateHighlight()
	{

		int left_y = 0 - height/2;
		int right_y = left_y + height;

		int left_x = 0 - width/2;
		int right_x = left_x + width;

		clearHighlight();

		voxels[highlight.y][highlight.x]->highLight(true);
		
		for(int j = left_y; j < right_y; j++)
		{
			for(int i = left_x; i < right_x; i++)
			{
				voxels[highlight.y+j][highlight.x+i]->highLight(true);
			}
		}
	}
	
	void highlitAsGrass()
	{
		for(int j = 0; j < VOXELS_Y; j++)
		{
			for(int i = 0; i < VOXELS_X;i++)
			{
				if(voxels[j][i]->getHighlit())
					voxels[j][i]->setAs(GRASS);
			}
		}
	}
	void highlitAsDirt()
	{
		for(int j = 0; j < VOXELS_Y; j++)
		{
			for(int i = 0; i < VOXELS_X;i++)
			{
				if(voxels[j][i]->getHighlit())
					voxels[j][i]->setAs(DIRT);
			}
		}
	}


	void extendHighlightWidth()
	{
		if((highlight.x + (width/2 + width % 2) < (VOXELS_X+1)) && (highlight.x - width/2 - width % 2 > 0))
			width++;	
		updateHighlight();
	}

	void extendHighlightHeight()
	{
		if((highlight.y + height/2 + height % 2 < VOXELS_Y+1) && (highlight.y - height/2 - height % 2 > 0))
			height++;	

		updateHighlight();
	}

	void shrinkHighlightWidth()
	{
		if(width > 0)
			width--;
		updateHighlight();
	}

	void shrinkHighlightHeight()
	{
		if(height > 0)
			height--;
		updateHighlight();
	}

	sf::Vector2i getHighlightPosition()
	{
		return highlight;
	}


	void saveToFile(std::string fname)
	{
		std::fstream fout(fname, std::ios::out);	
		if(fout.is_open())
		{
			for(int j = 0; j < VOXELS_Y; j++)
			{
				for(int i = 0; i < VOXELS_X; i++)
				{
					if(voxels[j][i]->getVoxelType() == GRASS)
					{
						fout << 'G';	
					}
					else if(voxels[j][i]->getVoxelType() == DIRT)
					{
						fout << 'D';
					}
					else
					{
						fout << ' ';
					}
				}
			}	
			fout.close();
			std::cout << "map saved to " << fname << std::endl;
		}
		else
			std::cout << "file " << fname << " failed to open." << std::endl;
	}

private:
	Voxel* voxels[VOXELS_Y][VOXELS_X];
	sf::Vector2i highlight;
	int height;
	int width;
};




