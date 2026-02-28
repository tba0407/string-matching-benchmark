#include "RabinKarp.h"

void RabinKarp::reset()
{
    matches = 0;
    comparisons = 0;
}

void RabinKarp::search(const std::string& text, const std::string& pattern)
{
    const auto n = text.length();
    const auto m = pattern.length();

    if (m > n)
    {
        return;
    }

    auto patHash = 0LL;
    auto textHash = 0LL;
    auto hMultiplier = 1LL;

    for (auto i = 0ULL; i < m - 1; ++i)
    {
        hMultiplier = (hMultiplier * rkBase) % rkMod;
    }

    for (auto i = 0ULL; i < m; ++i)
    {
        patHash = (patHash * rkBase + pattern[i]) % rkMod;
        textHash = (textHash * rkBase + text[i]) % rkMod;
    }

    for (auto i = 0ULL; i <= n - m; ++i)
    {
        if (patHash == textHash)
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

        if (i < n - m)
        {
            textHash = (rkBase * (textHash - text[i] * hMultiplier) + text[i + m]) % rkMod;

            if (textHash < 0)
            {
                textHash += rkMod;
            }
        }
    }
}