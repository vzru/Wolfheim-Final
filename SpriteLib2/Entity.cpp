#include "Entity.h"
#include "Object.h"

#include <iostream>

//helpers
ostream &operator<<(ostream &stream, vec3 &other) {
	stream << "x:" << other.x << "y:" << other.y << "z:" << other.z;
	return stream;
}

// destructor
Entity::~Entity() {
	std::cout << get_name() << " was destroyed" << std::endl;
}

// empty methods
Entity &Entity::Update()
{
	return *this;
}
Entity &Entity::Draw()
{
	return *this;
}
Entity &Entity::UpdateMouse(Window::Button, vec3, Window::EventType)
{
	return *this;
}
Entity &Entity::UpdateKey(Window::Key, Window::EventType)
{
	return *this;
}
bool Entity::Collide(Entity &) {
	return false;
}

// methods
vec3 PhysicsEntity::Move() {
	vec3 new_pos = sprite.get_position().add(velocity);
	sprite.set_position(new_pos.x, new_pos.y);
	velocity = velocity.add(acceleration);
	return new_pos;
}

// getters
std::string Entity::get_name() {
	return name;
}
vec3 PhysicsEntity::get_velocity() {
	return velocity;
}
vec3 PhysicsEntity::get_acceleration() {
	return acceleration;
}
// setters
Entity & Entity::set_name(std::string new_name) {
	name = new_name;
	return *this;
}
Entity & PhysicsEntity::set_velocity(vec3 new_velocity) {
	velocity = new_velocity;
	return *this;
}
Entity & PhysicsEntity::set_acceleration(vec3 new_acceleration) {
	acceleration = new_acceleration;
	return *this;
}