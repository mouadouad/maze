#include <SDL3/SDL.h>

class Edge {

public:
	Edge();
	~Edge();
	void render(SDL_Renderer* renderer, SDL_Texture* texture);
	void setPosition(int x, int y);
	void setDimensions(int w, int h);
	void setDirection(size_t d);
	int getX();
	int getY();
	int getH();
	int getW();

private:
	int x, y, w, h;
	size_t d;
};
