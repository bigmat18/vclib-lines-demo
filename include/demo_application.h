#include <vclib/imgui/imgui_drawer.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

#include "lines_objects_handler.h"
#include "lines_settings_handler.h"
#include "render_utils.h"

#ifndef DEMO_APPLICATION_H
#define DEMO_APPLICATION_H

template <typename DerivedRenderApp>
class DemoApplication : public vcl::imgui::ImGuiDrawer<DerivedRenderApp>
{
    using ParentDrawer = vcl::imgui::ImGuiDrawer<DerivedRenderApp>;
    std::vector<std::unique_ptr<vcl::DrawableObject>> mObjects;
    int mIndexSelected = -1;

public:
    using ParentDrawer::ParentDrawer;

    DemoApplication(uint, uint) 
    {

        std::vector<vcl::lines::LinesVertex> points;

        generatePointsInCube(points, 3, 1000);
        vcl::lines::DrawableLines<vcl::lines::CPUGeneratedLines> line1 = vcl::lines::DrawableLines<vcl::lines::CPUGeneratedLines>(points);
        line1.setName("Lines in cube 1");
        line1.setInfo("Set of lines in a cube of size " + std::to_string(5) + " with " + std::to_string(points.size() / 2) + " segments inside");
        line1.setVisibility(false);

        generatePointsInCube(points, 3, 1000);
        vcl::lines::DrawableLines<vcl::lines::CPUGeneratedLines> line2 = vcl::lines::DrawableLines<vcl::lines::CPUGeneratedLines>(points);
        line2.setName("Lines in cube 2");
        line2.setInfo("Set of lines in a cube of size " + std::to_string(5) + " with " + std::to_string(points.size() / 2) + " segments inside");
        line2.setVisibility(false);

        mObjects.push_back(std::make_unique<vcl::lines::DrawableLines<vcl::lines::CPUGeneratedLines>>(line1));
        mObjects.push_back(std::make_unique<vcl::lines::DrawableLines<vcl::lines::CPUGeneratedLines>>(line2));
    };

    virtual void onDraw(vcl::uint viewId) override
    {
        ParentDrawer::onDraw(viewId);

        if (!ParentDrawer::isWindowMinimized()) {
            int indexSelected = LinesObjectsHandler::ImGuiRender(mObjects);
            if(indexSelected != mIndexSelected)
                mIndexSelected = indexSelected;

            if(mIndexSelected != -1)
                LinesSettingsHandler::ImGuiRender(mObjects[mIndexSelected]);

            if(mIndexSelected != -1 && mObjects[mIndexSelected]->isVisible())
                mObjects[mIndexSelected]->draw(viewId);
        }
    }
};
#endif