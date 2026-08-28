set (MY_SOURCES
	"${CMAKE_SOURCE_DIR}/SaveVolumeToFile/source/main.cpp"
	"${CMAKE_SOURCE_DIR}/SaveVolumeToFile/source/io/ioData.cpp"
)

set(MY_HEADERS
    "${CMAKE_SOURCE_DIR}/SaveVolumeToFile/include/io/ioData.h"
)

source_group("Source Files\\source\\io" FILES "${CMAKE_SOURCE_DIR}/SaveVolumeToFile/source/io/ioData.cpp")
source_group("Source Files\\source" FILES "${CMAKE_SOURCE_DIR}/SaveVolumeToFile/source/main.cpp")
source_group("Header Files\\io" FILES ${MY_HEADERS})

add_executable(SaveVolumeToFile 
	${MY_SOURCES} 
)

target_sources(SaveVolumeToFile PRIVATE 
	${MY_SOURCES} 
    ${MY_HEADERS}
)

target_include_directories(SaveVolumeToFile PRIVATE 
	${CMAKE_SOURCE_DIR}/SaveVolumeToFile/include
)

target_compile_features(SaveVolumeToFile PRIVATE cxx_std_20)

if(WIN32)
add_custom_command(TARGET SaveVolumeToFile POST_BUILD

	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/SaveVolumeToFile/volumeHeader.txt" "${CMAKE_SOURCE_DIR}/build/SaveVolumeToFile/volumeHeader.txt"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/SaveVolumeToFile/volumeHeader.txt" "${CMAKE_SOURCE_DIR}/build/x64/$<CONFIG>/volumeHeader.txt"
)


set_target_properties(SaveVolumeToFile PROPERTIES
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
		
	set_target_properties(SaveVolumeToFile PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
		#RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
		#RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
	)

	add_custom_command(TARGET SaveVolumeToFile POST_BUILD

		# Copy shaders directory to both locations
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/SaveVolumeToFile/volumeHeader.txt" "${CMAKE_BINARY_DIR}/bin/volumeHeader.txt"
	)
endif()




