#include <vclib/imgui/imgui_drawer.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

#include "lines_objects_handlers/lines_cube_handler.h"
#include "lines_objects_handlers/polylines_cube_handler.h"
#include "lines_objects_handlers/mesh_handler.h"
#include "lines_objects_handlers/graph_handler.h"

#include <chrono>

#ifndef DEMO_APPLICATION_H
#define DEMO_APPLICATION_H

template <typename DerivedRenderApp>
class DemoApplication : public vcl::imgui::ImGuiDrawer<DerivedRenderApp>
{
    using ParentDrawer = vcl::imgui::ImGuiDrawer<DerivedRenderApp>;
    std::vector<std::unique_ptr<LinesObjectHandler>> mObjects;
    
    std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();
    
public:
    int mIndexSelected = -1;
    unsigned int maxNumPoints = 1000;
    #ifndef __APPLE__
    unsigned int actualNumPoints = 0;
    #else
    unsigned int actualNumPoints = 1;
    #endif

    unsigned int maxNumFrame = 1000;
    unsigned int actualNumFrame = 0;
    
    unsigned int stepTests = 10;
    unsigned int updatePerTest = 0;
    float avgFPS = 0;
    bool isTestRunning = false;
    bool blockWhenTestsEnd = false;


public:
    using ParentDrawer::ParentDrawer;

    DemoApplication(uint, uint) 
    {
        mObjects.push_back(std::make_unique<LinesCubeHandler>("Lines in cube", "Set of lines inside a cube with random position and random params"));
        mObjects.push_back(std::make_unique<PolylinesCubeHandler>("Polylines in cube", "A single polylines makes with random points and data"));
        mObjects.push_back(std::make_unique<MeshHandler>("greek_helmet.obj"));
        mObjects.push_back(std::make_unique<GraphHandler>("nodes.csv", "edges.csv", "shortest_path.csv"));
    };

    virtual void onDraw(vcl::uint viewId) override
    {

        if(isTestRunning) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            if(actualNumPoints <= maxNumPoints) {
                if(actualNumFrame < maxNumFrame) {
                    std::cerr << actualNumPoints << "," << deltaTime.count() << "\n";
                    actualNumFrame++;

                    if(updatePerTest != 0 && actualNumFrame % updatePerTest == 0)
                        mObjects[mIndexSelected]->udpateRandom(actualNumPoints);
                } else {
                    actualNumPoints += stepTests;
                    actualNumFrame = 0;
                    mObjects[mIndexSelected]->udpateRandom(actualNumPoints);
                }
            } else {
                maxNumPoints = 1000;
                actualNumPoints = 10;
                
                maxNumFrame = 1000;
                actualNumFrame = 0;
                
                stepTests = 10;
                isTestRunning = false;
            }
        } else if(blockWhenTestsEnd) {
            std::exit(1);
        }


        ParentDrawer::onDraw(viewId);

