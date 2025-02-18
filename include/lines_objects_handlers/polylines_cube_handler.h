#include "../lines_object_handler.h"

#ifndef POLYLINES_CUBE_HANDLER
#define POLYLINES_CUBE_HANDLER

class PolylinesCubeHandler : public LinesObjectHandler
{
    vcl::lines::CPUGeneratedPolylines mCpuGeneratedPolylines;
    vcl::lines::GPUGeneratedPolylines mGpuGeneratedPolylines;
    vcl::lines::InstancingBasedPolylines mInstancingBasedPolylines;
    vcl::lines::IndirectBasedPolylines mIndirectBasedPolylines;
    vcl::lines::TextureBasedPolylines mTextureBasedPolylines;

    std::vector<vcl::lines::LinesVertex> mPoints;
    vcl::lines::LinesTypes mType =
        vcl::lines::LinesTypes::CPU_GENERATED;

    uint mNumPoints = 100;
    float mCubeSize = 3.0f;

public:
    PolylinesCubeHandler(const std::string name, const std::string info);

    ~PolylinesCubeHandler() = default;

    virtual void drawObject(uint viewId) override { getPolylines().draw(viewId); }

    virtual void udpateRandom(uint num) override;

    virtual void drawImGuiSettings() override;
    
private:
    vcl::lines::Lines& getPolylines();

    void generatePoints();
};
#endif