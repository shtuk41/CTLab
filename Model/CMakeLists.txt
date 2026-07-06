
set (MY_SOURCES
	"${CMAKE_SOURCE_DIR}/Model/source/AcquisModel.cpp"
	"${CMAKE_SOURCE_DIR}/Model/source/detector.cpp"
	"${CMAKE_SOURCE_DIR}/Model/source/scanObject.cpp"
	"${CMAKE_SOURCE_DIR}/Model/source/source.cpp"
	"${CMAKE_SOURCE_DIR}/Model/source/volumeArea.cpp"
	"${CMAKE_SOURCE_DIR}/Model/source/cadrw/cad_common.cpp"
	"${CMAKE_SOURCE_DIR}/Model/source/cadrw/cad_parser.cpp"
	"${CMAKE_SOURCE_DIR}/Model/source/cadrw/stl_parser.cpp"
	"${CMAKE_SOURCE_DIR}/Model/source/detector.cu"
	"${CMAKE_SOURCE_DIR}/Model/source/volumeArea.cu"
	
)

set(MY_HEADERS
    "${CMAKE_SOURCE_DIR}/Model/include/detector.h"
    "${CMAKE_SOURCE_DIR}/Model/include/modelDefs.h"
	"${CMAKE_SOURCE_DIR}/Model/include/modelExceptions.h"
	"${CMAKE_SOURCE_DIR}/Model/include/scanObject.h"
	"${CMAKE_SOURCE_DIR}/Model/include/source.h"
	"${CMAKE_SOURCE_DIR}/Model/include/utilities.h"
	"${CMAKE_SOURCE_DIR}/Model/include/volumeArea.h"
	"${CMAKE_SOURCE_DIR}/Model/include/vecMath.cuh"
)

set(CADRW_SOURCES
    "${CMAKE_SOURCE_DIR}/Model/source/cadrw/cad_common.cpp"
    "${CMAKE_SOURCE_DIR}/Model/source/cadrw/cad_parser.cpp"
	"${CMAKE_SOURCE_DIR}/Model/source/cadrw/stl_parser.cpp"
)

set(CADRW_HEADERS
    "${CMAKE_SOURCE_DIR}/Model/include/cadrw/cad_common.h"
    "${CMAKE_SOURCE_DIR}/Model/include/cadrw/cad_parser.h"
	"${CMAKE_SOURCE_DIR}/Model/include/cadrw/stl_parser.h"
)

source_group("Source Files\\cadrw" FILES ${CADRW_SOURCES})
source_group("Header Files\\cadrw" FILES ${CADRW_HEADERS})
source_group("Header Files" FILES ${MY_HEADERS})


add_executable(AcquisitionModel 
	${MY_SOURCES} 
)

target_sources(AcquisitionModel PRIVATE 
	${MY_SOURCES} 
    ${CADRW_SOURCES}
    ${MY_HEADERS}
	${CADRW_HEADERS}
)

target_include_directories(AcquisitionModel PRIVATE 
	${CMAKE_SOURCE_DIR}/Model/include
	${CMAKE_SOURCE_DIR}/Model/include/cadrw	
)

target_link_libraries(AcquisitionModel PRIVATE 
	glm::glm
	${OpenCV_LIBS}
	CUDA::cudart
)

target_compile_features(AcquisitionModel PRIVATE cxx_std_20)

# Link the imported target

set_target_properties(AcquisitionModel PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/bin"
	CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
	CUDA_STANDARD 20
    CUDA_STANDARD_REQUIRED ON
    CUDA_ARCHITECTURES 80 90 100
)




