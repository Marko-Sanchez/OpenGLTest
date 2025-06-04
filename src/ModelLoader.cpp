#include "ModelLoader.h"
#include <cstdio>
#include <cstring>

/*
* Trivial custom 3D .obg loader, inspired by opengl-tutorial.org.
*
* @params:
* {const char*} path: path to .obg file.
* {vector<glm::vec3>&} out_vertices: vertex coordinates (v x y z).
* {vector<glm::vec2>&} out_uvs: vertex texture coordinate (vt x y)
* {vector<glm::vec3>&} out_normals: vertex normal (vn x y z).
*
* @note: there is a second variable in the obj file 'f' (face) which represents the 3 vertex of a triangle
* (ex. "f a_1/b_1/c_1 a_2/b_2/c_2 a_3/b_3/c_3")
* Where a is the index of which vertice to use, b index of which uv to use, and c which normal
* to use.
* @ref: https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
*/
bool ModelLoader::LoadOBJ(const std::string& path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{
    const size_t HEADER_SIZE{256};
    const size_t COMMENT_BUFFER{1024};

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::unique_ptr<FILE, decltype(std::fclose)&> file(std::fopen(path.c_str(), "r"), std::fclose);
    if (file && file.get() == nullptr)
        return false;

    while (true)
    {
        char lineheader[HEADER_SIZE];
        if (auto res = std::fscanf(file.get(), "%s", lineheader); res == EOF)
            break;


        // vertices.
        if ( std::strcmp(lineheader, "v") == 0)
        {
            glm::vec3 vertex;
            std::fscanf(file.get(), "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.emplace_back(vertex);
        }
        // texture coordinate of a vertex.
        else if (std::strcmp(lineheader, "vt") == 0)
        {
            glm::vec2 uv;
            std::fscanf(file.get(), "%f %f\n", &uv.x, &uv.y);
            temp_uvs.emplace_back(uv);
        }
        // normal of a vertex.
        else if (std::strcmp(lineheader, "vn") == 0)
        {
            glm::vec3 normal;
            std::fscanf(file.get(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.emplace_back(normal);
        }
        else if (std::strcmp(lineheader, "f") == 0)
        {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            auto matches = std::fscanf(file.get(), "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                           &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1],
                           &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9)
            {
                return false;
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        // comment, irrevelant.
        else
        {
            char discardBuffer[COMMENT_BUFFER];
            std::fgets(discardBuffer, std::size(discardBuffer), file.get());
        }
    }// while.

    // construct vertex, uv, vn now that we know which index to use from 'f'.
    for (size_t i{0}; i < vertexIndices.size(); ++i)
    {
        auto vertexIndex{vertexIndices[i]};
        auto uvIndex{uvIndices[i]};
        auto normalIndex{normalIndices[i]};

         glm::vec3 vertex{temp_vertices[vertexIndex - 1]};
         glm::vec2 uv{temp_uvs[uvIndex - 1]};
         glm::vec3 normal{temp_normals[normalIndex - 1]};

        out_vertices.emplace_back(vertex);
        out_uvs.emplace_back(uv);
        out_normals.emplace_back(normal);
    }

    return true;
}
