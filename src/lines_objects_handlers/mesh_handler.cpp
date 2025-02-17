#include <lines_objects_handlers/mesh_handler.h>
#include <imgui.h>

MeshHandler::MeshHandler(const std::string path) : LinesObjectHandler("", "")
{
    generateMesh(path);
    mName = mMesh.name();
    mInfo = "Mesh to test imported from " + mMesh.name() + ".obj";
}

void MeshHandler::drawImGuiSettings() {}

void MeshHandler::drawImGuiTesting() {}

void MeshHandler::generateMesh(const std::string path) 
{
    vcl::TriMesh m = vcl::load<vcl::TriMesh>(path);
    vcl::updatePerVertexAndFaceNormals(m);

    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::Gray);

    vcl::MeshRenderSettings settings(m);
    settings.setWireframeVisibility(true);
    settings.setSurfaceVisibility(false);
    settings.setWireframeWidth(1);

    vcl::DrawableMesh<vcl::TriMesh> drawable(m);
    drawable.setRenderSettings(settings);

    mMesh = drawable;
}