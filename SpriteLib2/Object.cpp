#include "Object.h"

Object::Object(vec3 a_pos, vec3 a_size)
	: pos(a_pos), size(a_size)
{
}
Object::~Object() {}

Ladder::Ladder(vec3 a_pos, float a_length)
	: Object(a_pos, { 20,a_length })
{
	set_name("Ladder");
}
Ladder::~Ladder() {}

Platform::Platform(vec3 a_pos, vec3 a_size)
	: Object(a_pos, a_size)
{
	set_name("Platform");
}
Platform::~Platform() {};

Wall::Wall(vec3 a_pos, vec3 a_size)
	: Object(a_pos, a_size)
{
	set_name("Wall");
}
Wall::~Wall() {};