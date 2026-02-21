

cmake_minimum_required(VERSION 3.10)
project(ChatAdminPanel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ChatAdminPanel
    main.cpp
    admin_panel.cpp
    admin_panel.h
)

target_link_libraries(ChatAdminPanel Qt6::Widgets)



