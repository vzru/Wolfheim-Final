#pragma once

#include "Gun.h"
#include <time.h>

// helper structures
struct Thrust {
	bool up, down, left, right, jump;		// is_key_pressed
};

class Player : public PhysicsEntity {
public:
	virtual Entity &Update() override;
	virtual Entity &Draw() override;
	
	virtual Entity &UpdateKey(Window::Key, Window::EventType) override;
	virtual Entity &UpdateMouse(Window::Button, vec3, Window::EventType) override;

	virtual bool Collide(Object&);
	virtual bool Collide(Enemy&);

	Player(vec3);

	Thrust thrust;
	float life = 100;
	
private:
	Gun gun;
	vec3 platform;
	vec3 ladder;
	string state = "standing";
	const string animation[5] {
		"idle",
		"running",
		"jumping",
		"landing",
		"climbing",
	};
};