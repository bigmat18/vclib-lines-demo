#include <lines_objects_handlers/lines_cube_handler.h>
#include <imgui.h>
#include <random>
#include <cmath>

LinesCubeHandler::LinesCubeHandler(const std::string name, const std::string info) : 
    LinesObjectHandler(name, info)
{ 
    generateLines(); 

    mCpuGeneratedLines = vcl::lines::CPUGeneratedLines(mPoints);
    mGpuGeneratedLines = vcl::lines::GPUGeneratedLines(mPoints);
    mInstancingBasedLines = vcl::lines::InstancingBasedLines(mPoints);
    mIndirectBasedLines = vcl::lines::IndirectBasedLines(mPoints);
    mTextureBasedLines = vcl::lines::TextureBasedLines(mPoints);
}

void LinesCubeHandler::drawImGuiSettings() 
{
    vcl::lines::LinesSettings &settings = getLines().settings();
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.25f, screenSize.y), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(screenSize.x - (screenSize.x * 0.25f), 0), ImGuiCond_Always);
    ImGui::Begin("Lines settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    
    uint8_t minSize = 0;
    uint8_t maxSize = 50;
    ImGui::Text("Thickness");
    ImGui::SliderScalar("0-50##0", ImGuiDataType_U8, &settings.mThickness, &minSize, &maxSize);

    ImGui::Text("Border");
    ImGui::SliderScalar("0-50##1", ImGuiDataType_U8, &settings.mBorder, &minSize, &maxSize);

    ImGui::Text("Antialias");
    ImGui::SliderScalar("0-50##2", ImGuiDataType_U8, &settings.mAntialias, &minSize, &maxSize);
    
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

    const char* optionsTypes[] = { "CPU Generated", "GPU Generated", "Instancing based",  "Indirect based", "Texture based"};
    ImGui::Text("Lines Render type");
    int type = static_cast<int>(mType);
    if (ImGui::Combo("##4", &type, optionsTypes, IM_ARRAYSIZE(optionsTypes))) {
        mType = static_cast<vcl::lines::LinesTypes>(type);
        vcl::lines::LinesSettings &newSettings = getLines().settings();
        newSettings = settings;
    }

    int previusNumLines = mNumLines;
    int minLinesNum = 1;
    int maxLinesNum = 1000000;
    ImGui::Text("Number of segments");
    ImGui::SliderScalar("1-1.000.000", ImGuiDataType_U32, &mNumLines, &minLinesNum, &maxLinesNum);

    if(ImGui::Button("Update")) {
        generateLines();
        mCpuGeneratedLines.update(mPoints);
        mGpuGeneratedLines.update(mPoints);
        mInstancingBasedLines.update(mPoints);
        mIndirectBasedLines.update(mPoints);
        mTextureBasedLines.update(mPoints);
    }

    ImGui::End();
}

void LinesCubeHandler::drawImGuiTesting() 
{
}

vcl::lines::Lines& LinesCubeHandler::getLines()
{
    switch(mType) {
        case vcl::lines::LinesTypes::CPU_GENERATED:
            return mCpuGeneratedLines;
        case vcl::lines::LinesTypes::GPU_GENERATED:
            return mGpuGeneratedLines;
        case vcl::lines::LinesTypes::INSTANCING_BASED:
            return mInstancingBasedLines;
        case vcl::lines::LinesTypes::INDIRECT_BASED:
            return mIndirectBasedLines;
        case vcl::lines::LinesTypes::TEXTURE_BASED:
            return mTextureBasedLines;
    }
}

void LinesCubeHandler::generateLines() 
{
    mPoints.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> color(0, 1);
    std::uniform_real_distribution<> coord(3.0, -3.0);

    for (int i = 0; i < mNumLines; i++) {
        mPoints.push_back(
            vcl::lines::LinesVertex(coord(gen), coord(gen), coord(gen), 
            vcl::lines::LinesVertex::COLOR(color(gen), color(gen), color(gen), 1.0))
        );
        mPoints.push_back(
            vcl::lines::LinesVertex(coord(gen), coord(gen), coord(gen), 
            vcl::lines::LinesVertex::COLOR(color(gen), color(gen), color(gen), 1.0))
        );
    }
}