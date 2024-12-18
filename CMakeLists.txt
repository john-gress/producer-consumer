
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
ENABLE_LANGUAGE(CXX)

PROJECT (pc)

SET(PROJECT_SRC ${pc_SOURCE_DIR}/src)
SET(DIR_UNIT_TEST ${pc_SOURCE_DIR}/test)
SET(DIR_3RDPARTY ${pc_SOURCE_DIR}/thirdparty)


INCLUDE_DIRECTORIES(${PROJECT_SRC})
MESSAGE(" PROJECT_SRC = : ${PROJECT_SRC}")
SET(ACTIVE_CPP0xx_DIR "Release")


IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   MESSAGE("")
   MESSAGE("cmake for GCC ")
   SET(PLATFORM_LINK_LIBRIES rt)
   SET(CMAKE_CXX_FLAGS "-Wall -rdynamic -Wunused -std=c++11 -pthread -D_GLIBCXX_USE_NANOSLEEP")
ENDIF()


# GENERIC STEPS
FILE(GLOB SRC_FILES ${PROJECT_SRC}/*.h ${PROJECT_SRC}/*.cpp) 

# Find thirdparty libs

# Setup Library name
SET(LIBRARY_TO_BUILD pc)

# Build the library
INCLUDE_DIRECTORIES(${PROJECT_SRC})

ADD_LIBRARY(${LIBRARY_TO_BUILD} SHARED ${SRC_FILES})
SET(${LIBRARY_TO_BUILD}_VERSION_STRING ${VERSION})
SET_TARGET_PROPERTIES(${LIBRARY_TO_BUILD} PROPERTIES LINKER_LANGUAGE CXX SOVERSION ${VERSION})
TARGET_LINK_LIBRARIES(${LIBRARY_TO_BUILD} ${LIBS})

# Setup unit tests
INCLUDE (${pc_SOURCE_DIR}/Test.cmake)

set(producer_exe producer)
add_executable(${producer_exe} src/producer/producer.cpp)
target_link_libraries(${producer_exe} ${LIBRARY_TO_BUILD} -lstdc++ ${PLATFORM_LINK_LIBRIES})

set(consumer_exe consumer)
add_executable(${consumer_exe} src/consumer/consumer.cpp)
target_link_libraries(${consumer_exe} ${LIBRARY_TO_BUILD} -lstdc++ ${PLATFORM_LINK_LIBRIES})

set(pcchaos_exe pcchaos)
add_executable(${pcchaos_exe} src/pcchaos/pcchaos.cpp)
target_link_libraries(${pcchaos_exe} ${LIBRARY_TO_BUILD} -lstdc++ ${PLATFORM_LINK_LIBRIES})

