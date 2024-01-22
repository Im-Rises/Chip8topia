file(GLOB_RECURSE SRCS_HEADERS_IMGUI "${CMAKE_SOURCE_DIR}/dependencies/imgui/*.cpp")
file(GLOB_RECURSE SRCS_HEADERS_IMGUIFILEDIALOG "${CMAKE_SOURCE_DIR}/dependencies/ImGuiFileDialog/*.cpp")
file(GLOB_RECURSE SRCS_HEADERS_GLAD "${CMAKE_SOURCE_DIR}/dependencies/glad/*.c")

file(GLOB_RECURSE SRCS "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/*.cpp")

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL 4.3 REQUIRED)

add_executable(${PROJECT_NAME}
        ${SRCS}
        ${SRCS_HEADERS_IMGUI}
        ${SRCS_HEADERS_IMGUIFILEDIALOG}
        ${SRCS_HEADERS_GLAD}
        )

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/imgui")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/glad/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/glfw/include")

target_link_libraries(${PROJECT_NAME} OpenGL::GL)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} winmm.lib)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/dependencies/glfw/lib-vc2019/glfw3.lib")
    elseif (MINGW)
        target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/dependencies/glfw/lib-mingw-w64/libglfw3.a")
    endif ()
elseif (UNIX AND NOT APPLE)
    find_package(glfw3 3.3 REQUIRED)
    target_link_libraries(${PROJECT_NAME} glfw ${CMAKE_DL_LIBS})
elseif (APPLE)
    find_package(glfw3 3.3 REQUIRED)
    target_link_libraries(${PROJECT_NAME} glfw ${CMAKE_DL_LIBS})
else ()
    message(FATAL_ERROR "OS not configured in CMakeLists.txt")
endif ()
