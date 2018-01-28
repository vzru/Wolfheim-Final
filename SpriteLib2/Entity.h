#pragma once
#include "spritelib/spritelib.h"

// helpers
using namespace std;
using namespace spritelib;
typedef spritelib::math::Vector3 vec3;
ostream &operator<<(ostream&, vec3&);
class Object;
class Player;
class Bullet;
class Enemy;

// Derived Classes: Object, Enemies, Gun, PhysicsEntity
class Entity {
public:
	virtual Entity &Update();
	virtual Entity &Draw();

	virtual Entity &UpdateMouse(Window::Button, vec3, Window::EventType);
	virtual Entity &UpdateKey(Window::Key, Window::EventType);

	virtual bool Collide(Entity&);

	//Entity(Sprite);
	~Entity();

	std::string get_name();
	Entity &set_name(std::string);

	//bool isAlive = true;
	Sprite sprite;

protected:

private:
	std::string name;

};

// Derived Classes: Player, Bullet(in Gun.h)
class PhysicsEntity : public Entity {
public:
	vec3 get_velocity();
	vec3 get_acceleration();

	Entity & set_velocity(vec3);
	Entity & set_acceleration(vec3);

protected:
	vec3 Move();	// returns the new position

	vec3 velocity, acceleration;

private:

};
