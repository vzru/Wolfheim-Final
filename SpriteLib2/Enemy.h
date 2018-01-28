#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
	virtual Entity &Update() override;
	virtual Entity &Draw() override;
	virtual Entity &UpdateKey(Window::Key, Window::EventType) override;
	virtual Entity &UpdateMouse(Window::Button, vec3, Window::EventType) override;

	Enemy(vec3 pos, float health, float dmg);
	float life;
	float damage;
private:
	const std::string animation[2]{
		"idle",
		"running"
	};

};