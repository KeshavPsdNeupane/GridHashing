#pragma once
#include<SFML/Graphics.hpp>
#include"GridHashing/HashGrid.h"

struct GameObject;

class Circle
{
private:
	sf::CircleShape circle;
	sf::Vector2f velocity;
	BoundingBox collisionBox;
	float deltaTime;

public:
	Circle() = default;
	Circle(const sf::Vector2f pos, const float rad);

private:
	//Update extra part
	void WindowBound();

public:
	void Load();
	void Update(float& deltaTime);
	void Draw(std::shared_ptr<sf::RenderWindow> window);
};

