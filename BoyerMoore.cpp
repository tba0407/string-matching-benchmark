#include "BoyerMoore.h"
#include <algorithm>

void BoyerMoore::reset()
{
    matches = 0;
    comparisons = 0;
}

void BoyerMoore::search(const std::string& text, const std::string& pattern)
{
    const auto n = text.length();
    const auto m = pattern.length();

    if (m > n || m == 0)
    {
        return;
    }

    std::vector<int> badChar(256, -1);

    for (auto i = 0ULL; i < m; ++i)
    {
        badChar[static_cast<int>(pattern[i])] = i;
    }

    auto s = 0LL;

    while (s <= n - m)
    {
        auto j = static_cast<long long>(m) - 1;

        while (j >= 0)
        {
            comparisons++;
            
            if (pattern[j] != text[s + j])
            {
                break;
            }
            
            j--;
        }

        if (j < 0)
        {
            matches++;
            
            if (s + m < n)
            {
                s += m - badChar[static_cast<int>(text[s + m])];
            }
            else
            {
                s += 1;
            }
        }
        else
        {
            s += std::max(1LL, j - badChar[static_cast<int>(text[s + j])]);
        }
    }
}