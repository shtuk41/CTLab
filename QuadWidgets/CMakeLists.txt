
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE MY_SHADERS "${CMAKE_SOURCE_DIR}/QuadWidgets/shaders/*.vert" "${CMAKE_SOURCE_DIR}/QuadWidgets/shaders/*.frag")

set(MY_HEADERS
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/axes3d.h" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/axisPlane.h" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/camera.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/context.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/controls.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/glview.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/glviewquad3d.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/glviewquad3d.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/glviewquadxy.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/glviewquadxz.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/glviewquadyz.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/render_object.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/shaders.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/viewBorder.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/volume3dview.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/volumeData.h"
)

set(MY_SOURCES
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/axes3d.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/axisPlane.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/camera.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/context.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/glview.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/glviewquad3d.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/glviewquadxy.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/glviewquadxz.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/glviewquadyz.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/render_object.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/shaders.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/viewBorder.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/volume3dview.cpp" 
)

set(ALL_FILES
	${MY_HEADERS}
	${MY_SOURCES}
	"${CMAKE_SOURCE_DIR}/QuadWidgets/main.cpp"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/quadwidgets.cpp"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/quadwidgets.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/io/ioData.h"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/source/io/ioData.cpp"
	${MY_SHADERS}
)

source_group("Header Files" FILES ${MY_HEADERS} "${CMAKE_SOURCE_DIR}/QuadWidgets/quadwidgets.h")
source_group("Source Files" FILES ${MY_SOURCES} "${CMAKE_SOURCE_DIR}/QuadWidgets/main.cpp" "${CMAKE_SOURCE_DIR}/QuadWidgets/quadwidgets.cpp")
source_group("Shaders" FILES ${MY_SHADERS})
source_group("Header Files\\io" FILES "${CMAKE_SOURCE_DIR}/QuadWidgets/include/io/ioData.h")
source_group("Source Files\\io" FILES "${CMAKE_SOURCE_DIR}/QuadWidgets/source/io/ioData.cpp")

qt_wrap_ui(ui_files quadwidgets.ui)
qt_add_resources(qrc_files quadwidgets.qrc)

add_executable(QuadWidgets 
	${ALL_FILES}
	${ui_files}
    ${qrc_files}
    quadwidgets.rc
	icon.png
	QuadWidgets.ico
)

target_sources(QuadWidgets PRIVATE 
    ${MY_HEADERS}
	${MY_SOURCES}
)

# Headers
target_include_directories(QuadWidgets PRIVATE 
	"${CMAKE_SOURCE_DIR}/QuadWidgets"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include"
	"${CMAKE_SOURCE_DIR}/QuadWidgets/include/io"
	${CMAKE_CURRENT_BINARY_DIR}
)

# Link the imported target
target_link_libraries(QuadWidgets PRIVATE 
		glm::glm
		soil
		Qt6::Core 
		Qt6::Gui
		Qt6::Widgets
		Qt6::OpenGL
		Qt6::OpenGLWidgets
		${OpenCV_LIBS}
)

add_custom_command(TARGET QuadWidgets POST_BUILD

    # Copy shaders directory to both locations
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/QuadWidgets/shaders" "${CMAKE_BINARY_DIR}/QuadWidgets/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/QuadWidgets/shaders" "${CMAKE_BINARY_DIR}/x64/$<CONFIG>/shaders"
)


set_target_properties(QuadWidgets PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/x64/$<CONFIG>"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

set_target_properties(QuadWidgets PROPERTIES
    RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/icon.png;${CMAKE_CURRENT_SOURCE_DIR}/QuadWidgets.ico"
)

get_filename_component(_qt6_root "${Qt6_DIR}/../../.." ABSOLUTE)
set(windeployqt_path "${_qt6_root}/bin/windeployqt.exe")

add_custom_command(
    TARGET QuadWidgets POST_BUILD
    COMMAND "${windeployqt_path}"
            --no-compiler-runtime
            "$<TARGET_FILE:QuadWidgets>"
    COMMENT "Deploying Qt dependencies directly to the executable folder..."
)