#include "../lines_object_handler.h"

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/render/drawable/drawable_mesh.h>

#ifndef MESH_HANDLER
#define MESH_HANDLER
class MeshHandler : public LinesObjectHandler
{
    vcl::DrawableMesh<vcl::TriMesh> mMesh;

public:
    MeshHandler(const std::string path);

    ~MeshHandler() = default;

    virtual void drawObject(uint viewId) override { mMesh.draw(viewId); }

    virtual void drawImGuiSettings() override;
private:
    void generateMesh(const std::string path);
};

#endif