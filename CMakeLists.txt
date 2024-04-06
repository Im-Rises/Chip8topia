cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CHIP8TOPIA_VERSION "1.8.0")

project("Chip8topia" VERSION ${CHIP8TOPIA_VERSION})
add_subdirectory(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE CHIP8TOPIA_VERSION="${CHIP8TOPIA_VERSION}")

set(CPACK_PACKAGE_VENDOR "im-rises.github.io")
#set(CPACK_PACKAGE_CONTACT "@hotmail.com")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/Im-Rises/cadmium")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Chip8topia is a Chip8 emulator written in C++ for Windows, Linux, and WebGL.")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
if (WIN32 OR APPLE)
    set(CPACK_GENERATOR "ZIP" CACHE STRING "Generators to support. semi-colon delimited list")
else ()
    set(CPACK_GENERATOR "TGZ" CACHE STRING "Generators to support. semi-colon delimited list")
endif ()
include(CPack)