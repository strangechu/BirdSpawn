#pragma once
#include <vector>

struct Point {
	int x;
	int y;
	Point() {
		x = 0;
		y = 0;
	}
	Point(int a, int b) {
		x = a;
		y = b;
	}
};

struct Vector3 {
	float x;
	float y;
	float z;
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float a, float b, float c) : x(a), y(b), z(c) {}

	Vector3 operator+ (const Vector3 v) { return Vector3(this->x + v.x, this->y + v.y, this->z + v.z); }
	Vector3 operator- (const Vector3 v) { return Vector3(this->x - v.x, this->y - v.y, this->z - v.z); }
};

class Entity
{
public:
	Entity();
	Entity(float a, float b, float c);
	~Entity();

	Vector3 pos;
};

class EntityManager
{
private:
	EntityManager();
	~EntityManager();
	
	static EntityManager* m_instance;
	std::vector<Entity> entities;

public:
	static EntityManager* EntityManager::getInstance() {
		if (m_instance == NULL) {
			m_instance = new EntityManager();
		}
		return m_instance;
	}

	void init();
	void clear();
	void addEntity(Entity entity);
	std::vector<Entity> getAllEntity();
};

