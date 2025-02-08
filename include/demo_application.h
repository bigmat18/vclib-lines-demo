#include <vclib/imgui/imgui_drawer.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>
#include <imgui.h>

#include <vclib/bgfx/drawable/lines/drawable_lines.h>

#ifndef DEMO_APPLICATION_H
#define DEMO_APPLICATION_H

template <typename DerivedRenderApp>
class DemoApplication : public vcl::imgui::ImGuiDrawer<DerivedRenderApp>
{
    using ParentDrawer = vcl::imgui::ImGuiDrawer<DerivedRenderApp>;
    
    std::vector<std::unique_ptr<vcl::DrawableObject>> mObjects;

public:
    using ParentDrawer::ParentDrawer;

    DemoApplication(uint, uint) 
    {
        std::vector<vcl::lines::LinesVertex> points = {
            vcl::lines::LinesVertex(0, 0.5, 0, vcl::lines::LinesVertex::COLOR(1, 0, 0, 1)),
            vcl::lines::LinesVertex(0.5, 0, 0, vcl::lines::LinesVertex::COLOR(1, 0, 0, 1)),
        };
        mObjects.push_back(std::make_unique<vcl::lines::DrawableLines<vcl::lines::CPUGeneratedLines>>(
            vcl::lines::DrawableLines<vcl::lines::CPUGeneratedLines>(points)
        ));

    };

    virtual void onDraw(vcl::uint viewId) override
    {
        ParentDrawer::onDraw(viewId);

        if (!ParentDrawer::isWindowMinimized()) {
            for(auto& obj : mObjects)
                obj->draw(viewId);
            ImGui::Begin("Test");
            ImGui::Text("Ciao");
            ImGui::End();
        }

    }
};
#endif