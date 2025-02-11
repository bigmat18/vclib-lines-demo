#include <lines_objects_handlers/mesh_handler.h>
#include <imgui.h>

MeshHandler::MeshHandler() : LinesObjectHandler("", "")
{
    generateMesh();
    mName = mMesh.name();
    mInfo = "Mesh to test imported from " + mMesh.name() + ".obj";
}

void MeshHandler::drawImGuiSettings() {}

void MeshHandler::drawImGuiTesting() {}

void MeshHandler::generateMesh() 
{
    vcl::TriMesh m = vcl::load<vcl::TriMesh>("./bimba.obj");
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