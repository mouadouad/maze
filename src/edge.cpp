#include "edge.h"

Edge::Edge() : x{0}, y{0}, w{0}, h{0}, d{0}{}

Edge::~Edge(){}

void Edge::render(SDL_Renderer* renderer, SDL_Texture* texture){
	SDL_FRect rect{static_cast<float>(x), static_cast<float>(y)
		, static_cast<float>(w), static_cast<float>(h)};
	if(!d){
		rect.w = static_cast<float>(h);
		rect.h = static_cast<float>(w);
	}
	SDL_RenderTexture(renderer, texture, nullptr, &rect);
}

void Edge::setPosition(int _x, int _y){
	x = _x;
	y = _y;
}

void Edge::setDimensions(int _w, int _h){
	w = _w;
	h = _h;
}

void Edge::setDirection(size_t _d){
	d = _d;
}

int Edge::getX(){
	return x;
}

int Edge::getY(){
	return y;
}

int Edge::getW(){
	if(d) return w;
	return h;
}

int Edge::getH(){
	if(d) return h;
	return w;
}

