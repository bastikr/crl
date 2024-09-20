cmake_minimum_required(VERSION 3.14)

set(SRM_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR})

function(add_resource_library TARGET_NAME SOURCE_DIR)
    get_filename_component(SOURCE_DIR "${SOURCE_DIR}" ABSOLUTE)

    # Collect all files to rebuild when something changes
    file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*")

    set(output_cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/src/${TARGET_NAME}.cpp")
    set(output_h_file "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/include/${TARGET_NAME}.h")
    add_custom_command(
        OUTPUT "${output_cpp_file}" "${output_h_file}"
        COMMAND ${CMAKE_COMMAND} -D TARGET_NAME=${TARGET_NAME}
                                 -D SOURCE_DIR=${SOURCE_DIR}
                                 -P "${SRM_CMAKE_DIR}/generate.cmake"
        DEPENDS ${RESOURCE_FILES} "${SRM_CMAKE_DIR}/generate.cmake"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}"
        COMMENT "Creating resource library for ${TARGET_NAME} from  ${SOURCE_DIR}"
        VERBATIM
    )
    add_custom_target(${TARGET_NAME}_gen_target DEPENDS "${output_cpp_file}")

    add_library(${TARGET_NAME} STATIC "${output_cpp_file}")
    add_dependencies(${TARGET_NAME} ${TARGET_NAME}_gen_target)
    target_include_directories(${TARGET_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/include")
    target_link_libraries(${TARGET_NAME} PUBLIC crl)
endfunction()
