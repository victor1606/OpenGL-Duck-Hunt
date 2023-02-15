#include "lab_m1/lab3/lab3.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <random>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}

Mesh* CreateTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool flip)
{
    glm::vec3 corner = leftBottomCorner;

    color.x /= 255;
    color.y /= 255;
    color.z /= 255;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
    };

    if (flip) {
        vertices.push_back(VertexFormat(corner + glm::vec3(0, length, 0), color));
    }
    else {
        vertices.push_back(VertexFormat(corner + glm::vec3(length, length, 0), color));
    }

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    triangle->SetDrawMode(GL_TRIANGLES);

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* CreateRoundObject(
    const std::string& name,
    float center_x,
    float center_y,
    int radius,
    glm::vec3 color,
    bool oval)
{

    color.x /= 255;
    color.y /= 255;
    color.z /= 255;

    std::vector<VertexFormat> vertices;

    vertices.push_back(VertexFormat(glm::vec3(center_x, center_y, 0), color));

    int poly_count = 180;

    int var_x = 0, var_y = 0;

    for (int i = 0; i < poly_count; i++) {
        if (oval) {
            if (i < poly_count / 4) {
                var_x = -5;
                var_y = +5;
            }
            else if (i >= poly_count / 4 && i < poly_count / 2) {
                var_x = +5;
                var_y = +5;
            }
            else if (i >= poly_count / 2 && i < 3 * poly_count / 4) {
                var_x = +5;
                var_y = -5;
            }
            else {
                var_x = -5;
                var_y = -5;
            }
        }

        double x = center_x + (radius * cos(i * 2 * M_PI / poly_count)) + var_x;
        double y = center_y + (radius * sin(i * 2 * M_PI / poly_count)) + var_y;

        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
    }

    Mesh* round = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 1; i < poly_count; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);

        if (i == poly_count - 1) {
            indices.push_back(0);
            indices.push_back(poly_count);
            indices.push_back(1);
        }
    }

    round->InitFromData(vertices, indices);
    return round;
}

