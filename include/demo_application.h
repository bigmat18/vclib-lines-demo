#include <vclib/imgui/imgui_drawer.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

#include "lines_objects_handlers/lines_cube_handler.h"

#ifndef DEMO_APPLICATION_H
#define DEMO_APPLICATION_H

template <typename DerivedRenderApp>
class DemoApplication : public vcl::imgui::ImGuiDrawer<DerivedRenderApp>
{
    using ParentDrawer = vcl::imgui::ImGuiDrawer<DerivedRenderApp>;
    std::vector<std::unique_ptr<LinesObjectHandler>> mObjects;
    int mIndexSelected = -1;

public:
    using ParentDrawer::ParentDrawer;

    DemoApplication(uint, uint) 
    {
        mObjects.push_back(std::make_unique<LinesCubeHandler>("Lines 1", "Set of lines with 100 points and 200 edge"));
        mObjects.push_back(std::make_unique<LinesCubeHandler>("Lines 2", "Set of lines with 100 points and 200 edge"));
    };

    virtual void onDraw(vcl::uint viewId) override
    {
        ParentDrawer::onDraw(viewId);

        if (!ParentDrawer::isWindowMinimized()) {
            int indexSelected = drawImGuiScene();
            if(indexSelected != mIndexSelected)
                mIndexSelected = indexSelected;

            if(mIndexSelected != -1 && mObjects[mIndexSelected]->isVisible()) {
                mObjects[mIndexSelected]->drawObject(viewId);
                mObjects[mIndexSelected]->drawImGuiSettings();
            }
        }
    }

    int drawImGuiScene() 
    {
        int indexSelected = -1;
        int previusIndexSelected = -1;

        ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.2f, screenSize.y), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::Begin("Scene elements", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        for(uint i = 0; i < mObjects.size(); ++i) {
            if(mObjects[i]->isVisible())
                previusIndexSelected = i;

            float blockHeight = 70.0f;
            
            ImVec2 min = ImGui::GetCursorScreenPos();
            ImVec2 max = ImVec2(min.x + ImGui::GetContentRegionAvail().x, min.y + blockHeight + 5);

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(min, max, ImColor(ImGui::GetStyle().Colors[ImGuiCol_Border]));

            bool visible = mObjects[i]->isVisible();
            ImGui::SetCursorScreenPos(ImVec2(min.x + 5, min.y + 2));
            if (ImGui::Selectable((mObjects[i]->name() + "##" + std::to_string(i)).c_str(), &visible, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0, blockHeight))) {
                for(uint j = 0; j < mObjects.size(); ++j) {
                    if(j == i)
                        mObjects[j]->setVisibility(!mObjects[j]->isVisible());
                    else
                        mObjects[j]->setVisibility(false);
                }
                indexSelected = i;
            }

            ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - (blockHeight * 0.8)));
            ImGui::Separator();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
            ImGui::TextWrapped("%s", mObjects[i]->info().c_str());
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);
        }

        ImGui::End();

        if(indexSelected == -1)
            return previusIndexSelected;

        return indexSelected;
    }
};
#endif