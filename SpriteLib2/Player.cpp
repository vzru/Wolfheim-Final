#include "Player.h"
#include "Object.h"
#include "Enemy.h"

#include <iostream>

Player::Player(vec3 a_pos) : gun(a_pos)
{
	set_name("Player");

	sprite.load_sprite_image("assets/images/Player/player.png")
		.set_sprite_frame_size(64, 64)
		.set_center(0.5f, 0.5f)
		.set_position(a_pos.x, a_pos.y)

		.push_frame_row("idle", 0, 128, 64, 0, 9)
		.push_frame_row("running", 0, 64, 64, 0, 7)
		.push_frame("jumping", 0, 64)
		.push_frame_row("climbing", 0, 0, 64, 0, 6)
		.push_frame("idle climb", 64, 0)

		.set_animation("idle");

	std::cout << get_name() << " was created" << std::endl;
}

Entity &Player::Update() {
	// get the game window
	Window &theGame = Window::get_game_window();

	// get position after moving
	vec3 pos = Move();
	// get size
	vec3 size = sprite.get_scale();

	// property manipulation based on state

	// if he's in the air
	if (state == "falling") {
		// make him look like he is jumping
		sprite.set_animation("jumping");
		// make him fall
		acceleration.y = -1;
	}
	// if he is not in the air
	else {
		// make him not fall
		acceleration.y = 0;
		velocity.y = 0;
	}
	// if he is climbing
	if (state == "climbing") {
		// make him not move horizontally
		velocity.x = 0;
		//acceleration.x = 0;
		// if he climbs off the ladder
		if (pos.y - size.y / 2 + velocity.y / 2 < ladder.y ||
			pos.y - size.y / 2 + velocity.y / 2 > ladder.y + ladder.z ||
			thrust.left || thrust.right)
		{
			velocity.y = 0;
			state = "falling";
		}
		else {
			// make him move up or down
			velocity.y = 10 * (int)thrust.up - 10 * (int)thrust.down;
		}
		// if he is climbing the ladder
		if (thrust.up || thrust.down) {
			// make it look like he is climbing the ladder
			sprite.set_animation("climbing");
		}
		else {
			// make it look like he is idle on the ladder
			sprite.set_animation("idle climb");
		}
	}

	// velocity clamping
	velocity.x = velocity.x > 10 ? 10 : velocity.x < -10 ? -10 : velocity.x;
	// friction setting
	if (velocity.x != 0) acceleration.x = velocity.x * -0.1;

	// key imput
	//cout << ", keys: " << thrust.left << thrust.right << thrust.up << thrust.down << thrust.jump << endl;
	// make him jump if he is not in the air
	if (thrust.jump && state != "falling") {
		state = "falling";
		velocity.y = 11.5;
	}

	// if he walks off the edge
	if (state == "standing" && (
		pos.x + size.x / 3 < platform.x - platform.z ||
		pos.x - size.x / 3 > platform.x + platform.z))
	{
		cout << "Walked off the edge of: " << platform << endl;
		state = "falling";
	}
	// kill him if he has falled off the map or ran out of health
	if (pos.y <= 0 || life <= 0)
	{
		pos.x = 70;
		pos.y = 500;
		life = 100;
		state = "falling";
	}
	// if he is on a platform
	if (state == "standing" && pos.y <= platform.y + size.y / 2)
		if (abs(velocity.x) <= 1)
			sprite.set_animation("idle");
		else
			sprite.set_animation("running");
	// acceleration movement
	acceleration.x = acceleration.x - (int)thrust.left + (int)thrust.right;
	
	// camera movement
	theGame.set_ortho_matrix(int(pos.x - theGame.get_width() / 2), int(pos.y - 100), theGame.get_width(), theGame.get_height());
	// animation movement
	sprite.next_frame();
	// sprite mirroring, still have to flip the gun
	sprite.set_flipped(velocity.x < -1 || sprite.get_flip() && velocity.x <= 1);
	
	// position resetting
	sprite.set_position(pos);
	gun.sprite.set_position(pos.add(gun.offset));
	// update gun
	gun.Update();
	// end player update
	return *this;
}
Entity &Player::Draw() {
	// draw player
	sprite.draw();
	// draw gun
	gun.Draw();
	// draw health bar
	Shapes::set_color(1.0f, 0.0f, 0.0f);
	Shapes::draw_rectangle(false, sprite.get_position().x - sprite.get_scale().x / 3, sprite.get_position().y + sprite.get_scale().y / 2, sprite.get_scale().x / 3 * 2, 5);
	Shapes::draw_rectangle(true, sprite.get_position().x - sprite.get_scale().x / 3, sprite.get_position().y + sprite.get_scale().y / 2, sprite.get_scale().x * life / 300 * 2, 5);
	Shapes::set_color(0.0f, 0.0f, 0.0f);
	// end player draw
	return *this;
}
Entity &Player::UpdateKey(Window::Key a_key, Window::EventType a_eventType) {
	bool pressed = a_eventType == Window::EventType::KeyPressed;
	switch (a_key)
	{
	case Window::Key::A:		thrust.left = pressed;	break;
	case Window::Key::D:		thrust.right = pressed;	break;
	case Window::Key::W:		thrust.up = pressed;	break;
	case Window::Key::S:		thrust.down = pressed;	break;
	case Window::Key::Space:	thrust.jump = pressed;	break;
	}

	// update gun keys
	gun.UpdateKey(a_key, a_eventType);
	// end player update key
	return *this;
}
Entity &Player::UpdateMouse(Window::Button a_button, vec3 a_mouse, Window::EventType a_eventType) {
	// update gun mouse
	gun.UpdateMouse(a_button, a_mouse, a_eventType);
	// end player update mouse
	return *this;
}

