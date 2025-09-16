#include"SpriteRenderer.h"

bool LOG = false;

SpriteRenderer::SpriteRenderer(Shader& shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}
void SpriteRenderer::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color, unsigned int vao) {
    // assumes origin on top-left
    if (vao == 0)
        vao = quadVAO;
    shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 1.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, (float)glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // i think this kinda re-centers the origin

    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader.SetMatrix4("model", model);
    shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void SpriteRenderer::DrawPolygon(VAO* vao, EBO* ebo, Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    // assumes origin on top-left
    this->shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 1.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, (float)glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // i think this kinda re-centers the origin
    
    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("spriteColor", color);
    // std::cout << model << position << size << rotate << color << std::endl;

    // glActiveTexture(GL_TEXTURE0);
    if(LOG)
        std::cout << "texture bound" << std::endl;
    // std::cout << "drawing polygon 1" << std::endl;
    texture.Bind();
    if(LOG)
        std::cout << "VAO bound" << std::endl;
    vao->Bind();
    ebo->Bind();
    vao->vbo->Bind();
    if (LOG)
        std::cout << "EBO bound" << std::endl;
    glDrawElements(GL_TRIANGLES, ebo->Size, GL_UNSIGNED_INT, 0);
    //if(LOG)
        //std::cout << "polygon drawn complete" << std::endl;
    //glBindVertexArray(vao);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    //glBindVertexArray(0);
}