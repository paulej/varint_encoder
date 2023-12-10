# Enable fetching content
include(FetchContent)

# Fetch the Simple Test Framework library
FetchContent_Declare(stf
    GIT_REPOSITORY https://github.com/paulej/stf.git
    GIT_TAG        master)

# It is not necessary to install the STF library
set(stf_INSTALL OFF)

# Make dependencies available
FetchContent_MakeAvailable(stf)
