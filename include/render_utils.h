#ifndef RENDER_UTILS
#define RENDER_UTILS

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/render/drawable/drawable_mesh.h>

template<vcl::MeshConcept MeshType>
inline vcl::DrawableMesh<MeshType> getMesh(const std::string& filename = "bimba.obj")
{
    MeshType m = vcl::load<MeshType>("./" + filename);
    vcl::updatePerVertexAndFaceNormals(m);

    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::Gray);

    vcl::MeshRenderSettings settings(m);
    settings.setWireframeVisibility(true);
    settings.setSurfaceVisibility(false);

    vcl::DrawableMesh<MeshType> drawable(m);
    drawable.setRenderSettings(settings);

    return drawable;
}

#endif