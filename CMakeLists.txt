cmake_minimum_required(VERSION 3.15..3.18)

project(LR)
find_package(MPI REQUIRED)
find_package(OpenMP)

enable_testing()

add_executable(LR main.c)
target_include_directories(LR PUBLIC ${MPI_INCLUDE_PATH})
if(NOT TARGET OpenMP::OpenMP_CXX)
    find_package(Threads REQUIRED)
    add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_COMPILE_OPTIONS ${OpenMP_CXX_FLAGS})
    # Only works if the same flag is passed to the linker; use CMake 3.9+ otherwise (Intel, AppleClang)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)

endif()
target_link_libraries(LR PUBLIC OpenMP::OpenMP_CXX ${MPI_LIBRARIES})

file(GLOB TEST_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.ctest")

foreach(TEST_FILE ${TEST_FILES})
	add_test(
		NAME ${TEST_FILE} 
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMAND sh
			${CMAKE_CURRENT_SOURCE_DIR}/${TEST_FILE}
	)
endforeach()

add_custom_target(distclean 
	COMMAND rm -rf CMakeCache.txt CMakeFiles CTestTestfile.cmake Makefile Testing cmake_install.cmake
)