cmake_minimum_required(VERSION 3.10)
project(UbuntuImageFetcher)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)
include_directories(lib)

# Detect operating system and set package installation commands
if (WIN32)
    # Windows: Use vcpkg
    if (NOT DEFINED VCPKG_PATH)
        message(WARNING "VCPKG_PATH not provided. Please set VCPKG_PATH manually: -DVCPKG_PATH=/path/to/vcpkg")
    else()
        message(STATUS "Using vcpkg path: ${VCPKG_PATH}")
        # Verify vcpkg.exe exists
        if (NOT EXISTS "${VCPKG_PATH}/vcpkg.exe")
            message(FATAL_ERROR "vcpkg.exe not found at ${VCPKG_PATH}. Please verify the VCPKG_PATH.")
        endif()
        set(VCPKG_PATH "${VCPKG_PATH}\\\\vcpkg.exe")
        set(OPENSSL_INSTALL_COMMAND "\"${VCPKG_PATH}\" install openssl --triplet x64-mingw-dynamic")
        set(NLOHMANN_JSON_INSTALL_COMMAND "\"${VCPKG_PATH}\" install nlohmann-json --triplet x64-mingw-dynamic")
    endif()
elseif (APPLE)
    # macOS: Use Homebrew
    set(OPENSSL_INSTALL_COMMAND "brew install openssl")
    set(NLOHMANN_JSON_INSTALL_COMMAND "brew install nlohmann-json")
elseif (UNIX)
    # Linux: Use apt-get for Debian/Ubuntu-based distributions
    set(OPENSSL_INSTALL_COMMAND "sudo apt-get install -y libssl-dev")
    set(NLOHMANN_JSON_INSTALL_COMMAND "sudo apt-get install -y nlohmann-json3-dev")
else()
    message(FATAL_ERROR "Unsupported operating system.")
endif()

# Define a macro to check and install a package if not found
macro(ensure_package_installed package_name install_command)
    find_package(${package_name} QUIET)

    if (NOT ${package_name}_FOUND)
        message(STATUS "${package_name} not found, installing via package manager...")

        execute_process(
            COMMAND ${install_command}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output
            ERROR_VARIABLE error_output
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE
        )

        message(STATUS "Output: ${output}")
        message(STATUS "Error Output: ${error_output}")

        if (NOT result EQUAL 0)
            message(FATAL_ERROR "Failed to install ${package_name}. Please check the error output above.")
        endif()

        # Re-run find_package after installation
        find_package(${package_name} REQUIRED)
    endif()
endmacro()

# Ensure nlohmann_json is installed
ensure_package_installed(nlohmann_json "${NLOHMANN_JSON_INSTALL_COMMAND}")

# Ensure OpenSSL is installed
ensure_package_installed(OpenSSL "${OPENSSL_INSTALL_COMMAND}")

# Define the executable
add_executable(UbuntuImageFetcher src/main.cpp src/ImageFetcher.cpp)

# Link libraries: nlohmann_json and OpenSSL (SSL and Crypto)
target_link_libraries(UbuntuImageFetcher PRIVATE
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
)

if(WIN32)
    target_link_libraries(UbuntuImageFetcher PRIVATE
        ws2_32
        crypt32
    )
endif()