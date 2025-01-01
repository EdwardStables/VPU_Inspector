#include "inspector_rpc.h"

#include "source_code_view.h"

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
    bool refresh_held_last = false;

    olc::QuickGUI::Button* step_button;
    bool step_held_last = false;

    olc::QuickGUI::Button* run_button;
    bool run_held_last = false;

    SourceCodeView source_code;


public:
	InspectorGUI()
        : client(grpc::CreateChannel("localhost:50101", grpc::InsecureChannelCredentials())),
          source_code(olc::vf2d(vpu::defs::FRAMEBUFFER_WIDTH,0), olc::vf2d(SOURCE_VIEW_WIDTH,ScreenHeight()), client),
          framebuffer_data(std::make_unique<std::array<uint32_t,FRAMEBUFFER_LEN>>()),
          toolbar(false)
	{
		// Name your application
		sAppName = "Inspector";
	}

public:
	bool OnUserCreate() override
	{
        source_code.init();

        for (int i = 0; i < FRAMEBUFFER_LEN; i++)
            framebuffer_data->at(i) = 0;

        float xoffs = 0;
        olc::vf2d button_size = {64, TOOLBAR_HEIGHT};
        refresh_button = new olc::QuickGUI::Button(toolbar,"Refresh",olc::vf2d{xoffs,vpu::defs::FRAMEBUFFER_HEIGHT},button_size);
        toolbar.AddControl(refresh_button);
        xoffs+=button_size.x;
        step_button = new olc::QuickGUI::Button(toolbar,"Step",olc::vf2d{xoffs,vpu::defs::FRAMEBUFFER_HEIGHT},button_size);
        toolbar.AddControl(step_button);
        xoffs+=button_size.x;
        run_button = new olc::QuickGUI::Button(toolbar,"Run",olc::vf2d{xoffs,vpu::defs::FRAMEBUFFER_HEIGHT},button_size);
        toolbar.AddControl(run_button);
        xoffs+=button_size.x;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        Clear(olc::BLACK);
        toolbar.Update(this);

        if (!refresh_button->bHeld && refresh_held_last) {
            std::cout << "Refreshing framebuffer" << std::endl;
            bool ok = client.GetFrameBuffer(framebuffer_data);
        }
        refresh_held_last = refresh_button->bHeld;

        if (!step_button->bHeld && step_held_last) {
            std::cout << "Step" << std::endl;
            bool ok = client.Step();
        }
        step_held_last = step_button->bHeld;

        if (!run_button->bHeld && run_held_last) {
            std::cout << "Run" << std::endl;
            bool ok = client.Run();
        }
        run_held_last = run_button->bHeld;

        source_code.update(fElapsedTime, *this);



        for (int y = 0; y < vpu::defs::FRAMEBUFFER_HEIGHT; y++) {
            for (int x = 0; x < vpu::defs::FRAMEBUFFER_WIDTH; x++) {
                size_t ind = vpu::defs::FRAMEBUFFER_WIDTH*y + x;
                uint32_t data = framebuffer_data->at(ind);
                olc::Pixel colour = olc::Pixel(data);
                Draw(x,y,colour);
            }
        }

        toolbar.DrawDecal(this);
        source_code.draw(*this);

		return true;
	}
};

int main() {
	InspectorGUI inspector;
    if (inspector.Construct(vpu::defs::FRAMEBUFFER_WIDTH + SOURCE_VIEW_WIDTH, vpu::defs::FRAMEBUFFER_HEIGHT + TOOLBAR_HEIGHT, 2, 2))
		inspector.Start();

    return 0;
}

