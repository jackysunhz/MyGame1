#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//some weird background animation:
	float background_fade = 0.0f;

	//player position:
	glm::vec2 player_at = glm::vec2(8,8);

	//boss position:
	glm::vec2 boss_at = glm::vec2(PPU466::ScreenWidth/2 - 16, PPU466::ScreenHeight - 64);

	//fireball position:
	std::array<glm::vec2, 4> fireballs_at = {glm::vec2(0, 255),glm::vec2(0, 255),glm::vec2(0, 255),glm::vec2(0, 255)};

	//----- drawing handled by PPU466 -----

	PPU466 ppu;


	PPU466::Tile spiderTile;
	std::array<PPU466::Tile, 16> demonTiles;
	PPU466::Tile fireballTile;
	PPU466::Tile playeraliveTile;
	PPU466::Tile playerdeadTile;

	bool gameEnd = false;

	//static float totalTime;
};
