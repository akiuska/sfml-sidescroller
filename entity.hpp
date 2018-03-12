#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "map.hpp"
#include <vector>
#include <cstdlib>

extern const int WIDTH;
extern const int HEIGHT;


enum Player_States { STANDING, RUNNING, FALLING, JUMPING, PUNCHING };
enum XDirection { RIGHT, LEFT };
enum YDirection { UP, DOWN, STRAIGHT};


class Entity : public sf::Drawable
{
public:
	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const
	{
		window.draw(rect, states);
	}

protected:
	sf::Texture texturesRight;
	sf::Texture texturesLeft;
	sf::RectangleShape rect;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	Player_States state;
	Voxel_Type standing_on;
	XDirection direction;	
};


class Player : public Entity
{
public:
	Player(int x, int y);
	void moveRight();
	void moveLeft();
	void jump();
	void doNothing();
	void walkAnimation(sf::Time dt);
	void attack(Map map, YDirection dir);
	void animationRight(sf::Time dt);
	void animationLeft(sf::Time dt);
	void update(Map map, sf::Time dt);
	sf::Vector2f getPosition();

private:
	sf::Texture texSheet;
	std::vector<sf::Texture> walking;
	sf::IntRect animationFrame;
	sf::IntRect standingFrame;
	sf::IntRect jumpingFrame;
	sf::IntRect punchingFrame;
	sf::Time elapsed;
	sf::Time timeStep; 
	int reach = 1;
};

