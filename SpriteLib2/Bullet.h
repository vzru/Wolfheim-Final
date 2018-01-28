#pragma once
#include "Entity.h"

// helper
struct GunStats {
	float dmg = 12.5;				// arbitrary
	float fireRate = 2500;			// in milliseconds
	float range = 300;				// in pixels
	int magSize = 6;				// number of bullets that can be shot until have to reload
	float reloadTime = 1000;		// in milliseconds
	float speed = 25;				// everything here except for this does nothing right now
};

class Bullet : public PhysicsEntity {
public:
	virtual Entity &Update() override;
	virtual Entity &Draw() override;
	virtual Entity &UpdateKey(Window::Key, Window::EventType) override;
	virtual Entity &UpdateMouse(Window::Button, vec3, Window::EventType) override;
	virtual bool Collide(Object&);
	virtual bool Collide(Enemy&);
	virtual bool Collide(Player&);

	Bullet(Sprite*, GunStats&, string file = "Player/bullet.png");

private:
	float dmg;
};