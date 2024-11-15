#include "ImageFetcher.h"
#include <nlohmann/json.hpp>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <iostream>

// Constructor
ImageFetcher::ImageFetcher() {
    // Initialize any necessary data (if needed)
}

// Destructor
ImageFetcher::~ImageFetcher() {
    // Clean up resources (if needed)
}

// Fetches the JSON data from the Ubuntu cloud images server
nlohmann::json ImageFetcher::fetchData() const {
    // Create an HTTP client pointing to the base URL
    httplib::Client client("https://cloud-images.ubuntu.com");

    // Perform a GET request to fetch the releases data in JSON format
    auto res = client.Get("/releases/streams/v1/com.ubuntu.cloud:released:download.json");

    // Check if the request was successful (status code 200)
    if (res && res->status == 200) {
        std::cout << "Data fetched successfully" << std::endl;
        // Parse and return the JSON data
        return nlohmann::json::parse(res->body);
    } else {
        // Log the error and throw an exception if the request failed
        std::cout << "Failed to fetch data: " << res.error() << std::endl;
        throw std::runtime_error("Failed to fetch data.");
    }
}

// Returns a list of supported Ubuntu releases
std::vector<std::string> ImageFetcher::getSupportedReleases() const {
    // Fetch the JSON data
    auto data = fetchData();
    std::set<std::string> releases;

    // Check if the JSON data contains the "products" field
    if (data.contains("products")) {
        // Iterate over each product in the "products" field
        for (const auto& product : data["products"].items()) {
            const auto& productData = product.value();

            // Check if the product is supported and has a "release" field
            if (productData.contains("release") && productData["supported"].get<bool>()) {
                // Extract the release title (e.g., "Ubuntu 20.04 LTS")
                std::string releaseTitle = productData["release_title"];

                // Insert the release title into the set (to avoid duplicates)
                releases.insert(releaseTitle);
            }
        }
    } else {
        // Log a message if the "products" field is missing
        std::cout << "'products' field not found in the fetched data!" << std::endl;
    }

    // Convert the set of releases to a vector and return it
    return std::vector<std::string>(releases.begin(), releases.end());
}

// Returns the current LTS (Long Term Support) version of Ubuntu
std::string ImageFetcher::getCurrentLTSVersion() const {
    // Fetch the JSON data
    auto data = fetchData();
    std::string latestLTSVersion;

    // Check if the JSON data contains the "products" field
    if (data.contains("products")) {
        // Iterate over each product in the "products" field
        for (const auto& product : data["products"].items()) {
            const auto& productData = product.value();

            // Check if the product is supported and its release title contains "LTS"
            if (productData.contains("release_title") && productData["supported"].get<bool>()) {
                std::string releaseTitle = productData["release_title"];

                // If "LTS" is found in the title, update the latest LTS version
                if (releaseTitle.find("LTS") != std::string::npos) {
                    if (latestLTSVersion.empty() || releaseTitle > latestLTSVersion) {
                        latestLTSVersion = releaseTitle;
                    }
                }
            }
        }
    }

    // Return the latest LTS version found, or an empty string if none was found
    return latestLTSVersion.empty() ? "" : latestLTSVersion;
}

// Fetches the SHA256 checksum for the disk image of a specified release
#include <regex> // Required for using regex
std::string ImageFetcher::getSha256ForDiskImage(const std::string& releaseName) const {
    // Fetch the JSON data
    auto data = fetchData();

    // Extract the numeric version part from the release name using regex (e.g., "20.04" from "Ubuntu 20.04 LTS")
    std::regex versionRegex(R"(\d+\.\d+)");
    std::smatch match;
    std::string versionPart = releaseName;
    if (std::regex_search(releaseName, match, versionRegex)) {
        versionPart = match.str();
    }

    // Check if the JSON data contains the "products" field
    if (data.contains("products")) {
        // Iterate over each product in the "products" field
        for (const auto& product : data["products"].items()) {
            const auto& productData = product.value();

            // Check if the release matches based on version, codename, or title
            bool isMatch = (productData.contains("release") && productData["release"].get<std::string>().find(versionPart) != std::string::npos) ||
                           (productData.contains("release_codename") && productData["release_codename"].get<std::string>().find(versionPart) != std::string::npos) ||
                           (productData.contains("release_title") && productData["release_title"].get<std::string>().find(versionPart) != std::string::npos);

            // If a matching supported release is found, search for the SHA256 checksum
            if (isMatch && productData["supported"].get<bool>()) {
                if (productData.contains("versions")) {
                    for (const auto& version : productData["versions"].items()) {
                        const auto& versionData = version.value();

                        // Look for the "disk1.img" item and return its SHA256 checksum
                        if (versionData.contains("items") && versionData["items"].contains("disk1.img")) {
                            std::string sha256 = versionData["items"]["disk1.img"]["sha256"].get<std::string>();
                            return sha256;
                        }
                    }
                }
            }
        }
    }

    // Log an error and return an empty string if the SHA256 checksum could not be found
    std::cerr << "Error: Could not find SHA256 for disk1.img of release " << releaseName << std::endl;
    return "";
}
