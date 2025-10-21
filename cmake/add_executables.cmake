# ---------------------------------------------------------------------------------------------------------------------
# Adds targets for lots of small executables (typically all the files from some directory).
# See: https://nessan.github.io/cmake/
#
# SPDX-FileCopyrightText:  2023 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
# SPDX-License-Identifier: MIT
# ---------------------------------------------------------------------------------------------------------------------

# add_executables(PATH [PATH ...] [LIBRARIES <lib1> <lib2> ...])
#
# This function processes source files in the provided PATH arguments (each of which may be a directory or an individual
# source file) and generates corresponding executable targets.
#
# Each source file is assumed to generate a standalone executable program (so Foo.cpp -> Foo).
#
# Parameters:
#   PATH      - One or more directories and/or individual source files to process.
#   LIBRARIES - Optional list of libraries to link executables to.
#
# Example:
#   add_executables(examples extra/example.cpp LIBRARIES gf2::gf2 utilities::utilities)
function(add_executables)

    # Parse arguments
    set(options)
    set(oneValueArgs)
    set(multiValueArgs LIBRARIES)
    cmake_parse_arguments(NEEDED "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(source_inputs ${NEEDED_UNPARSED_ARGUMENTS})
    if(NOT source_inputs)
        message(FATAL_ERROR "add_executables: At least one directory or source file must be specified.")
    endif()

    set(valid_extensions ".c" ".cc" ".cxx" ".cpp" ".C" ".c++")
    set(collected_sources "")

    foreach(input_path ${source_inputs})
        if(IS_ABSOLUTE "${input_path}")
            set(abs_input "${input_path}")
        else()
            get_filename_component(abs_input "${input_path}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
        endif()

        if(IS_DIRECTORY "${abs_input}")
            file(GLOB_RECURSE dir_sources CONFIGURE_DEPENDS LIST_DIRECTORIES false
                "${abs_input}/*.c"
                "${abs_input}/*.cc"
                "${abs_input}/*.cxx"
                "${abs_input}/*.cpp"
                "${abs_input}/*.C"
                "${abs_input}/*.c++")
            if(dir_sources)
                list(APPEND collected_sources ${dir_sources})
            else()
                message(WARNING "add_executables: No source files found in directory '${abs_input}'.")
            endif()
        elseif(EXISTS "${abs_input}")
            cmake_path(GET abs_input EXT input_ext)
            if(NOT input_ext IN_LIST valid_extensions)
                message(FATAL_ERROR "add_executables: File '${input_path}' does not have a supported C/C++ extension.")
            endif()
            list(APPEND collected_sources "${abs_input}")
        else()
            message(FATAL_ERROR "add_executables: Path '${input_path}' does not exist.")
        endif()
    endforeach()

    if(NOT collected_sources)
        message(WARNING "add_executables: No source files were discovered in the provided inputs.")
        return()
    endif()

    list(REMOVE_DUPLICATES collected_sources)
    list(SORT collected_sources)

    foreach(src_file ${collected_sources})
        cmake_path(GET src_file STEM target_name)

        if(TARGET ${target_name})
            message(WARNING "add_executables: Target '${target_name}' already exists; skipping source '${src_file}'.")
            continue()
        endif()

        add_executable(${target_name} ${src_file})
        if(NEEDED_LIBRARIES)
            target_link_libraries(${target_name} ${NEEDED_LIBRARIES})
        endif()
    endforeach()

endfunction()
