# CMakeList.txt : CMake project for td, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("tower-defense")
link_directories("../../SFML-2.5.1/lib")
include_directories("../../SFML-2.5.1/include")

# Add source to this project's executable.
add_executable (td src/Armor.cpp 
	src/Collision.cpp 
	src/Game.cpp 
	src/HealthBar.cpp 
	src/Helper.cpp 
	src/main.cpp 
	src/MapEditor.cpp 
	src/Projectile.cpp 
	src/ResizableComponent.cpp
	src/Round.cpp
	src/Scene.cpp
	src/Shop.cpp
	src/TextureManager.cpp
	src/Tower.cpp
	src/Waypoint.cpp)

# TODO: Add tests and install targets if needed.

target_link_libraries(td sfml-graphics-d sfml-window-d sfml-system-d)
