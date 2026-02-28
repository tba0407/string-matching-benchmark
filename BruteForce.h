#pragma once
#include <string>

struct BruteForce
{
    int matches = 0;
    long long comparisons = 0;

    void reset();
    void search(const std::string& text, const std::string& pattern);
};