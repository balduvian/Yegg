
#include "blockMap.h"

#include "image/image.h"
#include "blocks.h"

#include "../res.h"
#include "engine/transform.h"
#include "math/math.h"

namespace Game {
	BlockMap::BlockMap(const char* filePath) : GameResource(),
		filePath(filePath), width(), height(), cyanGlitch(), magentaGlitch(), yellowGlitch(), launchers() 
	{}

	auto BlockMap::customLoad() -> void {
		dummy = true;

		auto image = CNGE::Image2D(filePath);

		auto pixels = image.getPixels();
		width = image.getWidth();
		height = image.getHeight();

		/* allocate */
		blocks = tripleAlloc<Block>(height, width);
		circles = doubleAlloc<bool>(height, width);

		for (auto j = 0; j < height; ++j) {
			/* internal maps are stored bottom up */
			auto h = height - j - 1;

			for (auto i = 0; i < width; ++i) {
				auto red = pixels[j][i * 4];
				auto green = pixels[j][i * 4 + 1];
				auto blue = pixels[j][i * 4 + 2];

				auto color = (red << 16) | (green << 8) | blue;

				/* place in the block */
				auto block = Blocks::getBlock(color); 
				blocks[h][i] = block;

				if (block == &Blocks::spawnBlock) {
					dummy = false;

					eggSpawn.setXY((i + 0.5f) * BlockMap::TILE_SIZE, (h + 0.5f) * BlockMap::TILE_SIZE);
				}

				if (block == &Blocks::rightLauncher)
					launchers.emplace_back(Vector2f((i + 0.5f) * BlockMap::TILE_SIZE, (h + 0.5f) * BlockMap::TILE_SIZE), 0);

				if (block == &Blocks::upLauncher)
					launchers.emplace_back(Vector2f((i + 0.5f) * BlockMap::TILE_SIZE, (h + 0.5f) * BlockMap::TILE_SIZE), CNGE::PI / 2);

				if (block == &Blocks::leftLauncher)
					launchers.emplace_back(Vector2f((i + 0.5f) * BlockMap::TILE_SIZE, (h + 0.5f) * BlockMap::TILE_SIZE), CNGE::PI);

				if (block == &Blocks::downLauncher)
					launchers.emplace_back(Vector2f((i + 0.5f) * BlockMap::TILE_SIZE, (h + 0.5f) * BlockMap::TILE_SIZE), CNGE::PI * 3 / 2);

			}
		}

		if (!dummy) {
			/* find the anchor position directly above spawn position */
			auto [searchX, searchY] = worldToMap(eggSpawn.x(), eggSpawn.y());
			do {
				if (getBlock(searchX, searchY) == &Blocks::fullBlock) {
					anchorSpawn.setXY(eggSpawn.x(), searchY * BlockMap::TILE_SIZE);
					break;

				}
				else
					++searchY;

			} while (true);
		}

		/* place collision circles */
		circleRoutine(Rotation::right());
		circleRoutine(Rotation::up());
		circleRoutine(Rotation::left());
		circleRoutine(Rotation::down());

		/* setup initial rotations */
		cyanGlitch.reset();
		magentaGlitch.reset();
		yellowGlitch.reset();
	}

	auto BlockMap::circleRoutine(Rotation rotation) -> void {
		auto faceX = Rotation::rotate(rotation, Rotation::QUARTER).getX();
		auto faceY = Rotation::rotate(rotation, Rotation::QUARTER).getY();
		auto checkX = Rotation::rotate(rotation, Rotation::EIGHTH).getX();
		auto checkY = Rotation::rotate(rotation, Rotation::EIGHTH).getY();
		auto nextX = rotation.getX();
		auto nextY = rotation.getY();
		auto circleX = checkX;
		auto circleY = checkY;

		if (rotation == Rotation::left()) {
			circleX = 0;
			circleY = 0;
		}

		if (rotation == Rotation::down()) {
			circleX = 1;
			circleY = 0;
		}

		if (rotation == Rotation::up()) {
			circleX = 0;
			circleY = 1;
		}

		std::cout << "circleX: " << circleX << std::endl;
		std::cout << "circleY: " << circleY << std::endl;

		auto relativeBottom = Rotation::rotate(rotation, -Rotation::QUARTER);
		auto relativeTop = Rotation::rotate(rotation, Rotation::QUARTER);
		auto relativeLeft = Rotation::rotate(rotation, Rotation::HALF);

		std::cout << "rotation: " << rotation << std::endl;
		std::cout << "relativeLeft: " << relativeLeft << std::endl;

		for (auto j = 0; j < height; ++j) {
			for (auto i = 0; i < width; ++i) {
				auto current = getBlock(i, j);
				auto    face = getBlock(i + faceX, j + faceY);
				auto   check = getBlock(i + checkX, j + checkY);
				auto    next = getBlock(i + nextX, j + nextY);

				if (shouldPlaceCircle(rotation, relativeBottom, relativeTop, relativeLeft, current, face, check, next)) {
					putCircle(i + circleX, j + circleY);
				}
			}
		}
	}

