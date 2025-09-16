#include "game.h"
#include<string>
#include<iostream>
#include<math.h>

SpriteRenderer* Renderer;
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float PLAYER_SPEED = 500.0f;
const float BALL_RADIUS = 25.0f;
// using namespace std;
Game::Game(unsigned int width, unsigned int height)
     : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    //State = GAME_ACTIVE;
    //Width = width;
    //Height = height;
}

Game::~Game()
{
    std::cout << "destroying Game Manager" << std::endl;
}

std::vector<const char*> LEVELS_TO_LOAD = { // const char* is the same as a string but the GameLevel.Load wants const char* specifically
    "blank_level.txt",
    "holes_level.txt",
};

glm::vec2 Game::GetBallStartPos() {
    return Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
}

void Game::Init()
{
    ResourceManager::LoadShader("sprite1.vert", "sprite1.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, (float) this->Width, (float)this->Height, 0.0f, -1.0f, 1.0f); // matrix that maps something in the range (0,0) to (800,600) to (-1,-1), (1,1)

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection); // this shader will be our main viewing matrix, and now will map everything to screenspace, if its in range ((0,0) to (800, 600))
    std::cout << "shader loaded" << std::endl;
    // render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // ResourceManager::LoadTexture("block.png", true, "block");
    // ResourceManager::LoadTexture("block_solid.png", true, "block_solid");
    ResourceManager::LoadTexture("death-threats-1.jpg", false, "background");
    ResourceManager::LoadTexture("block.png", false, "block");
    ResourceManager::LoadTexture("block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("paddle.png", true, "paddle");
    ResourceManager::LoadTexture("cartoon-earth.png", true, "ball");

    // load levels
    for (const char* levelName : LEVELS_TO_LOAD) {
        GameLevel level;
        level.Load(levelName, this->Width, this->Height / 2);
        this->Levels.push_back(level);

    }

    this->Level = 0;

    std::cout << "levels loaded" << std::endl;

    // player stuff
    glm::vec2 playerPos = glm::vec2((Width - PLAYER_SIZE.x)/2.0f, Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    std::cout << playerPos.x << "," << playerPos.y << ":" << PLAYER_SIZE.x << "," << PLAYER_SIZE.y << std::endl;
    
    // ball stuff
    glm::vec2 ballPos = GetBallStartPos();
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));

    MakePolygons();
}

float clamp(float val, float min, float max) {
    return std::max(min, std::min(max, val));
}

/*bool Game::CheckCollision(GameObject& box) // remove
{
    glm::vec2 difference = GetObjectOffset(*Ball, box);
    float penetration = Ball->Radius - glm::length(difference);
    if (penetration >= 0) 
        CorrectBallPosition(*Ball, box, difference);

    return penetration >= 0;
}*/

glm::vec2 Game::GetObjectOffset(GameObject& box) 
{
    // aabb = axis aligned bounding box
    glm::vec2 ball_center(Ball->Position + Ball->Radius);
    glm::vec2 aabb_half_extends(box.Size.x / 2.0f, box.Size.y / 2.0f);
    glm::vec2 box_center(box.Position.x + aabb_half_extends.x, box.Position.y + aabb_half_extends.y);

    glm::vec2 difference = ball_center - box_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extends, aabb_half_extends);

    glm::vec2 closest = box_center + clamped; // gets the point on the box closest to the circle

    difference = closest - ball_center;
    //return ball.Radius - glm::length(difference);
    return difference;
}
void Game::CorrectBallPosition(GameObject& box, glm::vec2 difference) {
    Ball->Position -= glm::normalize(difference) * (Ball->Radius - glm::length(difference));
}

glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f),
};

bool CollisionIsHorizontal(glm::vec2 target) {
    unsigned short best_match = -1; // will instead be the max value
    float max = 0.0f;
    // order = NORTH, EAST, SOUTH, WEST
    for (unsigned int i = 0; i < 4; i++) {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max) {
            best_match = i;
            max = dot_product;
        }
    }
    return (best_match % 2) == 1;
}

