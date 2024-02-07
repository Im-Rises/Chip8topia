#[[[ ImGui_OpenGl
add_library(ImGui_OpenGl SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialog/ImGuiFileDialog.cpp
)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(ImGui_OpenGl PRIVATE glfw)

target_include_directories(ImGui_OpenGl PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(ImGui_OpenGl PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)
#]]