        if (!ParentDrawer::isWindowMinimized()) {
            if(!isTestRunning) {
                int indexSelected = drawImGuiScene();
                // int indexSelected = 2;
                if(indexSelected != mIndexSelected)
                    mIndexSelected = indexSelected;
            }

            if(mIndexSelected != -1) {
                mObjects[mIndexSelected]->drawObject(viewId);

                if(!isTestRunning) {
                    mObjects[mIndexSelected]->drawImGuiSettings();
                    drawImGuiTesting();
                }
            }
        }
    }

    void setObjType(vcl::lines::LinesTypes type)
    {
        mObjects[mIndexSelected]->setType(type);
    }

    void setupTest()
    {
        lastTime = std::chrono::high_resolution_clock::now();
        mObjects[mIndexSelected]->udpateRandom(actualNumPoints);
    }

    int drawImGuiScene() 
    {
        int indexSelected = -1;
        int previusIndexSelected = -1;

        ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.15f, screenSize.y), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::Begin("Scene elements", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        for(uint i = 0; i < mObjects.size(); ++i) {
            if(mObjects[i]->isVisible())
                previusIndexSelected = i;

            float blockHeight = 70.0f;
            
            ImVec2 min = ImGui::GetCursorScreenPos();
            ImVec2 max = ImVec2(min.x + ImGui::GetContentRegionAvail().x, min.y + blockHeight + 5);

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(min, max, ImColor(ImGui::GetStyle().Colors[ImGuiCol_Border]));

            bool visible = mObjects[i]->isVisible();
            ImGui::SetCursorScreenPos(ImVec2(min.x + 5, min.y + 2));
            if (ImGui::Selectable((mObjects[i]->name() + "##" + std::to_string(i)).c_str(), &visible, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0, blockHeight))) {
                for(uint j = 0; j < mObjects.size(); ++j) {
                    if(j == i)
                        mObjects[j]->setVisibility(!mObjects[j]->isVisible());
                    else
                        mObjects[j]->setVisibility(false);
                }
                indexSelected = i;
            }

            ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - (blockHeight * 0.8)));
            ImGui::Separator();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
            ImGui::TextWrapped("%s", mObjects[i]->info().c_str());
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);
        }

        ImGui::End();

        if(indexSelected == -1)
            return previusIndexSelected;

        return indexSelected;
    }

    void drawImGuiTesting() 
    {
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    
        ImGui::SetNextWindowSize(ImVec2(screenSize.x - (screenSize.x * 0.25f) - (screenSize.x * 0.15f), screenSize.y * 0.2), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.15, screenSize.y * 0.8), ImGuiCond_Always);
        ImGui::Begin("Lines Tests", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        
        #ifndef __APPLE__
        unsigned int minStartTest = 0;
        #else
        unsigned int minStartTest = 1;
        #endif
        
        unsigned int maxStartTest = maxNumPoints - 1;
        ImGui::Text("Start test value");
        if(ImGui::InputScalar((std::to_string(minStartTest) + "-" + std::to_string(maxStartTest) + "##0").c_str(), ImGuiDataType_U32, &actualNumPoints)) {
            if (actualNumPoints < minStartTest) actualNumPoints = minStartTest;
            if (actualNumPoints > maxStartTest) actualNumPoints = maxStartTest;
        }
        
        unsigned int minEndTest = actualNumPoints + 1;
        unsigned int maxEndTest = 1000000;
        ImGui::Text("End test value");
        if(ImGui::InputScalar((std::to_string(minEndTest) + "-" + std::to_string(maxEndTest) + "##1").c_str(), ImGuiDataType_U32, &maxNumPoints)) {
            if (maxNumPoints < minEndTest) maxNumPoints = minEndTest;
            if (maxNumPoints > maxEndTest) maxNumPoints = maxEndTest;
        }

        unsigned int minStepTest = 1;
        unsigned int maxStepTest = maxNumPoints - actualNumPoints;
        ImGui::Text("Steps for tests");
        if(ImGui::InputScalar((std::to_string(minStepTest) + "-" + std::to_string(maxStepTest) + + "##2").c_str(), ImGuiDataType_U32, &stepTests)) {
            if (stepTests < minStepTest) stepTests = minStepTest;
            if (stepTests > maxStepTest) stepTests = maxStepTest;
        }

        unsigned int minFrame = 1;
        unsigned int maxFrame = 1000;
        ImGui::Text("Frame per step");
        if(ImGui::InputScalar((std::to_string(minFrame) + "-" + std::to_string(maxFrame) + "##3").c_str(), ImGuiDataType_U32, &maxNumFrame)) {
            if (maxNumFrame < minFrame) maxNumFrame = minFrame;
            if (maxNumFrame > maxFrame) maxNumFrame = maxFrame;
        }

        unsigned int minUpdate = 0;
        unsigned int maxUpdate = maxNumFrame;
        ImGui::Text("Update per frame");
        if(ImGui::InputScalar((std::to_string(minUpdate) + "-" + std::to_string(maxUpdate) + "##4").c_str(), ImGuiDataType_U32, &updatePerTest)) {
            if (updatePerTest < minUpdate) updatePerTest = minUpdate;
            if (updatePerTest > maxUpdate) updatePerTest = maxUpdate;
        }

        ImGui::Separator();
        ImGui::Text("%s",("Range: " + std::to_string(actualNumPoints) + " - " + std::to_string(maxNumPoints) + " steps: " 
                          + std::to_string(stepTests) + "x" + std::to_string(maxNumFrame) + "fps\t" + "1 update x " + std::to_string(updatePerTest) + " fps").c_str());

        if(ImGui::Button("Start")) {
            isTestRunning = true;
            setupTest();
        }
        ImGui::End();   

    }
};
#endif