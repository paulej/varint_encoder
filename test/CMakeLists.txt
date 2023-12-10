# Create the test excutable
add_executable(test_varint_encoder test_varint_encoder.cpp)

# Link to the required libraries
target_link_libraries(test_varint_encoder varint_encoder STF::stf)

# Specify the C++ standard to observe
set_target_properties(test_varint_encoder
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

# Specify the compiler options
target_compile_options(test_varint_encoder
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wpedantic -Wextra -Wall>
        $<$<CXX_COMPILER_ID:MSVC>: >)

# Ensure CTest can find the test
add_test(NAME test_varint_encoder
         COMMAND test_varint_encoder)
