#pragma once
#include "Bullet.h"
#include <SFML\Audio.hpp>

class Gun : public Entity {
public:
	virtual Entity &Update() override;
	virtual Entity &Draw() override;
	virtual Entity &UpdateKey(Window::Key, Window::EventType) override;
	virtual Entity &UpdateMouse(Window::Button, vec3, Window::EventType) override;
	virtual bool Collide(Object&);
	virtual bool Collide(Enemy&);
	void setSound();

	Gun(vec3);
	~Gun();
	const vec3 offset = { 2, 10 };
private:
	std::vector<Bullet*> bullets;
	sf::SoundBuffer buffer;
	sf::Sound sound;
	int mag;
	int firing;
	Sprite cursor;
	GunStats stats;
	const std::string animation[2]{
		"idle",
		"firing",
	};
};
