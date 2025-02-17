#include <demo_application.h>

int main(int argc, char** argv)
{
    using App = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        DemoApplication,
        vcl::ViewerDrawer>;

    App tw("Lines demo application");
    tw.fitScene();
    tw.show();

    return 0;
}
