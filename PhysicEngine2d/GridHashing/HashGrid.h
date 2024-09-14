#pragma once
#include <SFML/Graphics.hpp>
#include<memory>
#include<vector>
#include<unordered_map>
#include<iostream>
#include<functional>
class BoundingBox;
class Cell;
using uint32 = uint32_t;
using int32 = int32_t;
using Map = std::unordered_map < uint32, std::pair<sf::RectangleShape, std::vector<std::shared_ptr < BoundingBox>>>>;


class BoundingBox {
public:
    int32 id;
    sf::RectangleShape box;

private:
    const sf::FloatRect bound;     
    const sf::Vector2f position;  

public:
    BoundingBox();   
    BoundingBox(const int& id, const sf::RectangleShape& rect);
    BoundingBox(int id, const float& left, const float& top, const float& width, const float& height);
    const sf::FloatRect& GetBound() const { return bound; };
    const sf::Vector2f& GetPosition() const { return position; };
};

class Cell {
public:
    std::pair<uint32,uint32> cellId;
    sf::RectangleShape box;
    Cell() :cellId(0,0), box() {}
    Cell(const std::pair<uint32, uint32>& id, sf::RectangleShape& bound):cellId(id) ,box(bound) {}
};

class HashGrid {
   Map worldGrid;
   const sf::Vector2i gridSize{ 80,60 };
private:
    std::pair<uint32, uint32> GetGridIndex(const BoundingBox& box)const;
    std::pair<uint32, uint32> GetGridIndex(const float& x ,const float& y )const;
    uint32 Hash(std::pair<uint32, uint32>& pos)const;
    uint32 Hash(uint32 x, uint32 y)const;
    void RemoveFromCell(const uint32& key, const int& objectId);
public:
    HashGrid() = default;
    void AddObject(const BoundingBox& box);
    void MoveGridMember(BoundingBox& gridMember,const sf::Vector2f& newPosition);
    std::vector<std::shared_ptr<BoundingBox>> FindNearbyObjects(const BoundingBox& obj) const;
    void DrawOnScreen(std::shared_ptr<sf::RenderWindow>window);
};