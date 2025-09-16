#include "GameLevel.h"
#include<iostream>
#include<fstream>
#include<sstream>

std::vector<glm::vec3> colors = {
	glm::vec3(1.0f),
	glm::vec3(1.0f),
	glm::vec3(0.2f, 0.6f, 1.0f),
	glm::vec3(0.0f, 0.7f, 0.0f),
	glm::vec3(0.8f, 0.8f, 0.4f),
	glm::vec3(1.0f, 0.0f, 0.0f),
};

GameLevel::GameLevel() {
	std::cout << "Game Level created" << std::endl;
}
void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight) {
	this->Bricks.clear();
	unsigned int tileCode;

	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;
	if (fstream) {
		while (std::getline(fstream, line)) { // read each line
			std::istringstream sstream(line); // converts a string to a std::stream
			std::vector<unsigned int> row;
			while (sstream >> tileCode) // for each element in the stream, put it in tileCode and execute loop contents
				row.push_back(tileCode);

			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
	else
		std::cout << "failed to load file: " << file << std::endl;
}

// whether level is complete
bool GameLevel::IsCompleted()
{
	for (GameObject& tile : this->Bricks)
		if (!tile.IsSolid && !tile.Destroyed)
			return false;
	return true;
}

void GameLevel::Draw(SpriteRenderer& renderer) {
	for (unsigned int i = 0; i < Bricks.size(); i++) { // could use GameObject &tile : this->Bricks
		if(!Bricks[i].Destroyed)
			renderer.DrawSprite(Bricks[i].Sprite, Bricks[i].Position, Bricks[i].Size, Bricks[i].Rotation, Bricks[i].Color);
	}
}
void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight) {
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();

	float unit_width = levelWidth / static_cast<float>(width);
	float unit_height = levelHeight / static_cast<float>(height);

	std::cout << unit_width << ":" << unit_height << ") " << std::endl;

	glm::vec2 size(unit_width, unit_height);

	// initialize level tiles based on tileData contents
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			glm::vec2 pos(unit_width * x, unit_height * y);
			if (tileData[y][x] == 1) {
				// solid
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = true;
				this->Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) {
				glm::vec3 col = glm::vec3(1.0f); // default

				if (tileData[y][x] < colors.size())
					col = colors[tileData[y][x]];

				GameObject obj(pos, size, ResourceManager::GetTexture("block"), col); // could be initialised inside Bricks.push_back

				this->Bricks.push_back(obj);
			}
		}
	}
}