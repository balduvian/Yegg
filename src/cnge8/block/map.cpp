
#include "map.h"

#include "../math/math.h"
#include "../image/image.h"
#include "block.h"
#include <iostream>

#include <random>

namespace CNGE {

	Map::Map(const char* path)
		: Resource(true), path(path), lines(), blocks(nullptr), mapImage(nullptr), width(0), height(0), numSpawns(), spawns(), maxFires(0) {}

	void Map::render(i32 left, i32 down, i32 right, i32 up, Camera* camera, Rect* rect, TextureShader* textureShader, TileGrid* blockTexture, GateShader* gateShader) {
		auto prx = [](i32 x, i32 y) {
			std::mt19937 engineX(x + y);
			//std::default_random_engine engineY(y);

			std::uniform_int_distribution<int> distribution(0, 3);
			
			return distribution(engineX);
		};

		for (int i = left; i < right; ++i) {
			for (int j = down; j < up; ++j) {
				auto block = accessEdge<Block>(blocks, width, height, i, j);

				blockTexture->bind();

				//TODO custom rendering

				switch (block.id) {
				case Block::BLOCK_GROUND:
					textureShader->enable(Transform::toModel(i, j, 0, 1, 1), camera->getProjview());
					textureShader->giveParams(1, 1, 1, 1, blockTexture->getSheet(0, 0));
					rect->render();

					break;
				case Block::BLOCK_WALL:
					textureShader->enable(Transform::toModel(i, j, 0, 1, 1), camera->getProjview());
					textureShader->giveParams(1, 1, 1, 1, blockTexture->getSheet(prx(i, j), 2));
					rect->render();

					break;
				case Block::BLOCK_SPAWN:
					textureShader->enable(Transform::toModel(i, j, 0, 1, 1), camera->getProjview());
					textureShader->giveParams(1, 1, 1, 1, blockTexture->getSheet(1, 0));
					rect->render();

					break;
				case Block::BLOCK_RECIEVER: {
					bool open = block.state >> 7;
					auto gateIndex = block.state & 0b01111111;
					auto color = GATE_COLORS[gateIndex];

					gateShader->enable(Transform::toModel(i, j, 0, 1, 1), camera->getProjview());
					gateShader->giveParams(color[0], color[1], color[2], color[3], blockTexture->getSheet(open ? 2 : 3, open ? 3 : 3));
					rect->render();

				} break;
				case Block::BLOCK_DOOR: {
					bool open = block.state >> 7;
					auto gateIndex = block.state & 0b01111111;
					auto color = GATE_COLORS[gateIndex];

					gateShader->enable(Transform::toModel(i, j, 0, 1, 1), camera->getProjview());
					gateShader->giveParams(color[0], color[1], color[2], color[3], blockTexture->getSheet(open ? 3 : 1, open ? 0 : 3));
					rect->render();

				} break;
				case Block::BLOCK_END:
					textureShader->enable(Transform::toModel(i, j, 0, 1, 1), camera->getProjview());
					textureShader->giveParams(1, 1, 1, 1, blockTexture->getSheet(2, 1));
					rect->render();

					break;
				}
			}
		}
	}

	void Map::customGather() {
		mapImage = new Image2D(path);

		width = mapImage->getWidth();
		height = mapImage->getHeight();
	}

