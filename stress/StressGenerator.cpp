#include <bits/stdc++.h>

using namespace std;

const int rkBase = 26;
const int rkMod = 1000000007;

void genKillRk(int testIndex, int r, int c, int k)
{
    auto patternLen = min(c, 75);
    auto bombLength = min(patternLen, 8);
    
    vector<string> bombs;
    auto maxVal = 1LL;
    
    for (auto i = 0; i < bombLength; ++i)
    {
        maxVal *= 26;
    }
    
    for (auto m = 1LL; m * rkMod < maxVal && bombs.size() < static_cast<size_t>(k); ++m)
    {
        auto val = m * rkMod;
        auto bomb = string("");
        
        for (auto i = 0; i < bombLength; ++i)
        {
            bomb += char('a' + (val % 26));
            val /= 26;
        }
        
        reverse(bomb.begin(), bomb.end());
        bombs.emplace_back(bomb);
    }
    
    vector<string> keywords;
    
    for (auto i = 0; i < k; ++i)
    {
        auto prefixLen = patternLen - bombLength;
        keywords.emplace_back(string(prefixLen, 'a') + bombs[i % bombs.size()]);
    }
    
    vector<string> grid(r, string(c, 'a'));
    
    auto fileName = format("{}.txt", testIndex);
    ofstream fout(fileName);
    
    fout << format("{} {}\n", r, c);
    
    for (const auto& row : grid)
    {
        for (auto j = 0; j < c; ++j)
        {
            fout << row[j];
            
            if (j < c - 1)
            {
                fout << " ";
            }
        }
        fout << "\n";
    }
    
    fout << format("{}\n", k);
    
    for (const auto& kw : keywords)
    {
        fout << kw << "\n";
    }
    
    cout << format("Generated Kill RK test {:02d} (R={}, C={}, K={})\n", testIndex, r, c, k);
}

void genKillBm(int testIndex, int r, int c, int k)
{
    vector<string> grid(r, string(c, 'a'));
    vector<string> keywords;
    
    auto baseLen = min(c, 50);
    
    for (auto i = 0; i < k; ++i)
    {
        auto ext = i % max(1, (c - baseLen + 1));
        keywords.emplace_back(string(baseLen + ext, 'a'));
    }
    
    auto fileName = format("{}.txt", testIndex);
    ofstream fout(fileName);
    
    fout << format("{} {}\n", r, c);
    
    for (const auto& row : grid)
    {
        for (auto j = 0; j < c; ++j)
        {
            fout << row[j];
            
            if (j < c - 1)
            {
                fout << " ";
            }
        }
        fout << "\n";
    }
    
    fout << format("{}\n", k);
    
    for (const auto& kw : keywords)
    {
        fout << kw << "\n";
    }
    
    cout << format("Generated Kill BM test {:02d} (R={}, C={}, K={})\n", testIndex, r, c, k);
}

void genNatural(int testIndex, int r, int c, int k)
{
    vector<string> grid(r, string(c, 'a'));
    mt19937 rng(1337 + testIndex);
    
    for (auto i = 0; i < r; ++i)
    {
        for (auto j = 0; j < c; ++j)
        {
            grid[i][j] = char('a' + rng() % 26);
        }
    }
    
    vector<string> keywords;
    
    for (auto i = 0; i < k; ++i)
    {
        auto len = static_cast<int>(5 + rng() % min(16, c - 4));
        auto isVertical = rng() % 2;
        auto kw = string("");
        
        if (isVertical)
        {
            auto startR = rng() % (r - len + 1);
            auto startC = rng() % c;
            
            for (auto step = 0; step < len; ++step)
            {
                kw += grid[startR + step][startC];
            }
        }
        else
        {
            auto startR = rng() % r;
            auto startC = rng() % (c - len + 1);
            
            for (auto step = 0; step < len; ++step)
            {
                kw += grid[startR][startC + step];
            }
        }
        
        keywords.emplace_back(kw);
    }
    
    auto fileName = format("{}.txt", testIndex);
    ofstream fout(fileName);
    
    fout << format("{} {}\n", r, c);
    
    for (const auto& row : grid)
    {
        for (auto j = 0; j < c; ++j)
        {
            fout << row[j];
            
            if (j < c - 1)
            {
                fout << " ";
            }
        }
        fout << "\n";
    }
    
    fout << format("{}\n", k);
    
    for (const auto& kw : keywords)
    {
        fout << kw << "\n";
    }
    
    cout << format("Generated Natural test {:02d} (R={}, C={}, K={})\n", testIndex, r, c, k);
}

int main()
{
    auto r = 0;
    auto c = 0;
    auto maxK = 0;
    auto totalTests = 0;

    cout << "=== STRESS TEST GENERATOR ===\n";
    cout << "Enter grid rows (R): ";
    cin >> r;
    cout << "Enter grid cols (C): ";
    cin >> c;
    
    if (r < 10 || c < 10)
    {
        cout << "R and C must be at least 10.\n";
        return 0;
    }
    
    cout << "Enter maximum keywords (Max K): ";
    cin >> maxK;
    cout << "Enter total number of tests: ";
    cin >> totalTests;

    if (1LL * r * c * maxK > 5000000LL)
    {
        cout << "\n[WARNING] Massive load detected!\n";
        cout << "Algorithms may take an extremely long time to finish.\n\n";
    }

    auto countRk = max(1, totalTests / 6);
    auto countBm = max(1, totalTests / 6);
    
    auto limitKillRk = countRk;
    auto limitKillBm = countRk + countBm;

    cout << string(40, '-') << "\n";
    cout << "Generating tests into current directory...\n";
    cout << string(40, '-') << "\n";

    for (auto i = 1; i <= totalTests; ++i)
    {
        if (i <= limitKillRk)
        {
            auto k = max(1, (maxK * i) / limitKillRk);
            genKillRk(i, r, c, k);
        }
        else if (i <= limitKillBm)
        {
            auto localIdx = i - limitKillRk;
            auto localTotal = limitKillBm - limitKillRk;
            auto k = max(1, (maxK * localIdx) / localTotal);
            genKillBm(i, r, c, k);
        }
        else
        {
            auto localIdx = i - limitKillBm;
            auto localTotal = totalTests - limitKillBm;
            auto k = max(1, (maxK * localIdx) / localTotal);
            genNatural(i, r, c, k);
        }
    }
    
    cout << string(40, '-') << "\n";
    cout << "All stress tests generated successfully.\n";
    
    return 0;
}