#include <imgui.h>
#include <vclib/bgfx/drawable/lines/drawable_lines.h>

#ifndef LINES_OBJECTS_HANDLER
#define LINES_OBJECTS_HANDLER

class LinesObjectsHandler 
{
public:
    static int ImGuiRender(const std::vector<std::unique_ptr<vcl::DrawableObject>>& objects) {
        int indexSelected = -1;
        int previusIndexSelected = -1;

        ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.2f, screenSize.y), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::Begin("Scene elements", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        

        for(uint i = 0; i < objects.size(); ++i) {
            if(objects[i]->isVisible())
                previusIndexSelected = i;

            float blockHeight = 70.0f;
            
            ImVec2 min = ImGui::GetCursorScreenPos();
            ImVec2 max = ImVec2(min.x + ImGui::GetContentRegionAvail().x, min.y + blockHeight + 5);

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(min, max, ImColor(ImGui::GetStyle().Colors[ImGuiCol_Border]));

            bool visible = objects[i]->isVisible();
            ImGui::SetCursorScreenPos(ImVec2(min.x + 5, min.y + 2));
            if (ImGui::Selectable(objects[i]->name().c_str(), &visible, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0, blockHeight))) {
                for(uint j = 0; j < objects.size(); ++j) {
                    if(j == i)
                        objects[j]->setVisibility(!objects[j]->isVisible());
                    else
                        objects[j]->setVisibility(false);
                }
                indexSelected = i;
            }

            ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - (blockHeight * 0.8)));
            ImGui::Separator();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
            ImGui::TextWrapped("%s", objects[i]->info().c_str());
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
        }

        ImGui::End();

        if(indexSelected == -1)
            return previusIndexSelected;

        return indexSelected;
    }
};

#endif