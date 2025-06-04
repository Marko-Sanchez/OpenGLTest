#pragma once

#include <string>
#include <vector>
#include <memory>

#include "vendors/glm/glm.hpp"

/*
* Handles loading 3D objects.
*
* TODO: Implement ASSIMP vendor.
*/
class ModelLoader
{
    public:
    bool LoadOBJ(const std::string& path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
};
