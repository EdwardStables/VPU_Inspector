#Based off CMakeLists.txt in grpc helloworld example

cmake_minimum_required(VERSION 3.8)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(VPUInspector CXX)

# Following is extracted from grpc examples/cmake/common.cmake for pre-installed grpc/protobuf 
# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto file
get_filename_component(hw_proto "protos/sim_control.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/sim_control.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/sim_control.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/sim_control.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/sim_control.grpc.pb.h")

add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# hw_grpc_proto
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})

target_link_libraries(hw_grpc_proto
  absl::check
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

add_executable(mock_simulator
    "simulator/simulator_rpc.cpp"
    "simulator/simulator_mock.cpp"
)
target_include_directories(mock_simulator
   PUBLIC simulator_include
)
target_link_libraries(mock_simulator
  hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

add_executable(inspector
    "src/inspector.cpp"
    "src/inspector_rpc.cpp"
    "src/olcPixelGameEngine.cpp"
)
target_include_directories(inspector
   PUBLIC include
   PUBLIC olcPixelGameEngine
)
target_link_libraries(inspector
  hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# OpenGL
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
target_include_directories(inspector PUBLIC ${OpenGL_INCLUDE_DIRS})
target_link_libraries(inspector ${OpenGL_LIBRARIES} OpenGL::GL)

# X11
find_package(X11 REQUIRED)
target_link_libraries(inspector X11::X11)
target_include_directories(inspector PUBLIC ${X11_INCLUDE_DIRS})

# Threads
find_package(Threads REQUIRED)
target_link_libraries(inspector Threads::Threads)
target_include_directories(inspector PUBLIC ${Threads_INCLUDE_DIRS})

target_link_libraries(inspector ${CMAKE_DL_LIBS})

find_package(PNG REQUIRED)
target_link_libraries(inspector PNG::PNG)
target_include_directories(inspector PUBLIC ${PNG_INCLUDE_DIRS})

# stdc++fs
target_link_libraries(inspector stdc++fs)
