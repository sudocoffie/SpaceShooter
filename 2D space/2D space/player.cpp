#include "player.h"
#include "game.h"
#include "BulletHandler.h"
#include <iostream>

Player * Player::sm_instance = new Player();

Player::Player() :
	_posx(100),
	_posy(500),
	_pos(_posx, _posy),
	_lives(175),
	_powerup(false){
	_speed = 6;
	keyStates = SDL_GetKeyboardState(0);

	_timer = new Timer(400);
}

Player::~Player() {
}

Player * Player::instance() {
	return sm_instance;
}

void Player::setPosY(int posy) {
	_pos.y = posy;
}

void Player::setPosX(int posx) {
	_pos.x = posx;
}

void Player::setWidth(int width) {
	_width = width;
}

void Player::setHeight(int height) {
	_height = height;
}

void Player::setHealth(int hp) {
	_lives -= hp;
	
}

void Player::setTexture() {
	auto surface = IMG_Load("images/player.png");

	_texture = SDL_CreateTextureFromSurface(Game::instance()->getRenderer(), surface);

	if (!_texture) {
		printf("Failed to create texture");
	}
	SDL_FreeSurface(surface);
}

void Player::setPowerup(bool condision) {
	_powerup = condision;
	_timer->start();
}

void Player::givePoints(int points) {
	_points += points;
}

Vector2D Player::getPos() const {
	return _pos;
}

void Player::movement( int speed, int dir ) {
	//Kollar efter vilken dir som ska k�ras och anv�nder hastigheten f�r att r�ra den
	switch (dir) {
		case 1: // V�nster
			_pos.x = _pos.x + speed;
			break;
		case 2: // H�ger
			_pos.y = _pos.y + speed;
			break;
		default:
			break;
	}
}

void Player::update() {
	
	if (_powerup) {
		if (!_timer->isOver()) {
			_timer->isRunning();
		} else {
			_powerup = false;
			_timer->stop();
		}
	}

	// Timer
	if (_timerunning == true) {
		// Om klassen retunerar true s� st�lls 
		//_timerunning till false och spelaren kan skjuta igen
		if (timer()) {
			_timerunning = false;
			_time = 130;
		} else {
			_timerunning = true;
		}
	}


	// Movement and fireing 
	if (_left) {
		if(_pos.x > 0)
			movement(-_speed, 1);
	} 
	if (_right) {
		if(Game::instance()->getWindowWidth() - _width > _pos.x)
			movement(_speed, 1);
	}
	if (_up) {
		if (0 < _pos.y)
			movement(-_speed, 2);
	}
	if (_down) {
		if(Game::instance()->getWindowHeight() > _pos.y + _height)
			movement(_speed, 2);
	}
	if (_fireing) {
		if (_timerunning == false) {
			if (_powerup) {
				BulletHandler::instance()->addBullet(new Bullet(_pos, true, false));  // G�r h�ger
				BulletHandler::instance()->addBullet(new Bullet(_pos, false, false)); // G�r rakt
				BulletHandler::instance()->addBullet(new Bullet(_pos, false, true));  // G�r v�nster
			} else {
				BulletHandler::instance()->addBullet(new Bullet(_pos, false, false)); // G�r rakt
			}
			
			_timerunning = true;
		}
	}

	if (_lives <= 0) {
		GameState::instance()->setGameState("Gameover");
	}
}

//Ritar ut spelarens rect (Ska vara en sprite senare)
void Player::draw() const {

	// Skapar en ny rect
	SDL_Rect rect;

	// S�tter storleken p� recten och positionen
	rect.w = _width;
	rect.h = _height;
	rect.x = _pos.x;
	rect.y = _pos.y;
	if (_texture) {
		SDL_RenderCopy(Game::instance()->getRenderer(), _texture, nullptr, &rect);
	}
	else {
		SDL_SetRenderDrawColor(Game::instance()->getRenderer(), 255, 255, 200, 10); // S�tter f�rgen f�r recten
		SDL_RenderFillRect(Game::instance()->getRenderer(), &rect);				  // Applicerar f�rgen till rectanglen

	}
	
}

void Player::eventHandler(SDL_Event &event) {


	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		keyStates = SDL_GetKeyboardState(0);
	}

	if (keyStates != 0)
	{
		if (keyStates[SDL_SCANCODE_SPACE] == 1)
		{
			_fireing = true;
		} else {
			_fireing = false;
		}
		if (keyStates[SDL_SCANCODE_RIGHT] == 1 || keyStates[SDL_SCANCODE_D] == 1)
		{
			_right = true;
		} else {
			_right = false;
		}

		if (keyStates[SDL_SCANCODE_LEFT] == 1 || keyStates[SDL_SCANCODE_A] == 1)
		{
			_left = true;
		}else {
			_left = false;
		}

		if (keyStates[SDL_SCANCODE_UP] == 1 || keyStates[SDL_SCANCODE_W] == 1)
		{
			_up = true;
		}
		else {
			_up = false;
		}

		if (keyStates[SDL_SCANCODE_DOWN] == 1 || keyStates[SDL_SCANCODE_S] == 1)
		{
			_down = true;
		}
		else {
			_down = false;
		}

	}
}

bool Player::timer() {
	// R�knar ner 
	_time -= 10;
	
	// Kollar om time �r mindre �n noll
	// Om den �r de retuneras true
	if (_time <= 0) {
		return true;
	} else {
		return false;
	}
	
}

int Player::getPowerUpTime() const {
	if (_powerup)
		return _timer->getTime();
	else
		return 0;
}