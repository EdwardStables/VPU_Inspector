#pragma once

#include "olcPixelGameEngine.h"
#undef Status //Fix conflict between X11 and gRPC

#include "inspector_rpc.h"
#include <vector>

class SourceCodeView {
    olc::vf2d pos;
    olc::vf2d size;
    InspectorClient& client;
    std::vector<std::string> lines;

    int scroll_offset = 0;
    int max_visible_lines;

public:
    SourceCodeView(olc::vf2d pos, olc::vf2d size, InspectorClient& client);
    void init();
    void update(float dt, olc::PixelGameEngine& pge);
    void draw(olc::PixelGameEngine& pge);
};