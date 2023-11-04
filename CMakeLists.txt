cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
## Project name

project(Spp VERSION 0.1.0 LANGUAGES CXX)

message("Entering ${PROJECT_NAME} project configuration:")

include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(TargetName ${PROJECT_NAME})

SET(FRAMEWORK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library(
        ${TargetName}
        SHARED
        include/${TargetName}/Exports.h             #global framework-wide macros definitions and dll export and import macros for MSVC.
        include/Spp/SppTypes.h                          Src/SppTypes.cxx
        include/Spp/Alu.h                               Src/Alu.cxx
        include/Spp/SppToken.h                          Src/SppToken.cxx
        include/Spp/Compiler/Lexer.h                    Src/Compiler/Lexer.cxx
        include/Spp/xio.h Src/xio.cxx
        include/Spp/Compiler/Experimentation/Grammar.h  Src/Compiler/Experimentation/Grammar.cxx
        include/Spp/Stack.h Src/Stack.cxx
        include/Spp/Runtime/Variable.h Src/Runtime/Variable.cxx
        include/Spp/Runtime/RTBase.h                    Src/Runtime/RTBase.cxx
        SppTests/Spp.cxx
        Src/Runtime/Interpreter.cxx
        include/Spp/Runtime/Interpreter.h
        include/Spp/Compiler/Compiler.cxx
        include/Spp/Compiler/Compiler.h
        Src/Runtime/Unit.cxx
        include/Spp/Runtime/Unit.h


        #       include/${TargetName}/alu.h                     src/alu.cc
#       include/${TargetName}/tokendata.h               src/tokendata.cc
#       include/${TargetName}/xio.h                     src/xio.cc  # La pièce maîtresse ;)
#       include/${TargetName}/xiobloc.h                 src/xiobloc.cc
#       include/${TargetName}/xiovar.h                  src/xiovar.cc
#       include/${TargetName}/compiler/lexer.h          src/compiler/lexer.cc
#       include/${TargetName}/compiler/grammar.h        src/compiler/grammar.cc
#       include/${TargetName}/compiler/compiler.h       src/compiler/compiler.cc
#       include/${TargetName}/s++/amu.h                 src/s++/amu.cc
#       include/${TargetName}/s++/spp.h                 src/s++/spp.cc

        # ...Will grow...
)


target_compile_definitions(${TargetName} PUBLIC "${TargetName}_DEBUG=$<CONFIG:Debug>")
target_compile_features(${TargetName} PUBLIC cxx_std_20)

include(GenerateExportHeader)
generate_export_header(${TargetName} EXPORT_FILE_NAME ${${TargetName}_export})

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
    EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            )
ENDIF()

target_include_directories(${TargetName} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        $<INSTALL_INTERFACE:include/${TargetName}>
        )

target_link_libraries(${TargetName} ${CMAKE_DL_LIBS} AppBook) # and normally logbook depends on chrtools


install(DIRECTORY
        include/${TargetName}/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${TargetName}"
)

install(TARGETS ${TargetName}
        EXPORT ${TargetName}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
)
#add_subdirectory(devprogapp)
