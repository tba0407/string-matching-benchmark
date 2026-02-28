#pragma once
#include <string>
#include <vector>

struct AcNode
{
    int next[26];
    int fail;
    int matchCount;

    AcNode();
};

struct AhoCorasick
{
    std::vector<AcNode> trie;
    int matches = 0;
    long long comparisons = 0;

    AhoCorasick();

    void reset();
    void build(const std::vector<std::string>& keywords);
    void search(const std::string& text);
};