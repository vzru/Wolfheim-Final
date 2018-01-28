#pragma once
#include "Entity.h"

class Object : public Entity {
public:
	Object(vec3 a_pos, vec3 a_size);
	~Object();

	vec3 size;
	vec3 pos;
};

class Ladder : public Object {
public:
	Ladder(vec3 a_pos, float a_length);
	~Ladder();

};

class Platform : public Object {
public:
	Platform(vec3 a_pos, vec3 a_size);
	~Platform();

};

class Wall : public Object {
public:
	Wall(vec3 a_pos, vec3 a_size);
	~Wall();

};