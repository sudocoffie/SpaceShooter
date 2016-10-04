#ifndef INCLUDED_PLAYER
#define INCLUDED_PLAYER

#include "GameObject.h"
#include "SDL.h"
#include "Vector2D.h"

class Player : public GameObject {
public:

	static Player * instance();

	Player();
	virtual ~Player();
	virtual void draw() const;
	virtual void update();
	virtual Vector2D getPos() const;
	void movement(int speed, int dir);
	void eventHandler(SDL_Event &event);
	void setPosY(int posy);
	void setPosX(int posx);
	void setWidth(int width);
	void setHeight(int height);
	inline int getWidth() const { return _width; }
	inline int getHeight() const { return _height; }
	


private:
	int _width = 25;
	int _height = 25;
	int _dir = 1;
	int _speed = 5;

	Vector2D _pos;

	float _posx;
	float _posy;

	static Player * sm_instance;
};

#endif // !INCLUDED_PLAYER