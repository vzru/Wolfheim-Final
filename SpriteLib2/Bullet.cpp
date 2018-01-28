#include <iostream>
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include "Object.h"

Bullet::Bullet(Sprite* gun, GunStats &stats, string file)
	: dmg(stats.dmg)
{
	set_name("Bullet");

	vec3 pos = gun->get_position().add(vec3(gun->get_scale().x * cos(gun->get_rotation() * (float)PI / 180), gun->get_scale().x * sin(gun->get_rotation() * (float)PI / 180)));

	sprite.load_sprite_image("assets/images/" + file)
		.set_center(0.5f, 0.5f)
		.set_scale(15, 10)
		.set_rotation(gun->get_rotation())
		.set_position(pos.x, pos.y);

	velocity = { stats.speed * cos(gun->get_rotation() * (float)PI / 180), stats.speed * sin(gun->get_rotation() * (float)PI / 180) };

	cout << get_name() << " was created" << endl;
}

Entity &Bullet::Update() {
	Move();
	sprite.next_frame();
	return *this;
}
Entity &Bullet::Draw() {
	sprite.draw();
	return *this;
}

Entity &Bullet::UpdateKey(Window::Key a_key, Window::EventType a_eventType) { return *this; }
Entity &Bullet::UpdateMouse(Window::Button a_button, vec3 a_mouse, Window::EventType a_eventType) { return *this; }

bool Bullet::Collide(Object &object) {
	if (object.get_name() == "Platform" || object.get_name() == "Wall")
	{
		vec3 pos = sprite.get_position();
		vec3 size = sprite.get_scale();
		vec3 objPos = object.sprite.get_position();
		vec3 objSize = object.sprite.get_scale();
		//std::cout << pos.x << ", " << pos.y << std::endl;
		//std::cout << objPos.x << ", " << objPos.y << std::endl;
		//cout << objSize.x << ", " << objSize.y << endl;
		if (pos.x + velocity.x < objPos.x + objSize.x / 2 && pos.x + velocity.x > objPos.x - objSize.x / 2 &&	// Colliding against sides of platform
			pos.y + velocity.y > objPos.y - objSize.y / 2 && pos.y + velocity.y < objPos.y + objSize.y / 2)		// Colliding against top and bottom of platform
		{
			cout << "Bullet@Platform/Wall" << pos << endl;
			return true;
		}

	}
	return false;
}
bool Bullet::Collide(Enemy &target) {
	vec3 pos = sprite.get_position();
	vec3 size = sprite.get_scale();
	vec3 targetPos = target.sprite.get_position();
	vec3 targetSize = target.sprite.get_scale();
	//std::cout << pos.x << ", " << pos.y << std::endl;
	//std::cout << enemyPos.x << ", " << enemytarget.y << std::endl;
	if (pos.x < targetPos.x + targetSize.x / 2 && pos.x > targetPos.x - targetSize.x / 2 &&
		pos.y < targetPos.y + targetSize.y / 2 && pos.y > targetPos.y - targetSize.y / 2)
	{
		target.life -= dmg;
		if (target.life <= 0)
			return true;
		cout << "Bullet@Enemy " << pos << endl;
	}
	return false;
}
bool Bullet::Collide(Player &target) {
	vec3 pos = sprite.get_position();
	vec3 size = sprite.get_scale();
	vec3 targetPos = target.sprite.get_position();
	vec3 targetSize = target.sprite.get_scale();
	//std::cout << pos.x << ", " << pos.y << std::endl;
	//std::cout << enemyPos.x << ", " << enemyPos.y << std::endl;
	if (pos.x < targetPos.x + targetSize.x / 2 &&
		pos.x > targetPos.x - targetSize.x / 2 &&
		pos.y < targetPos.y + targetSize.y / 2 &&
		pos.y > targetPos.y - targetSize.y / 2)
	{
		target.life -= dmg;
		if (target.life <= 0)
			return true;
		cout << "Bullet@Player " << pos << endl;
	}
	return false;
}