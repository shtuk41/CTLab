
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE MY_SHADERS "${CMAKE_SOURCE_DIR}/QuadControl/shaders/*.vert" "${CMAKE_SOURCE_DIR}/QuadControl/shaders/*.frag")

set(MY_HEADERS
	"${CMAKE_SOURCE_DIR}/QuadControl/include/axes3d.h" 
	"${CMAKE_SOURCE_DIR}/QuadControl/include/axisPlane.h" 
	"${CMAKE_SOURCE_DIR}/QuadControl/include/camera.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/context.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/contextWrapper.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/controls.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/glview.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/glview3d.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/glview3drenderer.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/glviewxy.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/glviewxyrenderer.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/glviewxz.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/glviewxzrenderer.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/glviewyz.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/glviewyzrenderer.h" 
	"${CMAKE_SOURCE_DIR}/QuadControl/include/render_object.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/shaders.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/viewBorder.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/volume3dview.h"
)

set(MY_SOURCES
	"${CMAKE_SOURCE_DIR}/QuadControl/source/axes3d.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/axisPlane.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/camera.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/context.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/glview.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/glview3drenderer.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/glviewxyrenderer.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/glviewxzrenderer.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/glviewyzrenderer.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/render_object.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/shaders.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/viewBorder.cpp" 
	"${CMAKE_SOURCE_DIR}/QuadControl/source/volume3dview.cpp" 
)

set(ALL_FILES
	${MY_HEADERS}
	${MY_SOURCES}
	"${CMAKE_SOURCE_DIR}/QuadControl/QuadControl.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/quadcontrol_global.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/plugin.cpp"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/io/ioData.h"
	"${CMAKE_SOURCE_DIR}/QuadControl/source/io/ioData.cpp"
	${MY_SHADERS}
)

source_group("Header Files" FILES ${MY_HEADERS} "${CMAKE_SOURCE_DIR}/QuadControl/QuadControl.h" "${CMAKE_SOURCE_DIR}/QuadControl/quadcontrol_global.h")
source_group("Source Files" FILES ${MY_SOURCES} "${CMAKE_SOURCE_DIR}/QuadControl/plugin.cpp")
source_group("Shaders" FILES ${MY_SHADERS})
source_group("Header Files\\io" FILES "${CMAKE_SOURCE_DIR}/QuadControl/include/io/ioData.h")
source_group("Source Files\\io" FILES "${CMAKE_SOURCE_DIR}/QuadControl/source/io/ioData.cpp")

add_library(QuadControl SHARED 
    ${ALL_FILES}
)

target_sources(QuadControl PRIVATE 
    ${MY_HEADERS}
	${MY_SOURCES}
)

# Headers
target_include_directories(QuadControl PRIVATE 
	"${CMAKE_SOURCE_DIR}/QuadControl"
	"${CMAKE_SOURCE_DIR}/QuadControl/include"
	"${CMAKE_SOURCE_DIR}/QuadControl/include/io"
)

# Link the imported target
target_link_libraries(QuadControl PRIVATE 
		glm::glm
		Qt6::Core 
		Qt6::Qml
		Qt6::Quick
		Qt6::OpenGL
)

target_compile_features(QuadControl PRIVATE cxx_std_20)

set(PYTHON_DEPLOY_DIR "${CMAKE_SOURCE_DIR}/CTPython/qml/QuadControl")

if(WIN32)

	set_target_properties(QuadControl PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/x64/$<CONFIG>"
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
	)


	
elseif(LINUX)
	
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
	
	set_target_properties(QuadControl PROPERTIES
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	    #RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
	    #RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
	)
endif()

add_custom_command(TARGET QuadControl POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy 
		$<TARGET_FILE:QuadControl> 
		"${PYTHON_DEPLOY_DIR}/"
	)



