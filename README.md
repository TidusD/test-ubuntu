
# Ubuntu Image Fetcher Setup Instructions

## Prerequisites

### On Windows:

To build and use this project on Windows, you need to have the correct compiler setup, particularly when using **MinGW** with **vcpkg**.

1. **Set the required environment variables for MinGW**:

   Open a PowerShell window and set the environment variables as follows:

   ```powershell
   $env:VCPKG_DEFAULT_HOST_TRIPLET="x64-mingw-dynamic"
   $env:VCPKG_DEFAULT_TRIPLET="x64-mingw-dynamic"
   ```

   These variables ensure that the proper toolchain is selected for MinGW when using vcpkg.

2. **Install dependencies via vcpkg**:

   Use vcpkg to install dependencies such as OpenSSL and nlohmann_json for the MinGW triplet.

   Example command to install OpenSSL:

   ```powershell
   .\vcpkg install openssl --triplet x64-mingw-dynamic
   ```

### On Linux:

On Linux, ensure that your system has OpenSSL and nlohmann_json available. You can install them through your package manager or compile from source.

For example, on Ubuntu, use:

```bash
sudo apt-get install libssl-dev nlohmann-json3-dev
```

### On macOS:

On macOS, you can use **Homebrew** to install the required dependencies:

```bash
brew install openssl
brew install nlohmann-json
```

## Build Instructions

### 1. Generate the Build Files

On Windows:

```bash
cmake -G "MinGW Makefiles" -B build -DCMAKE_BUILD_TYPE=Release -DVCPKG_PATH="C:\path\to\vcpkg" -DCMAKE_PREFIX_PATH="C:\path\to\vcpkg\installed\x64-mingw-dynamic\share" -DCMAKE_TOOLCHAIN_FILE="C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake"
```

On Linux:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

On macOS:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

- For Windows, replace `C:\path\to\vcpkg` with the actual path where you have your `vcpkg` installed.

### 2. Build the Project

Once the configuration is done, build the project with:

```bash
cmake --build build --config Release --target UbuntuImageFetcher
```

### 3. Run the Application

Once the build is complete, run the application using the following command:

```bash
./build/UbuntuImageFetcher --releases
```

Replace `--releases` with other flags as needed for your use case (`--lts`, `--sha256 <release_name>`).

## License

This project is licensed under the MIT License - see the LICENSE file for details.
