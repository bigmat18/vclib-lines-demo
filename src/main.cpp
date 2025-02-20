#include <demo_application.h>

int main(int argc, char** argv)
{
    using App = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        DemoApplication,
        vcl::ViewerDrawer>;

    App tw("Lines demo application");
    if(argc == 8) {
        tw.actualNumPoints = std::stoul(argv[1]);
        tw.maxNumPoints = std::stoul(argv[2]);
        tw.maxNumFrame = std::stoul(argv[3]);
        tw.stepTests = std::stoul(argv[4]);
        tw.mIndexSelected = atoi(argv[5]);
        tw.setObjType(static_cast<vcl::lines::LinesTypes>(atoi(argv[6])));
        tw.updatePerTest = std::stoul(argv[7]);
        tw.setupTest();
        tw.isTestRunning = true;
        tw.blockWhenTestsEnd = true;
    }
    tw.fitScene();
    tw.show();

    return 0;
}
