#include "source_code_view.h"


SourceCodeView::SourceCodeView(olc::vf2d pos, olc::vf2d size, InspectorClient& client)
    : pos(pos), size(size), client(client),
      max_visible_lines(size.y/8)    
{

}

void SourceCodeView::init() {
    client.GetSourceCode(lines);
}

void SourceCodeView::update(float dt, olc::PixelGameEngine& pge) {
    auto mpos = pge.GetMousePos();
    bool hovered = mpos.x > pos.x && mpos.y > pos.y && mpos.x < pos.x + size.x && mpos.y < pos.y + size.y;

    if (hovered) {
        int32_t mw = pge.GetMouseWheel();
        if (mw < 0) {
            scroll_offset = std::min(scroll_offset+1, int(lines.size())-max_visible_lines+1);
        } else if (mw > 0) {
            scroll_offset = std::max(scroll_offset-1, 0);
        }
    }
}

void SourceCodeView::draw(olc::PixelGameEngine& pge) {
    float ypos = 0;
    for (int i = scroll_offset; i < lines.size(); i++) {
        olc::Pixel colour = (highlight_pc && i*4 == highlight_pc.value()) ? olc::YELLOW : olc::WHITE;
        pge.DrawStringDecal({pos.x, pos.y + ypos}, lines[i], colour);
        ypos += 8;
        if (ypos > size.y) break;
    }
}

void SourceCodeView::set_highlight(uint32_t pc) {
    highlight_pc = pc;
}