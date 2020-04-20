
#ifndef CNGE_BLOCKMAP
#define CNGE_BLOCKMAP

#include "../Types.h"
#include "block.h"
#include "../load/resource.h"
#include "../engine/camera.h"
#include "../graphics/textureShader.h"
#include "../graphics/gateShader.h"
#include "../graphics/rect.h"
#include "../engine/tileGrid.h"

namespace CNGE {
	class Map : public Resource {
	public:
		constexpr static u32 MAX_NUM_SPAWNS = 8;
		constexpr static u32 MAX_NUM_GATES = 8;

		constexpr static u32 INDEX_RECEIVER_X = 0;
		constexpr static u32 INDEX_RECEIVER_Y = 1;
		constexpr static u32 INDEX_GATE_X = 2;
		constexpr static u32 INDEX_GATE_Y = 3;

		constexpr static f32 GATE_COLORS[MAX_NUM_GATES][4] = {
			{10_f32 / 255, 229_f32 / 255, 1, 1},
			{19_f32 / 255,  63_f32 / 255, 1, 1},
			{197_f32 / 255, 20_f32 / 255, 1, 1},
			{225_f32 / 255, 0, 0, 1},
			{232_f32 / 255, 220_f32 / 255, 20_f32 / 255, 1},
			{56_f32 / 255, 201_f32 / 255, 17_f32 / 255, 1},
			{89_f32 / 255, 106_f32 / 255, 149_f32 / 255, 1},
			{1, 1, 1, 1},
		};

	private:
		const char* path;

		i32 width;
		i32 height;

		u32 numSpawns;
		i32 spawns[MAX_NUM_SPAWNS][3];

		u32 numGates;
		i32 gatesPos[MAX_NUM_GATES][4];

		i32 maxFires;

		Image2D* mapImage;

		Block** blocks;
		bool** lines[4];
		
	protected:
		void customGather();
		void customProcess();
		void customDiscard();
		void customUnload();

	public:
		constexpr static i32
			INDEX_LINE_RIGHT = 0,
			INDEX_LINE_TOP = 1,
			INDEX_LINE_LEFT = 2,
			INDEX_LINE_BOTTOM = 3;

		Map(const char* path);

		void render(i32 left, i32 down, i32 right, i32 up, Camera* camera, Rect* rect, TextureShader* textureSader, TileGrid* blockTexture, GateShader* gateShader);

		u32 getNumSpawns();

		i32 getSpawnX(i32 index);
		i32 getSpawnY(i32 index);
		i32 getSpawnStrength(i32 index);

		u32 getMaxFires();

		i32 getReceiverX(i32 index);
		i32 getReceiverY(i32 index);
		i32 getGateX(i32 index);
		i32 getGateY(i32 index);

		u32 getNumGates();

		bool getRightLine(i32 x, i32 y);
		bool getTopLine(i32 x, i32 y);
		bool getLeftLine(i32 x, i32 y);
		bool getBottomLine(i32 x, i32 y);

		template<typename T>
		static T accessEdge(T** array, i32 width, i32 height, i32 x, i32 y);

		template<typename T>
		static T accessRepeat(T** array, i32 width, i32 height, i32 x, i32 y);

		Block& getBlock(i32 x, i32 y);

	};

}

#endif
