#include <lines_objects_handlers/mesh_handler.h>
#include <imgui.h>

MeshHandler::MeshHandler(const std::string path) : LinesObjectHandler("", "")
{
    generateMesh(path);
    mName = mMesh.name();
    mInfo = "Mesh to test imported from file named " + mMesh.name() + ".obj";
}

void MeshHandler::drawImGuiSettings() 
{
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.25f, screenSize.y), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(screenSize.x - (screenSize.x * 0.25f), 0), ImGuiCond_Always);
    ImGui::Begin("Lines settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::End();
}

void MeshHandler::generateMesh(const std::string path) 
{
    vcl::TriMesh m = vcl::load<vcl::TriMesh>(path);
    vcl::updatePerVertexAndFaceNormals(m);

    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::Gray);

    vcl::MeshRenderSettings settings(m);
    settings.setWireframeVisibility(true);
    settings.setSurfaceVisibility(false);
    settings.setWireframeWidth(3);

    mMesh = vcl::DrawableMesh<vcl::TriMesh>(m);
    mMesh.setRenderSettings(settings);
}