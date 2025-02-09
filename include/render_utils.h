#ifndef RENDER_UTILS
#define RENDER_UTILS

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/render/drawable/drawable_mesh.h>

#include <random>
#include <cmath>

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

void generatePointsInCube(std::vector<vcl::lines::LinesVertex> &points, float size, float n) {
    points.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> color(0, 1);
    std::uniform_real_distribution<> coord(size, -size);

    for (int i = 0; i < n; i++) {
        points.push_back(
            vcl::lines::LinesVertex(coord(gen), coord(gen), coord(gen), 
            vcl::lines::LinesVertex::COLOR(color(gen), color(gen), color(gen), 1.0))
        );
        points.push_back(
            vcl::lines::LinesVertex(coord(gen), coord(gen), coord(gen), 
            vcl::lines::LinesVertex::COLOR(color(gen), color(gen), color(gen), 1.0))
        );
    }
}

#endif