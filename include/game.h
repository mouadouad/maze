#include <SDL3/SDL.h>
#include <string>
#include <memory>
#include <vector>

#include "edge.h"
#include "path.h"
#include "circle.h"

class Game {

public:
	static const int kScreenWidth {650}, kScreenHeight {900};
	Game();
	~Game();
	bool init(SDL_Renderer* renderer);
	void destroy();
	bool loadLevel(const std::string path);
	void startGame();
	void pauseGame();
	void update();
	void render();
	void setCursor(float x, float y);

private:
	void setPathLimits(Path& path);

	std::vector<Edge> edges;
	std::vector<Path> paths;
	int row_size = 0;
	int edge_width, edge_height;
	int cursorX, cursorY, lastCursorX, lastCursorY;
	int last_validX, last_validY;
	SDL_Texture *edgeTexture, *pathTexture;
	Circle* circle;
	SDL_Renderer* renderer;
	
};
