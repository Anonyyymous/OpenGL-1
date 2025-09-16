#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include"ResourceManager.h"
// #include"Ball.h"
#include "VAO.h"

class SpriteRenderer {
public:
    SpriteRenderer(Shader& shader);
    ~SpriteRenderer();

    // void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f), unsigned int vao = 0);
    void DrawPolygon(VAO* vao, EBO* ebo, Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
    Shader shader;
    unsigned int quadVAO;
    void initRenderData();
};
#endif