#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>


#include "lab_m1/Tema1/Helpers.h"


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    srand(time(0));

    logicSpace.x = 4;       // logic x
    logicSpace.y = 4;       // logic y
    logicSpace.width = 8;   // logic width
    logicSpace.height = 8;  // logic height

    // map dimensions
    length1 = 15;
    length2 = 25;

    polygonMode = GL_FILL;
    glm::ivec2 resolution = window->GetResolution();

    jugadorX = logicSpace.x + logicSpace.width / 2 - 0.5;
    jugadorY = logicSpace.y + logicSpace.height / 2 - 0.5;

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    length = 1.0f;
    angularStep = 0;
    
    angularStep_antagonist = 0;
    prev_antagonist = 0;
    antagonist_speed = 0.1;
    spawn_rate = 3;

    fire_rate = 0.5;
    bullet_speed = 0.1;
    prev_bullet = 0;
    max_distance = length2 / 3;

    lost_life = 0;
    score = 0;
    score_time = 0;

    // here I create the map
    {
        vector<VertexFormat> vertices_map
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.5, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, length1, 0), glm::vec3(0.5, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(length2, length1, 0), glm::vec3(0.5, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(length2, 0, 0), glm::vec3(0.5, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
        };

        vector<unsigned int> indices_map = { 0, 1, 3, 1, 2, 3 };

        CreateMesh("map", vertices_map, indices_map);
        
    }

    // here I create the player
    {
        // create the body
        vector<VertexFormat> vertices_player
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0.3, 0, 0.6)),
            VertexFormat(glm::vec3(0, length, 0), glm::vec3(1, 0, 0), glm::vec3(0.3, 0, 0.6)),
            VertexFormat(glm::vec3(length, length, 0), glm::vec3(1, 0, 0), glm::vec3(0.3, 0, 0.6)),
            VertexFormat(glm::vec3(length, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0.3, 0, 0.6)),
        };

        vector<unsigned int> indices_player = { 0, 1, 3, 1, 2, 3};
        
        CreateMesh("player", vertices_player, indices_player);

        // create the hand mesh
        vector<VertexFormat> vertices_hand
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0, length / 5 * 2, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(length / 5, length / 5 * 2, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(length / 5, 0, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 0, 0)),
        };

        vector<unsigned int> indices_hand = { 0, 1, 3, 1, 2, 3 };

        CreateMesh("hand", vertices_hand, indices_hand);
    }

    // here I create the obstacles
    {
        // create the body
        vector<VertexFormat> vertices_obstacle
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0.6)),
            VertexFormat(glm::vec3(0, length, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0.6)),
            VertexFormat(glm::vec3(length, length, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0.6)),
            VertexFormat(glm::vec3(length, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0.6)),
        };

        vector<unsigned int> indices_obstacle = { 0, 1, 3, 1, 2, 3 };

        CreateMesh("obstacle", vertices_obstacle, indices_obstacle);

        // retain the horizontal positions of the obstacles
        obstacles.push_back(make_tuple(3, 2));
        obstacles.push_back(make_tuple(18, 2));
        obstacles.push_back(make_tuple(18, 12));
        obstacles.push_back(make_tuple(3, 12));

        // retain the vertical position of the obstacles
        obstacles.push_back(make_tuple(3, 2));
        obstacles.push_back(make_tuple(3, 9));
        obstacles.push_back(make_tuple(21, 2));
        obstacles.push_back(make_tuple(21, 9));

        obstacles.push_back(make_tuple(10, 5));
    }

    // here I create the mobs
    {
        // create the body
        vector<VertexFormat> vertices_player
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1)),
            VertexFormat(glm::vec3(0, length, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1)),
            VertexFormat(glm::vec3(length, length, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1)),
            VertexFormat(glm::vec3(length, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1)),
        };

        vector<unsigned int> indices_player = { 0, 1, 3, 1, 2, 3 };

        CreateMesh("antagonist", vertices_player, indices_player);

        // create the hand mesh
        vector<VertexFormat> vertices_hand
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 1, 0.6)),
            VertexFormat(glm::vec3(0, length / 5 * 2, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 1, 0.6)),
            VertexFormat(glm::vec3(length / 5, length / 5 * 2, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 1, 0.6)),
            VertexFormat(glm::vec3(length / 5, 0, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 1, 0.6)),
        };

        vector<unsigned int> indices_hand = { 0, 1, 3, 1, 2, 3 };

        CreateMesh("hand_antagonist", vertices_hand, indices_hand);
    }
    
    // here I create the bullets
    {
        vector<VertexFormat> bullet_vertices
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0.3, 1), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(0, length / 5, 0), glm::vec3(1, 0.3, 1), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(length / 5 * 2, length / 5, 0), glm::vec3(1, 0.3, 1), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(length / 5 * 2, 0, 0), glm::vec3(1, 0.3, 1), glm::vec3(0, 0, 0)),
        };

        vector<unsigned int> indices_bullet = { 0, 1, 3, 1, 2, 3 };

        CreateMesh("bullet", bullet_vertices, indices_bullet);
    }

    healthBar_width = 3;
    healthBar_height = 1;
    // here I create the healthbar
    {
        // the wireframe rectangle
        vector<VertexFormat> full_life_vertices
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0, healthBar_height, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(healthBar_width, healthBar_height, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(healthBar_width, 0, 0), glm::vec3(1, 0.3, 1), glm::vec3(1, 0, 0)),
        };

        vector<unsigned int> full_life_bullet = { 0, 1, 3, 1, 2, 3 };

        CreateMesh("full_life", full_life_vertices, full_life_bullet);

        // the life rectangle
        vector<VertexFormat> actual_life_vertices
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0.3, 1), glm::vec3(0.7, 0, 0)),
            VertexFormat(glm::vec3(0, healthBar_height, 0), glm::vec3(1, 0.3, 1), glm::vec3(0.7, 0, 0)),
            VertexFormat(glm::vec3(healthBar_width, healthBar_height, 0), glm::vec3(1, 0.3, 1), glm::vec3(0.7, 0, 0)),
            VertexFormat(glm::vec3(healthBar_width, 0, 0), glm::vec3(1, 0.3, 1), glm::vec3(0.7, 0, 0)),
        };

        vector<unsigned int> actual_life_bullet = { 0, 1, 3, 1, 2, 3 };

        CreateMesh("actual_life", actual_life_vertices, actual_life_bullet);
    }
}

// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds)
{

    if (lost_life >= 10)
    {
        cout << "You lost" << endl;
        while (true);
    }

    glm::ivec2 resolution = window->GetResolution();
    
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    DrawScene(visMatrix, deltaTimeSeconds);

    // display the score
    if (score_time >= 2) {
        score_time = 0;
        cout << "Your score: " << score << endl;
    }
    else {
        score_time += deltaTimeSeconds;
    }
}


void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    modelMatrix = visMatrix * helpers::Translate(0, 0);
    RenderMesh2D(meshes["map"], shaders["VertexNormal"], modelMatrix);
    
    // set the map to not be in front
    glClear(GL_DEPTH_BUFFER_BIT);

    // now I draw the obstacles
    modelMatrix = visMatrix * helpers::Translate(get<0>(obstacles[8]), get<1>(obstacles[8]));
    modelMatrix *= helpers::Scale(scale_factor_middle , scale_factor_middle);
    RenderMesh2D(meshes["obstacle"], shaders["VertexNormal"], modelMatrix);

    // render the horizontal obstacles
    modelMatrix = visMatrix * helpers::Translate(get<0>(obstacles[0]), get<1>(obstacles[0]));
    modelMatrix *= helpers::Scale(scale_factor_side_2, scale_factor_side_1);
    RenderMesh2D(meshes["obstacle"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = visMatrix * helpers::Translate(get<0>(obstacles[1]), get<1>(obstacles[1]));
    modelMatrix *= helpers::Scale(scale_factor_side_2, scale_factor_side_1);
    RenderMesh2D(meshes["obstacle"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = visMatrix * helpers::Translate(get<0>(obstacles[2]), get<1>(obstacles[2]));
    modelMatrix *= helpers::Scale(scale_factor_side_2, scale_factor_side_1);
    RenderMesh2D(meshes["obstacle"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = visMatrix * helpers::Translate(get<0>(obstacles[3]), get<1>(obstacles[3]));
    modelMatrix *= helpers::Scale(scale_factor_side_2, scale_factor_side_1);
    RenderMesh2D(meshes["obstacle"], shaders["VertexNormal"], modelMatrix);

    // render the vertical obstacles
    modelMatrix = visMatrix * helpers::Translate(get<0>(obstacles[4]), get<1>(obstacles[4]));
    modelMatrix *= helpers::Scale(scale_factor_side_1, scale_factor_side_2);
    RenderMesh2D(meshes["obstacle"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = visMatrix * helpers::Translate(get<0>(obstacles[5]), get<1>(obstacles[5]));
    modelMatrix *= helpers::Scale(scale_factor_side_1, scale_factor_side_2);
    RenderMesh2D(meshes["obstacle"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = visMatrix * helpers::Translate(get<0>(obstacles[6]), get<1>(obstacles[6]));
    modelMatrix *= helpers::Scale(scale_factor_side_1, scale_factor_side_2);
    RenderMesh2D(meshes["obstacle"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = visMatrix * helpers::Translate(get<0>(obstacles[7]), get<1>(obstacles[7]));
    modelMatrix *= helpers::Scale(scale_factor_side_1, scale_factor_side_2);
    RenderMesh2D(meshes["obstacle"], shaders["VertexNormal"], modelMatrix);

    // render the player
    modelMatrix = visMatrix * helpers::Translate(jugadorX, jugadorY);
    modelMatrix *= helpers::Translate(0.5, 0.5);
    modelMatrix *= helpers::Rotate(angularStep);
    modelMatrix *= helpers::Translate(-0.5, -0.5);
    RenderMesh2D(meshes["player"], shaders["VertexNormal"], modelMatrix);
    glClear(GL_DEPTH_BUFFER_BIT);

    float hand_1_x, hand_1_y, translate_1_x, translate_1_y;
    float hand_2_x, hand_2_y, translate_2_x, translate_2_y;

    // left hand
    hand_1_x = jugadorX + length - (float)length / 10;
    hand_1_y = jugadorY + length + (float)length / 10 - length / 5 * 2;
    modelMatrix = visMatrix * helpers::Translate(hand_1_x, hand_1_y);

    translate_1_y = (float)length / 5 * 2 - (float)length / 10;
    translate_1_x = (float)length / 10;

    modelMatrix *= helpers::Translate(translate_1_x, translate_1_y);
    modelMatrix *= helpers::Translate(-0.5, -0.5);
    modelMatrix *= helpers::Rotate(angularStep);
    modelMatrix *= helpers::Translate(0.5, 0.5);
    modelMatrix *= helpers::Translate(-translate_1_x, -translate_1_y);
    RenderMesh2D(meshes["hand"], shaders["VertexNormal"], modelMatrix);

    // right hand
    hand_2_x = jugadorX + length - (float)length / 10;
    hand_2_y = jugadorY - (float)length / 10;
    modelMatrix = visMatrix * helpers::Translate(hand_2_x, hand_2_y);

    translate_2_y = (float)length / 10;
    translate_2_x = (float)length / 10;

    modelMatrix *= helpers::Translate(translate_2_x, translate_2_y);
    modelMatrix *= helpers::Translate(-0.5, 0.5);
    modelMatrix *= helpers::Rotate(angularStep);
    modelMatrix *= helpers::Translate(0.5, -0.5);
    modelMatrix *= helpers::Translate(-translate_2_x, -translate_2_y);
    RenderMesh2D(meshes["hand"], shaders["VertexNormal"], modelMatrix);

    // create one more antagonist
    if (prev_antagonist > spawn_rate) {
        antagonistX = length2 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        antagonistY = length1 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float speed = antagonist_speed * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        antagonists.push_back(make_tuple(antagonistX, antagonistY, speed));

        prev_antagonist = 0;
    }
    prev_antagonist += deltaTimeSeconds;
    
    std::vector<std::tuple<float, float, float>> antagonists_new_after_collision;
    // render the antagonists
    int i;
    for (i = 0; i < antagonists.size(); i++)
    {
        antagonistX = get<0>(antagonists[i]);
        antagonistY = get<1>(antagonists[i]);

        // compute the angle
        float aux_y = antagonistY - jugadorY;
        float aux_x = antagonistX - jugadorX;

        angularStep_antagonist = atan(aux_y / aux_x);
        if ((aux_x < 0 && aux_y < 0) || (aux_y > 0 && aux_x < 0))
            angularStep_antagonist += 3.1408;

        modelMatrix = visMatrix * helpers::Translate(antagonistX, antagonistY);
        modelMatrix *= helpers::Translate(0.5, 0.5);
        modelMatrix *= helpers::Rotate(angularStep_antagonist);
        modelMatrix *= helpers::Translate(-0.5, -0.5);
        RenderMesh2D(meshes["antagonist"], shaders["VertexNormal"], modelMatrix);
        glClear(GL_DEPTH_BUFFER_BIT);

        // left hand
        hand_1_x = antagonistX - (float)length / 10;
        hand_1_y = antagonistY + length + (float)length / 10 - length / 5 * 2;
        modelMatrix = visMatrix * helpers::Translate(hand_1_x, hand_1_y);

        translate_1_y = (float)length / 5 * 2 - (float)length / 10;
        translate_1_x = (float)length / 10;

        modelMatrix *= helpers::Translate(translate_1_x, translate_1_y);
        modelMatrix *= helpers::Translate(0.5, -0.5);
        modelMatrix *= helpers::Rotate(angularStep_antagonist);
        modelMatrix *= helpers::Translate(-0.5, 0.5);
        modelMatrix *= helpers::Translate(-translate_1_x, -translate_1_y);
        RenderMesh2D(meshes["hand_antagonist"], shaders["VertexNormal"], modelMatrix);

        // right hand
        hand_2_x = antagonistX - (float)length / 10;
        hand_2_y = antagonistY - (float)length / 10;
        modelMatrix = visMatrix * helpers::Translate(hand_2_x, hand_2_y);

        translate_2_y = (float)length / 10;
        translate_2_x = (float)length / 10;

        modelMatrix *= helpers::Translate(translate_2_x, translate_2_y);
        modelMatrix *= helpers::Translate(0.5, 0.5);
        modelMatrix *= helpers::Rotate(angularStep_antagonist);
        modelMatrix *= helpers::Translate(-0.5, -0.5);
        modelMatrix *= helpers::Translate(-translate_2_x, -translate_2_y);
        RenderMesh2D(meshes["hand_antagonist"], shaders["VertexNormal"], modelMatrix);
        
        float sin_antagonist = sin(angularStep_antagonist);
        float cos_antagonist = cos(angularStep_antagonist);

        float aux_antagonist_x_after = get<1>(antagonists[i]) - get<2>(antagonists[i]) * sin_antagonist;
        float aux_antagonist_y_after = get<0>(antagonists[i]) - get<2>(antagonists[i]) * cos_antagonist;
        float middle_antagonist_X = aux_antagonist_x_after + length / 2;
        float middle_antagonist_Y = aux_antagonist_y_after + length / 2;
        
        // check collision of enemy with the wall
        if (!(middle_antagonist_X < 0))
            get<1>(antagonists[i]) = aux_antagonist_x_after;
        if (!(middle_antagonist_Y < 0))
            get<0>(antagonists[i]) = aux_antagonist_y_after;
        
        float middle_jugador_X = jugadorX + (float) length / 2;
        float middle_jugador_Y = jugadorY + (float) length / 2;
        float middle_mob_X = get<0>(antagonists[i]) + length / 2;
        float middle_mob_Y = get<1>(antagonists[i]) + length / 2;
        float distance = sqrt((middle_jugador_X - middle_mob_X) * (middle_jugador_X - middle_mob_X) +
            (middle_jugador_Y - middle_mob_Y) * (middle_jugador_Y - middle_mob_Y));
        
        if (distance > (float) length)
        {
            antagonists_new_after_collision.push_back(antagonists[i]);
            continue;
        }
        else {
            lost_life++;
        }

    }
    antagonists = antagonists_new_after_collision;

    float bullet_x, bullet_y;
    std::vector<std::tuple<float, float, int, float, float>> bullets_new;
    // render all the bullets
    for (int i = 0; i < bullets.size(); i++)
    {
        modelMatrix = visMatrix * helpers::Translate(get<0>(bullets[i]), get<1>(bullets[i]));
        if (get<2>(bullets[i]) == 0)
        {
            get<3>(bullets[i]) = angularStep;
            modelMatrix *= helpers::Translate((-1) * (float)length / 2, (float)length / 10);
            modelMatrix *= helpers::Rotate(angularStep);
            modelMatrix *= helpers::Translate((float)length / 2, (-1) * (float)length / 10);
            get<2>(bullets[i]) = 1;
        }
        else
        {
            modelMatrix *= helpers::Translate((-1) * (float)length / 2, (float)length / 10);
            modelMatrix *= helpers::Rotate(get<3>(bullets[i]));
            modelMatrix *= helpers::Translate((float)length / 2, (-1) * (float)length / 10);
        }

        get<0>(bullets[i]) = get<0>(bullets[i]) + bullet_speed * cos(get<3>(bullets[i]));
        get<1>(bullets[i]) = get<1>(bullets[i]) + bullet_speed * sin(get<3>(bullets[i]));
        RenderMesh2D(meshes["bullet"], shaders["VertexNormal"], modelMatrix);

        float middle_bullet_X = get<0>(bullets[i]) + (float)length / 5;
        float middle_bullet_Y = get<1>(bullets[i]) + (float)length / 10;

        get<4>(bullets[i]) += bullet_speed;
        // check collision
        if (middle_bullet_X < 1.5 * length && get<3>(bullets[i]) > (float) M_PI / 2 && get<3>(bullets[i]) < 3 * (float)M_PI / 2)
            continue;
        if (middle_bullet_X > length2 && get<3>(bullets[i]) < (float) M_PI / 2)
            continue;
        if (middle_bullet_Y < (float) length / 1.5 && (get<3>(bullets[i]) < 0 || get<3>(bullets[i]) > M_PI))
            continue;
        if (middle_bullet_Y > length1 - (float) length / 1.5 && (get<3>(bullets[i]) > 0 && get<3>(bullets[i]) < M_PI))
            continue;

        // check collision with big obstacle
        float aux_collision_x = false;
        float aux_collision_y = false;

        // collision with big obstacle
        if (middle_bullet_X > get<0>(obstacles[8]) + (float)length &&
            middle_bullet_X < get<0>(obstacles[8]) + scale_factor_middle)
            aux_collision_x = true;
        if (middle_bullet_Y > get<1>(obstacles[8]) &&
            middle_bullet_Y < get<1>(obstacles[8]) + scale_factor_middle)
            aux_collision_y = true;

        if (aux_collision_x && aux_collision_y)
        {
            continue;
        }

        aux_collision_x = false;
        aux_collision_y = false;

        // collision with horizontal obstacles
        if (middle_bullet_X > get<0>(obstacles[0]) + (float)length &&
            middle_bullet_X < get<0>(obstacles[0]) + scale_factor_side_2)
            aux_collision_x = true;
        if (middle_bullet_Y > get<1>(obstacles[0]) &&
            middle_bullet_Y < get<1>(obstacles[0]) + scale_factor_side_1)
            aux_collision_y = true;

        if (aux_collision_x && aux_collision_y)
        {
            continue;
        }

        aux_collision_x = false;
        aux_collision_y = false;

        // collision with horizontal obstacles
        if (middle_bullet_X > get<0>(obstacles[1]) + (float)length &&
            middle_bullet_X < get<0>(obstacles[1]) + scale_factor_side_2)
            aux_collision_x = true;
        if (middle_bullet_Y > get<1>(obstacles[1]) &&
            middle_bullet_Y < get<1>(obstacles[1]) + scale_factor_side_1)
            aux_collision_y = true;

        if (aux_collision_x && aux_collision_y)
        {
            continue;
        }

        aux_collision_x = false;
        aux_collision_y = false;

        // collision with horizontal obstacles
        if (middle_bullet_X > get<0>(obstacles[2]) + (float)length &&
            middle_bullet_X < get<0>(obstacles[2]) + scale_factor_side_2)
            aux_collision_x = true;
        if (middle_bullet_Y > get<1>(obstacles[2]) &&
            middle_bullet_Y < get<1>(obstacles[2]) + scale_factor_side_1)
            aux_collision_y = true;

        if (aux_collision_x && aux_collision_y)
        {
            continue;
        }

        aux_collision_x = false;
        aux_collision_y = false;

        // collision with horizontal obstacles
        if (middle_bullet_X > get<0>(obstacles[3]) + (float)length &&
            middle_bullet_X < get<0>(obstacles[3]) + scale_factor_side_2)
            aux_collision_x = true;
        if (middle_bullet_Y > get<1>(obstacles[3]) &&
            middle_bullet_Y < get<1>(obstacles[3]) + scale_factor_side_1)
            aux_collision_y = true;

        if (aux_collision_x && aux_collision_y)
        {
            continue;
        }

        aux_collision_x = false;
        aux_collision_y = false;

        // collision with vertical obstacles
        if (middle_bullet_X > get<0>(obstacles[4]) + (float)length &&
            middle_bullet_X < get<0>(obstacles[4]) + scale_factor_side_1 + (float)length / 2)
            aux_collision_x = true;
        if (middle_bullet_Y > get<1>(obstacles[4]) &&
            middle_bullet_Y < get<1>(obstacles[4]) + scale_factor_side_2 + (float)length / 2)
            aux_collision_y = true;

        if (aux_collision_x && aux_collision_y)
        {
            continue;
        }

        aux_collision_x = false;
        aux_collision_y = false;

        // collision with vertical obstacles
        if (middle_bullet_X > get<0>(obstacles[5]) + (float)length &&
            middle_bullet_X < get<0>(obstacles[5]) + scale_factor_side_1 + (float)length / 2)
            aux_collision_x = true;
        if (middle_bullet_Y > get<1>(obstacles[5]) &&
            middle_bullet_Y < get<1>(obstacles[5]) + scale_factor_side_2 + (float)length / 2)
            aux_collision_y = true;

        if (aux_collision_x && aux_collision_y)
        {
            continue;
        }

        aux_collision_x = false;
        aux_collision_y = false;

        // collision with vertical obstacles
        if (middle_bullet_X > get<0>(obstacles[6]) + (float)length &&
            middle_bullet_X < get<0>(obstacles[6]) + scale_factor_side_1 + (float)length / 2)
            aux_collision_x = true;
        if (middle_bullet_Y > get<1>(obstacles[6]) &&
            middle_bullet_Y < get<1>(obstacles[6]) + scale_factor_side_2 + (float)length / 2)
            aux_collision_y = true;

        if (aux_collision_x && aux_collision_y)
        {
            continue;
        }

        aux_collision_x = false;
        aux_collision_y = false;

        // collision with vertical obstacles
        if (middle_bullet_X > get<0>(obstacles[7]) + (float)length &&
            middle_bullet_X < get<0>(obstacles[7]) + scale_factor_side_1 + (float)length / 2)
            aux_collision_x = true;
        if (middle_bullet_Y > get<1>(obstacles[7]) &&
            middle_bullet_Y < get<1>(obstacles[7]) + scale_factor_side_2 + (float)length / 2)
            aux_collision_y = true;

        if (aux_collision_x && aux_collision_y)
        {
            continue;
        }

        // after a certain distance the bullet disappears
        if (get<4>(bullets[i]) > max_distance)
        {
            continue;
        }

        std::vector<std::tuple<float, float, float>> antagonists_new;
        int j, collision = 0;
        for (j = 0; j < antagonists.size(); j++)
        {
            int aux1 = ((get<0>(bullets[i]) > get<0>(antagonists[j])) && (get<0>(bullets[i]) < get<0>(antagonists[j]) + length));
            int aux2 = ((get<1>(bullets[i]) > get<1>(antagonists[j])) && (get<1>(bullets[i]) < get<1>(antagonists[j]) + length));
            
            float middle_antagonist_X = get<0>(antagonists[j]) + (float) length / 2;
            float middle_antagonist_Y = get<1>(antagonists[j]) + (float) length / 2;
            float distance = sqrt((middle_bullet_X - middle_antagonist_X) * (middle_bullet_X - middle_antagonist_X) +
                (middle_bullet_Y - middle_antagonist_Y) * (middle_bullet_Y - middle_antagonist_Y));

            if (middle_bullet_X < middle_antagonist_X && distance > (float)length / 2)
            {
                antagonists_new.push_back(antagonists[j]);
            }
            else if (middle_bullet_X > middle_antagonist_X && distance > 3 * (float)length / 2)
            {
                antagonists_new.push_back(antagonists[j]);
            }
            else 
            {
                collision = 1;
                score++;
                break;
            }
        }
        for (int k = j + 1; k < antagonists.size(); k++)
        {
            antagonists_new.push_back(antagonists[k]);
        }
        if (collision == 0)
        {
            bullets_new.push_back(bullets[i]);
        }
        antagonists = antagonists_new;
    }
    bullets = bullets_new;

    // render the healthbar
    float pos_health_bar_y = logicSpace.y + logicSpace.height - 1.2;
    float pos_health_bar_x = logicSpace.x + logicSpace.width;

    modelMatrix = visMatrix * helpers::Translate(pos_health_bar_x, pos_health_bar_y);
    RenderMesh2D(meshes["full_life"], shaders["VertexNormal"], modelMatrix);
    glClear(GL_DEPTH_BUFFER_BIT);

    modelMatrix = visMatrix * helpers::Translate(pos_health_bar_x, pos_health_bar_y);
    modelMatrix *= helpers::Translate((float)healthBar_width / 2, (float) healthBar_height / 2);
    modelMatrix *= helpers::Rotate(M_PI);
    modelMatrix *= helpers::Translate(-(float)healthBar_width / 2, -(float)healthBar_height / 2);
    modelMatrix *= helpers::Scale((float)(10 - lost_life) / 10, 1);
    RenderMesh2D(meshes["actual_life"], shaders["VertexNormal"], modelMatrix);
}


void Tema1::FrameEnd()
{
}


int Tema1::checkObstacleCollision(float pos_x, float pos_y)
{
    float middle_pl_x, middle_pl_y;

    middle_pl_x = pos_x + (float)length / 2;
    middle_pl_y = pos_y + (float)length / 2;

    bool aux1 = false, aux2 = false;

    // collision with big obstacle
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[8]) &&
        middle_pl_x < get<0>(obstacles[8]) + scale_factor_middle + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[8]) &&
        middle_pl_y < get<1>(obstacles[8]) + scale_factor_middle + (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    aux1 = false;
    aux2 = false;

    // collision with horizontal obstacles
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[0]) &&
        middle_pl_x < get<0>(obstacles[0]) + scale_factor_side_2 + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[0]) &&
        middle_pl_y < get<1>(obstacles[0]) + scale_factor_side_1 + (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    aux1 = false;
    aux2 = false;

    // collision with horizontal obstacles
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[1]) &&
        middle_pl_x < get<0>(obstacles[1]) + scale_factor_side_2 * length + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[1]) &&
        middle_pl_y < get<1>(obstacles[1]) + scale_factor_side_1  * length+ (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    aux1 = false;
    aux2 = false;

    // collision with horizontal obstacles
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[1]) &&
        middle_pl_x < get<0>(obstacles[1]) + scale_factor_side_2 + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[1]) &&
        middle_pl_y < get<1>(obstacles[1]) + scale_factor_side_1 + (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    aux1 = false;
    aux2 = false;

    // collision with horizontal obstacles
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[2]) &&
        middle_pl_x < get<0>(obstacles[2]) + scale_factor_side_2 + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[2]) &&
        middle_pl_y < get<1>(obstacles[2]) + scale_factor_side_1 + (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    aux1 = false;
    aux2 = false;

    // collision with horizontal obstacles
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[3]) &&
        middle_pl_x < get<0>(obstacles[3]) + scale_factor_side_2 + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[3]) &&
        middle_pl_y < get<1>(obstacles[3]) + scale_factor_side_1 + (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    aux1 = false;
    aux2 = false;

    // collision with vertical obstacles
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[4]) &&
        middle_pl_x < get<0>(obstacles[4]) + scale_factor_side_1 + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[4]) &&
        middle_pl_y < get<1>(obstacles[4]) + scale_factor_side_2 + (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    aux1 = false;
    aux2 = false;

    // collision with vertical obstacles
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[5]) &&
        middle_pl_x < get<0>(obstacles[5]) + scale_factor_side_1 * length + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[5]) &&
        middle_pl_y < get<1>(obstacles[5]) + scale_factor_side_2 * length + (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    aux1 = false;
    aux2 = false;

    // collision with vertical obstacles
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[6]) &&
        middle_pl_x < get<0>(obstacles[6]) + scale_factor_side_1 + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[6]) &&
        middle_pl_y < get<1>(obstacles[6]) + scale_factor_side_2 + (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    aux1 = false;
    aux2 = false;

    // collision with vertical obstacles
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[7]) &&
        middle_pl_x < get<0>(obstacles[7]) + scale_factor_side_1 + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[7]) &&
        middle_pl_y < get<1>(obstacles[7]) + scale_factor_side_2 + (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    aux1 = false;
    aux2 = false;

    // collision with horizontal obstacles
    if (middle_pl_x + (float)length / 2 > get<0>(obstacles[3]) &&
        middle_pl_x < get<0>(obstacles[3]) + scale_factor_side_2 + (float)length / 2)
        aux1 = true;
    if (middle_pl_y + (float)length / 2 > get<1>(obstacles[3]) &&
        middle_pl_y < get<1>(obstacles[3]) + scale_factor_side_1 + (float)length / 2)
        aux2 = true;

    if (aux1 && aux2)
    {
        return 1;
    }

    return 0;
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    float aux_x, aux_y;
    float logic_space_x, logic_space_y;
    float obstacle_collision = 0;

    if (window->KeyHold(GLFW_KEY_W))
    {
        logic_space_y = logicSpace.y + deltaTime * 3;
        aux_y = logic_space_y + logicSpace.height / 2 - 0.5;
        
        // check collision with obstacles
        obstacle_collision = checkObstacleCollision(jugadorX, aux_y);

        if (!((aux_y > length1 - length - (float)length / 10)) && obstacle_collision == 0)
        {
            jugadorY = aux_y;
            logicSpace.y += deltaTime * 3;
        }
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        logic_space_x = logicSpace.x - deltaTime * 3;
        aux_x = logic_space_x + logicSpace.width / 2 - 0.5;

        // check collision with obstacles
        obstacle_collision = checkObstacleCollision(aux_x, jugadorY);

        if (!(aux_x < (float)length / 10) && obstacle_collision == 0)
        {
            jugadorX = aux_x;
            logicSpace.x -= deltaTime * 3;
        }
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        logic_space_y = logicSpace.y - deltaTime * 3;
        aux_y = logic_space_y + logicSpace.height / 2 - 0.5;

        // check collision with obstacles
        obstacle_collision = checkObstacleCollision(jugadorX, aux_y);

        if (!(aux_y < (float)length / 10) && obstacle_collision == 0)
        {
            jugadorY = aux_y;
            logicSpace.y -= deltaTime * 3;
        }
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        logic_space_x = logicSpace.x + deltaTime * 3;
        aux_x = logic_space_x + logicSpace.width / 2 - 0.5;

        // check collision with obstacles
        obstacle_collision = checkObstacleCollision(aux_x, jugadorY);

        if (!(aux_x > length2 - length - (float)length / 10) && obstacle_collision == 0)
        {
            jugadorX = aux_x;
            logicSpace.x += deltaTime * 3;
        }
    }

    
    if (window->KeyHold(GLFW_KEY_Z))
    {
        logicSpace.x += deltaTime / 2;
        logicSpace.y += deltaTime / 2;

        logicSpace.width -= deltaTime;
        logicSpace.height -= deltaTime;
 
        jugadorX = logicSpace.x + logicSpace.width / 2 - 0.5;
        jugadorY = logicSpace.y + logicSpace.height / 2 - 0.5;
    }
    if (window->KeyHold(GLFW_KEY_X))
    {
        logicSpace.x -= deltaTime / 2;
        logicSpace.y -= deltaTime / 2;

        logicSpace.width += deltaTime;
        logicSpace.height += deltaTime;

        jugadorX = logicSpace.x + logicSpace.width / 2 - 0.5;
        jugadorY = logicSpace.y + logicSpace.height / 2 - 0.5;
    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        if (prev_bullet > fire_rate) {
            bullets.push_back(make_tuple(jugadorX + length, jugadorY + (float)((float)(4 * length)) / 10, 0, 0.0, 0.0));
            prev_bullet = 0;
        }
    }
    if (prev_bullet <= 1) {
        prev_bullet += deltaTime;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_M)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float X = mouseX - (float)window->GetResolution()[0] / 2;
    float Y = (window->GetResolution()[1] - mouseY) - (float) window->GetResolution()[1] / 2;
    // compute the angle
    angularStep = atan(Y / X);
    if ((X < 0  && Y > 0) || (Y < 0 && X < 0))
        angularStep += 3.1408;
    if (Y == 0 && X < 0)
        angularStep = 3.1408;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

void Tema1::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec3)));

    glBindVertexArray(0);

    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}