void Game::DoCollisions() {
    // can be otimised to just use penetration on the axis we're colliding with
    for (GameObject& obj : Levels[Level].Bricks) {
        //Test(Ball, obj);
        if (!obj.Destroyed) {
            glm::vec2 difference = GetObjectOffset(obj);
            float penetration = Ball->Radius - glm::length(difference);
            // float penetration = GetCollisionPenetration(*Ball, obj);
            if(penetration >= 0) {
                CorrectBallPosition(obj, difference); // avoids the ball clipping through stuff slightly
                if (!obj.IsSolid)
                    obj.Destroyed = true;
                if(CollisionIsHorizontal(difference))
                    Ball->Velocity.x = -Ball->Velocity.x;
                else
                    Ball->Velocity.y = -Ball->Velocity.y;
            }
        }
    }

    // player
    glm::vec2 difference = GetObjectOffset(*Player);
    float penetration = Ball->Radius - glm::length(difference);
    // float penetration = GetCollisionPenetration(*Ball, obj);
    if (penetration >= 0) {
        CorrectBallPosition(*Player, difference); // avoids the ball clipping through stuff slightly
        if (Ball->Position.y < Player->Position.y) // can be simplified to just check if we're above the player
        {
            // the further the ball hits from the centre of the paddle,the greater deflection it gets
            Ball->Velocity.y = -abs(Ball->Velocity.y);
            float board_center = Player->Position.x + Player->Size.x / 2.0f;
            float distance = (Ball->Position.x + Ball->Radius) - board_center;
            float percentage = distance / (Player->Size.x / 2.0f);

            float strength = 2.0f;
            glm::vec2 old_veloctity = Ball->Velocity;
            Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
            Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(old_veloctity);
        }
    }

    for (const Polygon polygon : polygons) {
        //Test(Ball, obj);
        DoPolygonCollisions(polygon);
    }
}
glm::vec2 Rotate(glm::vec2 vector, float rad)
{
    // glm::mat2 rotator = glm::mat2(cos(rad), -sin(rad), sin(rad), cos(rad));
    return glm::mat2(cos(rad), -sin(rad), sin(rad), cos(rad)) * vector;
}
Collision CollisionData = Collision(glm::vec2(0.0f));
int currentTriangle = 0;
void Game::DoPolygonCollisions(Polygon polygon)
{
    glm::vec2 polygon_center = glm::vec2(polygon.Position.x, (polygon.Position.y + polygon.Size.y));
    glm::vec2 scaled_position = ((Ball->Position + Ball->Radius) - polygon_center) / (polygon.Size.x);
    scaled_position = Rotate(scaled_position, polygon.Rotation);
    float scaled_radius = Ball->Radius / (polygon.Size.x * 2);
    // std::cout << "testing " << scaled_position.x << "," << scaled_position.y << std::endl;
    bool collided = false;
    int i = 0;
    glm::vec2 offset = glm::vec2(0.0f);
    for (Triangle triangle : polygon.Triangles)
    {
        //std::cout << (triangle.BallCollidesWithTriangle(glm::vec2(scaled_position.x, -scaled_position.y), scaled_radius, false)) ? "I" : "O") << std::endl;
        CollisionData = triangle.BallCollidesWithTriangle(glm::vec2(scaled_position.x, -scaled_position.y), scaled_radius);
        if (CollisionData.Collided) // if magnitude > 0
        {
            collided = true;
            offset += CollisionData.MinimumTranslationVector * polygon.Size.x;
            glm::vec2 normalized_velocity = glm::normalize(Ball->Velocity);
            if (glm::dot(CollisionData.Normal, normalized_velocity) == 0)
                Ball->Velocity *= -1;
            else
                Ball->Velocity = glm::reflect(normalized_velocity, CollisionData.Normal) * glm::length(Ball->Velocity);

        }
        i++;
    }
    Ball->Position += offset;
    
    //std::cout << "BALL NOT COLLIDING" << std::endl;
}
bool Game::TestPositionAgainstPolygons(glm::vec2 pos)
{
    for (const Polygon polygon : polygons) {
        //Test(Ball, obj);
        glm::vec2 polygon_center = glm::vec2(polygon.Position.x, (polygon.Position.y + polygon.Size.y));
        glm::vec2 scaled_position = (pos - polygon_center) / (polygon.Size.x);
        std::cout << "testing " << scaled_position.x << "," << scaled_position.y << std::endl;
        for (Triangle triangle : polygon.Triangles)
        {
            // CollisionData = triangle.TestPosition(glm::vec2(scaled_position.x, -scaled_position.y));
            std::cout << (triangle.TestPosition(glm::vec2(scaled_position.x, -scaled_position.y)) ? "I" : "O") << std::endl;

        }
    }

    return false;
}

