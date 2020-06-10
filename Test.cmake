
set(GTEST_DIR ${DIR_3RDPARTY}/gtest-1.7.0)
SET(PROJECT_SRC ${pc_SOURCE_DIR}/src)
set(GTEST_INCLUDE_DIRECTORIES ${GTEST_DIR}/include ${GTEST_DIR} ${GTEST_DIR}/src)
MESSAGE( "Attempt to build gtest. gtest directory: " ${GTEST_DIR})
include_directories(${GTEST_INCLUDE_DIRECTORIES})
add_library(gtest_170_lib ${GTEST_DIR}/src/gtest-all.cc)
set_target_properties(gtest_170_lib PROPERTIES COMPILE_DEFINITIONS "GTEST_HAS_RTTI=0")
enable_testing(true)

include_directories(test)
include_directories(${PROJECT_SRC})

file(GLOB TEST_SRC_FILES "test/*.cpp")
 
set(TestRunner UnitTestRunner)


# build the unit tests   
SET(CMAKE_CXX_FLAGS "-Wall -Wno-sign-compare -rdynamic -Wunused -std=c++11 -pthread -D_GLIBCXX_USE_NANOSLEEP")
add_executable(${TestRunner} thirdparty/test_main.cpp ${TEST_SRC_FILES} )
set_target_properties(${TestRunner} PROPERTIES COMPILE_DEFINITIONS "GTEST_HAS_TR1_TUPLE=0")
set_target_properties(${TestRunner} PROPERTIES COMPILE_DEFINITIONS "GTEST_HAS_RTTI=0")
set_target_properties(${TestRunner} PROPERTIES COMPILE_FLAGS "-isystem -pthread ")
target_link_libraries(${TestRunner} ${LIBRARY_TO_BUILD} gtest_170_lib -lstdc++ ${PLATFORM_LINK_LIBRIES})

# to manually run cmake
#/usr/local/probe/bin/cmake -DCMAKE_CXX_COMPILER_ARG1:STRING=' -fPIC -Ofast -m64 -Wl,-rpath -Wl,.' -DCMAKE_BUILD_TYPE:STRING=Release -DBUILD_SHARED_LIBS:BOOL=ON ..
