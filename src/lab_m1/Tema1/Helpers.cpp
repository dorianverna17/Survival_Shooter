#include "Helpers.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;


Mesh* helpers::CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* helpers::CreateDreptunghi(const std::string& name, glm::vec3 leftBottomCorner, float length1, float length2, glm::vec3 color, bool fill )
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length1, 0, 0), color),
        VertexFormat(corner + glm::vec3(length1, length2, 0), color),
        VertexFormat(corner + glm::vec3(0, length2, 0), color)
    };

    Mesh* dreptunghi = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        dreptunghi->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    dreptunghi->InitFromData(vertices, indices);
    return dreptunghi;
}

// Translate matrix
glm::mat3 helpers::Translate(float translateX, float translateY)
{
    return glm::transpose(
        glm::mat3(1, 0, translateX,
                  0, 1, translateY,
                  0, 0, 1)
    );
}

// Scale matrix
glm::mat3 helpers::Scale(float scaleX, float scaleY)
{
    return glm::transpose(
        glm::mat3(scaleX, 0, 0,
                  0, scaleY, 0,
                  0, 0, 1)
    );

}

// Rotate matrix
glm::mat3 helpers::Rotate(float radians)
{
    return glm::transpose(
        glm::mat3(cos(radians), -sin(radians), 0,
                  sin(radians), cos(radians), 0,
                  0, 0, 1)
    );

}
