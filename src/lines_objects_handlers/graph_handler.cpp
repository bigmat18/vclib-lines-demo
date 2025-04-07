#include <lines_objects_handlers/graph_handler.h>
#include <vclib/bgfx/context/load_program.h>
#include <bx/math.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <sstream>

GraphHandler::GraphHandler(const std::string& fileNode, const std::string& fileEdge, const std::string& filePath) : 
    LinesObjectHandler("Grafo", "Grafo con nodi ed archi in spazio 3D")
{
    loadNodes(fileNode);
    std::vector<Edge> edges = loadEdges(fileEdge);
    loadPath(filePath);

    generateLines(edges);
    mGPULines = vcl::lines::CPUGeneratedLines(mLines);
    mGPULines.settings().setThickness(5);

    mGPUPath = vcl::lines::CPUGeneratedPolylines(mNodesPath);
    mGPUPath.settings().setThickness(10);

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

    for(const auto& pair : mNodes) {
        float mtx[16];
        bx::mtxTranslate(mtx, pair.second.x, pair.second.y, pair.second.z);
        bgfx::setTransform(mtx);
        bgfx::setVertexBuffer(0, VBO);
        bgfx::setIndexBuffer(IBO);
        bgfx::setState(state);
        bgfx::submit(viewId, mProgram);

    }

    mGPULines.draw(viewId);
    mGPUPath.draw(viewId);
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

void GraphHandler::loadNodes(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Errore nell'aprire il file: " << filename << std::endl;
        std::exit(0);
    }
    
    // Salta la prima riga (intestazione)
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Node node;
        char comma;
        
        if (ss >> node.id >> comma >> node.x >> comma >> node.y >> comma >> node.z) {
            mNodes[node.id] = node;
        }
    }
    
    file.close();
}

std::vector<GraphHandler::Edge> GraphHandler::loadEdges(const std::string& filename) 
{
    std::vector<GraphHandler::Edge> edges;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Errore nell'aprire il file: " << filename << std::endl;
        std::exit(0);
    }
    
    // Salta la prima riga (intestazione)
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Edge edge;
        char comma;
        
        if (ss >> edge.node0 >> comma >> edge.node1 >> comma >> edge.value) {
            edges.push_back(edge);
        }
    }
    
    file.close();
    return edges;
}

void GraphHandler::loadPath(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Errore nell'aprire il file: " << filename << std::endl;
        std::exit(0);
    }
    
    // Salta la prima riga (intestazione)
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int id;
        char comma;
        
        if (ss >> id) {
            if(mNodes.find(id) != mNodes.end()) {
                Node node = mNodes[id];
                mNodesPath.push_back(vcl::lines::LinesVertex(node.x, node.y, node.z, vcl::lines::LinesVertex::COLOR(0, 1, 0, 1)));
            }
        }
    }
    
    file.close();
}

void GraphHandler::generateLines(const std::vector<Edge>& edges)
{
    for(auto& edge : edges) {
        if(mNodes.find(edge.node0) != mNodes.end() && mNodes.find(edge.node1) != mNodes.end()) {
            Node n1 = mNodes[edge.node0];
            Node n2 = mNodes[edge.node1];
            mLines.push_back(vcl::lines::LinesVertex(n1.x, n1.y, n1.z, vcl::lines::LinesVertex::COLOR(0, 0, 0, 0.5)));
            mLines.push_back(vcl::lines::LinesVertex(n2.x, n2.y, n2.z, vcl::lines::LinesVertex::COLOR(0, 0, 0, 0.5)));
        } else {
            std::cout << "Chiave non trovata!" << std::endl;
            std::exit(0);
        }
    }
}