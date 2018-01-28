/*
Version 1.6 (final)
Updated bug fix
*/

#include "Player.h"
#include "Object.h"
#include "Enemy.h"

#include <fstream>
#include <streambuf>
#include <iostream>
#include <time.h>

Sprite cutscene, level;
Sprite menu;
Sprite title;
Sprite startB, quitB;

Player *player;
vector<Enemy*> enemies;
vector<Object*> objects;

enum GameState
{
	INTRO,	MENU,
	CUTSCENE,	WINSCENE,
	TUTORIAL,
	FOREST1,	FOREST2,	FOREST3,	FOREST4,	FOREST5,
	BOSS,
	QUIT,
	COUNT,
};

// the state the game is in (including which level it is on)
GameState gameState = INTRO;
// whether the game is 'running' or not (as in actually playable)
bool running = false;

void KeyboardFunc(Window::Key a_key, Window::EventType a_eventType) {
	if (running) {
		player->UpdateKey(a_key, a_eventType);

		if (a_key == Window::Key::Escape) {
			std::cout << "Escape was pressed!" << std::endl;
			gameState = MENU;
		}
	}
}

void MouseFunc(Window::Button a_button, vec3 a_mouse, Window::EventType a_eventType) {
	if (running)
		player->UpdateMouse(a_button, a_mouse, a_eventType);
	else if (gameState == MENU) {
		// start button
		if (a_mouse.x < 1200 && a_mouse.x > 690 && a_mouse.y > 425 && a_mouse.y < 650) {	// within the boundries of the button
			startB.set_frame(1);
			if (a_eventType == Window::EventType::MouseButtonReleased)
				gameState = CUTSCENE;
		}
		else
			startB.set_frame(0);
		// quit button
		if (a_mouse.x < 1140 && a_mouse.x > 750 && a_mouse.y > 170 && a_mouse.y < 400) {	// within the boundries of the button
			quitB.set_frame(1);
			if (a_eventType == Window::EventType::MouseButtonReleased)
				gameState = QUIT;
		}
		else
			quitB.set_frame(0);
	}
}

void Update() {
	//update
	player->Update();
	for (auto itr : enemies)
		itr->Update();

	//collide
	for (auto itr = objects.begin(); itr != objects.end();)
		if (player->Collide(**itr)) {
			delete *itr;
			itr = objects.erase(itr);
		} else itr++;
	for (auto itr = enemies.begin(); itr != enemies.end();)
		if (player->Collide(**itr)) {
			delete *itr;
			itr = enemies.erase(itr);
		} else itr++;
}

void DrawMenu() {
	menu.draw();
	title.draw();
	startB.draw();
	quitB.draw();
}

void DrawSprites() {
	// draw main sprites
	level.draw();
	player->Draw();
	// draw all the enemies
	for (auto itr : enemies)
		itr->Draw();
}

// for testing
void DrawCollision() {
	for (auto itr : enemies)
		Shapes::draw_rectangle(false, itr->sprite.get_position().x - itr->sprite.get_scale().x/2, itr->sprite.get_position().y - itr->sprite.get_scale().y/2, itr->sprite.get_scale().x, itr->sprite.get_scale().y);
	Shapes::draw_rectangle(false, player->sprite.get_position().x - player->sprite.get_scale().x / 3, player->sprite.get_position().y - player->sprite.get_scale().y / 2, player->sprite.get_scale().x / 3 * 2, player->sprite.get_scale().y);
	for (auto itr : objects)
		if (itr->get_name() == "Ladder")
			Shapes::draw_rectangle(false, itr->pos.x - itr->size.x / 2, itr->pos.y, itr->size.x, itr->size.y);
		else
			Shapes::draw_rectangle(false, itr->pos.x - itr->size.x / 2, itr->pos.y - itr->size.y / 2, itr->size.x, itr->size.y);
}

