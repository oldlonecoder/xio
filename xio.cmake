#project(tlux VERSION 0.0.1 LANGUAGES CXX)


include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

#include_directories(/usr/local/include)
add_library(
        ${PROJECT_NAME}
        SHARED
        include/${PROJECT_NAME}/types.h src/types.cc
        include/${PROJECT_NAME}/lexer/lexer.h src/lexer/lexer.cc
        include/${PROJECT_NAME}/lexer/tokendata.h src/lexer/tokendata.cc
        include/${PROJECT_NAME}/lexer/lexer_color.h src/lexer/lexer_color.cc
        include/${PROJECT_NAME}/alu.h src/alu.cc
        include/${PROJECT_NAME}/xiobject.h src/xiobject.cc
        include/${PROJECT_NAME}/xiobloc.h src/xiobloc.cc
        include/${PROJECT_NAME}/xiovar.h src/xiovar.cc
)


target_compile_definitions(${PROJECT_NAME} PUBLIC "${PROJECT_NAME}_DEBUG=$<CONFIG:Debug>")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${${PROJECT_NAME}_export})

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
    EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            )
ENDIF()

target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:/usr/local/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
        )

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS} tlux)


install(DIRECTORY
        include/${PROJECT_NAME}/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}"
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

