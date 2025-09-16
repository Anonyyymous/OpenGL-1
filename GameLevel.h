#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include"Polygon.h"
#include<vector>
// #include<std>
#include<stdlib.h>


class GameLevel {
	public:
		std::vector<GameObject> Bricks;

		GameLevel();

		void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);

		void Draw(SpriteRenderer& renderer);
		bool IsCompleted();

	private:
		void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);

};


#endif
