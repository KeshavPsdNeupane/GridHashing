#pragma once
#include<memory>
#include<vector>
#include<unordered_map>
#include<SFML/Graphics.hpp>
#include<iostream>

class Object {
public:
    int id;
    sf::RectangleShape rectangle;

public:
    Object();
    Object(int id, const sf::FloatRect& bound);
    Object(int id, const float& left, const float& top, const float& width, const float& height);
    void UpdateCoordinate(float x, float y);
};

class Grid {
private:
    std::unordered_map<int32_t, std::vector<std::shared_ptr<Object>>> gridspace;
    sf::Vector2i gridSize;

private:
    std::pair<int32_t, int32_t> GridCell(const Object& obj)const;
    std::pair<int32_t, int32_t> GridCell(float x, float y) const;
    int32_t Hash(int32_t x, int32_t y) const;
    void RemoveFromCell(int32_t key, int objectId);

public:
    Grid():gridSize(36,24){}
    void AddObject(const std::shared_ptr<Object>  obj);
    void MoveGridMember(std::shared_ptr<Object> gridMember, float newX, float newY);
    std::vector<std::shared_ptr<Object>> FindNearbyObjects(const Object& obj) const;
    void Update(std::shared_ptr<sf::RenderWindow>window);
};



