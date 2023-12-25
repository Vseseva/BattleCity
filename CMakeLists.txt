# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("BattleCity")

set(SFML_STATIC_LIBRARIES TRUE)

set(SFML_DIR "SFML/lib/cmake/SFML")

find_package (SFML 2.6.1 REQUIRED system window graphics audio)

add_executable (BattleCity "BattleCity/BattleCity.cpp" "BattleCity/main.cpp" "BattleCity/Button.cpp" "BattleCity/Block.h" "BattleCity/Block.cpp" "BattleCity/Tank.hpp" "BattleCity/Bullet.h" "BattleCity/Bonus.h")
target_link_libraries(BattleCity sfml-graphics sfml-window sfml-audio sfml-system)
