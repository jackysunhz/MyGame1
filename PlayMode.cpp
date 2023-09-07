#include "PlayMode.hpp"
#include "load_save_png.hpp"
#include <bitset>
//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>

PlayMode::PlayMode() {
	//TODO:
	// you *must* use an asset pipeline of some sort to generate tiles.
	// don't hardcode them like this!
	// or, at least, if you do hardcode them like this,
	//  make yourself a script that spits out the code that you paste in here
	//   and check that script into your repository.

	//Also, *don't* use these tiles in your game:

	{ //use tiles 0-16 as some weird dot pattern thing:
		std::array< uint8_t, 8*8 > distance;
		for (uint32_t y = 0; y < 8; ++y) {
			for (uint32_t x = 0; x < 8; ++x) {
				float d = glm::length(glm::vec2((x + 0.5f) - 4.0f, (y + 0.5f) - 4.0f));
				d /= glm::length(glm::vec2(4.0f, 4.0f));
				distance[x+8*y] = uint8_t(std::max(0,std::min(255,int32_t( 255.0f * d ))));
			}
		}
		for (uint32_t index = 0; index < 16; ++index) {
			PPU466::Tile tile;
			uint8_t t = uint8_t((255 * index) / 16);
			for (uint32_t y = 0; y < 8; ++y) {
				uint8_t bit0 = 0;
				uint8_t bit1 = 0;
				for (uint32_t x = 0; x < 8; ++x) {
					uint8_t d = distance[x+8*y];
					if (d > t) {
						bit0 |= (1 << x);
					} else {
						bit1 |= (1 << x);
					}
				}
				tile.bit0[y] = bit0;
				tile.bit1[y] = bit1;
			}
			ppu.tile_table[index] = tile;
		}

		
		
	}


	///Load PNGs into vector of colors

	//load spider tile
	glm::uvec2 spider_size(8,8);
	std::vector< glm::u8vec4> spider_tile_vec;
	load_png("../Assets/spider.png", &spider_size, &spider_tile_vec, LowerLeftOrigin);
	//load spider pallete
	glm::uvec2 spider_pallete_size(2,2);
	std::vector< glm::u8vec4> spider_pal_vec;
	load_png("../Assets/spider_pal.png", &spider_pallete_size, &spider_pal_vec, LowerLeftOrigin);

	//load demon tiles
	glm::uvec2 demon_size(32,32);
	std::vector< glm::u8vec4> demon_tile_vec;
	load_png("../Assets/demon.png", &demon_size, &demon_tile_vec, LowerLeftOrigin);
	//load demon pallete
	glm::uvec2 demon_pallete_size(2,2);
	std::vector< glm::u8vec4> demon_pal_vec;
	load_png("../Assets/demon_pal.png", &demon_pallete_size, &demon_pal_vec, LowerLeftOrigin);

	//load fireball tiles
	glm::uvec2 fireball_size(8,8);
	std::vector< glm::u8vec4> fireball_tile_vec;
	load_png("../Assets/fireball.png", &fireball_size, &fireball_tile_vec, LowerLeftOrigin);
	//load fireball pallete
	glm::uvec2 fireball_pallete_size(2,2);
	std::vector< glm::u8vec4> fireball_pal_vec;
	load_png("../Assets/fireball_pal.png", &fireball_pallete_size, &fireball_pal_vec, LowerLeftOrigin);

	//load player tiles
	glm::uvec2 player_size(8,8);
	std::vector< glm::u8vec4> playeralive_tile_vec;
	load_png("../Assets/playeralive.png", &player_size, &playeralive_tile_vec, LowerLeftOrigin);
	std::vector< glm::u8vec4> playerdead_tile_vec;
	load_png("../Assets/playerdead.png", &player_size, &playerdead_tile_vec, LowerLeftOrigin);
	//load player pallete
	glm::uvec2 player_pallete_size(2,2);
	std::vector< glm::u8vec4> player_pal_vec;
	load_png("../Assets/player_pal.png", &player_pallete_size, &player_pal_vec, LowerLeftOrigin);

	// std::cout<<(int)demon_pal_vec[0].r << " "<< (int)demon_pal_vec[0].g << " "<< (int)demon_pal_vec[0].b << " "<< (int)demon_pal_vec[0].a << " "<< std::endl;
	// std::cout<<(int)demon_pal_vec[1].r << " "<< (int)demon_pal_vec[1].g << " "<< (int)demon_pal_vec[1].b << " "<< (int)demon_pal_vec[1].a << " "<< std::endl;
	// std::cout<<(int)demon_pal_vec[2].r << " "<< (int)demon_pal_vec[2].g << " "<< (int)demon_pal_vec[2].b << " "<< (int)demon_pal_vec[2].a << " "<< std::endl;
	// std::cout<<(int)demon_pal_vec[3].r << " "<< (int)demon_pal_vec[3].g << " "<< (int)demon_pal_vec[3].b << " "<< (int)demon_pal_vec[3].a << " "<< std::endl;
	// std::cout << demon_tile_vec.size() << std::endl;

	///Convert PNGs to tiles and palletes

	//convert spider tile
	for(int row = 0; row < 8; row ++){
		for(int col = 0; col < 8; col ++){
			int vec_idx = row * 8 + col;
			if(spider_tile_vec[vec_idx] == spider_pal_vec[0]){
				//bit idx is 00
				spiderTile.bit0[row] &= ~(1 << col);
				spiderTile.bit1[row] &= ~(1 << col);
			}
			else if(spider_tile_vec[vec_idx] == spider_pal_vec[1]){
				//bit idx is 01
				spiderTile.bit0[row] &= ~(1 << col);
				spiderTile.bit1[row] |= (1 << col);
			}
			else if(spider_tile_vec[vec_idx] == spider_pal_vec[2]){
				//bit idx is 10
				spiderTile.bit0[row] |= (1 << col);
				spiderTile.bit1[row] &= ~(1 << col);
			}
			else if(spider_tile_vec[vec_idx] == spider_pal_vec[3]){
				//bit idx is 11
				spiderTile.bit0[row] |= (1 << col);
				spiderTile.bit1[row] |= (1 << col);
			}
		}
	}

	//convert player tiles
	for(int row = 0; row < 8; row ++){
		for(int col = 0; col < 8; col ++){
			int vec_idx = row * 8 + col;
			if(playeralive_tile_vec[vec_idx] == player_pal_vec[0]){
				//bit idx is 00
				playeraliveTile.bit0[row] &= ~(1 << col);
				playeraliveTile.bit1[row] &= ~(1 << col);
			}
			else if(playeralive_tile_vec[vec_idx] == player_pal_vec[1]){
				//bit idx is 01
				playeraliveTile.bit0[row] &= ~(1 << col);
				playeraliveTile.bit1[row] |= (1 << col);
			}
			else if(playeralive_tile_vec[vec_idx] == player_pal_vec[2]){
				//bit idx is 10
				playeraliveTile.bit0[row] |= (1 << col);
				playeraliveTile.bit1[row] &= ~(1 << col);
			}
			else if(playeralive_tile_vec[vec_idx] == player_pal_vec[3]){
				//bit idx is 11
				playeraliveTile.bit0[row] |= (1 << col);
				playeraliveTile.bit1[row] |= (1 << col);
			}
		}
	}

	//convert player tiles
	for(int row = 0; row < 8; row ++){
		for(int col = 0; col < 8; col ++){
			int vec_idx = row * 8 + col;
			if(playerdead_tile_vec[vec_idx] == player_pal_vec[0]){
				//bit idx is 00
				playerdeadTile.bit0[row] &= ~(1 << col);
				playerdeadTile.bit1[row] &= ~(1 << col);
			}
			else if(playerdead_tile_vec[vec_idx] == player_pal_vec[1]){
				//bit idx is 01
				playerdeadTile.bit0[row] &= ~(1 << col);
				playerdeadTile.bit1[row] |= (1 << col);
			}
			else if(playerdead_tile_vec[vec_idx] == player_pal_vec[2]){
				//bit idx is 10
				playerdeadTile.bit0[row] |= (1 << col);
				playerdeadTile.bit1[row] &= ~(1 << col);
			}
			else if(playerdead_tile_vec[vec_idx] == player_pal_vec[3]){
				//bit idx is 11
				playerdeadTile.bit0[row] |= (1 << col);
				playerdeadTile.bit1[row] |= (1 << col);
			}
		}
	}
	

	//convert fireball tiles
	for(int row = 0; row < 8; row ++){
		for(int col = 0; col < 8; col ++){
			int vec_idx = row * 8 + col;
			if(fireball_tile_vec[vec_idx] == fireball_pal_vec[0]){
				//bit idx is 00
				fireballTile.bit0[row] &= ~(1 << col);
				fireballTile.bit1[row] &= ~(1 << col);
			}
			else if(fireball_tile_vec[vec_idx] == fireball_pal_vec[1]){
				//bit idx is 01
				fireballTile.bit0[row] &= ~(1 << col);
				fireballTile.bit1[row] |= (1 << col);
			}
			else if(fireball_tile_vec[vec_idx] == fireball_pal_vec[2]){
				//bit idx is 10
				fireballTile.bit0[row] |= (1 << col);
				fireballTile.bit1[row] &= ~(1 << col);
			}
			else if(fireball_tile_vec[vec_idx] == fireball_pal_vec[3]){
				//bit idx is 11
				fireballTile.bit0[row] |= (1 << col);
				fireballTile.bit1[row] |= (1 << col);
			}
		}
	}

	//convert demon tiles
	for(int tile_idx = 0; tile_idx < 16; tile_idx ++){
		for(int row = 0; row < 8; row ++){
			for(int col = 0; col < 8; col ++){
				int vec_row = tile_idx / 4 * 8 + row;
				int vec_col = tile_idx % 4 * 8 + col;
				int vec_idx = vec_row * 32 + vec_col;
				if(demon_tile_vec[vec_idx] == demon_pal_vec[0]){
					//bit idx is 00
					demonTiles[tile_idx].bit0[row] &= ~(1 << col);
					demonTiles[tile_idx].bit1[row] &= ~(1 << col);
				}
				else if(demon_tile_vec[vec_idx] == demon_pal_vec[1]){
					//bit idx is 01
					demonTiles[tile_idx].bit0[row] &= ~(1 << col);
					demonTiles[tile_idx].bit1[row] |= (1 << col);
				}
				else if(demon_tile_vec[vec_idx] == demon_pal_vec[2]){
					//bit idx is 10
					demonTiles[tile_idx].bit0[row] |= (1 << col);
					demonTiles[tile_idx].bit1[row] &= ~(1 << col);
				}
				else if(demon_tile_vec[vec_idx] == demon_pal_vec[3]){
					//bit idx is 11
					demonTiles[tile_idx].bit0[row] |= (1 << col);
					demonTiles[tile_idx].bit1[row] |= (1 << col);
				}
			}
		}
	}
	
	//use sprite 33 - 48 as a "demon":
	for(int i = 0; i < 16; i++){
		ppu.tile_table[33 + i] = demonTiles[i];
	}


	//use sprite 32 as a "player":
	ppu.tile_table[32] = playeraliveTile;

	//use sprite 49 as a "fireball":
	ppu.tile_table[49] = fireballTile;

	//makes the outside of tiles 0-16 solid:
	ppu.palette_table[0] = {
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
	};

	//makes the center of tiles 0-16 solid:
	ppu.palette_table[1] = {
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
	};

	//used for the player:
	ppu.palette_table[7] = {
		player_pal_vec[0],
		player_pal_vec[2],
		player_pal_vec[1],
		player_pal_vec[3]
	};

	//used for the demon:
	ppu.palette_table[2] = {
		demon_pal_vec[0],
		demon_pal_vec[2],
		demon_pal_vec[3],
		demon_pal_vec[1]
	};

	ppu.palette_table[3] = {
		glm::u8vec4(0x4f, 0x24, 0x45, 0xff),
		glm::u8vec4(0x4f, 0x24, 0x45, 0xff),
		glm::u8vec4(0x4f, 0x24, 0x45, 0xff),
		glm::u8vec4(0x4f, 0x24, 0x45, 0xff)
	};

	//used for the fireball:
	ppu.palette_table[4] = {
		fireball_pal_vec[0],
		fireball_pal_vec[2],
		fireball_pal_vec[1],
		fireball_pal_vec[3]
	};

	ppu.palette_table[5] = {
		glm::u8vec4(0x30, 0x30, 0x30, 0xff),
		glm::u8vec4(0x30, 0x30, 0x30, 0xff),
		glm::u8vec4(0x30, 0x30, 0x30, 0xff),
		glm::u8vec4(0x30, 0x30, 0x30, 0xff)
	};

	//used for the misc other sprites:
	ppu.palette_table[6] = {
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x88, 0x88, 0xff, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
	};

}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {

	if(gameEnd){
		return;
	}
	static float totalTime = 0;
	totalTime += elapsed;

	static float fireTimer = 0;
	fireTimer += elapsed;
	if (fireTimer >= 2)
	{
		//trigger fire event

		for(auto &fireball_at : fireballs_at){
			if(fireball_at.y > 250){
				fireball_at.x = boss_at.x;
				fireball_at.y = boss_at.y;
				break;
			}
		}
		fireTimer = 0;
	}
	
	//move fireballs
	for(auto &fireball_at : fireballs_at){
		if(fireball_at.y < 240)
		{
			fireball_at.y -= 65 * elapsed;
		}
		if(fireball_at.y <= -8){
			fireball_at.y = 255;
		}
		//std::cout << "fireball: " << (int)fireball_at.x << " " << (int)fireball_at.y << std::endl;
		//std::cout << "player: " << (int)player_at.x << " " << (int)player_at.y << std::endl;
		glm::vec2 d = fireball_at - player_at;
		if((d.x*d.x + d.y*d.y) < 64){
			gameEnd = true;
			ppu.tile_table[32] = playerdeadTile;
			std::cout << "Game Over" << std::endl;
		}
	}

	// std::cout << "fireball 1: " << (int)fireballs_at[0].x << " " << (int)fireballs_at[0].y << std::endl;
	// std::cout << "fireball 2: " << (int)fireballs_at[1].x << " " << (int)fireballs_at[1].y << std::endl;
	// std::cout << "fireball 3: " << (int)fireballs_at[2].x << " " << (int)fireballs_at[2].y << std::endl;
	// std::cout << "fireball 4: " << (int)fireballs_at[3].x << " " << (int)fireballs_at[3].y << std::endl;
	// std::cout<<std::endl;
	//slowly rotates through [0,1):
	// (will be used to set background color)
	//background_fade += elapsed / 10.0f;
	//background_fade -= std::floor(background_fade);

	boss_at.x = sin(totalTime/2) * 100 + PPU466::ScreenWidth/2 - 16;

	constexpr float PlayerSpeed = 50.0f;
	if (left.pressed) player_at.x -= PlayerSpeed * elapsed;
	if (right.pressed) player_at.x += PlayerSpeed * elapsed;
	if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	//background color will be some hsv-like fade:
	ppu.background_color = glm::u8vec4(
		0xff,
		0xff,
		0xff,
		0xff
	);

	//tilemap gets recomputed every frame as some weird plasma thing:
	//NOTE: don't do this in your game! actually make a map or something :-)
	for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
		for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
			//TODO: make weird plasma thing
			ppu.background[x+PPU466::BackgroundWidth*y] = 0b0000001100100000;
			if(x == 0 || x == PPU466::ScreenWidth/8 - 1 || y == 0 || y == PPU466::ScreenHeight/8 -1){
				ppu.background[x+PPU466::BackgroundWidth*y] = 0b0000010100100000;
			}

		}
	}

	//background scroll:
	//ppu.background_position.x = int32_t(-0.5f * player_at.x);
	//ppu.background_position.y = int32_t(-0.5f * player_at.y);

	//player sprite:
	ppu.sprites[0].x = int8_t(player_at.x);
	ppu.sprites[0].y = int8_t(player_at.y);
	ppu.sprites[0].index = 32;
	ppu.sprites[0].attributes = 7;

	//some other misc sprites:
	// for (uint32_t i = 1; i < 63; ++i) {
	// 	float amt = (i + 2.0f * background_fade) / 62.0f;
	// 	ppu.sprites[i].x = int8_t(0.5f * PPU466::ScreenWidth + std::cos( 2.0f * M_PI * amt * 5.0f + 0.01f * player_at.x) * 0.4f * PPU466::ScreenWidth);
	// 	ppu.sprites[i].y = int8_t(0.5f * PPU466::ScreenHeight + std::sin( 2.0f * M_PI * amt * 3.0f + 0.01f * player_at.y) * 0.4f * PPU466::ScreenWidth);
	// 	ppu.sprites[i].index = 32;
	// 	ppu.sprites[i].attributes = 6;
	// 	if (i % 2) ppu.sprites[i].attributes |= 0x80; //'behind' bit
	// }

	//demon sprite:
	for(int spriteidx = 0; spriteidx < 16; spriteidx ++){
		ppu.sprites[spriteidx + 1].x = (uint8_t)(boss_at.x + spriteidx % 4 * 8);
		ppu.sprites[spriteidx + 1].y = (uint8_t)(boss_at.y + spriteidx / 4 * 8);
		ppu.sprites[spriteidx + 1].index = (uint8_t)(33 + spriteidx);
		ppu.sprites[spriteidx + 1].attributes = 2;
		//if (spriteidx % 2) ppu.sprites[spriteidx].attributes |= 0x80; //'behind' bit
	}

	//fireball sprites:
	for(int spriteidx = 0; spriteidx < 4; spriteidx ++){
		ppu.sprites[spriteidx + 17].x = (uint8_t)(fireballs_at[spriteidx].x);
		ppu.sprites[spriteidx + 17].y = (uint8_t)(fireballs_at[spriteidx].y);
		ppu.sprites[spriteidx + 17].index = 49;
		ppu.sprites[spriteidx + 17].attributes = 4;
		//if (spriteidx % 2) ppu.sprites[spriteidx].attributes |= 0x80; //'behind' bit
	}

	//ppu.sprites[17].x = (uint8_t)(player_at.x + 8);
	//ppu.sprites[17].y = (uint8_t)(player_at.y + 8);


	//--- actually draw ---
	ppu.draw(drawable_size);
}
