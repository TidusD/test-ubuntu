#ifndef IMAGE_FETCHER_INTERFACE_H
#define IMAGE_FETCHER_INTERFACE_H

#include <string>
#include <vector>

class ImageFetcherInterface {
public:
    // Virtual destructor to allow proper cleanup of derived classes
    virtual ~ImageFetcherInterface() = default;

    // Method to get all supported Ubuntu releases (e.g., "20.04", "21.10", etc.)
    virtual std::vector<std::string> getSupportedReleases() const = 0;

    // Method to get the current Ubuntu LTS (Long Term Support) version
    virtual std::string getCurrentLTSVersion() const = 0;

    // Method to get the SHA256 hash of the disk1.img file for a specific release
    virtual std::string getSha256ForDiskImage(const std::string& release) const = 0;
};

#endif // IMAGE_FETCHER_INTERFACE_H