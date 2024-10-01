cmake_minimum_required(VERSION 3.17)

# add_resource_library(<target_name> <source_dir> [GPERF])
function(add_resource_library TARGET_NAME SOURCE_DIR)
    set(options "GPERF")
    set(oneValueArgs "")
    set(multiValueArgs "")
    cmake_parse_arguments(CRL "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

    get_filename_component(SOURCE_DIR "${SOURCE_DIR}" ABSOLUTE)

    # Collect all files to rebuild when something changes
    file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*")

    set(target_directory "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}")
    set(output_cpp_file "${target_directory}/src/${TARGET_NAME}.cpp")
    set(output_h_file "${target_directory}/include/${TARGET_NAME}.h")

    file(MAKE_DIRECTORY "${target_directory}")
    add_custom_command(
        OUTPUT "${output_cpp_file}" "${output_h_file}"
        COMMAND ${CMAKE_COMMAND} -D TARGET_NAME=${TARGET_NAME}
                                 -D SOURCE_DIR=${SOURCE_DIR}
                                 -D ENABLE_GPERF=${CRL_GPERF}
                                 -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/generate.cmake"
        DEPENDS ${RESOURCE_FILES} "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/generate.cmake"
        WORKING_DIRECTORY "${target_directory}"
        COMMENT "Creating resource library for ${TARGET_NAME} from  ${SOURCE_DIR}"
        VERBATIM
    )
    add_custom_target(${TARGET_NAME}_gen_target DEPENDS "${output_cpp_file}")

    add_library(${TARGET_NAME} STATIC "${output_cpp_file}")
    add_dependencies(${TARGET_NAME} ${TARGET_NAME}_gen_target)
    target_include_directories(${TARGET_NAME} PUBLIC "${target_directory}/include")
    target_link_libraries(${TARGET_NAME} PUBLIC crl)
endfunction()
