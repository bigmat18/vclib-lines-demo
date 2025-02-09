#include <iostream>
#include <vclib/bgfx/drawable/lines/drawable_lines.h>

#ifndef LINES_SETTINGS_HANDLER
#define LINES_SETTINGS_HANDLER

class LinesSettingsHandler 
{
public:
    static void ImGuiRender(const std::unique_ptr<vcl::DrawableObject>& obj) 
    {
        if(auto* line = dynamic_cast<vcl::lines::DrawableLines<vcl::lines::CPUGeneratedLines>*>(obj.get())) {
            vcl::lines::LinesSettings &settings = line->settings();
            
            ImVec2 screenSize = ImGui::GetIO().DisplaySize;

            ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.25f, screenSize.y), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(screenSize.x - (screenSize.x * 0.25f), 0), ImGuiCond_Always);
            ImGui::Begin("Lines settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            
            uint8_t minValue = 0;
            uint8_t maxValue = 50;
            ImGui::Text("Thickness");
            ImGui::SliderScalar("0-50##0", ImGuiDataType_U8, &settings.mThickness, &minValue, &maxValue);

            ImGui::Text("Border");
            ImGui::SliderScalar("0-50##1", ImGuiDataType_U8, &settings.mBorder, &minValue, &maxValue);


            
            ImGui::Text("Border color");
            vcl::Color colorBorder(settings.mBorderColor, vcl::Color::Format::RGBA);
            ImVec4 borderColor = ImVec4(colorBorder.redF(), colorBorder.greenF(), colorBorder.blueF(), colorBorder.alphaF());
            if(ImGui::ColorPicker4("##0", (float*)&borderColor)) {
                settings.setBorderColor(vcl::lines::LinesVertex::COLOR(borderColor.x, borderColor.y, borderColor.z, borderColor.w));
            }

            bool useGeneralColor = settings.mColorToUse == vcl::lines::ColorToUse::GENERAL_COLOR;
            if(ImGui::Checkbox("Use general color", &useGeneralColor)) {
                if(useGeneralColor)
                    settings.setColorToUse(vcl::lines::ColorToUse::GENERAL_COLOR);
                else
                    settings.setColorToUse(vcl::lines::ColorToUse::PER_VERTEX_COLOR);
            }

            ImGui::Text("General color");
            vcl::Color colorGeneral(settings.mGeneralColor, vcl::Color::Format::RGBA);
            ImVec4 generalColor = ImVec4(colorGeneral.redF(), colorGeneral.greenF(), colorGeneral.blueF(), colorGeneral.alphaF());
            if(ImGui::ColorPicker4("##1", (float*)&generalColor)) {
                settings.setGeneralColor(vcl::lines::LinesVertex::COLOR(generalColor.x, generalColor.y, generalColor.z, generalColor.w));
            }

            int selectedLeftCap = settings.mLeftCap;
            const char* options[] = { "Butt cap", "Square cap", "Round cap",  "Triangle cap"};

            ImGui::Text("Left cap");
            if (ImGui::Combo("##2", &selectedLeftCap, options, IM_ARRAYSIZE(options))) {
                settings.setLeftCap(static_cast<vcl::lines::Caps>(selectedLeftCap));
            }

            int selectedRightCap = settings.mRigthCap;
            ImGui::Text("Right cap");
            if (ImGui::Combo("##3", &selectedRightCap, options, IM_ARRAYSIZE(options))) {
                settings.setRigthCap(static_cast<vcl::lines::Caps>(selectedRightCap));
            }

            ImGui::End();
        }
    }
};

#endif