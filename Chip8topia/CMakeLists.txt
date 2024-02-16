file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE SRCS_IMGUI_FILEDIALOG "${CMAKE_SOURCE_DIR}/dependencies/ImGuiFileDialog/*.cpp")

if (EMSCRIPTEN)
    #    set(CMAKE_EXECUTABLE_SUFFIX ".html") # Add default .html file
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1")
endif ()

add_executable(${PROJECT_NAME}
        ${SRCS}
        ${SRCS_IMGUI_FILEDIALOG}
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/imgui_club")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/utils")

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Setting WIN32_EXECUTABLE to TRUE in Release mode")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
elseif (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Setting WIN32_EXECUTABLE to FALSE in Debug mode")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE FALSE)
endif ()


if (NOT EMSCRIPTEN)

    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

    find_package(glad CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL 3.3 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

else ()
    #    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/web)
    #    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index") # Change output name of .html and .js file
endif ()

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(Stb REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC winmm.lib)
endif ()