void Game::Update(float dt)
{
    Ball->Move(dt, Width);
    DoCollisions();
}

void Game::ProcessInput(float dt)
{
    if (State == GAME_ACTIVE) {
        float velocity = PLAYER_SPEED * dt;

        if (Keys[GLFW_KEY_A]) {
            if (Player->Position.x >= 0.0f) {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }
        if (Keys[GLFW_KEY_D]) {
            if (Player->Position.x <= Width - Player->Size.x) {
                Player->Position.x += velocity;
                if (Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }

        if (Keys[GLFW_KEY_SPACE]) {
            Ball->Stuck = false;
        }
        if (Keys[GLFW_KEY_R]) {
            Ball->Reset(GetBallStartPos(), INITIAL_BALL_VELOCITY);
        }
    }
}

void Game::Render()
{
    // Renderer->DrawSprite(ResourceManager::GetTexture("death threats"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f)); // draws given texture at 45 degrees, kinda middle screen, large, and green
    Renderer->DrawSprite(ResourceManager::GetTexture("death threats"), glm::vec2(200.0f, 100.0f), glm::vec2(400.0f, 400.0f), 15.0f, glm::vec3(1.0f, 1.0f, 1.0f)); // background

    if (State == GAME_ACTIVE) {
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        this->Levels[this->Level].Draw(*Renderer);
    }
    Player->Draw(*Renderer);
    Ball->Draw(*Renderer);
    RenderPolygons();
}

void Game::MakePolygons() 
{
    GLfloat vertices[] =
    {
        // pos              // tex
        0.0f,  0.0f,      0.0f,   0.0f,
        0.0f,  1.0f,       0.0f,   1.0f,
        2.0f,   1.0f,       1.0f,   1.0f,
        2.0f,   0.0f,      1.0f,   0.0f,
        1.0f,   0.5f,       0.75f,  0.5f,
        0.5f,  -0.25f,     0.0f, 0.0f
    };
    GLuint indices[] = {
        0, 1, 4,
        1, 2, 4,
        0, 4, 5
    };
    /*GLfloat vertices[] =
    {
        // pos              // tex
        -1.0f,  -1.0f,      0.0f,   0.0f,
        -1.0f,  1.0f,       0.0f,   1.0f,
        1.0f,   1.0f,       1.0f,   1.0f
    };
    GLuint indices[] = {
        1, 2, 3,
        3, 2, 1
    };*/
    /*GLfloat vertices[] =
    { // COORDINATES  / TexCoord  //
        -0.5f, 0.0f,    0.0f, 0.0f,
        0.5f, 0.0f,		1.0f, 0.0f,
         0.0f, 0.9f,	0.5f, 1.0f,
        1.0f, 1.0f,		1.0f, 1.0f
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 1, 2,
        2, 1, 3
    };*/
    //GLfloat* verts = vertices;
    //GLuint* tris = indices;
    std::cout << "sizes = " << sizeof(vertices) << "|" << sizeof(GLfloat) << std::endl;
    Polygon poly(vertices, sizeof(vertices), indices, sizeof(indices), ResourceManager::GetTexture("block_solid"));
    poly.Size = glm::vec2(200.0f, 200.0f);
    poly.Color = glm::vec3(1.0f, 0.0f, 0.0f);
    poly.Rotation = glm::radians(45.0f);
    poly.Position = Player->Position + glm::vec2(0.0f, -400.0f);
    
    polygons.push_back(poly);
    

}
bool printed = false;
//GameObject* Test;
//GameObject* Test2;
void Game::RenderPolygons()
{
    for (Polygon polygon : polygons)
    {
        if (!printed)
        {
            printed = true;
            std::cout << "drawing polygon 1" << std::endl;
            //Test = new GameObject(Player->Position + glm::vec2(0.0f, -200.0f), glm::vec2(50.0f, 50.0f), ResourceManager::GetTexture("block_solid"));
            //Test2 = new GameObject(Player->Position + glm::vec2(0.0f, -200.0f), glm::vec2(25.0f, 25.0f), ResourceManager::GetTexture("block_solid"), glm::vec3(1.0f, 0.0f, 0.0f));
            // polygon.Position = Player->Position + glm::vec2(150.0f, -400.0f);
        }
        // polygon.Position = Player->Position + glm::vec2(-50.0f, -50.0f);
        polygon.Draw(*Renderer);
        // std::cout << "drawing polygon 1" << std::endl;
    }
}