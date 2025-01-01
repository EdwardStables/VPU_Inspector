#include "inspector_rpc.h"

#include "olcPixelGameEngine.h"
#include "extensions/olcPGEX_QuickGUI.h"
#include "../defs/defs_pkg.h"

const int SOURCE_VIEW_WIDTH = 160;
const int TOOLBAR_HEIGHT = 32;
const int FRAMEBUFFER_LEN = vpu::defs::FRAMEBUFFER_WIDTH * vpu::defs::FRAMEBUFFER_HEIGHT;

class InspectorGUI : public olc::PixelGameEngine {
    InspectorClient client;
    std::unique_ptr<std::array<uint32_t,FRAMEBUFFER_LEN>> framebuffer_data;
    olc::QuickGUI::Manager toolbar;
    olc::QuickGUI::Button* refresh_button;

    std::vector<std::string> source_code;    
    olc::QuickGUI::TextBox* source_code_box;

    bool held_last = false;

    std::string get_source_code_string() {
        std::string ret;
        for (auto& line : source_code) {
            ret += line + "\n";
        }
        return ret;
    }

public:
	InspectorGUI()
        : client(grpc::CreateChannel("localhost:50101", grpc::InsecureChannelCredentials())),
          framebuffer_data(std::make_unique<std::array<uint32_t,200*300>>()),
          toolbar(false)
	{
		// Name your application
		sAppName = "Inspector";
	}

public:
	bool OnUserCreate() override
	{
        //TODO run configuration here
        bool ok = client.SendCommand();
        std::cout << "Server gave " << (ok ? "OK" : "not OK") << std::endl;
        client.GetSourceCode(source_code);

        for (int i = 0; i < FRAMEBUFFER_LEN; i++)
            framebuffer_data->at(i) = 0;

        refresh_button = new olc::QuickGUI::Button(toolbar,"Refresh",olc::vf2d{0,vpu::defs::FRAMEBUFFER_HEIGHT},olc::vf2d{64.0f,TOOLBAR_HEIGHT});
        toolbar.AddControl(refresh_button);
        source_code_box = new olc::QuickGUI::TextBox(toolbar, get_source_code_string(), olc::vf2d(300.0f, 8.0f), olc::vf2d(SOURCE_VIEW_WIDTH, 8*source_code.size()));
        source_code_box->nAlign = olc::QuickGUI::Label::Alignment::Left;
        source_code_box->bHasBorder = false;
        toolbar.AddControl(source_code_box);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        Clear(olc::BLACK);
        toolbar.Update(this);

        if (!refresh_button->bHeld && held_last) {
            std::cout << "Refreshing framebuffer" << std::endl;
            bool ok = client.GetFrameBuffer(framebuffer_data);
        }
        held_last = refresh_button->bHeld;



        for (int y = 0; y < vpu::defs::FRAMEBUFFER_HEIGHT; y++) {
            for (int x = 0; x < vpu::defs::FRAMEBUFFER_WIDTH; x++) {
                size_t ind = vpu::defs::FRAMEBUFFER_WIDTH*y + x;
                uint32_t data = framebuffer_data->at(ind);
                olc::Pixel colour = olc::Pixel(data);
                Draw(x,y,colour);
            }
        }

        toolbar.DrawDecal(this);

		return true;
	}
};

int main() {
	InspectorGUI inspector;
    if (inspector.Construct(vpu::defs::FRAMEBUFFER_WIDTH + SOURCE_VIEW_WIDTH, vpu::defs::FRAMEBUFFER_HEIGHT + TOOLBAR_HEIGHT, 4, 4))
		inspector.Start();

    return 0;
}

