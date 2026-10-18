cmake_minimum_required(VERSION 3.21)
PROJECT(scruffy VERSION "2023.08.27" LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS ON)

if(PROJECT_IS_TOP_LEVEL)
  cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
  cmake_host_system_information(RESULT OSN QUERY OS_NAME)
  cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
  cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

  message(STATUS "-------------------------------------")
  message(STATUS "  Project: ${PROJECT_NAME}")
  message(STATUS "  C ${CMAKE_C_STANDARD}")
  message(STATUS "-------------------------------------")
  message(STATUS " ${HOST}")
  message(STATUS " ${OSN}: ${OS_VERSION}")
  message(STATUS " ${PROC}")
  message(STATUS "-------------------------------------")

  set(BUILD_EXAMPLES ON)
else()
  message(STATUS "-> ${PROJECT_NAME} is submodule")
  set(BUILD_EXAMPLES OFF)
endif()

# print some system info using CMake
function(scruffy_info project_name)
    get_property(enabled_langs GLOBAL PROPERTY ENABLED_LANGUAGES)

    if("CXX" IN_LIST enabled_langs)
        target_compile_definitions(${project_name} PRIVATE
            CXX_INFO="C++${CMAKE_CXX_STANDARD} ${CMAKE_CXX_COMPILER_ID}: ${CMAKE_CXX_COMPILER}")
    endif()

    if("C" IN_LIST enabled_langs)
        target_compile_definitions(${project_name} PRIVATE 
            C_INFO="C${CMAKE_C_STANDARD} ${CMAKE_C_COMPILER_ID}: ${CMAKE_C_COMPILER}")
    endif()

    cmake_host_system_information(RESULT TOTAL_MEM  QUERY TOTAL_PHYSICAL_MEMORY)
    cmake_host_system_information(RESULT OS_NAME    QUERY OS_NAME)
    cmake_host_system_information(RESULT OS_VERSION QUERY OS_VERSION)
    cmake_host_system_information(RESULT OS_RELEASE QUERY OS_RELEASE)
    cmake_host_system_information(RESULT PROC       QUERY PROCESSOR_DESCRIPTION)
    target_compile_definitions(${project_name} PRIVATE
        OS_INFO="${OS_NAME} ${OS_RELEASE} [${OS_VERSION}]"
        CPU_INFO="CPU: ${PROC}"
        RAM_INFO="RAM: ${TOTAL_MEM} MB"
    )
endfunction()

add_library(${PROJECT_NAME} 
    src/scruffy.c
)
target_include_directories(${PROJECT_NAME} PUBLIC src/)

# Examples -----------------------------------------------------------
message(STATUS "-> Building ${PROJECT_NAME} examples is ${BUILD_EXAMPLES}")
if(${BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()
