#include "Circle.h"
#include"GameObject.h"

Circle::Circle(const sf::Vector2f pos, const float rad)
	: velocity(70.0f, 70.0f), deltaTime(0),collisionBox(1, pos.x, pos.y, 2 * rad, 2 * rad) {
	this->circle.setPosition(pos);
	this->circle.setRadius(rad);
	auto box = collisionBox.box.getGlobalBounds();
	//std::cerr << "box = " << box.top << " " << box.left << " " << box.width << " " << box.height << std::endl;
}


void Circle::WindowBound() {
	auto& position = this->circle.getPosition();
	auto size = 2*this->circle.getRadius();
	if (position.x < 0 || position.x + size  > 800) { velocity.x = -velocity.x; }
	if (position.y < 0 || position.y + size > 600) { velocity.y = -velocity.y; }
}


void Circle::Load() {
	gameObject.worldGrid.AddObject(collisionBox);
	this->circle.setFillColor(sf::Color::Green);
	this->collisionBox.box.setFillColor(sf::Color::Transparent);
	this->collisionBox.box.setOutlineThickness(1.0f);
	this->collisionBox.box.setOutlineColor(sf::Color::Blue);
}

void Circle::Update(float& deltaTime) {
	this->deltaTime = deltaTime;
	WindowBound();
	gameObject.worldGrid.MoveGridMember(this->collisionBox, collisionBox.box.getPosition());
	this->circle.move(this->velocity * deltaTime);
	this->collisionBox.box.setPosition(this->circle.getPosition());
}


void Circle::Draw(std::shared_ptr<sf::RenderWindow> window) {
	if (!window) {
		std::cerr << "Error: window is null in Circle::Draw." << std::endl;
		return;
	}
	window->draw(this->circle);
	window->draw(this->collisionBox.box);
}