Mesh* CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    color.x /= 255;
    color.y /= 255;
    color.z /= 255;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(leftBottomCorner, color),
        VertexFormat(leftBottomCorner + glm::vec3(length, 0, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(0, width, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(length, width, 0), color),
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 3, 2 };

   if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
   } else {
        indices.push_back(0);
        indices.push_back(3);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    
    glm::vec3 wing_color = glm::vec3(189, 174, 171);
    Mesh* left_wing = CreateTriangle("left_wing", lw_corner, wing_len, wing_color, false);
    AddMeshToList(left_wing);
    
    Mesh* right_wing = CreateTriangle("right_wing", rw_corner, wing_len, wing_color, true);
    AddMeshToList(right_wing);


    glm::vec3 body_color = glm::vec3(45, 21, 29);
    body_x = 75;
    body_y = 0;

    body_radius = 32;

    Mesh* body = CreateRoundObject("body", body_x, body_y, body_radius, body_color, true);
    AddMeshToList(body);

    head_radius = 20;

    glm::vec3 head_color = glm::vec3(3, 73, 39);
    Mesh* head = CreateRoundObject("head", head_x, head_y, head_radius, head_color, true);
    AddMeshToList(head);

    beak_radius = 10;

    glm::vec3 beak_color = glm::vec3(183, 139, 48);
    Mesh* beak = CreateRoundObject("beak", beak_x, beak_y, beak_radius, beak_color, true);
    AddMeshToList(beak);

    glm::vec3 leg_color = glm::vec3(195, 82, 19);
    Mesh* leg = CreateRoundObject("leg", beak_x, beak_y, beak_radius, leg_color, true);
    AddMeshToList(leg);

    glm::vec3 grass_color = glm::vec3(86, 150, 79);
    Mesh* grass = CreateRectangle("grass", glm::vec3(0, 0, 0), 1280, 160, grass_color, true);
    AddMeshToList(grass);

    glm::vec3 sky_color = glm::vec3(186, 224, 255);
    Mesh* sky = CreateRectangle("sky", glm::vec3(0, 0, 0), 1280, 720, sky_color, true);
    AddMeshToList(sky);

    glm::vec3 trunk_color = glm::vec3(70, 51, 40);
    Mesh* trunk = CreateRectangle("trunk", glm::vec3(200, 100, 0), 50, 450, trunk_color, true);
    AddMeshToList(trunk);

    glm::vec3 leaves_color = glm::vec3(82, 114, 47);
    Mesh* leaves = CreateRoundObject("leaves", 225, 450, 200, leaves_color, false);
    AddMeshToList(leaves);

    glm::vec3 lives_color = glm::vec3(255, 10, 20);
    Mesh* life1 = CreateRoundObject("life1", 300, 470, 15, lives_color, false);
    AddMeshToList(life1);

    Mesh* life2 = CreateRoundObject("life2", 175, 540, 20, lives_color, false);
    AddMeshToList(life2);

    Mesh* life3 = CreateRoundObject("life3", 250, 370, 10, lives_color, false);
    AddMeshToList(life3);

    glm::vec3 score_color = glm::vec3(255, 160, 67);
    // Rectangle score
    //Mesh* score = CreateRectangle("score", glm::vec3(1000, 600, 0), 200, 50, score_color, false);
    // Circle score
    Mesh* score = CreateRoundObject("score", 1100, 600, 80, score_color, false);
    AddMeshToList(score);

    glm::vec3 bullet_color = glm::vec3(201, 24, 53);
    Mesh* bullet1 = CreateRectangle("bullet1", glm::vec3(1000, 100, 0), 20, 40, bullet_color, true);
    AddMeshToList(bullet1);

    Mesh* bullet2 = CreateRectangle("bullet2", glm::vec3(1050, 100, 0), 20, 40, bullet_color, true);
    AddMeshToList(bullet2);

    Mesh* bullet3 = CreateRectangle("bullet3", glm::vec3(1100, 100, 0), 20, 40, bullet_color, true);
    AddMeshToList(bullet3);

    glm::vec3 crosshair_color = glm::vec3(12, 21, 27);
    Mesh* crosshair = CreateRoundObject("crosshair", 0, 0, 8, crosshair_color, false);
    AddMeshToList(crosshair);

    Mesh* collision_model = CreateRectangle("collision_model", glm::vec3(initial_x, initial_y, 0), 120, 130, bullet_color, true);
    AddMeshToList(collision_model);

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 40);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.73f, 0.88f, 1.f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab3::RenderDuck(float deltaTimeSeconds) {
    // Crosshair
    extraMatrix = glm::mat3(1);
    extraMatrix *= transform2D::Translate(cross_x, 720 - cross_y);
    RenderMesh2D(meshes["crosshair"], shaders["VertexColor"], extraMatrix);

    // Bullets
    extraMatrix = glm::mat3(1);
    if (bullets < 3) {
        extraMatrix *= transform2D::Translate(-10000, -10000);
    }
    RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], extraMatrix);

    extraMatrix = glm::mat3(1);
    if (bullets < 2) {
        extraMatrix *= transform2D::Translate(-10000, -10000);
    }
    RenderMesh2D(meshes["bullet2"], shaders["VertexColor"], extraMatrix);

    extraMatrix = glm::mat3(1);
    if (bullets < 1) {
        extraMatrix *= transform2D::Translate(-10000, -10000);
    }
    RenderMesh2D(meshes["bullet3"], shaders["VertexColor"], extraMatrix);

    // Grass
    extraMatrix = glm::mat3(1);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], extraMatrix);

    // Wing flap animation
    if (wing_flap > (M_PI / 12) || rotate == true) {
        wing_flap -= deltaTimeSeconds;
        rotate = true;

        if (wing_flap <= 0)
            rotate = false;
    }
    else {
        wing_flap += deltaTimeSeconds;
    }

    // Whole duck
    fullMatrix = glm::mat3(1);
    fullMatrix *= transform2D::Translate(speed_x, speed_y); // flight

    // Hitbox
    extraMatrix = fullMatrix;
    model_x = initial_x + 10;
    model_y = initial_y - 35;

    extraMatrix *= transform2D::Translate(model_x, model_y);
    //RenderMesh2D(meshes["collision_model"], shaders["VertexColor"], extraMatrix); // Spawn hitbox

    fullMatrix *= transform2D::Translate(initial_x + wing_len + 30, 30);
    fullMatrix *= transform2D::Rotate(-skew); // duck angle
    fullMatrix *= transform2D::Translate(-initial_x - wing_len - 30, -30);

    modelMatrix = fullMatrix;

    // Left wing
    wingMatrix = modelMatrix;

    wingMatrix *= transform2D::Translate(initial_x + 25, 0); // initial position

    wingMatrix *= transform2D::Translate(wing_len, wing_len);
    wingMatrix *= transform2D::Rotate(-wing_flap); // wing flap
    wingMatrix *= transform2D::Translate(-wing_len, -wing_len);

    RenderMesh2D(meshes["left_wing"], shaders["VertexColor"], wingMatrix);


    // Right wing
    wingMatrix = modelMatrix;

    wingMatrix *= transform2D::Translate(initial_x - 5, 0); // initial position

    wingMatrix *= transform2D::Translate(100, wing_len);
    wingMatrix *= transform2D::Rotate(wing_flap); // wing flap
    wingMatrix *= transform2D::Translate(-100, -wing_len);

    RenderMesh2D(meshes["right_wing"], shaders["VertexColor"], wingMatrix);

    // Body
    bodyMatrix = modelMatrix;
    bodyMatrix *= transform2D::Translate(initial_x, initial_y); // initial position

    RenderMesh2D(meshes["body"], shaders["VertexColor"], bodyMatrix);

    // Head
    headMatrix = modelMatrix;
    headMatrix *= transform2D::Translate(initial_x + 75, initial_y + 55); // initial position

    RenderMesh2D(meshes["head"], shaders["VertexColor"], headMatrix);

    // Beak
    beakMatrix = modelMatrix;
    beakMatrix *= transform2D::Translate(initial_x + 75, initial_y + 80); // initial position

    RenderMesh2D(meshes["beak"], shaders["VertexColor"], beakMatrix);

    // Legs
    legMatrix = modelMatrix;
    legMatrix *= transform2D::Translate(initial_x + 62, initial_y - 35); // initial position

    RenderMesh2D(meshes["leg"], shaders["VertexColor"], legMatrix);

    legMatrix = modelMatrix;
    legMatrix *= transform2D::Translate(initial_x + 85, initial_y - 35); // initial position

    RenderMesh2D(meshes["leg"], shaders["VertexColor"], legMatrix);

    // Flight angle based on oX and oY speed
    //duck_angle = (int(skew * TO_DEGREES)) % 360;

    if (flip == true) {
        skew = float(atan(x_speed_modifier / y_speed_modifier)) + M_PI;
    }
    else {
        skew = float(atan(x_speed_modifier / y_speed_modifier));
    }

    if (hits % 2 == 0) {
        skew += 2 * M_PI;
    }
    else {
        skew += M_PI;
    }
}

