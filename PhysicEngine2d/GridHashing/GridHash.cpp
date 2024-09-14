#include "GridHash.h"
Object::Object():id() , rectangle(){}

Object::Object(int id, const sf::FloatRect& bound)
	: id(id) {
	this->rectangle.setPosition(bound.left, bound.top);
	this->rectangle.setSize(sf::Vector2f(bound.width, bound.height));
	this->rectangle.setOutlineThickness(6.0f);
	this->rectangle.setOutlineColor(sf::Color::Red);
	this->rectangle.setFillColor(sf::Color::Transparent);
}

Object::Object(int id, const float& left, const float& top, const float& width, const float& height)
	: id(id) {
	this->rectangle.setSize(sf::Vector2f(width, height));
	this->rectangle.setPosition(left, top);
	this->rectangle.setOutlineThickness(1.0f);
	this->rectangle.setOutlineColor(sf::Color::Red);
	this->rectangle.setFillColor(sf::Color::Transparent);
}

void Object::UpdateCoordinate(float x,float y){
	this->rectangle.setPosition(x,y); 
}

//--------------------------------------------------------------------------------------------------------------------------



std::pair<int32_t, int32_t> Grid::GridCell(const Object& obj)const{
	sf::Vector2f pos = obj.rectangle.getPosition();
	if (pos.x < 0 || pos.y < 0) {
		std::cerr << "Warning: Object position is out of bounds. Position: (" << pos.x << ", " << pos.y << ")" << std::endl;
	}
	int32_t a = static_cast<int32_t>(pos.x / gridSize.x);
	int32_t b = static_cast<int32_t>(pos.y / gridSize.y);
	//std::cout << "a1,b1 position = " << a << " " << b << std::endl;
	return { a, b };
}

std::pair<int32_t, int32_t> Grid::GridCell(float x, float y) const{
	int32_t a = static_cast<int32_t>(x / gridSize.x);
	int32_t b = static_cast<int32_t>(y / gridSize.y);
	//std::cout << "a2,b2 position = " << a << " " << b << std::endl;
	return { a, b };
}


int32_t Grid::Hash(int32_t x, int32_t y) const{
	int32_t hash = x * 31 + y;
	hash = (hash << 16) ^ (hash >> 16);
	return (hash<0)?-hash:hash;
}

void Grid::RemoveFromCell(int32_t key, int objectId){
	auto it = gridspace.find(key);
	if (it != gridspace.end()) {
		auto& objects = it->second;
		objects.erase(std::remove_if(objects.begin(), objects.end(),
			[objectId](const std::shared_ptr<Object>& obj) { return obj->id == objectId; }),
			objects.end());

		if (objects.empty()) {
			gridspace.erase(it);
		}
	}
}


void Grid::AddObject(const std::shared_ptr<Object> obj) {
	//if (!obj) {
	//	std::cerr << "Error: obj is null in Grid::AddObject." << std::endl;
	//	return;
	//}
	//sf::Vector2f pos = obj->rectangle.getPosition();
	//std::cerr << "d1 - Cell: (" << pos.x<< ", " << pos.y << ")" << std::endl;
	//auto o = *obj;
	//auto cell = GridCell(o);
	//std::cerr << "d1 - Cell: (" << cell.first << ", " << cell.second << ")" << std::endl;
	////
	//auto key = Hash(cell.first, cell.second);
	//std::cerr << "d2 - Key: " << key << std::endl;

	//auto nn = obj->rectangle.getGlobalBounds();

	//std::shared_ptr<Object> newObj;
	//try {
	//	newObj = std::make_shared<Object>(obj->id, nn.left, nn.top, nn.width, nn.height);
	//	std::cerr << "d4 - New Object Created Successfully." << std::endl;
	//}
	//catch (const std::exception& e) {
	//	std::cerr << "Exception creating newObj: " << e.what() << std::endl;
	//	return;
	//}

	//if (gridspace.find(key) != gridspace.end()) {
	//	gridspace[key].push_back(newObj);
	//}
	//else {
	//	gridspace[key] = { newObj };
	//}

	//std::cerr << "d5 - Object Added to Grid." << std::endl;
}


void Grid::MoveGridMember(std::shared_ptr<Object> gridMember, float newX, float newY) {
	if (!gridMember) {
		std::cerr << "Error: gridMember is null in MoveGridMember." << std::endl;
		return;
	}
	//std::cerr << "Old pos = " << gridMember->rectangle.getPosition().x << " " << gridMember->rectangle.getPosition().y << std::endl;
	//std::cerr << "new pos = " << newX << " " << newY << std::endl;
	auto oldCell = GridCell(*gridMember);
	auto newCell = GridCell(newX, newY);

	//if (oldCell != newCell) {
		std::cerr << "old == new" << oldCell.first << " " << oldCell.second
			<< "  " << newCell.first << " " << newCell.second << std::endl;
		auto oldKey = Hash(oldCell.first, oldCell.second);
		RemoveFromCell(oldKey, gridMember->id);
		gridMember->UpdateCoordinate(newX, newY);
		auto newKey = Hash(newCell.first, newCell.second);
		gridspace[newKey].push_back(gridMember);
	//}
	//else {
	//	std::cerr << "old == new" << oldCell.first << " " << oldCell.second 
	//		<< "  " << newCell.first <<" "<< newCell.second << std::endl;
	//	gridMember->UpdateCoordinate(newX, newY);
	//}
}

std::vector<std::shared_ptr<Object>> Grid::FindNearbyObjects(const Object& obj) const {
	std::vector<std::shared_ptr<Object>> result;
	auto gridCell = GridCell(obj);
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			auto key = Hash(gridCell.first + dx, gridCell.second + dy);
			auto it = gridspace.find(key);
			if (it != gridspace.end()) {
				auto& objects = it->second;
				for (const auto& object : objects) {
					if (object->id != obj.id) {
						result.push_back(object);
					}
				}
			}
		}
	}
	return result;
}

void Grid::Update(std::shared_ptr<sf::RenderWindow> window) {
	for (auto& cell : gridspace) {
		for (auto& obj : cell.second) {
			window->draw(obj->rectangle);
		}
	}
}
