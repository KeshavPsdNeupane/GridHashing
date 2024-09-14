#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include<iostream>
#include"Circle.h"
#include"GridHashing/GridHash.h"
class Runner {
private:
    std::shared_ptr<sf::RenderWindow> window;
    sf::Event event;
    Circle circle;
    sf::Clock clock;
    float deltaTime;
public:
    Runner();
    inline  std::shared_ptr<sf::RenderWindow>& GetWindows() { return this->window; }

public:
    void GameRunner();
    void Load();
    void Update();
    void SfmlEvent();
    void Draw();
};
