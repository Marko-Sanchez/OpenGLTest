#include <string>

class Mesh
{
    public:
    Mesh();
    ~Mesh();

    void Clear();
    void LoadMesh(const std::string& filepath);
    void LoadMeshTrivial(const std::string& filepath);

    void Render();
};
