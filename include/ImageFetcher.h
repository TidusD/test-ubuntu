#ifndef IMAGE_FETCHER_H
#define IMAGE_FETCHER_H

#include "ImageFetcherInterface.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp> // Include this to ensure json type is recognized

class ImageFetcher : public ImageFetcherInterface {
public:
    // Constructor and destructor (if needed)
    ImageFetcher();
    virtual ~ImageFetcher();

    // Override interface methods
    std::vector<std::string> getSupportedReleases() const override;
    std::string getCurrentLTSVersion() const override;
    std::string getSha256ForDiskImage(const std::string& release) const override;

private:
    // Declare the fetchData method
    nlohmann::json fetchData() const;  // This should match the definition in ImageFetcher.cpp
};

#endif // IMAGE_FETCHER_H
