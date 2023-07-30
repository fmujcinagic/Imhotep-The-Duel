cmake_minimum_required(VERSION 3.10)

project(imhotep_duel_server)

add_executable(imhotep_duel_server
  src/main.cpp
  src/Parse.cpp
  src/LegalMoves.cpp
  src/PlayerStats.cpp
  src/HandleGame.cpp
  src/PrimaryGame.cpp
  src/AImove.cpp

  # Add your other source files here (space or newline seperated)
)

# Set cpp standard to 17
set_target_properties(imhotep_duel_server PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF
)

# Enable warnings
if(MSVC)
  target_compile_options(imhotep_duel_server PRIVATE /W4)
else()
  target_compile_options(imhotep_duel_server PRIVATE -Wall)
endif()

target_include_directories(imhotep_duel_server PRIVATE include)

# Add dependencies
set(FRAMEWORK_GIT_FOLDER "framework/" CACHE STRING "Set to the framework git directory")
add_subdirectory(${FRAMEWORK_GIT_FOLDER} "framework")
target_link_libraries(imhotep_duel_server PRIVATE OOP1_framework)

add_executable(imhotep_duel_ai
  src/mainAI.cpp
  src/Parse.cpp
  src/LegalMoves.cpp
  src/PlayerStats.cpp
  src/HandleGame.cpp
  src/PrimaryGame.cpp
  src/PrimaryGameAI.cpp
  src/AImove.cpp

  # Add your other source files here (space or newline seperated)
  # If you are using classes from ass1 and ass2 you have to add them here as well
)

# Set cpp standard to 17
set_target_properties(imhotep_duel_ai PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF
)

# Enable warnings
if(MSVC)
  target_compile_options(imhotep_duel_ai PRIVATE /W4)
else()
  target_compile_options(imhotep_duel_ai PRIVATE -Wall)
endif()

target_include_directories(imhotep_duel_ai PRIVATE include)
target_link_libraries(imhotep_duel_ai PRIVATE OOP1_framework)