bool Player::Collide(Object &target) {
	vec3 pos = sprite.get_position();
	vec3 size = sprite.get_scale();
	vec3 targetPos = target.pos;
	vec3 targetSize = target.size;
	//cout << pos << endl << size << endl << objPos << endl << objSize << endl;

	if (target.get_name() == "Ladder") {
		// ladder movement
		if (pos.x < targetPos.x + targetSize.x / 2 &&
			pos.x > targetPos.x - targetSize.x / 2 &&
			pos.y - size.y / 2 + velocity.y >= targetPos.y &&
			pos.y - size.y / 2 + velocity.y <= targetPos.y + targetSize.y)
		{
			ladder = { targetPos.x, targetPos.y, targetSize.y };
			cout << "Player@Ladder@@" << endl;
			if (thrust.up || thrust.down) {
				state = "climbing";
				pos.x = targetPos.x;
			}
			else if (state == "climbing") {
				velocity.y = 0;
			}
		}
	}
	if (target.get_name() == "Platform") {
		// platform movement
		if (pos.x - size.x / 3 < targetPos.x + targetSize.x / 2 &&	// Colliding against right of platform
			pos.x + size.x / 3 > targetPos.x - targetSize.x / 2 &&	// Colliding against left of platform

			pos.y + size.y / 2 + velocity.y > targetPos.y - targetSize.y / 2 &&	// Colliding against bottom of platform
			pos.y - size.y / 2 + velocity.y < targetPos.y + targetSize.y / 2)	// Colliding against top of platform
		{
			//cout << pos.subtract(size.divide({ 3,2 })) << "," << pos.add(size.divide({ 3,2 })) << endl
			//	<< objPos.subtract(objSize.divide(2)) << "," << objPos.add(objSize.divide(2)) << endl;
			platform = { targetPos.x, targetPos.y + targetSize.y / 2, targetSize.x / 2 };
			cout << "Player@Platform" << endl;
			if (state != "climbing") {
				state = "standing";
				pos.y = targetPos.y + (targetSize.y / 2) + (size.y / 2);
				velocity.y = 0;
			}
		}
	}
	if (target.get_name() == "Wall") {
		if (pos.x - size.x / 3 + velocity.x < targetPos.x + targetSize.x / 2 &&	// Colliding against right of wall
			pos.x + size.x / 3 + velocity.x > targetPos.x - targetSize.x / 2 &&	// Colliding against left of wall

			pos.y + size.y / 2 > targetPos.y - targetSize.y / 2 &&	// Colliding against bottom of wall
			pos.y - size.y / 2 < targetPos.y + targetSize.y / 2)	// Colliding against top of wall
		{
			cout << "Player@Wall@x:" << pos.x << " y:" << pos.y << endl;
			//pos.x = pos.x - velocity.x;
			velocity.x = 0;
			acceleration.x = 0;
		}
	}
	sprite.set_position(pos.x, pos.y);
	gun.sprite.set_position(pos.add(gun.offset));
	gun.Collide(target);
	return false;
}

bool Player::Collide(Enemy &target) {
	vec3 pos = sprite.get_position();
	vec3 size = sprite.get_scale();
	vec3 targetPos = target.sprite.get_position();
	vec3 targetSize = target.sprite.get_scale();

	// Enemy Collision
	if ((pos.x - size.x / 3) < (targetPos.x + targetSize.x / 2) &&
		(pos.x + size.x / 3) > (targetPos.x - targetSize.x / 2) &&
		pos.y >= targetPos.y &&
		pos.y < targetPos.y + targetSize.y)
	{
		cout << "Player@Enemy" << endl;
		static time_t start;
		double diffT = difftime(time(0), start);
		if (diffT >= 1) {
			life -= target.damage;
			start = time(0);
		}
	}

	// continue the collision check and return the result of it
	return gun.Collide(target);;
}