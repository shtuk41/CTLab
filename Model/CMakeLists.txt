
file(GLOB_RECURSE MY_SOURCES "${CMAKE_SOURCE_DIR}/source/*.cpp")
file(GLOB_RECURSE MY_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

set(CADRW_SOURCES
    "${CMAKE_SOURCE_DIR}/source/cadrw/cad_common.cpp"
    "${CMAKE_SOURCE_DIR}/source/cadrw/cad_parser.cpp"
	"${CMAKE_SOURCE_DIR}/source/cadrw/stl_parser.cpp"
	"${CMAKE_SOURCE_DIR}/source/cadrw/stp_parser.cpp"
)

set(CADRW_HEADERS
    "${CMAKE_SOURCE_DIR}/include/cadrw/cad_common.h"
    "${CMAKE_SOURCE_DIR}/include/cadrw/cad_parser.h"
	"${CMAKE_SOURCE_DIR}/include/cadrw/stl_parser.h"
	"${CMAKE_SOURCE_DIR}/include/cadrw/stp_parser.h"
)

source_group("Source Files\\imgui" FILES ${MY_SOURCES})
source_group("Source Files\\cadrw" FILES ${CADRW_SOURCES})
source_group("Header Files" FILES ${MY_HEADERS})
source_group("Header Files\\cadrw" FILES ${CADRW_HEADERS})

add_executable(Model 
	main.cpp
)

target_sources(Model PRIVATE 
	${MY_SOURCES} 
    ${CADRW_SOURCES}
    ${MY_HEADERS}
	${CADRW_HEADERS}
)

# Link the imported target
target_link_libraries(KonstruktorLib PRIVATE 
		glfw
		glm::glm
		soil
		GLEW::GLEW
		freetype
		opengl32)

set_target_properties(Konstruktor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/bin"
	CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)




