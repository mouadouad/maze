#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string> 
#include <sstream>

#include "game.h"
#include "timer.h"

constexpr int kScreenFps{ 60 };
static const std::string basePath = "levels/level-";
bool init();
bool loadMedia();
void close();

SDL_Window* gWindow{nullptr};
SDL_Renderer* gRenderer{nullptr};
Game game;

bool init() {
	bool success{true};

	if(!SDL_Init(SDL_INIT_VIDEO)){
		success = false;
		SDL_Log("SDL couldn't initialize %s\n", SDL_GetError());
	}else {

		if(!SDL_CreateWindowAndRenderer("Mirror Mirror", Game::kScreenWidth, Game::kScreenHeight, 0, &gWindow, &gRenderer)){
			success = false;
			SDL_Log("SDL couldn't create window %s\n", SDL_GetError());
		}	
	}
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderVSync(gRenderer, 1);
	if(!game.init(gRenderer)){
		SDL_Log("SDL couldn't initialize game %s\n", SDL_GetError());

	}

	return success;
}

bool loadMedia(){
	std::ostringstream path;
	int level {1};
	path << basePath << level << ".json";
	if(!game.loadLevel(path.str())){
		SDL_Log("SDL couldn't load level %s\n", SDL_GetError());
		return false;
	}
	return true;
}

void close(){
	game.destroy();
	
	SDL_DestroyRenderer(gRenderer);
	gRenderer= nullptr;

	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	
	SDL_Quit();

}
int main(){
	if(!init()){
		close();
		return 1;
	}
	if(!loadMedia()){
		close();
		return 2;
	}
	bool quit{false};
	
	SDL_Event e;
	SDL_zero(e);
	
	LTimer capTimer;
	while (!quit){
		capTimer.start();

		while (SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				quit = true;
			}else if (e.type == SDL_EVENT_KEY_DOWN){
				switch( e.key.key )
				{
					case SDLK_RETURN:
						game.startGame();
						break;
					case SDLK_SPACE:
						game.pauseGame();
						break;
				}
			}else if(e.type == SDL_EVENT_MOUSE_MOTION){
				float x=-1.0f, y=-1.0f;
				SDL_GetMouseState(&x, &y);
				game.setCursor(x, y);
			}
		}
 		SDL_SetRenderDrawColor(gRenderer, 39, 42, 46, 0xFF);
		SDL_RenderClear(gRenderer);

		game.update();
		game.render();
		
		SDL_RenderPresent(gRenderer);
		constexpr Uint64 nsPerFrame = 1000000000 / kScreenFps;
                Uint64 frameNs = capTimer.getTicksNS();
                if( frameNs < nsPerFrame )
                {
                    SDL_DelayNS( nsPerFrame - frameNs );
                }
	}

	close();
	return 0;

}

















