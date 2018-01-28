#include "Enemy.h"

#include <iostream>

Enemy::Enemy(vec3 pos, float health, float dmg)
	: life(health), damage(dmg)
{
	set_name("Wolf");

	sprite.load_sprite_image("assets/images/Enemies/wolf.png")
		.set_sprite_frame_size(42, 30)
		.set_scale(65, 50)
		.set_center(0.5f, 0.5f)
		.set_position(pos.x, pos.y)
		.set_animation("idle");

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			sprite.push_frame("idle", i * 42, 60);

	std::cout << get_name() << " was created" << std::endl;
}

Entity &Enemy::Update() {
	// get the game window
	Window &theGame = Window::get_game_window();

	sprite.next_frame();
	// end enemy update
	return *this;
}
Entity &Enemy::Draw() {
	// draw enemy
	sprite.draw();

	// end enemy draw
	return *this;
}
Entity &Enemy::UpdateKey(Window::Key a_key, Window::EventType a_eventType) {
	bool pressed = a_eventType == Window::EventType::KeyPressed ? true : false;

	// end enemy update key
	return *this;
}
Entity &Enemy::UpdateMouse(Window::Button a_button, vec3 a_mouse, Window::EventType a_eventType) {
	// end enemy update mouse
	return *this;
}