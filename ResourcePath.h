#pragma once

#include <fstream>
#include <string>
#include <vector>

inline bool fileExists(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    return file.good();
}

inline std::string findExistingPath(const std::string& relativePath) {
    const std::vector<std::string> prefixes = {
        "",
        "../",
        "../../",
        "../../../"
    };

    for (const std::string& prefix : prefixes) {
        const std::string candidate = prefix + relativePath;
        if (fileExists(candidate)) {
            return candidate;
        }
    }
    return relativePath;
}
