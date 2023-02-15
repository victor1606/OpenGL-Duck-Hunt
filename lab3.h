#pragma once

#include "components/simple_scene.h"
#include <components/text_renderer.h>


namespace m1
{
    class Lab3 : public gfxc::SimpleScene
    {
     public:
        Lab3();
        ~Lab3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void GenerateRandomXY(glm::ivec2 resolution);
        void ResetRound(float deltaTimeSeconds, glm::ivec2 resolution);
        void RenderDuck(float deltaTimeSeconds);
        void RenderScene(float deltaTimeSeconds);
        void CollisionLogic(glm::ivec2 resolution);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        float cx, cy;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;

        // TODO(student): If you need any other class variables, define them here.
        gfxc::TextRenderer* textRenderer;
        glm::mat3 modelMatrix, fullMatrix, wingMatrix, bodyMatrix, headMatrix, beakMatrix, legMatrix, grassMatrix, skyMatrix, extraMatrix;

        float initial_x = 0, initial_y = 0;
        
        glm::vec3 lw_corner = glm::vec3(0, 0, 0);
        glm::vec3 rw_corner = glm::vec3(100, 0, 0);
        
        float wing_flap = 0;
        float wing_len = 30;
        float wing_x = 1.f, wing_y = 1.f, body_x = 1.f, body_y = 1.f, head_x = 1.f, head_y = 1.f, beak_x = 1.f, beak_y = 1.f;
        int head_radius = 0, body_radius = 0, beak_radius = 0;

        float speed_x = 1.f, speed_y = 1.f;
        float x_speed_modifier = 0, y_speed_modifier = 0;
        
        bool rotate;
        bool flip = false;
        float skew = 0;
        
        float abs_time = 0, total_time = 0;
        float apple_fall_1 = 0, apple_fall_2 = 0, apple_fall_3 = 0;
        int lives = 3;
        int first_life = 0, second_life = 0, third_life = 0;
        bool lost_round = false;
        bool won_round = false;
        
        int time_per_round = 6;

        float cross_x = 0, cross_y = 0;
        int model_x = 0, model_y = 0;
        bool on_target = false;
        int hits = 0;
        int bullets = 3;
        bool shot = false;
        int duck_angle = 0;
        int score = 0;
        int duck_count = 0;
        int speed_increase = 0;
    };
}   // namespace m1
