#include <demo_application.h>
#include <render_utils.h>

int main(int argc, char** argv)
{
    using DemoApplication = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        DemoApplication,
        vcl::ViewerDrawer>;

    DemoApplication tw("Viewer ImGui GLFW");

    vcl::DrawableMesh<vcl::TriMesh> drawable = getMesh<vcl::TriMesh>();
    drawable.setWireframeType(vcl::lines::LinesTypes::CPU_GENERATED);

    tw.pushDrawableObject(drawable);

    tw.fitScene();

    tw.show();

    return 0;
}
