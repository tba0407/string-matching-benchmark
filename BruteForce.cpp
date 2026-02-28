#include "BruteForce.h"

void BruteForce::reset()
{
    matches = 0;
    comparisons = 0;
}

void BruteForce::search(const std::string& text, const std::string& pattern)
{
    const auto n = text.length();
    const auto m = pattern.length();

    if (m > n)
    {
        return;
    }

    for (auto i = 0ULL; i <= n - m; ++i)
    {
        auto isMatch = true;

        for (auto j = 0ULL; j < m; ++j)
        {
            comparisons++;
            
            if (text[i + j] != pattern[j])
            {
                isMatch = false;
                break;
            }
        }

        if (isMatch)
        {
            matches++;
        }
    }
}