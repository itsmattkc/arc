cmake_minimum_required(VERSION 3.14)

project(Arc LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(arc
  src/archive/adapters/ziparchive.cpp
  src/archive/adapters/ziparchive.h
  src/archive/archive.cpp
  src/archive/archive.h
  src/archive/item.cpp
  src/archive/item.h
  src/main.cpp
  src/mainwindow.cpp
  src/mainwindow.h
  src/ui/dirviewsortfilterproxymodel.cpp
  src/ui/dirviewsortfilterproxymodel.h
  src/ui/errordialog.cpp
  src/ui/errordialog.h
  src/ui/fileviewsortfilterproxymodel.cpp
  src/ui/fileviewsortfilterproxymodel.h
)

target_link_libraries(arc PRIVATE Qt5::Widgets)
target_include_directories(arc PRIVATE src)
target_compile_options(arc PRIVATE -Werror -Wuninitialized -pedantic-errors -Wall -Wextra -Wshadow)

# Find LibZip for ZIP support
find_package(LibZip)
if (${LIBZIP_FOUND})
  target_link_libraries(arc PRIVATE ${LIBZIP_LIBRARY})
  target_include_directories(arc PRIVATE ${LIBZIP_INCLUDE_DIRS})
  target_compile_definitions(arc PRIVATE USE_LIBZIP)
else()
  message("   ZIP support will be disabled.")
endif()

# Find LZMA for 7-Zip support
find_package(LibLZMA)
if (${LIBLZMA_FOUND})
  target_link_libraries(arc PRIVATE ${LIBLZMA_LIBRARIES})
  target_include_directories(arc PRIVATE ${LIBLZMA_INCLUDE_DIRS})
  target_compile_definitions(arc PRIVATE USE_LZMA)
else()
  message("   7-Zip support will be disabled.")
endif()
