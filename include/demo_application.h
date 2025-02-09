#include <vclib/imgui/imgui_drawer.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>
#include <imgui.h>

#include <vclib/bgfx/drawable/lines/drawable_lines.h>
#include "render_utils.h"

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

            ImVec2 screenSize = ImGui::GetIO().DisplaySize;

            ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.25f, screenSize.y), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            
            ImGui::Begin("Scene elements", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

            for(uint i = 0; i < mObjects.size(); ++i) {
                if(mObjects[i]->isVisible())
                    mObjects[i]->draw(viewId);

                float blockHeight = 70.0f;
                
                ImVec2 min = ImGui::GetCursorScreenPos();
                ImVec2 max = ImVec2(min.x + ImGui::GetContentRegionAvail().x + 5, min.y + blockHeight + 5);

                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                draw_list->AddRectFilled(min, max, IM_COL32(34, 55, 83, 128));

                bool visible = mObjects[i]->isVisible();
                ImGui::SetCursorScreenPos(ImVec2(min.x + 5, min.y + 5));
                if (ImGui::Selectable((mObjects[i]->name() + std::to_string(i)).c_str(), &visible, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0, blockHeight))) {
                    mObjects[i]->setVisibility(!mObjects[i]->isVisible());
                }

                ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - (blockHeight * 0.8)));
                ImGui::Separator();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                ImGui::TextWrapped("%s", mObjects[i]->info().c_str());
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30);

            }

            ImGui::End();
        }

    }
};
#endif