	auto BlockMap::shouldPlaceCircle(Rotation rotation, Rotation bottom, Rotation top, Rotation left, Block* current, Block* face, Block* check, Block* next) -> bool {
		if (current == &Blocks::airBlock || current == &Blocks::spawnBlock)
			return false;

		/* bottom of face block has to be null */
		if (face->getDirection(bottom) != nullptr)
			return false;
		
		/* top of current block can't be down right */
		if (Rotation::is(current->getDirection(top), Rotation::downRight() + rotation))
			return false;

		/* left of check block can't be up */
		auto leftCheck = check->getDirection(left);
		if (Rotation::is(leftCheck, Rotation::up() + rotation) || Rotation::is(leftCheck, Rotation::upRight() + rotation))
			return false;

		/* if current is a slope up then yes */
		if (current->getDirection(top) == nullptr)
			return true;

		/* if the next surface is flat then no */
		if (Rotation::is(next->getDirection(top), Rotation::right() + rotation))
			return false;

		return true;
	}

	auto BlockMap::update(CNGE::Timing* timing, std::vector<Bullet>& bullets) -> void {
		cyanGlitch.update(timing);
		magentaGlitch.update(timing);
		yellowGlitch.update(timing);

		for (auto &launcher : launchers) {
			if (launcher.update(timing))
				bullets.push_back(launcher.getBullet());
		}
	}

	auto BlockMap::render(CNGE::Camera* camera) -> void {

		auto cameraX = camera->transform.translation.x();
		auto cameraY = camera->transform.translation.y();

		auto[left, down] = worldToMap(cameraX, cameraY);
		auto[right, up] = worldToMap(cameraX + camera->width, cameraY + camera->height);

		--left;
		--down;

		auto    cyanOffset = cyanGlitch.getOffset();
		auto magentaOffset = magentaGlitch.getOffset();
		auto  yellowOffset = yellowGlitch.getOffset();

		renderTiles(camera, right, up, left, down,    cyanOffset.x(),    cyanOffset.y(),    cyan, true);
		renderTiles(camera, right, up, left, down, magentaOffset.x(), magentaOffset.y(), magenta, true);
		renderTiles(camera, right, up, left, down,  yellowOffset.x(),  yellowOffset.y(),  yellow, true);
		renderTiles(camera, right, up, left, down, 0, 0, black, false);

		//for (auto j = down; j <= up; ++j) {
		//	for (auto i = left; i <= right; ++i) {
		//		if (getCircle(i, j)) {
		//			Res::circleShader.enable(CNGE::Transform::toModel(i * TILE_SIZE - 8, j * TILE_SIZE - 8, 0, 16, 16), camera->getProjview());
		//			Res::colorShader.giveColor(0, 1, 0, 1);
		//			Res::rect.render();
		//		}
		//	}
		//}
	}

	auto BlockMap::renderTiles(CNGE::Camera* camera, i32 right, i32 up, i32 left, i32 down, f32 offY, f32 offX, CNGE::Color& color, bool forceColor) -> void {
		for (auto j = down; j <= up; ++j) {
			for (auto i = left; i <= right; ++i) {
				auto block = outOfMap(i, j) ? &Blocks::fullBlock : blocks[j][i];

				if (block->getVAO() != nullptr) {
					Res::colorShader.enable(CNGE::Transform::toModelRC(i * TILE_SIZE + offX, j * TILE_SIZE + offY, block->getRotation(), TILE_SIZE, TILE_SIZE), camera->getProjview());
					Res::colorShader.giveColor(forceColor ? color : block->getDisplayColor());
					block->getVAO()->render();
				}
			}
		}
	}

	auto BlockMap::customUnload() -> void {
		launchers.clear();
		killArray(blocks);
		killArray(circles);
	}

	auto BlockMap::worldToMap(f32 x, f32 y) -> mapPos {
		return { (i32)(x / TILE_SIZE), (i32)(y / TILE_SIZE) };
	}

	auto BlockMap::outOfMap(i32 x, i32 y) -> bool {
		return (x < 0 || y < 0 || x >= width || y >= height);
	}

	auto BlockMap::putCircle(i32 x, i32 y) -> void {
		if (!outOfMap(x, y)) {
			circles[y][x] = true;
		}
	}

	auto BlockMap::getWorldBlock(f32 worldX, f32 worldY) -> Block* {
		auto [x, y] = worldToMap(worldX, worldY);

		return outOfMap(x, y) ? &Blocks::fullBlock : blocks[y][x];
	}

	auto BlockMap::getBlock(i32 mapX, i32 mapY) -> Block* {
		return outOfMap(mapX, mapY) ? &Blocks::fullBlock : blocks[mapY][mapX];
	}

	auto BlockMap::getCircle(i32 mapX, i32 mapY) -> bool {
		return outOfMap(mapX, mapY) ? false : circles[mapY][mapX];
	}

	auto BlockMap::getEggSpawn() -> Vector2f& {
		return eggSpawn;
	}

	auto BlockMap::getAnchorSpawn() -> Vector2f& {
		return anchorSpawn;
	}

	CNGE::Color BlockMap::   cyan = CNGE::Color(  0, 255, 255);
	CNGE::Color BlockMap::magenta = CNGE::Color(255,   0, 255);
	CNGE::Color BlockMap:: yellow = CNGE::Color(255, 255,   0);
	CNGE::Color BlockMap::  black = CNGE::Color(  0,   0,   0);

	BlockMap::~BlockMap() {
		unload();
	}
}
