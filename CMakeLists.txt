cmake_minimum_required (VERSION 3.1.3)
project (VirtualPCCpp)
add_executable (vpc
./VirtualPCCpp/NSSDL.cpp
./VirtualPCCpp/SDLWindow.cpp
./VirtualPCCpp/Next.cpp
./VirtualPCCpp/Memory.cpp
./VirtualPCCpp/Assembly.cpp
./VirtualPCCpp/CPU.cpp
./VirtualPCCpp/PC.cpp
./VirtualPCCpp/Main.cpp
./VirtualPCCpp/GPU.cpp)
find_package(SDL2 REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})
target_link_libraries(vpc ${SDL2_LIBRARIES})
