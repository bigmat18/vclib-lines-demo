#include "../lines_object_handler.h"

#ifndef LINES_CUBE_HANDLER
#define LINES_CUBE_HANDLER

class LinesCubeHandler : public LinesObjectHandler
{
    vcl::lines::CPUGeneratedLines mCpuGeneratedLines;
    vcl::lines::GPUGeneratedLines mGpuGeneratedLines;
    vcl::lines::InstancingBasedLines mInstancingBasedLines;
    vcl::lines::IndirectBasedLines mIndirectBasedLines;
    vcl::lines::TextureBasedLines mTextureBasedLines;

    std::vector<vcl::lines::LinesVertex> mPoints;
    vcl::lines::LinesTypes mType =
        vcl::lines::LinesTypes::CPU_GENERATED;

    uint mNumLines = 100;
    uint mCubeSize = 3;

public:
    LinesCubeHandler(const std::string name, const std::string info);

    ~LinesCubeHandler() = default;

    virtual void drawObject(uint viewId) override { getLines().draw(viewId); }

    virtual void udpateRandom(uint num) override;

    virtual void drawImGuiSettings() override;

private:
    vcl::lines::Lines& getLines();

    void generateLines();
};
#endif