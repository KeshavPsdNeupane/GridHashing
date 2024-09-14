#include "Runner.h"
#include"GridHashing/GridHash.h"
#include"GameObject.h"
Runner::Runner()
    : window(std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "SFML works!")),
    event(),
    circle(sf::Vector2f(100.0f,100.0f) , 5.0f),
    deltaTime(0.0f){
    window->setFramerateLimit(60);
}


void Runner::GameRunner() {
    Load();
    while (window->isOpen()) {
        SfmlEvent();
        Update();
        window->clear();
        Draw();
        window->display();
    }
}

void Runner::Load() {
    this->circle.Load();
}

void Runner::Update() {
    deltaTime = clock.restart().asSeconds();
    this->circle.Update(this->deltaTime);
}

void Runner::SfmlEvent() {
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
    }
}

void Runner::Draw() {
    this->circle.Draw(window);
    gameObject.worldGrid.DrawOnScreen(window);
}


