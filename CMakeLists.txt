cmake_minimum_required(VERSION 3.17)

# Set options depending on whether this is a subproject
if(DEFINED PROJECT_NAME)
    # Option to control whether tests are built
    option(varint_encoder_BUILD_TESTS "Build Tests for the VarInt Encoder Library" OFF)
else()
    # Option to control whether tests are built
    option(varint_encoder_BUILD_TESTS "Build Tests for the VarInt Encoder Library" ON)
endif()

# Option to control ability to install the library
option(varint_encoder_INSTALL "Install the VarInt Encoder Library" ON)

project(varint_encoder
        VERSION 1.0.0.0
        DESCRIPTION "Variable Length Integer Encoding Library"
        LANGUAGES CXX)

add_subdirectory(dependencies)
add_subdirectory(src)

include(CTest)
add_subdirectory(test)

