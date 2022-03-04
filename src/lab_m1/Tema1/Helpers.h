#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace helpers
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = true);

    Mesh* CreateDreptunghi(const std::string& name, glm::vec3 leftBottomCorner, float length1, float length2, glm::vec3 color, bool fill = true);

    glm::mat3 Translate(float translateX, float translateY);
    glm::mat3 Scale(float scaleX, float scaleY);
    glm::mat3 Rotate(float radians);
}
