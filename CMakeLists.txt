cmake_minimum_required(VERSION 3.23)

# Our project
project(utilities DESCRIPTION "C++ utilities Library" LANGUAGES CXX)

# Add a target for the "library" we are building (it is header-only, hence INTERFACE).
# Also add the usual alias for this library name.
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# We use C++20 features
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

# Where to find the project headers (e.g., how to resolve `#include "utilities/format.h"`).
target_sources(${PROJECT_NAME} INTERFACE
    FILE_SET    library_headers
    TYPE        HEADERS
    BASE_DIRS   include/
)

# That's it unless we are developing the library instead of just using it!
# If we are developing the library, then we go ahead and create targets for the executables in the examples/ directory
if (PROJECT_IS_TOP_LEVEL)

    # Append our local directory of CMake modules to the default ones searched by CMake
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

    # Prevent in-source builds for the example programs
    include(disable_in_source_builds)

    # Make the compiler issue warnings for "bad" code, etc.
    include(compiler_init)
    compiler_init(${PROJECT_NAME})

    # For neatness, we want all the example executables to go in build/bin/.
    include(GNUInstallDirs)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")

    # Finally, we walk through the examples directory and build a target for each .cpp file with appropriate linkage.
    # We have a CMake function that makes that traversal straightforward.
    include(add_executables)
    add_executables(examples ${PROJECT_NAME}::${PROJECT_NAME})

endif()
