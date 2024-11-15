#include "ImageFetcher.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    // Check if the user provided at least one argument
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [--releases | --lts | --sha256 <release_name>]" << std::endl;
        return 1;
    }

    std::cout << "Program started!" << std::endl;

    // Instantiate the ImageFetcher object
    ImageFetcher fetcher;

    // Parse the first command-line argument
    std::string option = argv[1];

    // If the user requests a list of supported releases
    if (option == "--releases") {
        try {
            // Fetch the list of supported Ubuntu releases
            auto releases = fetcher.getSupportedReleases();
            std::cout << "Supported Ubuntu Releases:" << std::endl;

            // Print each release
            for (const auto& release : releases) {
                std::cout << release << std::endl;
            }
        } catch (const std::exception& e) {
            // Handle any errors during fetching
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
    // If the user requests the current LTS version
    else if (option == "--lts") {
        try {
            // Fetch the current LTS version of Ubuntu
            std::string ltsVersion = fetcher.getCurrentLTSVersion();

            // Check if an LTS version was found
            if (!ltsVersion.empty()) {
                std::cout << "Current LTS Version: " << ltsVersion << std::endl;
            } else {
                std::cerr << "Error: No LTS version found!" << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            // Handle any errors during fetching
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
    // If the user requests the SHA256 checksum for a specific release
    else if (option == "--sha256" && argc == 3) {
        std::string releaseName = argv[2];
        try {
            // Fetch the SHA256 checksum for the specified release
            std::string sha256 = fetcher.getSha256ForDiskImage(releaseName);

            // Check if the checksum was found
            if (!sha256.empty()) {
                std::cout << "SHA256 for " << releaseName << ": " << sha256 << std::endl;
            } else {
                std::cerr << "Error: SHA256 not found for release " << releaseName << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            // Handle any errors during fetching
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
    // If the user provides an unknown option or the wrong number of arguments
    else {
        std::cerr << "Unknown option or missing argument." << std::endl;
        std::cerr << "Usage: " << argv[0] << " [--releases | --lts | --sha256 <release_name>]" << std::endl;
        return 1;
    }

    return 0;
}