	void Map::customProcess() {
		//reset number of spawns
		numSpawns = 0;
		numGates = 0;
		maxFires = 0;

		auto pixels = mapImage->getPixels();

		// allocate and place blocks
		blocks = new Block * [width];
		for (auto i = 0; i < width; ++i) {
			blocks[i] = new Block[height];

			for (auto j = 0; j < height; ++j) {
				// grab color from picture
				// and match it against the templates
				auto colorIdentifier = pixels[height - j - 1][i * 4];
				auto colorData = pixels[height - j - 1][i * 4 + 1];
				auto colorDetails = pixels[height - j - 1][i * 4 + 2];

				// TODO custom loading for blocks

				switch (colorIdentifier) {
				case Block::COLOR_BLOCK_GROUND:
					blocks[i][j].id = Block::BLOCK_GROUND;
					break;
				case Block::COLOR_BLOCK_WALL:
					blocks[i][j].id = Block::BLOCK_WALL;
					break;
				case Block::COLOR_BLOCK_SPAWN: {
					blocks[i][j].id = Block::BLOCK_SPAWN;

					// we increase our number of spawns
					++numSpawns;

					// and find which spawn this is
					// and get the strength of the fire at this spawn
					auto spawnIndex = colorData / Block::SPAWN_COLOR_INCREMENT;
					auto spawnStrength = colorDetails / Block::SPAWN_COLOR_INCREMENT;

					spawns[spawnIndex][0] = i;
					spawns[spawnIndex][1] = j;
					spawns[spawnIndex][2] = spawnStrength;

					maxFires += spawnStrength;

				} break;
				case Block::COLOR_BLOCK_RECIEVER: {
					blocks[i][j].id = Block::BLOCK_RECIEVER;

					auto spawnIndex = colorData / Block::SPAWN_COLOR_INCREMENT;

					gatesPos[spawnIndex][INDEX_RECEIVER_X] = i;
					gatesPos[spawnIndex][INDEX_RECEIVER_Y] = j;

					blocks[i][j].state = spawnIndex;
				} break;
				case Block::COLOR_BLOCK_DOOR: {
					blocks[i][j].id = Block::BLOCK_DOOR;

					auto spawnIndex = colorData / Block::SPAWN_COLOR_INCREMENT;

					gatesPos[spawnIndex][INDEX_GATE_X] = i;
					gatesPos[spawnIndex][INDEX_GATE_Y] = j;

					blocks[i][j].state = spawnIndex;

					++numGates;
				} break;
				case Block::COLOR_BLOCK_END:
					blocks[i][j].id = Block::BLOCK_END;
					break;
				}
			}
		}

		// initialize the line ptrs array
		for (auto i = 0; i < 4; ++i) {
			// each block will get pointers to 4 lines on each side of it
			// used for checking collision real time and fast
			lines[i] = new bool* [width];
			for (auto j = 0; j < width; ++j) {
				lines[i][j] = new bool[height] {};
			}
		}

		// place in blocks based off of block templates
		for (auto i = 0; i < width; ++i) {
			for (auto j = 0; j < height; ++j) {
				// place in lines for collision based off this block
				auto tempBlock = blocks[i][j];

				// we place lines on the outside of non solid blocks when
				// they border a solid block
				if (!Block::solids[tempBlock.id]) {
					// to the right
					if (Block::solids[accessEdge<Block>(blocks, width, height, i + 1, j).id]) {
						lines[INDEX_LINE_RIGHT][i][j] = true;
					}
					// to the top
					if (Block::solids[accessEdge<Block>(blocks, width, height, i, j + 1).id]) {
						lines[INDEX_LINE_TOP][i][j] = true;
					}
					// to the left
					if (Block::solids[accessEdge<Block>(blocks, width, height, i - 1, j).id]) {
						lines[INDEX_LINE_LEFT][i][j] = true;
					}
					// to the bottom
					if (Block::solids[accessEdge<Block>(blocks, width, height, i, j - 1).id]) {
						lines[INDEX_LINE_BOTTOM][i][j] = true;
					}
				}

			}
		}
	}

	void Map::customUnload() {
		// cleanup each of the line ptrs arrays
		for (auto o = 0; o < 4; ++o) {
			for (auto i = 0; i < width; ++i) {
				delete[] lines[o][i];
			}

			delete[] lines[o];
		}

		// cleanup blocks
		for (auto i = 0; i < width; ++i)
			delete[] blocks[i];

		delete[] blocks;
	}

	void Map::customDiscard() {
		delete mapImage;
	}

	u32 Map::getNumSpawns() {
		return numSpawns;
	}

	bool Map::getRightLine(i32 x, i32 y) {
		return accessEdge<bool>(lines[INDEX_LINE_RIGHT], width, height, x, y);
	}

	bool Map::getTopLine(i32 x, i32 y) {
		return accessEdge<bool>(lines[INDEX_LINE_TOP], width, height, x, y);
	}

	bool Map::getLeftLine(i32 x, i32 y) {
		return accessEdge<bool>(lines[INDEX_LINE_LEFT], width, height, x, y);
	}

	bool Map::getBottomLine(i32 x, i32 y) {
		return accessEdge<bool>(lines[INDEX_LINE_BOTTOM], width, height, x, y);
	}

	i32 Map::getSpawnX(i32 index) {
		return spawns[index][0];
	}

	i32 Map::getSpawnY(i32 index) {
		return spawns[index][1];
	}

	i32 Map::getSpawnStrength(i32 index) {
		return spawns[index][2];
	}

	u32 Map::getMaxFires() {
		return maxFires;
	}

	i32 Map::getReceiverX(i32 index) {
		return gatesPos[index][INDEX_RECEIVER_X];
	}

	i32 Map::getReceiverY(i32 index) {
		return gatesPos[index][INDEX_RECEIVER_Y];
	}

	i32 Map::getGateX(i32 index) {
		return gatesPos[index][INDEX_GATE_X];
	}

	i32 Map::getGateY(i32 index) {
		return gatesPos[index][INDEX_GATE_Y];
	}

	u32 Map::getNumGates() {
		return numGates;
	}

	template <typename T>
	T Map::accessEdge(T** array, i32 width, i32 height, i32 x, i32 y)
	{
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		if (x > width - 1)
			x = width - 1;
		if (y > height - 1)
			y = height - 1;

		return array[x][y];
	}

	template <typename T>
	T Map::accessRepeat(T** array, i32 width, i32 height, i32 x, i32 y)
	{
		x = mod(x, width);
		y = mod(y, width);

		return array[x][y];
	}
	
	Block& Map::getBlock(i32 x, i32 y) {
		return blocks[x][y];
	}

}
