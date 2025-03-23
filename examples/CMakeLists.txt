cmake_minimum_required(VERSION 3.24)

file(GLOB_RECURSE EXAMPLE_FILES *.cpp)

foreach(FILE ${EXAMPLE_FILES})
    get_filename_component(FILENAME ${FILE} NAME_WE)
    get_filename_component(DIR ${FILE} DIRECTORY)
    file(RELATIVE_PATH REL_DIR ${CMAKE_SOURCE_DIR}/examples ${DIR})

    set(TARGET_NAME "${FILENAME}")
    add_executable(${TARGET_NAME} ${FILE})

    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/${REL_DIR}"
    )
    message(STATUS "Added target: ${TARGET_NAME} -> ${CMAKE_BINARY_DIR}/bin/${REL_DIR}/${FILENAME}.exe")

    target_link_libraries(${TARGET_NAME} core)
endforeach()
