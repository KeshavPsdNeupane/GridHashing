#include "HashGrid.h"


BoundingBox::BoundingBox()
    : id(0), box(), bound(), position() {}

BoundingBox::BoundingBox(const int& id, const sf::RectangleShape& rect)
    : id(id), box(rect), bound(rect.getGlobalBounds()), position(rect.getPosition()) {}

BoundingBox::BoundingBox(int id, const float& left, const float& top, const float& width, const float& height)
:id(id){
    this->box.setSize(sf::Vector2f(width, height));
    this->box.setPosition(left, top);
}


std::pair<uint32, uint32> HashGrid::GetGridIndex(const BoundingBox& box)const {
    sf::Vector2f pos = box.GetPosition();
   // std::cout << "cell raw  pos = " << pos.x << " " << pos.y << std::endl;
    uint32 xIndex = static_cast<uint32>(pos.x / gridSize.x);
    uint32 yIndex = static_cast<uint32>(pos.y / gridSize.y);
    if (xIndex > std::numeric_limits<uint32>::max() || yIndex > std::numeric_limits<uint32>::max()) {
        throw std::out_of_range("Calculated grid index is out of range.");
    }
   // std::cout << "cell raw  index = " << xIndex << " " << yIndex << std::endl;
    return {xIndex,yIndex};
}

std::pair<uint32, uint32> HashGrid::GetGridIndex(const float& x, const float& y)const {
    //std::cout << "cell raw  pos = " << x << " " << y << std::endl;
    uint32 xIndex = static_cast<uint32>(x / gridSize.x);
    uint32 yIndex = static_cast<uint32>(y / gridSize.y);
    if (xIndex > std::numeric_limits<uint32>::max() || yIndex > std::numeric_limits<uint32>::max()) {
        throw std::out_of_range("Calculated grid index is out of range.");
    }
    //std::cout << "cell raw  index = " << xIndex << " " << yIndex << std::endl;
    return { xIndex,yIndex };
}

uint32 HashGrid::Hash(std::pair<uint32,uint32>& pos)const {
    uint32 h1 = std::hash<uint32>{}(pos.first);
    uint32 h2 = std::hash<uint32>{}(pos.second);
    return h1 ^ (h2 << 7);
}

uint32 HashGrid::Hash(uint32 x, uint32 y)const {
    uint32 h1 = std::hash<uint32>{}(x);
    uint32 h2 = std::hash<uint32>{}(y);
    return h1 ^ (h2 << 7);
}

void HashGrid::RemoveFromCell(const uint32& key, const int& objectId) {
    auto it = worldGrid.find(key);
    if (it != worldGrid.end()) {
        std::vector<std::shared_ptr < BoundingBox>>&objectVector = it->second.second;
        //std::cerr << "Removing object with id = " << objectId << " from cell with key = " << key << std::endl;
        auto oldSize = objectVector.size();
        objectVector.erase(std::remove_if(objectVector.begin(), objectVector.end(),
            [objectId](const std::shared_ptr<BoundingBox>& obj) {
                return obj->id == objectId;
            }), objectVector.end());
        auto newSize = objectVector.size();
        //std::cerr << "Vector old size = " << oldSize << ", new size = " << newSize << std::endl;
        if (objectVector.empty()) {
           // std::cerr << "Emptying cell, erasing key: " << key << std::endl;
            worldGrid.erase(it);
        }
    }
    else {
        //std::cerr << "Error: Key not found in RemoveFromCell method." << std::endl;
    }
}



void HashGrid::AddObject(const BoundingBox& box){;
    auto pos = box.box.getPosition();
    //std::cout << "cell index = " << pos.x << " " << pos.x << std::endl;
    auto gridIndex = GetGridIndex(pos.x , pos.y);
   // std::cout << "cell index = " << gridIndex.first << " " << gridIndex.second << std::endl;
    auto key = Hash(gridIndex);
    auto it = worldGrid.find(key);  //iterator
    if (it != worldGrid.end()) {
        auto& entry = it->second;
        std::vector<std::shared_ptr<BoundingBox>>& vec = entry.second;
        vec.push_back(std::make_shared<BoundingBox>(box));
    }
    else {
        sf::Vector2f cellSize = sf::Vector2f(static_cast<float>(gridSize.x)
            , static_cast<float>(gridSize.y));
       // std::cout << "cell size = " << cellSize.x << " " << cellSize.y << std::endl;
        sf::Vector2f cellPosition = sf::Vector2f(gridIndex.first * cellSize.x,
            gridIndex.second * cellSize.y);
       // std::cout << "cell position = " << cellPosition.x << " " << cellPosition.y << std::endl;
        sf::RectangleShape gridCellShape;
        gridCellShape.setSize(cellSize);
        gridCellShape.setPosition(cellPosition);
        gridCellShape.setOutlineThickness(4);
        gridCellShape.setFillColor(sf::Color::Transparent);
        gridCellShape.setOutlineColor(sf::Color::Red);
       // Cell newcCell(gridIndex, gridCellShape);
        std::vector<std::shared_ptr<BoundingBox>> newVector;
        newVector.push_back(std::make_shared<BoundingBox>(box));
        worldGrid[key] = { gridCellShape , newVector };
    }
}

void HashGrid::MoveGridMember(BoundingBox& gridMember, const sf::Vector2f& newPosition) {
    auto tpos = gridMember.box.getPosition();
    auto oldIndex = GetGridIndex(tpos.x, tpos.y);
    auto newIndex = GetGridIndex(newPosition.x, newPosition.y);

    auto oldKey = Hash(oldIndex);
    auto newKey = Hash(newIndex);
    auto it = worldGrid.find(oldKey);
    if (it == worldGrid.end()) {
        std::cerr << "Error: Old key not found in grid." << std::endl;
        return; 
    }
    const sf::FloatRect& cellbound = it->second.first.getGlobalBounds(); 
    const sf::FloatRect& objectBound = gridMember.box.getGlobalBounds();
    if (oldKey == newKey || cellbound.intersects(objectBound) || cellbound.contains(objectBound.getPosition())) {
        std::cout << "oldisnew" << std::endl;
        gridMember.box.setPosition(newPosition);
    }
    else {
        std::cout << "oldnotnew" << std::endl;
        RemoveFromCell(oldKey, gridMember.id);
        gridMember.box.setPosition(newPosition);  
        AddObject(gridMember);
    }
}


std::vector<std::shared_ptr<BoundingBox>> HashGrid::FindNearbyObjects(const BoundingBox& obj) const {
    std::vector<std::shared_ptr<BoundingBox>> result;
    auto cell = GetGridIndex(obj);
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            std::pair<uint32, uint32>tcell = GetGridIndex(cell.first + dx, cell.second + dy);
            uint32 key = Hash(tcell); 
            auto it = worldGrid.find(key);
            if (it != worldGrid.end()) {
                const std::vector<std::shared_ptr<BoundingBox>>& objects = it->second.second; 
                for (const auto& bbox : objects) {
                    result.push_back(bbox);
                }
            }
        }
    }
    return result;
}


void HashGrid::DrawOnScreen(std::shared_ptr<sf::RenderWindow> window) {
    if (!window) {
        std::cerr << "Error: RenderWindow is null in DrawOnScreen method." << std::endl;
        return;
    }
    int index = 0;
    for (auto it = worldGrid.begin(); it != worldGrid.end(); ++it) {
        const sf::RectangleShape& shape = it->second.first;
        window->draw(shape);
       // std::cout << "Drawing shape at index: " << index << std::endl;
        ++index;
    }
}

