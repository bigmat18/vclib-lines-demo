#include <demo_application.h>
#include <render_utils.h>

int main(int argc, char** argv)
{
    using DemoApplication = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        DemoApplication,
        vcl::ViewerDrawer>;

    DemoApplication tw("Lines demo application");
    tw.fitScene();
    tw.show();

    return 0;
}
