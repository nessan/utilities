# ---------------------------------------------------------------------------------------------------------------------
# Extract tests from comments in header files and add targets for each resulting test file.
# See: https://nessan.github.io/doxytest/
#
# SPDX-FileCopyrightText:  2025 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
# SPDX-License-Identifier: MIT
# ---------------------------------------------------------------------------------------------------------------------

# doxytest(HEADER_PATH [HEADER_PATH ...]
#          [DOXY_DIR <dir>]
#          [DOXY_PREFIX <prefix>]
#          [INCLUDES <file1> <file2> ...]
#          [LIBRARIES <lib1> <lib2> ...]
#          [SILENT]
#          [DOXY_MODE <mode>]
#          [DOXY_COMBINED_NAME <basename>])
#          [DOXY_MAX_FAILS <count>])
#          [SCRIPT <path to the doxytest.py script>])
#
# This is a wrapper around the `doxytest.py` script which extracts tests from comments in header files.
#
# Parameters:
#   HEADER_PATH(s)      - The directories and/or individual header files we will process (require at least one)
#   INCLUDES            - Optional: list of header files to include in the generated test source files
#   LIBRARIES           - Optional: list of libraries to link to the test executables
#   DOXY_DIR            - Optional: directory where the test source files will be written (default: doxytests/)
#   DOXY_PREFIX         - Optional: prefix for the generated test source filenames (by default `Foo.h` -> `doxy_Foo.cpp`)
#   DOXY_MODE           - Optional: controls which outputs are generated. Valid modes:
#                         * PER_HEADER  - just generate individual test files (doxy_Foo.cpp, doxy_Bar.cpp, etc.)
#                         * COMBINED    - just generate a single test file containing all tests (doxytests.cpp by default)
#                         * BOTH        - generate both individual tests and a combined test file (this is the default)
#   DOXY_COMBINED_NAME  - Optional: override the basename for the combined test program (default basename is: doxytests)
#   DOXY_MAX_FAILS      - Optional: override the maximum number of allowed failures before aborting (default 10)
#   SILENT              - Optional: flag to suppress progress messages from the doxytest.py script
#   SCRIPT              - Optional: full path to the doxytest.py script (defaults to alongside this module)
#
# Example:
#   doxytest(include/my_lib INCLUDES "<my_lib/my_lib.h>" "<iostream>" LIBRARIES my_lib::my_lib)
#   doxytest(include/foo.h include/bar.h INCLUDES "<my_lib/my_lib.h>" "<iostream>" LIBRARIES my_lib::my_lib)
#   doxytest(include/foo.h DOXY_MODE COMBINED)
#
# This function processes all the `*.h` header files found in the provided directories/files.
# Those files should have comments with embedded code examples that can be extracted and run as tests.
# The function runs the `doxytest.py` script on each header file (e.g. `Foo.h`) to extract the tests and creates a
# corresponding source file in the DOXY_DIR directory (so by default `Foo.h` generates `doxytests/doxy_Foo.cpp`).
# That test source file is then used to create an executable target for a test program (e.g. `doxy_Foo`).
#
# The module also creates an overall `doxytest` target that can be used to force create those sources and targets.

# Set the directory where the doxytest.cmake module is located.
set(_DOXYTEST_MODULE_DIR "${CMAKE_CURRENT_LIST_DIR}")

