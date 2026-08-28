file(GLOB_RECURSE MY_SHADERS "${CMAKE_SOURCE_DIR}/shaders/*.vert" "${CMAKE_SOURCE_DIR}/shaders/*.frag")

set(IMGUI_SOURCES 
	../ThirdParty/imgui/imgui.cpp
	../ThirdParty/imgui/imgui_draw.cpp
	../ThirdParty/imgui/imgui_widgets.cpp
	../ThirdParty/imgui/imgui_tables.cpp
	../ThirdParty/imgui/imgui_demo.cpp
	../ThirdParty/imgui/backends/imgui_impl_glfw.cpp
    ../ThirdParty/imgui/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_HEADERS
    ../ThirdParty/imgui/imgui.h
    ../ThirdParty/imgui/imconfig.h
)

set (MY_SOURCES
	"${CMAKE_SOURCE_DIR}/CTLab/sources/axes3d.cpp"
	"${CMAKE_SOURCE_DIR}/CTLab/sources/axisPlane.cpp"
	"${CMAKE_SOURCE_DIR}/CTLab/sources/camera.cpp"
	"${CMAKE_SOURCE_DIR}/CTLab/sources/context.cpp"
	"${CMAKE_SOURCE_DIR}/CTLab/sources/framebufferCapture.cpp"
	"${CMAKE_SOURCE_DIR}/CTLab/sources/optionsreader.cpp"
	"${CMAKE_SOURCE_DIR}/CTLab/sources/render_object.cpp"
	"${CMAKE_SOURCE_DIR}/CTLab/sources/shaders.cpp"
	"${CMAKE_SOURCE_DIR}/CTLab/sources/volume.cpp"
)

set(MY_HEADERS
    "${CMAKE_SOURCE_DIR}/CTLab/include/axes3d.h"
    "${CMAKE_SOURCE_DIR}/CTLab/include/axisPlane.h"
	"${CMAKE_SOURCE_DIR}/CTLab/include/camera.h"
	"${CMAKE_SOURCE_DIR}/CTLab/include/context.h"
	"${CMAKE_SOURCE_DIR}/CTLab/include/controls.h"
	"${CMAKE_SOURCE_DIR}/CTLab/include/exceptions.h"
	"${CMAKE_SOURCE_DIR}/CTLab/include/framebufferCapture.h"
	"${CMAKE_SOURCE_DIR}/CTLab/include/optionsreader.h"
	"${CMAKE_SOURCE_DIR}/CTLab/include/render_object.h"
	"${CMAKE_SOURCE_DIR}/CTLab/include/shaders.h"
	"${CMAKE_SOURCE_DIR}/CTLab/include/volume.h"
	"${CMAKE_SOURCE_DIR}/CTLab/include/window.h"
)

source_group("Source Files\\imgui" FILES ${IMGUI_SOURCES})
source_group("Header Files\\imgui" FILES ${IMGUI_HEADERS})
source_group("Header Files" FILES ${MY_HEADERS})
source_group("Shaders" FILES ${MY_SHADERS})

add_executable(CTLab 
	CTLab.cpp
	${MY_SOURCES} 
)

target_sources(CTLab PRIVATE 
	${IMGUI_SOURCES} 
    ${IMGUI_HEADERS}
	${MY_SOURCES} 
    ${MY_HEADERS}
)

target_include_directories(CTLab PRIVATE 
	${CMAKE_SOURCE_DIR}/CTLab/include
	../ThirdParty/imgui
    ../ThirdParty/imgui/backends
)

if(WIN32)
target_link_libraries(CTLab PRIVATE 
	glfw
	glm::glm
	GLEW::GLEW
	opengl32
	soil
	${OpenCV_LIBS}
)
elseif(LINUX)
target_link_libraries(CTLab PRIVATE 
	glfw
	glm::glm
	GLEW::GLEW
	OpenGL::GL
	soil
	${OpenCV_LIBS}
)
endif()

target_compile_features(CTLab PRIVATE cxx_std_20)

if(WIN32)
	add_custom_command(TARGET CTLab POST_BUILD

		# Copy shaders directory to both locations
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/CTLab/shaders" "${CMAKE_BINARY_DIR}/CTLab/shaders"
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/CTLab/shaders" "${CMAKE_BINARY_DIR}/x64/$<CONFIG>/shaders"
		
		#COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/incorrectFileName.png" "${CMAKE_SOURCE_DIR}/build/incorrectFileName.png"
		#COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/incorrectFileName.png" "${CMAKE_SOURCE_DIR}/build/x64/$<CONFIG>/incorrectFileName.png"
		
		#COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/notdefinedfilename.png" "${CMAKE_SOURCE_DIR}/build/notdefinedfilename.png"
		#COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/notdefinedfilename.png" "${CMAKE_SOURCE_DIR}/build/x64/$<CONFIG>/notdefinedfilename.png"
	)

	set_target_properties(CTLab PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/x64/$<CONFIG>"
		#RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/bin"
		#RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/bin"
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
	)
elseif(LINUX)
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
	
	set_target_properties(CTLab PROPERTIES
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	    #RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
	    #RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
	)

	add_custom_command(TARGET CTLab POST_BUILD

		# Copy shaders directory to both locations
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/CTLab/shaders" "${CMAKE_BINARY_DIR}/bin/shaders"
	)
endif()