int main()
{
	Window& theGame = Window::get_game_window();

	// Let's make a game window. use the static function get_game_window to get a handle to the main game window
	theGame.init("Wolfheim", 1280, 640, true)
		.set_ortho_matrix(0, 0, 1280, 640)
		.set_size(640, 320)
		.set_keyboard_callback(KeyboardFunc)
		.set_mouse_callback(MouseFunc)
		.show_mouse(false)
		.set_clear_color(0, 0, 0);
	Text::load_font("assets/fonts/times.ttf", "TimesNewRoman");

	// music loading
	sf::SoundBuffer buffer;
	sf::Sound sound;
	sf::Music music;
	if (!buffer.loadFromFile("assets/music/BulletShot.wav"))
		return -1;
	sound.setVolume(50);
	sound.setBuffer(buffer);

	// Game Loop
	while (gameState != QUIT) {
		if (gameState == INTRO) {
			theGame.set_ortho_matrix(0, 0, 1280, 640);
			running = false;
			// music
			if (!music.openFromFile("assets/music/in-game battle music.wav"))
				return -1;
			music.play();
			music.setVolume(75);
			music.setLoop(true);

			time_t start = time(0);
			while (theGame.update(30) && gameState == INTRO) {
				double deltatime = difftime(time(0), start);
				if (deltatime > 6)	gameState = MENU;

				switch ((int)deltatime) {
				case 0:	cutscene.load_sprite_image("assets/images/Story/wolfheim title.png");	break;
				case 3:	cutscene.load_sprite_image("assets/images/Story/Logo.png");				break;
				}
				cutscene.set_scale(1280, 640)
					.set_position(0, 0)
					.draw();
			}
		}
		else if (gameState == MENU) {
			theGame.set_ortho_matrix(0, 0, 1280, 640)
				.show_mouse(true);
			running = false;
			time_t start = time(0);

			menu.load_sprite_image("assets/images/Story/menu background.png")
				.set_sprite_frame_size(1280, 640)
				.push_frame_row("menu", 0, 0, 1280, 0, 5)
				.set_animation("menu")
				.set_position(0, 0);
			title.load_sprite_image("assets/images/Story/menu title.png")
				.set_scale(1280, 640)
				.set_position(0, 0);
			startB.load_sprite_image("assets/images/Story/start independent.png")
				.set_sprite_frame_size(1280, 136)
				.push_frame_row("startB", 0, 0, 1280, 0, 2)
				.set_animation("startB")
				.set_position(0, 250);
			quitB.load_sprite_image("assets/images/Story/quit independent.png")
				.set_sprite_frame_size(1280, 136)
				.push_frame_row("quitB", 0, 0, 1280, 0, 2)
				.set_animation("quitB")
				.set_position(0, 100);

			while (theGame.update(7) && gameState == MENU) {
				// update
				menu.next_frame();

				DrawMenu();
			}
		}
		else if (gameState == CUTSCENE) {
			theGame.set_ortho_matrix(0, 0, 1280, 640)
				.show_mouse(false);
			running = false;
			bool played = false;
			time_t start = time(0);

			while (theGame.update(30) && gameState == CUTSCENE) {
				double deltatime = difftime(time(0), start);
				string file;
				if (deltatime > 20)	gameState = TUTORIAL;

				switch ((int)deltatime) {
				case 0:		cutscene.load_sprite_image("assets/images/Story/intro slide 1.png");	break;
				case 3:		cutscene.load_sprite_image("assets/images/Story/intro slide 2.png");	break;
				case 6:		cutscene.load_sprite_image("assets/images/Story/intro slide 3.png");	break;
				case 9:		cutscene.load_sprite_image("assets/images/Story/intro slide 4.png");	break;
				case 12:	cutscene.load_sprite_image("assets/images/Story/intro slide 5.png");
					played = played || (sound.play(), true);										break;
				case 14:	cutscene.load_sprite_image("assets/images/Story/intro slide 6.png");	break;
				case 17:	cutscene.load_sprite_image("assets/images/Story/intro slide 3.png");	break;
				case 20:	cutscene.load_sprite_image("assets/images/Story/intro slide 7.png");	break;
				}
				cutscene.set_scale(1280, 640)
					.set_position(0, 0)
					.draw();
			}
			music.stop();
		}
		else if (gameState == TUTORIAL) {
			running = true;
			// delete
			if (player != nullptr)
				delete player;
			for (auto itr : enemies)
				delete itr;
			enemies.clear();
			for (auto itr : objects)
				delete itr;
			objects.clear();
			// music
			if (!music.openFromFile("assets/music/game stuff.wav"))
				return -1;
			music.play();
			music.setVolume(75);
			music.setLoop(true);
			// background
			level.load_sprite_image("assets/images/Levels/Tutorial.png")
				.set_scale(1280, 640)
				.set_position(0, 0);

			// player
			player = new Player({ 1200, 640 - 212 + 32 });
			player->sprite.set_flipped(true);
			// enemies
			enemies.push_back(new Enemy({ 700, 17 + 25 }, 50, 1));
			// ladders
			objects.push_back(new Ladder({ 169, 17 }, 185));
			objects.push_back(new Ladder({ 1086, 17 }, 185));

			// walls
			vector<pair<vec3, vec3>> wall = {
				{{ -10, 0 },{ 0,640 } },
				{ { 1280, 0 },{ 1300, 640 } },
				{ { 210, 382 },{ 513, 432 } },
				{{ 513, 363},{561, 408}},
				{{ 698, 360 },{ 744, 408 } },
				{ { 744, 360 },{ 812, 426 } },
				{{ 490, 200 },{ 745, 356 } },
				{{ 0, 382 },{ 107, 432 } }
			};
			for (auto itr : wall)
				objects.push_back(new Wall(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));
			// platforms
			vector<pair<vec3, vec3>> plat = {
				{ { 0, 15 },{ 1280,17 } },
				{ { 0, 199 },{ 1280,201 } },
				{ { 0, 430 },{ 107,432 } },
				{ { 210, 430 },{ 513,432 } },
				{ { 514 , 408 },{ 561, 410 } },
				{ { 564 , 356 },{ 686, 358 } },
				{ { 698 , 408 },{ 743, 410 } },
				{ { 744, 426 },{ 1280, 428 } }
			};
			for (auto itr : plat)
				objects.push_back(new Platform(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));

			// IMPORTANT: This is how spritelib 2.0 handles the main game loop
			while (theGame.update(15) && gameState == TUTORIAL) {
				Update();

				DrawSprites();
				//DrawCollision();

				// next
				if (player->sprite.get_position().x >= 1240 && player->sprite.get_position().x <= 1280 &&
					player->sprite.get_position().y >= 200 && player->sprite.get_position().y <= 420)
				{
					gameState = FOREST1;
				}
			}
			music.stop();
		}
		else if (gameState == FOREST1) {
			running = true;
			// delete
			if (player != nullptr)
				delete player;
			for (auto itr : enemies)
				delete itr;
			enemies.clear();
			for (auto itr : objects)
				delete itr;
			objects.clear();
			// music
			if (!music.openFromFile("assets/music/slow and steady.wav"))
				return -1;
			music.play();
			music.setVolume(75);
			music.setLoop(true);
			// background
			level.load_sprite_image("assets/images/Levels/forest_1.png")
				.set_scale(1280, 640)
				.set_position(0, 0);

			// player
			player = new Player({ 70, 192 + 32 });
			player->sprite.set_flipped(false);
			// enemies
			enemies.push_back(new Enemy({ 700, 192 + 25 }, 50, 5));
			enemies.push_back(new Enemy({ 925, 256 + 25 }, 50, 5));

			// walls
			vector<pair<vec3, vec3>> wall = {
				{ { -10, 0 },{ 0, 640 } },
				{ { 1280, 0 },{ 1300, 640 } },
				{ { 298, 192 },{ 432, 254 } },
				{ { 768, 192 },{ 960, 254 } },
				{ { 960, 256 },{ 1024, 318 } },
				{ { 1024, 320 },{ 1280, 382 } },
			};
			for (auto itr : wall)
				objects.push_back(new Wall(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));
			// platforms
			vector<pair<vec3, vec3>> plat = {
				{ { 0, 190 },{ 768, 192 } },
				{ { 302, 254 },{ 428, 256 } },
				{ { 770, 254 },{ 1024, 256 } },
				{ { 962, 318 },{ 1024, 320 } },
				{ { 1026, 382 },{ 1280, 384 } },
			};
			for (auto itr : plat)
				objects.push_back(new Platform(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));

			// IMPORTANT: This is how spritelib 2.0 handles the main game loop
			while (theGame.update(15) && gameState == FOREST1) {
				Update();

				DrawSprites();
				//DrawCollision();

				if (player->sprite.get_position().x >= 1240 && player->sprite.get_position().x <= 1280 &&
					player->sprite.get_position().y >= 384 && player->sprite.get_position().y <= 420)
				{
					gameState = FOREST2;
				}
			}
		}
		else if (gameState == FOREST2) {
			running = true;
			// delete
			if (player != nullptr)
				delete player;
			for (auto itr : enemies)
				delete itr;
			enemies.clear();
			for (auto itr : objects)
				delete itr;
			objects.clear();
			// background
			level.load_sprite_image("assets/images/Levels/forest_2.png")
				.set_scale(1280, 640)
				.set_position(0, 0);

			// player
			player = new Player({ 70, 384 + 32 });
			player->sprite.set_flipped(false);
			// enemies
			enemies.push_back(new Enemy({ 400, 254 + 25 }, 100, 10));
			enemies.push_back(new Enemy({ 900, 254 + 25 }, 100, 10));

			// walls
			vector<pair<vec3, vec3>> wall = {
				{ { -10, 0 },{ 0, 640 } },
				{ { 1280, 0 },{ 1300, 640 } },
				{ { 0, 256 },{ 256, 384 } },
				{ { 448, 256 },{ 512, 314 } },
				{ { 704, 0 },{ 960, 256 } },
				{ { 960, 256 },{ 1024, 316 } },
				{ { 1024, 316 },{ 1088, 380 } },
				{ { 1088, 380 },{ 1280, 444 } },
			};
			for (auto itr : wall)
				objects.push_back(new Wall(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));
			// platforms
			vector<pair<vec3, vec3>> plat = {
				{ { 0,382 },{ 256,384 } },
				{ { 256, 254 },{ 512, 256} },
				{ { 448, 314 },{ 512, 316} },
				{ { 704, 254 },{ 960, 256 } },
				{ { 960, 314 },{ 1024, 316 } },
				{ { 1024, 378 },{ 1088, 380 } },
				{ { 1088, 442 },{ 1280, 444 } },
			};
			for (auto itr : plat)
				objects.push_back(new Platform(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));
						
			// IMPORTANT: This is how spritelib 2.0 handles the main game loop
			while (theGame.update(15) && gameState == FOREST2) {
				Update();

				DrawSprites();
				//DrawCollision();

				// next
				if (player->sprite.get_position().x >= 1240 && player->sprite.get_position().x <= 1280 &&
					player->sprite.get_position().y >= 444 && player->sprite.get_position().y <= 600)
				{
					gameState = FOREST3;
				}
			}
		}
		else if (gameState == FOREST3) {
			running = true;
			// delete
			if (player != nullptr)
				delete player;
			for (auto itr : enemies)
				delete itr;
			enemies.clear();
			for (auto itr : objects)
				delete itr;
			objects.clear();
			// background
			level.load_sprite_image("assets/images/Levels/forest_3.png")
				.set_scale(1280, 640)
				.set_position(0, 0);

			// player
			player = new Player({ 70, 444 + 32 });
			player->sprite.set_flipped(false);
			// enemies
			enemies.push_back(new Enemy({ 600, 128 + 25 }, 100, 10));
			
			// walls
			vector<pair<vec3, vec3>> wall = {
				{ { -10, 0 },{ 0,640 } },
				{ { 1280, 0 },{ 1300, 640 } },
				{ { 0, 128 },{ 192, 444 } },
				{ { 192, 0 },{ 384, 128 } },
				{ { 512, 0 },{ 640, 128 } },
				{ { 768, 0 },{ 896, 128 } },
				{ { 960, 0 },{ 1024, 128 } },
				{ { 1088, 0 },{ 1216, 128 } },
				{ { 1216, 128 },{ 1280, 192 } },
			};
			for (auto itr : wall)
				objects.push_back(new Wall(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));
			// platforms
			vector<pair<vec3, vec3>> plat = {
				{ { 0, 446 },{ 192, 448 } },
				{ { 192, 126 },{ 384, 128 } },
				{ { 512, 126 },{ 640, 128 } },
				{ { 768, 126 },{ 896, 128 } },
				{ { 960, 126 },{ 1024, 128 } },
				{ { 1068, 126 },{ 1216, 128 } },
				{ { 1216, 190 },{ 1280, 192 } },
			};
			for (auto itr : plat)
				objects.push_back(new Platform(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));

			// IMPORTANT: This is how spritelib 2.0 handles the main game loop
			while (theGame.update(15) && gameState == FOREST3) {
				Update();
				
				DrawSprites();
				//DrawCollision();
				
				// next
				if (player->sprite.get_position().x >= 1240 && player->sprite.get_position().x <= 1280 &&
					player->sprite.get_position().y >= 190 && player->sprite.get_position().y <= 420)
				{
					gameState = FOREST4;
				}
			}
		}
		else if (gameState == FOREST4) {
			running = true;
			// delete
			if (player != nullptr)
				delete player;
			for (auto itr : enemies)
				delete itr;
			enemies.clear();
			for (auto itr : objects)
				delete itr;
			objects.clear();
			// background
			level.load_sprite_image("assets/images/Levels/forest_4.png")
				.set_scale(1280, 640)
				.set_position(0, 0);

			// player
			player = new Player({ 70, 256 + 32 });
			player->sprite.set_flipped(false);
			// enemies
			enemies.push_back(new Enemy({ 400, 64 + 25 }, 150, 10));
			enemies.push_back(new Enemy({ 1050, 64 + 25 }, 150, 10));

			// walls
			vector<pair<vec3, vec3>> wall = {
				{ { -10, 0 },{ 0,640 } },
				{ { 1280, 0 },{ 1300, 640 } },
				{ { 128, 64 },{ 320, 320 } },
				{ { 256, 192 },{ 448, 384 } },
				{ { 320, 384 },{ 384, 448 } },
				{ { 384, 256 },{ 640, 512 } },
				{ { 1088, 64 },{ 1152, 128 } },
				{ { 1152, 128 },{ 1216, 192 } },
				{ { 1216, 192 },{ 1280, 256 } },
			};
			for (auto itr : wall)
				objects.push_back(new Wall(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));
			// platforms
			vector<pair<vec3, vec3>> plat = {
				{ { 0, 254 },{ 128, 256 } },
				{ { 128, 318 },{ 256, 320 } },
				{ { 256, 382 },{ 320, 384 } },
				{ { 320, 446 },{ 384, 448 } },
				{ { 384, 510 },{ 640, 512 } },
				{ { 320, 62 },{ 1152, 64 } },
				{ { 1088, 126 },{ 1152, 128 } },
				{ { 1152, 190 },{ 1216, 192 } },
				{ { 1216, 254 },{ 1280, 256 } },
			};
			for (auto itr : plat)
				objects.push_back(new Platform(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));

			// IMPORTANT: This is how spritelib 2.0 handles the main game loop
			while (theGame.update(15) && gameState == FOREST4) {
				Update();

				DrawSprites();
				//DrawCollision();

				// next
				if (player->sprite.get_position().x >= 1240 && player->sprite.get_position().x <= 1280 &&
					player->sprite.get_position().y >= 256 && player->sprite.get_position().y <= 420)
				{
					gameState = FOREST5;
				}
			}
		}
		else if (gameState == FOREST5) {
			running = true;
			// delete
			if (player != nullptr)
				delete player;
			for (auto itr : enemies)
				delete itr;
			enemies.clear();
			for (auto itr : objects)
				delete itr;
			objects.clear();
			// background
			level.load_sprite_image("assets/images/Levels/forest_5.png")
				.set_scale(1280, 640)
				.set_position(0, 0);

			// player
			player = new Player({ 70, 256 + 32 });
			player->sprite.set_flipped(false);
			// enemies
			enemies.push_back(new Enemy({ 600, 128 + 25 }, 200, 20));
			enemies.push_back(new Enemy({ 1000, 64 + 25 }, 250, 20));
			
			// walls
			vector<pair<vec3, vec3>> wall = {
				{ { -10, 0 },{ 0,640 } },
				{ { 1280, 0 },{ 1300, 640 } },
				{ { 0, 128 },{ 192, 256 } },
				{ { 640, 0 },{ 704, 192 } },
				{ { 768, 0 },{ 832, 192 } },
				{ { 1088, 64 },{ 1152, 128 } },
				{ { 1152, 128 },{ 1216, 192 } },
				{ { 1216, 192 },{ 1280, 256 } },
			};
			for (auto itr : wall)
				objects.push_back(new Wall(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));
			// platforms
			vector<pair<vec3, vec3>> plat = {
				{ { 0, 254 },{ 192, 256 } },
				{ { 192, 126 },{ 640, 128 } },
				{ { 640, 190 },{ 704, 192 } },
				{ { 768, 190 },{ 832, 192 } },
				{ { 832, 62 },{ 1088, 64 } },
				{ { 1088, 126 },{ 1152, 128 } },
				{ { 1152, 190 },{ 1216, 192 } },
				{ { 1216, 254 },{ 1280, 256 } },
			};
			for (auto itr : plat)
				objects.push_back(new Platform(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));

			// IMPORTANT: This is how spritelib 2.0 handles the main game loop
			while (theGame.update(15) && gameState == FOREST5) {
				Update();

				DrawSprites();
				//DrawCollision();

				// next
				if (player->sprite.get_position().x >= 1240 && player->sprite.get_position().x <= 1280 &&
					player->sprite.get_position().y >= 256 && player->sprite.get_position().y <= 420)
				{
					gameState = BOSS;
				}
			}
		}
		else if (gameState == BOSS) {
			running = true;
			// delete
			if (player != nullptr)
				delete player;
			for (auto itr : enemies)
				delete itr;
			enemies.clear();
			for (auto itr : objects)
				delete itr;
			objects.clear();
			// background
			level.load_sprite_image("assets/images/Levels/forest_arena.png")
				.set_scale(1280, 640)
				.set_position(0, 0);

			// player
			player = new Player({ 70, 256 + 32 });
			player->sprite.set_flipped(false);
			// enemies
			enemies.push_back(new Enemy({ 400, 64 + 25 }, 300, 25));
			
			// walls
			vector<pair<vec3, vec3>> wall = {
				{ { -10, 0 },{ 0,640 } },
				{ { 1280, 0 },{ 1300, 640 } },
				{ { 0, 0 },{ 128, 256 } },
				{ { 192, 0 },{ 256, 64 } },
			};
			for (auto itr : wall)
				objects.push_back(new Wall(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));
			// platforms
			vector<pair<vec3, vec3>> plat = {
				{ { 0, 254 },{ 128, 256 } },
				{ { 192, 62 },{ 1280, 64 } },
			};
			for (auto itr : plat)
				objects.push_back(new Platform(itr.second.subtract(itr.first).divide(2).add(itr.first), itr.second.subtract(itr.first)));

			// IMPORTANT: This is how spritelib 2.0 handles the main game loop
			while (theGame.update(15) && gameState == BOSS) {
				Update();

				DrawSprites();
				//DrawCollision();
				
				if (player->sprite.get_position().x >= 1240 && player->sprite.get_position().x <= 1280 &&
					player->sprite.get_position().y >= 64 && player->sprite.get_position().y <= 200)
				{
					gameState = WINSCENE;
				}
			}
		}
		else if (gameState == WINSCENE) {
			theGame.set_ortho_matrix(0, 0, 1280, 640)
				.show_mouse(false);
			running = false;
			bool played = false;
			time_t start = time(0);

			cutscene.load_sprite_image("assets/images/Story/wolfheim_end.png")
				.set_scale(1280, 640)
				.set_position(0, 0);

			while (theGame.update(30) && gameState == WINSCENE) {
				double deltatime = difftime(time(0), start);
				if (deltatime > 5)	gameState = INTRO;

				cutscene.draw();
			}
			// end
			music.stop();
			if (player != nullptr)
				delete player;
			for (auto itr : enemies)
				delete itr;
			enemies.clear();
			for (auto itr : objects)
				delete itr;
			objects.clear();
		}
	}

	// End the program
	music.stop();
	// delete enemies
	for (auto itr : enemies)
		delete itr;
	enemies.clear();
	// delete objects
	for (auto itr : objects)
		delete itr;
	objects.clear();
	//system("pause");			// keep the console window open for debugging purposes
	return 0;
}