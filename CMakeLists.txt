
cmake_minimum_required(VERSION 3.20)

project(
    teaParser
    VERSION 0.1.0
    LANGUAGES C
)

add_library(teaparser
    src/parser.c
)

add_library(teaParser::teaParser ALIAS teaparser)

target_compile_features(teaparser PUBLIC c_std_99)

target_include_directories(teaparser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

include(CTest)

if(BUILD_TESTING)
    add_executable(test_parser
        tests/test_parser.c
    )

    target_link_libraries(test_parser
        PRIVATE teaparser
    )

    add_test(
        NAME parser_lifecycle
        COMMAND test_parser
    )
endif()
