#include "AhoCorasick.h"
#include <queue>

AcNode::AcNode()
{
    fail = 0;
    matchCount = 0;

    for (auto i = 0; i < 26; ++i)
    {
        next[i] = -1;
    }
}

AhoCorasick::AhoCorasick()
{
    trie.emplace_back();
}

void AhoCorasick::reset()
{
    trie.clear();
    trie.emplace_back();
    matches = 0;
    comparisons = 0;
}

void AhoCorasick::build(const std::vector<std::string>& keywords)
{
    for (const auto& kw : keywords)
    {
        auto curr = 0;

        for (auto ch : kw)
        {
            auto c = ch - 'a';

            if (trie[curr].next[c] == -1)
            {
                trie[curr].next[c] = trie.size();
                trie.emplace_back();
            }

            curr = trie[curr].next[c];
        }

        trie[curr].matchCount++;
    }

    std::queue<int> q;

    for (auto i = 0; i < 26; ++i)
    {
        if (trie[0].next[i] != -1)
        {
            q.push(trie[0].next[i]);
        }
        else
        {
            trie[0].next[i] = 0;
        }
    }

    while (!q.empty())
    {
        auto u = q.front();
        q.pop();

        trie[u].matchCount += trie[trie[u].fail].matchCount;

        for (auto i = 0; i < 26; ++i)
        {
            if (trie[u].next[i] != -1)
            {
                trie[trie[u].next[i]].fail = trie[trie[u].fail].next[i];
                q.push(trie[u].next[i]);
            }
            else
            {
                trie[u].next[i] = trie[trie[u].fail].next[i];
            }
        }
    }
}

void AhoCorasick::search(const std::string& text)
{
    auto curr = 0;

    for (auto ch : text)
    {
        comparisons++;
        auto c = ch - 'a';
        curr = trie[curr].next[c];
        matches += trie[curr].matchCount;
    }
}