void Lab3::RenderScene(float deltaTimeSeconds) {
    // Score box
    extraMatrix = glm::mat3(1);
    RenderMesh2D(meshes["score"], shaders["VertexColor"], extraMatrix);

    // Lives
    if (lives == 2) {
        first_life = 1;
        apple_fall_1 += 300 * deltaTimeSeconds;
    }

    if (lives == 1) {
        second_life = 1;
        apple_fall_2 += 300 * deltaTimeSeconds;
    }

    if (lives == 0) {
        third_life = 1;
        apple_fall_3 += 300 * deltaTimeSeconds;
    }

    extraMatrix = glm::mat3(1);
    extraMatrix *= transform2D::Translate(0, -apple_fall_1 * first_life);
    RenderMesh2D(meshes["life1"], shaders["VertexColor"], extraMatrix);
    if (first_life == 1)

    extraMatrix = glm::mat3(1);
    extraMatrix *= transform2D::Translate(0, -apple_fall_2 * second_life);
    RenderMesh2D(meshes["life2"], shaders["VertexColor"], extraMatrix);

    extraMatrix = glm::mat3(1);
    extraMatrix *= transform2D::Translate(0, -apple_fall_3 * third_life);
    RenderMesh2D(meshes["life3"], shaders["VertexColor"], extraMatrix);

    // Leaves
    extraMatrix = glm::mat3(1);
    RenderMesh2D(meshes["leaves"], shaders["VertexColor"], extraMatrix);

    // Tree
    extraMatrix = glm::mat3(1);
    RenderMesh2D(meshes["trunk"], shaders["VertexColor"], extraMatrix);
}

void Lab3::CollisionLogic(glm::ivec2 resolution) {
    // Collision
    if (speed_x + initial_x + 80 > resolution.x) {
        x_speed_modifier = -x_speed_modifier;
    }

    if (speed_x + initial_x + 50 < 0) {
        x_speed_modifier = -x_speed_modifier;
    }

    if (speed_y > resolution.y - 50 && lost_round == false) {
        hits++;
        y_speed_modifier = -y_speed_modifier;
    }

    if (speed_y < 0 && won_round == false) {
        hits++;
        y_speed_modifier = -y_speed_modifier;
    }
}

