# VPU Inspector

Tool for inspecting state of the VPU simulator. This is a standalone application that communicates with the server via gRPC, however it cannot be built standalone as it requires the autogenerates defs file from the VPU_Model repository.

VPU_Simulation does not depend on this submodule unless it is to be built with the gRPC server enabled.

## Using

This tool will provide a simple GUI intended primarily for monitoring the framebuffer, but also with the capability of controlling and stepping simulator execution and giving a view at memory and current program state. These are of decreasing priority, the main aim is for visualising the framebuffer.

Viewing is done via the olcPixelGameEngine, and this is a required dependency.

This repo contains a mock simulator useful for standalone testing.

## Building

This assumes gRPC and protobuf are already built and located somewhere in the system. Run cmake with `cmake -DCMAKE_PREFIX_PATH=${GRPC_INSTALL_LOCATION}`. Protobuf should be on your path.

The defs files must be available in `../defs` (this assumes this tool is being built as a submodule). If necessary in the future this can be restructured to allow standalone builds with locally generated defs files, but this is currently not supported.

