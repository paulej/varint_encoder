# Create the library
add_library(varint_encoder varint_encoder.cpp)

# Make project include directory available to external projects
target_include_directories(varint_encoder
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

# Specify the C++ standard to observe
set_target_properties(varint_encoder
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

# Use the following compile options
target_compile_options(varint_encoder
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wpedantic -Wextra -Wall>
        $<$<CXX_COMPILER_ID:MSVC>: >)

# Install target and associated include files
if(varint_encoder_INSTALL)
    install(TARGETS varint_encoder EXPORT VarIntEncoderTargets ARCHIVE)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ TYPE INCLUDE)
    install(EXPORT VarIntEncoderTargets
            FILE varint_encoderConfig.cmake
            NAMESPACE Dyius::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/varint_encoder)
endif()
