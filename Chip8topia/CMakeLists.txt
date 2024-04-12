if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message("Setting CMAKE_BUILD_TYPE to Debug as none was specified")
endif ()

file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE SRCS_IMGUI_FILEDIALOG "${CMAKE_SOURCE_DIR}/dependencies/ImGuiFileDialog/*.cpp")

add_executable(${PROJECT_NAME}
        ${SRCS}
        ${SRCS_IMGUI_FILEDIALOG}
)

if (WIN32)
    target_sources(${PROJECT_NAME} PRIVATE app.rc)
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/ImGuiNotify/unixExample/backends")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/IconFontCppHeaders")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/imgui_club")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/emscripten-browser-file")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/utils")

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)

if (NOT EMSCRIPTEN)
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)

    find_package(glad CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL 3.3 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

    find_package(Stb REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${STB_LIBRARIES})

    if (WIN32)
        target_link_libraries(${PROJECT_NAME} PUBLIC Pdh.lib) # For RAM usage
        target_link_libraries(${PROJECT_NAME} PUBLIC psapi.lib) # For CPU usage
    endif ()

    if (UNIX AND NOT APPLE)
        find_package(X11 REQUIRED)
        target_include_directories(${PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIR})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
    endif ()
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)
endif ()

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT EMSCRIPTEN)
    find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_DEBUG)
else ()
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_RELEASE)
endif ()

if (FALSE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_PARAM_SAFE)
    message("Building with safe parameter")
else ()
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_PARAM_UNSAFE)
    message("Building with unsafe parameter")
endif ()

if (WIN32)
    set(PLATFORM_NAME "Windows")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_NAME="${PLATFORM_NAME}")
elseif (UNIX AND NOT APPLE)
    set(PLATFORM_NAME "Linux")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_NAME="${PLATFORM_NAME}")
elseif (APPLE)
    set(PLATFORM_NAME "MacOS")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_MACOS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_NAME="${PLATFORM_NAME}")
    #elseif (EMSCRIPTEN) # Use linux when building for web with emscripten
    #    set(PLATFORM_NAME "Web")
    #    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WEB)
    #    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_NAME="${PLATFORM_NAME}")
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_CPP_VERSION="${CMAKE_CXX_STANDARD}")

message("Building ${PROJECT_NAME} for ${PLATFORM_NAME} with C++${CMAKE_CXX_STANDARD} on ${CMAKE_SYSTEM_NAME}")