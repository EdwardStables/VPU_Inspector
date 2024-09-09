# VPU Inspector

Tool for inspecting state of the VPU simulator. This is a standalone application that communicates with the server via gRPC. Once this is complete, the VPU simulator can be built standalone, or can be built with the gRPC server included, which requires this repo as a submodule as the implementation is held here.

## Using

This tool will provide a simple GUI intended primarily for monitoring the framebuffer, but also with the capability of controlling and stepping simulator execution and giving a view at memory and current program state. These are of decreasing priority, the main aim is for visualising the framebuffer.

Viewing is done via the olcPixelGameEngine, and this is a required dependency.

This repo contains a mock simulator useful for standalone testing.

## Building

This assumes gRPC and protobuf are already built and located somewhere in the system. Run cmake with `cmake -DCMAKE_PREFIX_PATH=${GRPC_INSTALL_LOCATION}`. Protobuf should be on your path.

