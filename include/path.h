#include <SDL3/SDL.h>

class Path {

public:
	Path();
	~Path();
	void render(SDL_Renderer* renderer, SDL_Texture* texture);
	void setPosition(int x, int y);
	void setDimensions(int w, int h);
	void setDirection(size_t d);
	void update(int cursorX, int cursorY);

	void setMinH(int h);
	void setMaxH(int h);

	int getX();
	int getY();
	int getH();
	int getW();
	size_t getD();

private:
	int x, y, w, h;
	int minH, maxH;
	size_t d;
};
