#include "source_code_view.h"


SourceCodeView::SourceCodeView(olc::vf2d pos, olc::vf2d size, InspectorClient& client)
    : pos(pos), size(size), client(client)  {

}

void SourceCodeView::init() {
    client.GetSourceCode(lines);
}

void SourceCodeView::update(float dt, olc::PixelGameEngine& pge) {

}

void SourceCodeView::draw(olc::PixelGameEngine& pge) {
    float ypos = 0;
    for (auto& line : lines) {
        pge.DrawStringDecal({pos.x, pos.y + ypos}, line);
        ypos += 8;
        if (ypos > size.y) break;
    }
}