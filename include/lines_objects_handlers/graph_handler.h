#include "../lines_object_handler.h"
#include <bgfx/bgfx.h>

#ifndef GRAPH_HANDLER
#define GRAPH_HANDLER

class GraphHandler : public LinesObjectHandler 
{

public:
    struct Node {
        int id;
        float x, y, z;
    };

    struct Edge {
        int node0, node1;
        float value;
    };

private:
    std::vector<float> mVertices;
    std::vector<uint32_t> mIndices;

    std::unordered_map<int, Node> mNodes;

    bgfx::VertexBufferHandle VBO = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle IBO = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle mProgram = BGFX_INVALID_HANDLE;

    std::vector<vcl::lines::LinesVertex> mLines;
    std::vector<vcl::lines::LinesVertex> mNodesPath;

    vcl::lines::GPUGeneratedLines mGPULines;
    vcl::lines::GPUGeneratedPolylines mGPUPath;

public:
    GraphHandler(const std::string& fileNode, const std::string& fileEdge, const std::string& filePath);
    ~GraphHandler() = default;

    void drawObject(unsigned int viewId) override;

private:
    void generateSphere(uint32_t stacks, uint32_t slices, float radius);

    void loadNodes(const std::string& filename);

    std::vector<Edge> loadEdges(const std::string& filename);

    void loadPath(const std::string& filename);

    void generateLines(const std::vector<Edge>& edges);

};

#endif
