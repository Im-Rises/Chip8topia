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

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/imgui_club")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/emscripten-browser-file")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/utils")

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)

if (NOT EMSCRIPTEN)
    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

    find_package(glad CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL 3.3 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

    find_package(Stb REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

    if (WIN32)
        target_link_libraries(${PROJECT_NAME} PUBLIC winmm.lib)
    endif ()
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
endif ()

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_DEBUG)
    #    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_CONFIG "$<$<CONFIG:Debug>:Debug>")
else ()
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_RELEASE)
    #    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_CONFIG "$<$<CONFIG:Release>:Release>")
endif ()

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_NAME="Windows")
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_NAME="Linux")
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_CPP_VERSION="${CMAKE_CXX_STANDARD}")