Player::Player(int x, int y)
	{
		velocity.y = 0;
		velocity.x = 0;
		acceleration.x = 0.01;
		acceleration.y = 0.001;
		timeStep = sf::milliseconds(40);
		elapsed = sf::milliseconds(0);

	
		if(texturesRight.loadFromFile("images/walking/right/littleGuy1_right.png"))
		{
		}
		//sf::Texture sheet;
		if(texturesLeft.loadFromFile("images/walking/left/littleGuy1_left.png"))
		{
		}

		animationFrame = sf::IntRect(20, 0, 20, 40);
		standingFrame = sf::IntRect(0,0,20,40);
		jumpingFrame = sf::IntRect(80, 0, 20, 40);
		punchingFrame = sf::IntRect(100, 0, 20, 40);


		rect.setTexture(&texturesRight);
		rect.setTextureRect(animationFrame);

		rect.setSize(sf::Vector2f(VOXEL_WIDTH*2, VOXEL_HEIGHT*4));	
		rect.setOrigin(VOXEL_WIDTH, VOXEL_HEIGHT*4);
		standing_on = AIR;
		state = FALLING;	
		direction = RIGHT;
		rect.setTexture(&texturesRight);
		position.x = x;
		position.y = y;
		rect.setPosition(sf::Vector2f(x, y));
	}

	void Player::moveRight()
	{
		if(state != RUNNING)
		{
			state = RUNNING;
			//this allows a slight change in direction while in air
			velocity.x+=0.02;
		}
		if(standing_on == IMPASSABLE && velocity.x <= 0.2)
		{
			if(velocity.x < std::abs(0.1)){
				velocity.x += 1.5*acceleration.x;
			}
			velocity.x += acceleration.x;
			direction =	RIGHT;
		}
	}

	void Player::moveLeft()
	{
		if(state != RUNNING)
		{
			state = RUNNING;
			//this allows a slight change in direction while in air
			velocity.x-=0.02;
		}

		if(standing_on == IMPASSABLE && velocity.x >= (-0.2))
		{	
			if(velocity.x < std::abs(0.1)){
				velocity.x -= 1.5*acceleration.x;
			}
			else
			{
				velocity.x -= acceleration.x;
			}
			direction =	LEFT;
		}
	}
	
	void Player::jump()
	{
		if(standing_on == IMPASSABLE)
		{
			state = JUMPING;
			rect.setTextureRect(jumpingFrame);
			position.y -= 0.5;
			velocity.y = (-0.4);
			rect.setPosition(position);
		}
	}
	void Player::doNothing()
	{
		if(state == RUNNING)
			state = STANDING;
			rect.setTextureRect(standingFrame);	
	}

	void Player::walkAnimation(sf::Time dt)
	{
		if(state == RUNNING && direction == RIGHT)
		{
			animationRight(dt);
		}
		else if(state == RUNNING && direction == LEFT)
		{
			animationLeft(dt);
		}
		else 
		{
			rect.setTextureRect(animationFrame);
		}
	}

	void Player::attack(Map map, YDirection dir)
	{
		if(direction == RIGHT)
			reach = 1;
		else
			reach = -1;
		if(dir == UP)
		{
			state = PUNCHING;
			if(map.getVoxelType(sf::Vector2f(position.x+(reach*VOXEL_WIDTH), position.y-3*VOXEL_HEIGHT)) == IMPASSABLE){
				map.damageVoxel(sf::Vector2f(position.x+(reach*VOXEL_WIDTH), position.y-3*VOXEL_HEIGHT));
			}
		}
		else if (dir == DOWN)
		{
			state = PUNCHING;
			if(map.getVoxelType(sf::Vector2f(position.x+(reach*VOXEL_WIDTH), position.y-VOXEL_HEIGHT)) == IMPASSABLE){
				map.damageVoxel(sf::Vector2f(position.x+(reach*VOXEL_WIDTH), position.y-VOXEL_HEIGHT));
			}
		}
		else if(dir == STRAIGHT)
		{
			state = PUNCHING;
			if(map.getVoxelType(sf::Vector2f(position.x+(reach*VOXEL_WIDTH), position.y-2*VOXEL_HEIGHT)) == IMPASSABLE){
				map.damageVoxel(sf::Vector2f(position.x+(reach*VOXEL_WIDTH), position.y-2*VOXEL_HEIGHT));
			}
		}
	}

	

	void Player::animationRight(sf::Time dt)
	{
		elapsed += dt;
		static int frame = 1; 
		if(elapsed > sf::milliseconds(100))
		{
			rect.setTexture(&texturesRight);
			animationFrame.left = frame * 20;
			rect.setTextureRect(animationFrame);
			if(frame == 3)
			{
				frame= 1;
			}
			else
				frame++;
			elapsed = sf::milliseconds(0);
		}
	}

	void Player::animationLeft(sf::Time dt)
	{
		elapsed += dt;
		static int frame = 1; 
			
		if(elapsed > sf::milliseconds(100))
		{

			rect.setTexture(&texturesLeft);
			animationFrame.left = frame * 20;
			rect.setTextureRect(animationFrame);
			if(frame == 3)
			{
				frame = 1;
			}
			else 
				frame++;
			elapsed = sf::milliseconds(0);
		}
	}

	void Player::update(Map map, sf::Time dt){
		
		standing_on = map.getVoxelType(this->position);

		if(state == PUNCHING)
		{
			static int elapsed=0;
			elapsed += dt.asMilliseconds();		
			rect.setTextureRect(punchingFrame);
			if(elapsed > 200)
			{
				state = STANDING;
				elapsed = 0;
			}
		}
		else if(state == RUNNING)
		{
			walkAnimation(dt);
		}

		dt = std::min(dt, timeStep);
		
		std::cout << "elapsed time " << dt.asMilliseconds() << std::endl;

		//moving right
		if(direction == RIGHT && velocity.x > 0)
		{
			if(map.getVoxelType(sf::Vector2f(position.x+0.5*VOXEL_WIDTH, position.y-2*VOXEL_HEIGHT )) == IMPASSABLE)
			{
				velocity.x = 0;
			}
			//if stair
			else if(map.getVoxelType(sf::Vector2f(position.x+0.25*VOXEL_WIDTH, position.y-VOXEL_HEIGHT/2)) == IMPASSABLE)
			{
				if(velocity.x > (0.1))
				{
					velocity.x = 0.1;
				}
				position.y = map.getVoxelSurface(sf::Vector2f(position.x+VOXEL_WIDTH, position.y-VOXEL_HEIGHT/2));
			}
		}
		//moving left
		else if(direction == LEFT && velocity.x < 0)
		{
			if(map.getVoxelType(sf::Vector2f(position.x-0.5*VOXEL_WIDTH, position.y-2*VOXEL_HEIGHT)) == IMPASSABLE)
			{
				velocity.x = 0;
			}
			//if stair
			else if(map.getVoxelType(sf::Vector2f(position.x-0.25*VOXEL_WIDTH, position.y-VOXEL_HEIGHT/2)) == IMPASSABLE)
			{
				if(velocity.x <= (-0.1))
				{
					velocity.x = -0.1;
				}
				position.y = map.getVoxelSurface(sf::Vector2f(position.x-VOXEL_WIDTH, position.y-VOXEL_HEIGHT/2));
			}	
		}
		if(map.getVoxelType(sf::Vector2f(position.x, position.y-2*VOXEL_HEIGHT)) == IMPASSABLE && velocity.y < 0)
		{
			velocity.y = 0;
		}
	
		if(standing_on == IMPASSABLE )
		{
			velocity.y = 0;
			position.y = map.getVoxelSurface(sf::Vector2f(position.x, position.y));
			if(velocity.x == 0)
			{
				state = STANDING;
			}

			if(state == STANDING)
			{
				velocity.x *= map.getVoxelFriction(sf::Vector2f(position.x, position.y+0.5));
				rect.setTextureRect(standingFrame);
			}
		}
		else if (standing_on == PASSABLE || standing_on == AIR)
		{
			if(state != PUNCHING)
				rect.setTextureRect(jumpingFrame);
			if(state == JUMPING && velocity.y >= 0)
			{
				state = FALLING;
			}
			if(velocity.y <= 0.15)
				velocity.y += acceleration.y * dt.asMilliseconds();	
		}
		position.y += velocity.y * dt.asMilliseconds();
		position.x += velocity.x * dt.asMilliseconds();
		rect.setPosition(position);
	}	


	sf::Vector2f Player::getPosition()
	{
		return position;
	}

