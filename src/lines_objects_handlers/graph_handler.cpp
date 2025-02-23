#include <lines_objects_handlers/graph_handler.h>
#include <vclib/bgfx/context/load_program.h>

GraphHandler::GraphHandler(const std::string& file) : 
    LinesObjectHandler("Grafo", "Grafo con nodi ed archi in spazio 3D")
{
    generateSphere(20, 20, 0.2f);
    bgfx::VertexLayout layout;
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .end();

    VBO = bgfx::createVertexBuffer(
        bgfx::makeRef(&mVertices[0], mVertices.size() * sizeof(float)),
        layout
    );

    IBO = bgfx::createIndexBuffer(
        bgfx::makeRef(&mIndices[0], mIndices.size() * sizeof(uint32_t)),
        BGFX_BUFFER_INDEX32
    );

    mProgram = vcl::loadProgram("vs_cube", "fs_cube");
}

void GraphHandler::drawObject(unsigned int viewId)
{

    uint64_t state = 0
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | UINT64_C(0)
        | BGFX_STATE_BLEND_ALPHA;

    bgfx::setVertexBuffer(0, VBO);
    bgfx::setIndexBuffer(IBO);
    bgfx::setState(state);
    bgfx::submit(viewId, mProgram);
}

void GraphHandler::generateSphere(uint32_t stacks, uint32_t slices, float radius)
{
    for (uint32_t i = 0; i <= stacks; ++i) {
        float v = float(i) / stacks;
        float phi = v * M_PI; // da 0 a pi

        for (uint32_t j = 0; j <= slices; ++j) {
            float u = float(j) / slices;
            float theta = u * 2.0f * M_PI; // da 0 a 2pi

            float x = radius * sinf(phi) * cosf(theta);
            float y = radius * cosf(phi);
            float z = radius * sinf(phi) * sinf(theta);

            mVertices.push_back(x);
            mVertices.push_back(y);
            mVertices.push_back(z);
        }
    }

    for (uint32_t i = 0; i < stacks; ++i) {
        for (uint32_t j = 0; j < slices; ++j) {
            uint32_t first = i * (slices + 1) + j;
            uint32_t second = first + slices + 1;

            mIndices.push_back(first);
            mIndices.push_back(second);
            mIndices.push_back(first + 1);

            mIndices.push_back(second);
            mIndices.push_back(second + 1);
            mIndices.push_back(first + 1);
        }
    }
}