function(doxytest)

    # Upfront test to make sure the doxytest.py script exists.

    # Parse any recognized arguments (the remaining unparsed args are the header paths we will process).
    set(options SILENT)
    set(oneValueArgs DOXY_DIR DOXY_PREFIX DOXY_MODE DOXY_COMBINED_NAME DOXY_MAX_FAILS SCRIPT)
    set(multiValueArgs INCLUDES LIBRARIES)
    cmake_parse_arguments(DT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Determine the script to execute. Default is alongside this module.
    if(DT_SCRIPT)
        set(DT_SCRIPT_PATH "${DT_SCRIPT}")
    else()
        set(DT_SCRIPT_PATH "${_DOXYTEST_MODULE_DIR}/doxytest.py")
    endif()

    # Error out early if the script was not found.
    if(NOT EXISTS "${DT_SCRIPT_PATH}")
        message(FATAL_ERROR "doxytest: The script '${DT_SCRIPT_PATH}' was not found.")
    endif()

    # Collect the header paths we will process (directories and/or individual header files).
    set(header_inputs ${DT_UNPARSED_ARGUMENTS})
    if(NOT header_inputs)
        message(FATAL_ERROR "doxytest: At least one header file or directory must be specified.")
    endif()

    # Set default DOXY_DIR if not provided -- this is where the generated test sources will be written.
    if(NOT DT_DOXY_DIR)
        set(DT_DOXY_DIR "${CMAKE_SOURCE_DIR}/doxytests")
    endif()

    # Ensure DT_DOXY_DIR is an absolute path
    if(NOT IS_ABSOLUTE ${DT_DOXY_DIR})
        set(DT_DOXY_DIR "${CMAKE_SOURCE_DIR}/${DT_DOXY_DIR}")
    endif()

    # Create DT_DOXY_DIR if it doesn't exist
    if(NOT EXISTS ${DT_DOXY_DIR})
        file(MAKE_DIRECTORY ${DT_DOXY_DIR})
        if(NOT EXISTS ${DT_DOXY_DIR})
            message(FATAL_ERROR "Failed to create test directory: ${DT_DOXY_DIR}")
        endif()
    endif()

    # Establish the filename prefix for the generated test sources (by default header `Foo.h` generates `doxy_Foo.cpp`)
    set(doxy_prefix "doxy_")
    if(DT_DOXY_PREFIX)
        set(doxy_prefix "${DT_DOXY_PREFIX}")
    endif()

    # Determine which outputs we should generate (either individual test files, a combined test file, or both [the default]).
    set(valid_modes PER_HEADER COMBINED BOTH)
    set(doxy_mode "BOTH")
    if(DT_DOXY_MODE)
        string(TOUPPER "${DT_DOXY_MODE}" doxy_mode)
    endif()
    list(FIND valid_modes "${doxy_mode}" mode_index)
    if(mode_index EQUAL -1)
        message(FATAL_ERROR
            "doxytest: DOXY_MODE must be one of PER_HEADER, COMBINED, or BOTH (got: ${DT_DOXY_MODE})")
    endif()
    set(generate_individuals TRUE)
    set(generate_combined TRUE)
    if(doxy_mode STREQUAL "PER_HEADER")
        set(generate_combined FALSE)
    elseif(doxy_mode STREQUAL "COMBINED")
        set(generate_individuals FALSE)
    endif()

    # Resolve the input paths and collect all header files to process.
    set(resolved_inputs "")
    set(header_files "")
    foreach(input_path ${header_inputs})
        if(IS_ABSOLUTE "${input_path}")
            set(abs_input "${input_path}")
        else()
            get_filename_component(abs_input "${input_path}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
        endif()

        if(IS_DIRECTORY "${abs_input}")
            list(APPEND resolved_inputs "${abs_input}")
            file(GLOB dir_headers CONFIGURE_DEPENDS LIST_DIRECTORIES false "${abs_input}/*.h")
            if(dir_headers)
                list(APPEND header_files ${dir_headers})
            else()
                message(WARNING "No header files found in directory: ${abs_input}")
            endif()
        elseif(EXISTS "${abs_input}")
            get_filename_component(input_ext "${abs_input}" EXT)
            if(NOT input_ext STREQUAL ".h")
                message(FATAL_ERROR "doxytest: File is not a header (.h): ${abs_input}")
            endif()
            list(APPEND resolved_inputs "${abs_input}")
            list(APPEND header_files "${abs_input}")
        else()
            message(FATAL_ERROR "doxytest: Header path does not exist: ${input_path}")
        endif()
    endforeach()

    # Error out early if no header files were discovered.
    if(NOT header_files)
        message(WARNING "No header files were discovered in the provided inputs.")
        return()
    endif()

    # Remove duplicates and sort the lists.
    list(REMOVE_DUPLICATES resolved_inputs)
    list(REMOVE_DUPLICATES header_files)
    list(SORT header_files)

    # Determine max fails threshold (default aligns with script default).
    set(doxy_max_fails 10)
    if(DT_DOXY_MAX_FAILS)
        set(doxy_max_fails "${DT_DOXY_MAX_FAILS}")
    endif()
    if(NOT doxy_max_fails MATCHES "^[0-9]+$")
        message(FATAL_ERROR "doxytest: DOXY_MAX_FAILS must be a positive integer")
    endif()
    if(doxy_max_fails STREQUAL "0")
        message(FATAL_ERROR "doxytest: DOXY_MAX_FAILS must be greater than zero")
    endif()

    # Build the base arguments we will pass to the `doxytest.py` script.
    # Note: we use CMake to handle the dependency tracking so we don't need the script to check timestamps.
    # Note: we always generate trivial test files even if no test code blocks are found (so CMake can track dependencies).
    set(base_args "--force" "--always")

    # Add output directory flag.
    list(APPEND base_args "--dir" "${DT_DOXY_DIR}")
    list(APPEND base_args "--max_fails" "${doxy_max_fails}")

    # Add the silent flag if requested (this suppresses progress messages from the script but not errors).
    if(DT_SILENT)
        list(APPEND base_args "--silent")
    endif()

    # Add any extra `#include <...>` flags if requested (these are included in the test source files).
    if(DT_INCLUDES)
        foreach(include_file ${DT_INCLUDES})
            list(APPEND base_args "--include" "${include_file}")
        endforeach()
    endif()

    # Have the python script use the requested filename prefix (so `Foo.h` generates `doxy_Foo.cpp` by default).
    list(APPEND base_args "--prefix" "${doxy_prefix}")

    # Get the list of test source file names & paths for the individual test runs.
    set(individual_sources "")
    if(generate_individuals)
        foreach(header_file ${header_files})
            cmake_path(GET header_file STEM header)
            set(src_filename "${doxy_prefix}${header}.cpp")
            list(APPEND individual_sources "${DT_DOXY_DIR}/${src_filename}")
        endforeach()

        set(individual_command_args ${base_args})
        list(APPEND individual_command_args ${resolved_inputs})

        # Run the `doxytest.py` script to populate the individual test source files.
        # Set a dependency on the header files and the script itself to ensure the test sources are regenerated when needed.
        add_custom_command(
            OUTPUT ${individual_sources}
            COMMAND ${DT_SCRIPT_PATH} ${individual_command_args}
            DEPENDS ${header_files} ${DT_SCRIPT_PATH}
            VERBATIM
        )
    endif()

    # Collect all generated sources so that the governing target can track them.
    set(all_generated_sources "")
    if(individual_sources)
        list(APPEND all_generated_sources ${individual_sources})
    endif()

    # If a combined test file is requested, generate it with a second invocation of the script.
    set(combined_name "")
    set(combined_src "")
    if(generate_combined)
        set(combined_args ${base_args})
        if(DT_DOXY_COMBINED_NAME)
            list(APPEND combined_args "--combined" "${DT_DOXY_COMBINED_NAME}")
            set(combined_basename "${DT_DOXY_COMBINED_NAME}")
        else()
            list(APPEND combined_args "--combined")
            set(combined_basename "doxytests")
        endif()
        set(combined_command_args ${combined_args})
        list(APPEND combined_command_args ${resolved_inputs})

        set(combined_name "${combined_basename}")
        if(NOT combined_name MATCHES "\\.cpp$")
            string(APPEND combined_name ".cpp")
        endif()
        set(combined_src "${DT_DOXY_DIR}/${combined_name}")
        list(APPEND all_generated_sources "${combined_src}")

        add_custom_command(
            OUTPUT ${combined_src}
            COMMAND ${DT_SCRIPT_PATH} ${combined_command_args}
            DEPENDS ${header_files} ${DT_SCRIPT_PATH}
            VERBATIM
        )
    endif()

    # If no outputs are going to be generated, error out.
    if(NOT all_generated_sources)
        message(FATAL_ERROR "doxytest: No outputs would be generated. Check DOXY_MODE and header inputs.")
    endif()

    # Add a target to govern the extraction process itself.
    set(doxytest_target "Generate_doxytest_sources")
    add_custom_target(${doxytest_target} DEPENDS ${all_generated_sources})

    # If a combined test file is requested, add a target for it with appropriate linkage and dependencies.
    if(generate_combined)
        get_filename_component(combined_target "${combined_name}" NAME_WE)
        set_source_files_properties(${combined_src} PROPERTIES GENERATED TRUE)
        add_executable(${combined_target} ${combined_src})
        add_dependencies(${combined_target} ${doxytest_target})
        if(DT_LIBRARIES)
            target_link_libraries(${combined_target} ${DT_LIBRARIES})
        endif()
    endif()

    # Add targets for each test executable, with dependencies on the `doxytest` target and with any needed library linkage.
    if(generate_individuals)
        foreach(header_file ${header_files})
            cmake_path(GET header_file STEM header)
            set(doxy_src_path "${DT_DOXY_DIR}/${doxy_prefix}${header}.cpp")
            set(test_target "${doxy_prefix}${header}")
            set_source_files_properties(${doxy_src_path} PROPERTIES GENERATED TRUE)
            add_executable(${test_target} ${doxy_src_path})
            add_dependencies(${test_target} ${doxytest_target})
            if(DT_LIBRARIES)
                target_link_libraries(${test_target} ${DT_LIBRARIES})
            endif()
        endforeach()
    endif()

endfunction()
