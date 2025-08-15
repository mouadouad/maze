#include "path.h"

Path::Path() : x{0}, y{0}, w{0}, h{0}, 
	d{0}, minH{0}, maxH{0} {}

Path::~Path(){}

void Path::render(SDL_Renderer* renderer, SDL_Texture* texture){
	SDL_FRect rect{static_cast<float>(x), static_cast<float>(y)
		, static_cast<float>(w), static_cast<float>(h)};
	if(!d){
		rect.w = static_cast<float>(h);
		rect.h = static_cast<float>(w);
	}
	SDL_RenderTexture(renderer, texture, nullptr, &rect);
}

void Path::setPosition(int _x, int _y){
	x = _x;
	y = _y;
}

void Path::setDimensions(int _w, int _h){
	w = _w;
	h = _h;
}

void Path::setDirection(size_t _d){
	d = _d;
}

void Path::update(int cursorX, int cursorY){
	h = d * (cursorY - y) + (1 - d) * (cursorX - x);
	if(h < minH) h = minH;
	if(h > maxH) h = maxH;
}

void Path::setMinH(int _h){
	minH = _h;
}

void Path::setMaxH(int _h){
	maxH = _h;
}

int Path::getX(){
	return x;
}

int Path::getY(){
	return y;
}

int Path::getW(){
	if(d) return w;
	return h;
}

int Path::getH(){
	if(d) return h;
	return w;
}

size_t Path::getD(){
	return d;
}
