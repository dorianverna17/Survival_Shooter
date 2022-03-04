#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        int checkObstacleCollision(float pos_x, float pos_y);
        // create Mesh function
        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);

    protected:
        float length;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;

        float angularStep;

        float jugadorX, jugadorY;
        float length1, length2;

        GLenum polygonMode;

        // obstacles positions
        std::vector<std::tuple<int, int>> obstacles;

        // antagonist positions
        // third float is for the speed of the enemies
        std::vector<std::tuple<float, float, float>> antagonists;
        float prev_antagonist;
        float antagonistX, antagonistY;
        float angularStep_antagonist;
        float antagonist_speed;
        float spawn_rate;

        // object scale factors
        int scale_factor_middle = 5;
        int scale_factor_side_1 = 1;
        int scale_factor_side_2 = 4;

        // bullets
        // the int is a flag for knowing when the bullet is fired
        // fourth float is to retain the angle
        // last float is to retain the distance traveled by the bullet
        std::vector<std::tuple<float, float, int, float, float>> bullets;
        float fire_rate;
        float bullet_speed;
        float prev_bullet;
        float max_distance;

        // the amount of time at which the score is displayed
        float score_time;
        int score;
        float healthBar_width, healthBar_height;
        // the amount of enemies that collided with me
        int lost_life;
    };
}   // namespace m1
