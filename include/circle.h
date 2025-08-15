#include <SDL3/SDL.h>
#include <cmath>

class Circle {

private:
	SDL_Texture* maskTexture;
	SDL_Renderer* renderer;
	int radius;

public:
	Circle(SDL_Renderer* _renderer, int r) : renderer{_renderer}, radius{r}{
	
		maskTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
						SDL_TEXTUREACCESS_TARGET,
						1000, 1000);
	}

	~Circle(){
		if(maskTexture){
			SDL_DestroyTexture(maskTexture);
		}
		renderer = nullptr;
	}

	void render(int cx, int cy){
	
		SDL_SetRenderTarget(renderer, maskTexture);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		drawFilledCircle(cx, cy);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_SetTextureBlendMode(maskTexture, SDL_BLENDMODE_MOD);

		SDL_FRect rect{0, 0, 1000, 1000};
		SDL_RenderTexture(renderer, maskTexture, nullptr, &rect);

	}

	void drawFilledCircle(int cx, int cy){
		for (int y = -radius; y <= radius; y++) {
		    for (int x = -radius; x <= radius; x++) {
			// Distance squared from center
			int distanceSquared = x * x + y * y;
			
			// If point is inside circle
			if (distanceSquared <= radius * radius) {
			    SDL_RenderPoint(renderer, cx + x, cy + y);
			}
		    }
		}
	}

};
