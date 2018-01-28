#include "Gun.h"
#include "Object.h"
#include "Enemy.h"

#include <iostream>

Gun::Gun(vec3 a_pos)
{
	set_name("Gun");

	sprite.load_sprite_image("assets/images/Player/arm.png")
		.set_center(0.07f, 0.25f)
		.set_sprite_frame_size(40, 10)

		.push_frame("idle", 0, 0)
		.push_frame_row("firing", 0, 40, 11, 0, 2)

		//.set_scale(55, 30)
		.set_rotation(0)
		.set_position(a_pos.add(offset))
		.set_animation("idle");
	
	cursor.load_sprite_image("assets/images/Player/crosshair.png")
		.set_scale(60, 60);
	
	setSound();
	std::cout << get_name() << " was created" << std::endl;
}
Gun::~Gun()
{
	for (auto itr : bullets)
		delete itr;
}

Entity &Gun::Update() {
	//static sf::SoundBuffer buffer;
	//static sf::Sound sound;
	if (firing == 1)
	{
		sound.play();
		bullets.push_back(new Bullet(&sprite, stats));
	}
	if (firing > 0) {
		firing = firing++ % (int)stats.fireRate / 1 ;
	}
	for (auto itr : bullets)
		itr->Update();
	sprite.next_frame();
	return *this;
}
Entity &Gun::Draw() {
	sprite.draw();
	for (auto itr : bullets)
		itr->Draw();
	cursor.draw();
	return *this;
}

Entity &Gun::UpdateKey(Window::Key a_key, Window::EventType a_eventType) {
	for (auto itr : bullets)
		itr->UpdateKey(a_key, a_eventType);
	return *this;
}
Entity &Gun::UpdateMouse(Window::Button a_button, vec3 a_mouse, Window::EventType a_eventType) {
	if (a_eventType == Window::EventType::MouseButtonPressed)
		firing = 1;
	else if (a_eventType == Window::EventType::MouseButtonReleased)
		firing = 0;

	Window& theGame = Window::get_game_window();
	
	//.multiply(theGame.get_size(false)).divide(theGame.get_size(true))
	//.divide({ 5, 3 })
	vec3 mouse = a_mouse.add(theGame.get_position()).subtract(cursor.get_scale().divide(2)).divide(2);
	cursor.set_position(mouse.x, mouse.y);

	vec3 target = cursor.get_position().add(cursor.get_scale().divide(2));
	vec3 start = sprite.get_position();
	float rotation = atan2(target.y - start.y, target.x - start.x);
	sprite.set_rotation(rotation / (float)PI * 180);

	for (auto itr : bullets)
		itr->UpdateMouse(a_button, a_mouse, a_eventType);
	return *this;
}


bool Gun::Collide(Object &object) {
	for (auto itr = bullets.begin(); itr != bullets.end();)
		if ((*itr)->Collide(object)) {
			delete *itr;
			itr = bullets.erase(itr);
		}
		else itr++;
	return false;
}
bool Gun::Collide(Enemy &enemy) {
	bool collision = false;
	for (auto itr = bullets.begin(); itr != bullets.end();)
		if ((*itr)->Collide(enemy)) {

			delete *itr;
			itr = bullets.erase(itr);
			collision = true;
		}
		else itr++;
	return collision;
}

void Gun::setSound()
{
	if (!buffer.loadFromFile("assets/music/BulletShot.wav"))
	{
		exit(-1);
	}
	sound.setBuffer(buffer);
}
