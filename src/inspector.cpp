#include "inspector_rpc.h"

#include "olcPixelGameEngine.h"

class InspectorGUI : public olc::PixelGameEngine {
    InspectorClient client;
    std::unique_ptr<std::array<uint32_t,200*300>> framebuffer_data;
public:
	InspectorGUI()
        : client(grpc::CreateChannel("localhost:50101", grpc::InsecureChannelCredentials())),
          framebuffer_data(std::make_unique<std::array<uint32_t,200*300>>())
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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        if (GetKey(olc::SPACE).bPressed){
            bool ok = client.GetFrameBuffer(framebuffer_data);

            for (int y = 0; y < ScreenHeight(); y++) {
                for (int x = 0; x < ScreenWidth(); x++) {
                    size_t ind = ScreenWidth()*y + x;
                    uint32_t data = framebuffer_data->at(ind);
                    olc::Pixel colour = olc::Pixel(data);

                    if (colour != olc::Pixel(0,0,0,0)){
                        std::cout << "not black, " << x << " " << y << " ";
                        std::cout << std::hex << int(colour.r) << ",";
                        std::cout << int(colour.g) << ",";
                        std::cout << int(colour.b) << ",";
                        std::cout << int(colour.a) << std::dec << "\n";
                    }

                    Draw(x,y,colour);
                }
            }

            std::cout << "Finished getting framebuffer" << std::endl;
        }

		// Called once per frame, draws random coloured pixels
		return true;
	}
};

int main() {
	InspectorGUI inspector;
    if (inspector.Construct(300, 200, 4, 4))
		inspector.Start();

    return 0;
}

