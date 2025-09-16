#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include"SpriteRenderer.h"
#include"GameLevel.h" // has sprite renderer, which has ball, whihc has polygon, which has gameobject
// #include "Ball.h" // has polygons, gameobjects

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


// holds game-related state/functionality
class Game
{
	public:
		GameState State;
		bool Keys[1024];
		unsigned int Width, Height;
		std::vector<GameLevel> Levels;
		unsigned int Level;

		GameObject* Player;
		BallObject* Ball;

		// constructor/destructor
		Game(unsigned int width, unsigned int height);
		~Game();

		glm::vec2 GetBallStartPos();
		void Init();

		void ProcessInput(float dt);
		void DoCollisions();
		//bool HasCollisions(GameObject& obj, BallObject* Ball);
		void DoPolygonCollisions(Polygon polygon);
		void CorrectBallPosition(GameObject& box, glm::vec2 difference);
		glm::vec2 GetObjectOffset(GameObject& box);
		// bool TestCircleAgainstPolygons(glm::vec2 pos);
		bool TestPositionAgainstPolygons(glm::vec2 pos);
		// bool CheckCollision(GameObject& box);
		void Update(float dt);
		void Render();

		std::vector<Polygon> polygons;
		void MakePolygons();
		void RenderPolygons();
};
#endif