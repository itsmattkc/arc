cmake_minimum_required(VERSION 3.5)

project(Arc LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(arc
  src/archive/archive.cpp
  src/archive/archive.h
  src/archive/item.cpp
  src/archive/item.h
  src/main.cpp
  src/mainwindow.cpp
  src/mainwindow.h
  src/ui/dirview.cpp
  src/ui/dirview.h
  src/ui/dirviewsortfilterproxymodel.cpp
  src/ui/dirviewsortfilterproxymodel.h
  src/ui/fileviewsortfilterproxymodel.cpp
  src/ui/fileviewsortfilterproxymodel.h
)

target_link_libraries(arc PRIVATE Qt5::Widgets)
target_include_directories(arc PRIVATE src)
target_compile_options(arc PRIVATE -Werror -Wuninitialized -pedantic-errors -Wall -Wextra -Wshadow)
