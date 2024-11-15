
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

- For Windows, replace `C:\path\to\vcpkg` with the actual path where you have your vcpkg installed.

### 2. Build the Project

Once the configuration is done, build the project with:

```bash
cmake --build build --config Release --target UbuntuImageFetcher
```

### 3. Run the Application

Once the build is complete, you can run the application using the commands below.

## Usage Instructions

After building the executable, you can use it to interact with Ubuntu releases through various commands. Note that the path separator (`/` or `\`) and the format to access the executable may vary depending on your operating system. For instance:

- On **Linux** and **macOS**, use `/` (forward slash).
- On **Windows**, use `\` (backslash) or specify paths accordingly.

Below are the available options:

1. **List All Supported Ubuntu Releases**

   To display a list of all currently supported Ubuntu releases, run:

   - On Linux/macOS:
     ```bash
     ./build/UbuntuImageFetcher --releases
     ```
   - On Windows:
     ```bash
     .\build\UbuntuImageFetcher.exe --releases
     ```

2. **Get the Current Ubuntu LTS Version**

   To retrieve the current Long-Term Support (LTS) version of Ubuntu, use:

   - On Linux/macOS:
     ```bash
     ./build/UbuntuImageFetcher --lts
     ```
   - On Windows:
     ```bash
     .\build\UbuntuImageFetcher.exe --lts
     ```

3. **Get the SHA256 of the Disk Image for a Given Ubuntu Release**

   To get the SHA256 hash of the `disk1.img` file for a specific Ubuntu release, replace `<release_name>` with the desired release name (e.g., "focal" for Ubuntu 20.04):

   - On Linux/macOS:
     ```bash
     ./build/UbuntuImageFetcher --sha256 <release_name>
     ```
   - On Windows:
     ```bash
     .\build\UbuntuImageFetcher.exe --sha256 <release_name>
     ```

   Example:

   - On Linux/macOS:
     ```bash
     ./build/UbuntuImageFetcher --sha256 focal
     ```
   - On Windows:
     ```bash
     .\build\UbuntuImageFetcher.exe --sha256 focal
     ```

## License

This project is licensed under the MIT License - see the LICENSE file for details.
