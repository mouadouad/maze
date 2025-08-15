#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "json.hpp"
#include "game.h"

SDL_Texture* createTexture(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b);
bool checkIntersect(Path& path, Edge& edge);

Game::Game(): renderer{nullptr}, 
		edgeTexture{nullptr}, pathTexture{nullptr},
		edge_width{20}, edge_height{50} {}


Game::~Game(){
	delete circle;
}

bool Game::init(SDL_Renderer* _renderer) {
	renderer = _renderer;
	circle = new Circle(renderer, 70);
	edgeTexture = createTexture(renderer, 100, 100, 240);
	pathTexture = createTexture(renderer, 100, 240, 180);
	return true;
}

void Game::destroy() {
	SDL_DestroyTexture(edgeTexture);
	SDL_DestroyTexture(pathTexture);
	edgeTexture = nullptr;
	pathTexture = nullptr;
}

bool Game::loadLevel(const std::string path) {
	std::ifstream level_file(path);
	nlohmann::json level;
	level_file >> level;

	auto json_edges = level["edges"];
	row_size = json_edges[0].size();
	
	edge_height = kScreenWidth / row_size;

	size_t index = 0;
	for (const auto& row : json_edges) {
	    for (const auto& element : row) {
		if(element > 0){
			edges.emplace_back();
			Edge& last_edge = edges.back();
			last_edge.setDimensions(edge_width, edge_height+edge_width);
			last_edge.setPosition((index % row_size) * edge_height,
						(index / (row_size*2)) * edge_height);
			last_edge.setDirection((index / row_size) % 2);
		}
		index++;
	    }
	}

	int startingX = level["startingX"];
	int startingY = level["startingY"];
	last_validX = startingX * edge_height;
	last_validY = (startingY - 1) * edge_height / 2 + edge_height /2;
	cursorX = last_validX;
	cursorY = last_validY;
	
	paths.emplace_back();
	auto& last_path = paths.back();
	last_path.setPosition(last_validX, last_validY);
	last_path.setDirection(0);
	setPathLimits(last_path);
	return true;
}

void Game::startGame(){}

void Game::pauseGame() {
	
}

void Game::update() {
	
	if(paths.size() == 0) return;

	if(abs(cursorX - last_validX) > 150 || abs(cursorY - last_validY) > 150){
		for(size_t i = 0; i < paths.size() - 1 ; i++){
			paths.pop_back();
		}
		return;
	}

	auto& last_path = paths.back();
	last_path.update(cursorX, cursorY);

	if(abs(last_path.getH()) < edge_width || abs(last_path.getW()) < edge_width){
		if(paths.size() > 1){
			paths.pop_back();
		}

	}else if(!last_path.getD() && abs(last_path.getY() - cursorY) > 30 &&
		(abs(last_path.getH()) > 30 || abs(last_path.getW()) > 30)){

		int x = last_path.getX() + last_path.getW();
		if(last_path.getW() > 0){
			x -= edge_width;
		}
		int y = last_path.getY();

		paths.emplace_back();
		auto& new_path = paths.back();
		new_path.setPosition(x, y);
		new_path.setDirection(1);
		setPathLimits(new_path);

	} else if(last_path.getD() && abs(last_path.getX() - cursorX) > 30 &&
		(abs(last_path.getH()) > 30 || abs(last_path.getW()) > 30)){

		int x = last_path.getX();
		int y = last_path.getY() + last_path.getH();
		if(last_path.getH() > 0){
			y -= edge_width;
		}

		paths.emplace_back();
		auto& new_path = paths.back();
		new_path.setPosition(x, y);
		new_path.setDirection(0);
		setPathLimits(new_path);
	}
	
	auto& path = paths.back();
	last_validX = path.getX() + path.getW();
	last_validY = path.getY() + path.getH();

}

void Game::render() {
	for(auto& edge : edges){
		edge.render(renderer, edgeTexture);
	}
	for(auto& path : paths){
		path.render(renderer, pathTexture);
	}

	circle->render(cursorX, cursorY);
}

void Game::setCursor(float x, float y){
	cursorX = static_cast<int>(x);
	cursorY = static_cast<int>(y);

}

void Game::setPathLimits(Path& path){
	int minh{0}, maxh{0};
	for(maxh; maxh < row_size * edge_height; maxh++){
		path.setDimensions(edge_width, maxh);
		bool intersect {false};
		for(auto& edge : edges){
			if(checkIntersect(path, edge)){
				intersect = true;
				break;
			}
		}

		if(intersect) break;
	}
	for(minh; minh > -row_size * edge_height; minh--){
		path.setDimensions(edge_width, minh);
		bool intersect {false};
		for(auto& edge : edges){
			if(checkIntersect(path, edge)){
				intersect = true;
				break;
			}
		}

		if(intersect) break;
	}

	path.setDimensions(edge_width, 6);
	path.setMinH(minh + edge_height/2 - edge_width);
	path.setMaxH(maxh - edge_height/2 + edge_width);

}

bool checkIntersect(Path& path, Edge& edge){

	int aMinX{ std::min(path.getX() + path.getW(), path.getX()) };
	int aMaxX{ std::max(path.getX() + path.getW(), path.getX()) }; 
	int aMinY{ std::min(path.getY() + path.getH(), path.getY()) };
	int aMaxY{ std::max(path.getY() + path.getH(), path.getY()) }; 

	int bMinX{ std::min(edge.getX() + edge.getW(), edge.getX()) };
	int bMaxX{ std::max(edge.getX() + edge.getW(), edge.getX()) }; 
	int bMinY{ std::min(edge.getY() + edge.getH(), edge.getY()) };
	int bMaxY{ std::max(edge.getY() + edge.getH(), edge.getY()) }; 

	if( aMinX >= bMaxX || aMaxX <= bMinX)
	{
		return false;
	}

	if( aMinY >= bMaxY || aMaxY <= bMinY)
	{
		return false;
	}

	return true;

}

SDL_Texture* createTexture(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b){
	
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
					SDL_TEXTUREACCESS_TARGET, Game::kScreenWidth, Game::kScreenHeight);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	
	return texture;
}