void Lab3::GenerateRandomXY(glm::ivec2 resolution) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr_x(300, resolution.x - 300); // defines the range

    initial_x = distr_x(gen);

    uniform_int_distribution<> distr_speed_x(400, 500);
    uniform_int_distribution<> distr_speed_y(200, 300);

    x_speed_modifier = distr_speed_x(gen) + speed_increase;
    if ((int)x_speed_modifier % 2 == 0) {
        x_speed_modifier = -x_speed_modifier;
    }
    
    // Increment speed every 5 rounds
    if (duck_count % 5 == 0 && duck_count > 0) {
        speed_increase += 100;
    }

    y_speed_modifier = distr_speed_y(gen) + speed_increase;
}

void Lab3::ResetRound(float deltaTimeSeconds, glm::ivec2 resolution) {
    if (speed_y < -200 || speed_y > 800) {
        bullets = 3;
        if (lost_round == true) {
            lives--;
        }

        duck_count++;
        lost_round = false;
        won_round = false;

        GenerateRandomXY(resolution);

        speed_x = 0;
        speed_y = 0;

        abs_time = 0;
        shot = false;
    }
}

void Lab3::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Game Over
    if (lives <= 0) {
        speed_x = -1000;
        speed_y = -1000;
        x_speed_modifier = 0;
        y_speed_modifier = 0;

        textRenderer->RenderText("GAME OVER", 450.f, 200.f, 1.0f, NormalizedRGB(200, 12, 25));
    }

    if (score == 80) {
        textRenderer->RenderText("MAX SCORE ACHIEVED", 990.f, 220.f, 0.5f, NormalizedRGB(200, 12, 25));
    }

    textRenderer->RenderText("Duck count: ", 990.f, 240.f, 0.5f, NormalizedRGB(200, 12, 25));
    textRenderer->RenderText(to_string(duck_count), 1200.f, 240.f, 0.5f, NormalizedRGB(200, 12, 25));

    // Generate random starting position, direction and speed
    if (initial_x == 0) {
        GenerateRandomXY(resolution);
    }

    // Keeps track of total time per round
    abs_time += deltaTimeSeconds;

    // Round lost - all 3 shots missed OR 6 seconds elapsed
    if (won_round == false) {
        if ((int)abs_time % time_per_round == 0 && abs_time > time_per_round ||
            (bullets == 0 && shot == false)) {
            x_speed_modifier = 0;
            y_speed_modifier = 700;

            lost_round = true;
            ResetRound(deltaTimeSeconds, resolution);
        }
    }

    // Round won - shot hit the target
    if (shot) {
        x_speed_modifier = 0;
        y_speed_modifier = -700;
        won_round = true;

        ResetRound(deltaTimeSeconds, resolution);

        glm::vec3 score_color = glm::vec3(255, 225, 67);
        // Rectangle score
        //Mesh* score_aux = CreateRectangle("score_aux", glm::vec3(1000, 600, 0), score, 50, score_color, true);
        // Circle score
        Mesh* score_aux = CreateRoundObject("score_aux", 1100, 600, score, score_color, false);

        AddMeshToList(score_aux);
    }

    extraMatrix = glm::mat3(1);
    RenderMesh2D(meshes["score_aux"], shaders["VertexColor"], extraMatrix);
    RenderDuck(deltaTimeSeconds);

    CollisionLogic(resolution);
    
    speed_x += x_speed_modifier * deltaTimeSeconds;
    speed_y += y_speed_modifier * deltaTimeSeconds;
    
    RenderScene(deltaTimeSeconds);
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Crosshair coords
    cross_x = mouseX;
    cross_y = mouseY;

    // Checks if the crosshair is above the collision model of the duck
    if (mouseX >= model_x + speed_x && mouseX <= model_x + speed_x + 120 &&
        720 - mouseY >= model_y + speed_y && 720 - mouseY <= model_y + speed_y + 130) {
        on_target = true;
    }
    else {
        on_target = false;
    }
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    bullets--;
    if (on_target) {
        if (flip == true) {
            flip = false;
        } else 
        if (y_speed_modifier < 0) {
            flip = true;
        }
        
        shot = true;
        if (score < 80) {
            score += 4;
        }
    }
    else if (bullets == 0 && y_speed_modifier < 0) {
        if (flip == true) {
            flip = false;
        }
        else {
            flip = true;
        }
    }
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
