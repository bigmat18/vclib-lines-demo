#include <lines_objects_handlers/polylines_cube_handler.h>
#include <imgui.h>
#include <random>
#include <cmath>

PolylinesCubeHandler::PolylinesCubeHandler(const std::string name, const std::string info) : 
    LinesObjectHandler(name, info)
{ 
    generatePoints(); 

    vcl::lines::CPUGeneratedPolylines        cpu(mPoints);
    vcl::lines::GPUGeneratedPolylines        gpu(mPoints);
    vcl::lines::InstancingBasedPolylines     instancing(mPoints);
    vcl::lines::IndirectBasedPolylines       indirect(mPoints);
    vcl::lines::TextureBasedPolylines        texture(mPoints);

    mCpuGeneratedPolylines.swap(cpu);
    mGpuGeneratedPolylines.swap(gpu);
    mInstancingBasedPolylines.swap(instancing);
    mIndirectBasedPolylines.swap(indirect);
    mTextureBasedPolylines.swap(texture);
}

void PolylinesCubeHandler::drawImGuiSettings() 
{
    vcl::lines::LinesSettings &settings = getPolylines().settings();
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.25f, screenSize.y), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(screenSize.x - (screenSize.x * 0.25f), 0), ImGuiCond_Always);
    ImGui::Begin("Lines settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    
    uint8_t minSize = 0;
    uint8_t maxSize = 50;
    ImGui::Text("Thickness");
    ImGui::SliderScalar("0-50##0", ImGuiDataType_U8, &settings.mThickness, &minSize, &maxSize);

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

    int selectedLeftCap = static_cast<int>(settings.mLeftCap);
    const char* optionsCaps[] = { "Butt cap", "Square cap", "Round cap",  "Triangle cap"};

    ImGui::Text("Left cap");
    if (ImGui::Combo("##2", &selectedLeftCap, optionsCaps, IM_ARRAYSIZE(optionsCaps))) {
        settings.setLeftCap(static_cast<vcl::lines::Caps>(selectedLeftCap));
    }

    int selectedRightCap = static_cast<int>(settings.mRigthCap);
    ImGui::Text("Right cap");
    if (ImGui::Combo("##3", &selectedRightCap, optionsCaps, IM_ARRAYSIZE(optionsCaps))) {
        settings.setRigthCap(static_cast<vcl::lines::Caps>(selectedRightCap));
    }

    const char* optionsJoint[] = { "Round", "Bevel", "Miter" };
    int selectedJoint = static_cast<int>(settings.mJoin);
    ImGui::Text("Joints options");
    if (ImGui::Combo("##5", &selectedJoint, optionsJoint, IM_ARRAYSIZE(optionsJoint))) {
        settings.setJoin(static_cast<vcl::lines::Joins>(selectedJoint));
    }

    if(settings.mJoin == vcl::lines::Joins::MITER_JOIN) {
        ImGui::Text("Miter limit");
        int minMiterLimit = settings.mThickness;
        int maxMiterLimit = settings.mThickness * 4;
        ImGui::SliderScalar((std::to_string(minMiterLimit) + "-" + std::to_string(maxMiterLimit)).c_str(), ImGuiDataType_U8, &settings.mMiterLimit, &minMiterLimit, &maxMiterLimit);
    }

    const char* optionsTypes[] = { "CPU Generated", "GPU Generated", "Instancing based",  "Indirect based", "Texture based"};
    ImGui::Text("Lines Render type");
    int type = static_cast<int>(mType);
    if (ImGui::Combo("##4", &type, optionsTypes, IM_ARRAYSIZE(optionsTypes))) {
        mType = static_cast<vcl::lines::LinesTypes>(type);
        vcl::lines::LinesSettings &newSettings = getPolylines().settings();
        newSettings = settings;
    }

    int previusNumPoints = mNumPoints;
    int minPointsNum = 3;
    int maxPointsNum = 1000000;
    ImGui::Text("Number of points");
    ImGui::SliderScalar("3-1.000.000", ImGuiDataType_U32, &mNumPoints, &minPointsNum, &maxPointsNum);

    if(ImGui::Button("Update")) {
        generatePoints();
        mCpuGeneratedPolylines.update(mPoints);
        mGpuGeneratedPolylines.update(mPoints);
        mInstancingBasedPolylines.update(mPoints);
        mIndirectBasedPolylines.update(mPoints);
        mTextureBasedPolylines.update(mPoints);
    }

    ImGui::End();
}

vcl::lines::Lines& PolylinesCubeHandler::getPolylines()
{
    switch(mType) {
        case vcl::lines::LinesTypes::CPU_GENERATED:
            return mCpuGeneratedPolylines;
        case vcl::lines::LinesTypes::GPU_GENERATED:
            return mGpuGeneratedPolylines;
        case vcl::lines::LinesTypes::INSTANCING_BASED:
            return mInstancingBasedPolylines;
        case vcl::lines::LinesTypes::INDIRECT_BASED:
            return mIndirectBasedPolylines;
        case vcl::lines::LinesTypes::TEXTURE_BASED:
            return mTextureBasedPolylines;
    }
}

void PolylinesCubeHandler::generatePoints() 
{
    mPoints.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> color(0, 1);
    std::uniform_real_distribution<> coord(mCubeSize, -mCubeSize);

    for (int i = 0; i < mNumPoints; i++) {
        mPoints.push_back(
            vcl::lines::LinesVertex(coord(gen), coord(gen), coord(gen), 
            vcl::lines::LinesVertex::COLOR(color(gen), color(gen), color(gen), 1.0))
        );
    }
}

void PolylinesCubeHandler::udpateRandom(uint num)
{
    mNumPoints = num;
    generatePoints();
    getPolylines().update(mPoints);
}