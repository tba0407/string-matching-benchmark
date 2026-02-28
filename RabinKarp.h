#pragma once
#include <string>

struct RabinKarp
{
    int matches = 0;
    long long comparisons = 0;
    
    const int rkBase = 26;
    const int rkMod = 1000000007;

    void reset();
    void search(const std::string& text, const std::string& pattern);
};