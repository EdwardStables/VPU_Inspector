#include "inspector_rpc.h"

#include "olcPixelGameEngine.h"
#include "extensions/olcPGEX_QuickGUI.h"
#include "../defs/defs_pkg.h"

const int TOOLBAR_HEIGHT = 32;
const int FRAMEBUFFER_LEN = vpu::defs::FRAMEBUFFER_WIDTH * vpu::defs::FRAMEBUFFER_HEIGHT;

class InspectorGUI : public olc::PixelGameEngine {
    InspectorClient client;
    std::unique_ptr<std::array<uint32_t,FRAMEBUFFER_LEN>> framebuffer_data;
    olc::QuickGUI::Manager toolbar;
    olc::QuickGUI::Button* refresh_button;
    bool held_last = false;

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

        for (int i = 0; i < FRAMEBUFFER_LEN; i++)
            framebuffer_data->at(i) = 0;

        refresh_button = new olc::QuickGUI::Button(toolbar,"Refresh",olc::vf2d{0,vpu::defs::FRAMEBUFFER_HEIGHT},olc::vf2d{64.0f,TOOLBAR_HEIGHT});
        toolbar.AddControl(refresh_button);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        toolbar.Update(this);

        if (!refresh_button->bHeld && held_last) {
            std::cout << "Refreshing framebuffer" << std::endl;
            bool ok = client.GetFrameBuffer(framebuffer_data);
        }
        held_last = refresh_button->bHeld;



        for (int y = 0; y < vpu::defs::FRAMEBUFFER_HEIGHT; y++) {
            for (int x = 0; x < vpu::defs::FRAMEBUFFER_WIDTH; x++) {
                size_t ind = ScreenWidth()*y + x;
                uint32_t data = framebuffer_data->at(ind);
                olc::Pixel colour = olc::Pixel(data);
                Draw(x,y,colour);
            }
        }

        toolbar.Draw(this);

		return true;
	}
};

int main() {
	InspectorGUI inspector;
    if (inspector.Construct(vpu::defs::FRAMEBUFFER_WIDTH, vpu::defs::FRAMEBUFFER_HEIGHT + TOOLBAR_HEIGHT, 4, 4))
		inspector.Start();

    return 0;
}

