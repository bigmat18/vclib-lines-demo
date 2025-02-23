#include <vclib/bgfx/drawable/lines/drawable_lines.h>
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

#ifndef LINES_OBJECT_HANDLER
#define LINES_OBJECT_HANDLER

class LinesObjectHandler
{
protected:
    bool mVisible         = false;
    std::string mName     = ""; 
    std::string mInfo     = "";

public:
    LinesObjectHandler() = default;

    LinesObjectHandler(const std::string name, const std::string info) :
        mName(name), mInfo(info) {}

    virtual ~LinesObjectHandler() {}

    virtual void drawObject(unsigned int viewId) = 0;

    virtual void udpateRandom(unsigned int num) {};

    virtual void drawImGuiSettings() {};

    virtual void setType(vcl::lines::LinesTypes type) {};

    const std::string& name() const { return mName; }

    std::string& name() { return mName; }

    const std::string& info() const { return mInfo; }

    std::string& info() { return mInfo; }

    bool isVisible() const { return mVisible; }

    void setVisibility(bool vis) { mVisible = vis; }
};

#endif