#include "../lines_object_handler.h"
#include <bgfx/bgfx.h>

#ifndef GRAPH_HANDLER
#define GRAPH_HANDLER

class GraphHandler : public LinesObjectHandler 
{

    std::vector<float> mVertices;
    std::vector<uint32_t> mIndices;

    bgfx::VertexBufferHandle VBO = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle IBO = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle mProgram = BGFX_INVALID_HANDLE;

public:
    GraphHandler(const std::string& file);
    ~GraphHandler() = default;

    void drawObject(unsigned int viewId) override;

private:
    void generateSphere(uint32_t stacks, uint32_t slices, float radius);
};

#endif
