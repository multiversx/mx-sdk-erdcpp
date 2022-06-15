#ifndef ERDCPP_TEST_COMMON_H
#define ERDCPP_TEST_COMMON_H

#include "string"

std::string getCanonicPath(std::string const &path)
{
    // Get absolute path to executable
    std::string canonicPath = std::string(canonicalize_file_name("/proc/self/exe"));

    // Remove everything in path until tests directory and concatenate it with test file data path
    canonicPath = canonicPath.substr(0, canonicPath.find("tests"));
    return canonicPath + "tests/" + path;
}

#endif //ERDCPP_TEST_COMMON_H