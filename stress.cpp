#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <format>
#include <limits>
#include <filesystem>
#include <iomanip>
#include "BruteForce.h"
#include "RabinKarp.h"
#include "BoyerMoore.h"
#include "AhoCorasick.h"

using namespace std;
using namespace std::chrono;
using namespace std::filesystem;

namespace ConsoleColor
{
    const string reset = "\033[0m";
    const string green = "\033[32m";
    const string yellow = "\033[33m";
    const string cyan = "\033[36m";
    const string magenta = "\033[35m";
    const string bold = "\033[1m";
    const string red = "\033[31m";
}

struct AlgoStats
{
    long long totalTime = 0;
    long long totalComparisons = 0;
    long long minTime = numeric_limits<long long>::max();
    long long maxTime = -1;
    int fastestTest = -1;
    int slowestTest = -1;
};

string formatLargeNum(long long num)
{
    if (num >= 1000000000) return format("{:.2f}B", num / 1e9);
    if (num >= 1000000) return format("{:.2f}M", num / 1e6);
    if (num >= 1000) return format("{:.2f}K", num / 1e3);
    return to_string(num);
}

void showProgressBar(int current, int total, const string& status)
{
    int barWidth = 30;
    float progress = static_cast<float>(current) / total;
    int pos = static_cast<int>(barWidth * progress);

    cout << "\r" << ConsoleColor::cyan << "[";
    
    for (int i = 0; i < barWidth; ++i) 
    {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    
    cout << "] " << static_cast<int>(progress * 100.0) << "% | " << ConsoleColor::yellow << status << ConsoleColor::reset << "    ";
    cout.flush();
}

int main()
{
    cout << ConsoleColor::cyan << ConsoleColor::bold << R"(
    =======================================================
               ALGORITHM BENCHMARK DASHBOARD v3.0
    =======================================================
    )" << ConsoleColor::reset;

    auto choice = 0;
    cout << ConsoleColor::bold << " [>] Select Scenario:\n" << ConsoleColor::reset;
    cout << "  1. Scenario 1 (Fixed Dictionary)\n";
    cout << "  2. Scenario 2 (Dictionary Growth)\n";
    cout << "  3. Stress Test (Custom Parameters)\n";
    cout << "\n Enter choice (1-3): ";
    cin >> choice;

    auto folderName = string("");
    
    if (choice == 1) folderName = "scen1";
    else if (choice == 2) folderName = "scen2";
    else folderName = "stress";

    auto totalTests = 0;
    
    while (exists(format("{}/{}.txt", folderName, totalTests + 1)))
    {
        totalTests++;
    }

    if (totalTests == 0)
    {
        cout << ConsoleColor::red << "\n [!] Error: No test files found in /" << folderName << ConsoleColor::reset << "\n";
        return 0;
    }

    cout << ConsoleColor::green << "\n [*] Environment Ready. Found " << totalTests << " tests in /" << folderName << "\n" << ConsoleColor::reset;
    cout << ConsoleColor::cyan << " -------------------------------------------------------\n" << ConsoleColor::reset;

    AlgoStats statsBF, statsRK, statsBM, statsAC;
    ofstream logFile(format("{}_benchmark.log", folderName));

    vector<string> tableLines;
    const auto tableHeader = format("{:<4} | {:<7} | {:<8} | {:<5} | {:<8} | {:<5} | {:<8} | {:<5} | {:<8} | {:<5}", 
                                    "Test", "Matches", "BF Comp", "BF ms", "RK Comp", "RK ms", "BM Comp", "BM ms", "AC Comp", "AC ms");
    tableLines.push_back(tableHeader);

    for (auto i = 1; i <= totalTests; ++i)
    {
        showProgressBar(i, totalTests, format("Processing Test {:02d}...", i));

        const auto fileName = format("{}/{}.txt", folderName, i);
        ifstream fin(fileName);
        
        if (!fin) continue;

        auto r = 0;
        auto c = 0;
        fin >> r >> c;
        
        vector<string> rows(r, "");
        vector<string> cols(c, "");
        
        for (auto rowIdx = 0; rowIdx < r; ++rowIdx)
        {
            for (auto colIdx = 0; colIdx < c; ++colIdx)
            {
                auto ch = char(0); 
                fin >> ch;
                rows[rowIdx] += ch; 
                cols[colIdx] += ch;
            }
        }
        
        auto k = 0; 
        fin >> k;
        vector<string> keywords(k, "");
        
        for (auto kwIdx = 0; kwIdx < k; ++kwIdx)
        {
            fin >> keywords[kwIdx];
        }

        BruteForce bf; 
        RabinKarp rk; 
        BoyerMoore bm;
        AhoCorasick ac;

        auto measure = [&](auto& obj) 
        {
            auto start = high_resolution_clock::now();
            
            for (const auto& kw : keywords)
            {
                for (const auto& row : rows) obj.search(row, kw);
                for (const auto& col : cols) obj.search(col, kw);
            }
            
            return duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
        };

        auto tBF = measure(bf);
        auto tRK = measure(rk);
        auto tBM = measure(bm);

        auto startAC = high_resolution_clock::now();
        ac.build(keywords);
        
        for (const auto& row : rows) ac.search(row);
        for (const auto& col : cols) ac.search(col);
        
        auto tAC = duration_cast<milliseconds>(high_resolution_clock::now() - startAC).count();

        auto update = [](AlgoStats& s, long long t, long long comp, int idx) 
        {
            s.totalTime += t;
            s.totalComparisons += comp;
            
            if (t < s.minTime) 
            { 
                s.minTime = t; 
                s.fastestTest = idx; 
            }
            if (t > s.maxTime) 
            { 
                s.maxTime = t; 
                s.slowestTest = idx; 
            }
        };

        update(statsBF, tBF, bf.comparisons, i); 
        update(statsRK, tRK, rk.comparisons, i); 
        update(statsBM, tBM, bm.comparisons, i);
        update(statsAC, tAC, ac.comparisons, i);

        tableLines.push_back(format("{:<4} | {:<7} | {:<8} | {:<5} | {:<8} | {:<5} | {:<8} | {:<5} | {:<8} | {:<5}", 
                             i, formatLargeNum(bf.matches), 
                             formatLargeNum(bf.comparisons), tBF, 
                             formatLargeNum(rk.comparisons), tRK, 
                             formatLargeNum(bm.comparisons), tBM,
                             formatLargeNum(ac.comparisons), tAC));
    }

    cout << "\n\n" << ConsoleColor::bold << " [>] Detailed Test Results:\n" << ConsoleColor::reset;
    const auto sep = string(105, '-');
    cout << ConsoleColor::cyan << sep << "\n" << tableLines[0] << "\n" << sep << ConsoleColor::reset << "\n";
    logFile << sep << "\n" << tableLines[0] << "\n" << sep << "\n";

    for (size_t i = 1; i < tableLines.size(); ++i)
    {
        cout << tableLines[i] << "\n";
        logFile << tableLines[i] << "\n";
    }

    cout << ConsoleColor::cyan << "\n" << string(105, '=') << "\n";
    cout << ConsoleColor::bold << " FINAL PERFORMANCE SUMMARY\n";
    cout << string(105, '=') << ConsoleColor::reset << "\n";
    
    const auto summaryHeader = format("{:<13} | {:<12} | {:<12} | {:<10} | {:<18} | {:<18}\n", 
                                      "Algorithm", "Total Comp", "Total Time", "Avg Time", "Peak Time (Test)", "Min Time (Test)");
    const auto summarySep = string(105, '-');
    
    cout << summaryHeader << summarySep << "\n";
    logFile << "\n" << string(105, '=') << "\nFINAL PERFORMANCE SUMMARY\n" << string(105, '=') << "\n" << summaryHeader << summarySep << "\n";

    auto printSummaryRow = [&](const string& name, const AlgoStats& stats, const string& color) 
    {
        auto avg = stats.totalTime / max(1LL, static_cast<long long>(totalTests));
        auto peakStr = format("{} ms ({})", stats.maxTime, stats.slowestTest);
        auto minStr = format("{} ms ({})", stats.minTime, stats.fastestTest);
        
        auto rowStr = format("{:<13} | {:<12} | {:<9} ms | {:<7} ms | {:<18} | {:<18}\n", 
                             name, formatLargeNum(stats.totalComparisons), stats.totalTime, avg, peakStr, minStr);
        
        cout << color << rowStr << ConsoleColor::reset;
        logFile << rowStr;
    };

    printSummaryRow("Brute Force", statsBF, ConsoleColor::red);
    printSummaryRow("Rabin-Karp", statsRK, ConsoleColor::yellow);
    printSummaryRow("Boyer-Moore", statsBM, ConsoleColor::green);
    printSummaryRow("Aho-Corasick", statsAC, ConsoleColor::magenta);

    cout << ConsoleColor::cyan << string(105, '=') << ConsoleColor::reset << "\n\n";
    
    return 0;
}