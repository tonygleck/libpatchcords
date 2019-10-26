#Licensed under the MIT license. See LICENSE file in the project root for full license information.

cmake_minimum_required(VERSION 3.3.0)

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(patchcords)

option(patchcords_ut "Include unittest in build" OFF)
option(patchcords_samples "Include samples in build" OFF)

include("${CMAKE_CURRENT_LIST_DIR}/cmake_configs/proj_config.cmake")

if ((NOT TARGET azure_macro_utils_c) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/azure-macro-utils-c/CMakeLists.txt))
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/deps/azure-macro-utils-c)
    include_directories(${MACRO_UTILS_INC_FOLDER})
endif()
if (NOT TARGET umock_c)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/deps/umock-c)
    include_directories(${UMOCK_C_INC_FOLDER})
endif()
if (NOT TARGET lib-util-c)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/deps/lib-util-c)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/deps/lib-util-c/inc/)
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)

#these are the C source files
set(source_c_files
    ./src/socket_debug_shim.c
    ./src/xio_client.c
)

#these are the C headers
set(source_h_files
    ./inc/patchcords/socket_debug_shim.h
    ./inc/patchcords/xio_client.h
)

#this is the product (a library)
add_library(patchcords ${source_c_files} ${source_h_files})

# Berkley socket library
set(socket_berkley_h_files
    ./inc/patchcords/xio_socket.h
)
set(socket_berkley_c_files
    ./src/xio_client_berkley.c
)
add_library(cord_berkley ${socket_berkley_c_files} ${socket_berkley_h_files})
target_link_libraries(cord_berkley patchcords lib-util-c)

if (${patchcords_ut})
    enable_testing()

    include("${CMAKE_CURRENT_LIST_DIR}/cmake_configs/proj_testing.cmake")

    if ((NOT TARGET testrunnerswitcher) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/azure-c-testrunnerswitcher/CMakeLists.txt))
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/azure-c-testrunnerswitcher)
        include_directories(${TESTRUNNERSWITCHER_INC_FOLDER})
    endif()
    if ((NOT TARGET ctest) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/azure-ctest/CMakeLists.txt))
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/azure-ctest)
        include_directories(${CTEST_INC_FOLDER})
    endif()

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if (${patchcords_samples})
    add_subdirectory(samples)
endif()