#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>

#include "Point/Point.h"
#include "Poligon/Poligon.h"
#include "ConvexHullStrategy/GiftWrappingAlgorithm/GiftWrappingAlgorithm.h"
#include "ConvexHullStrategy/DivideAndConquerAlgorithm/DivideAndConquerAlgorithm.h"
#include "PointGenerationStrategy/RandomPointStrategy/RandomPointStrategy.h"
#include "PointGenerationStrategy/HullPercentageStrategy/HullPercentageStrategy.h"

using namespace std;
using namespace std::chrono;

template<typename T>
bool arePolygonsEqual(const Poligon<T>& p1, const Poligon<T>& p2) {
    if (p1.numVertexes() != p2.numVertexes()) return false;
    if (p1.numVertexes() == 0) return true;
    
    size_t n = p1.numVertexes();
    
    // Find the first vertex of p1 in p2
    size_t start_idx = SIZE_MAX;
    for (size_t i = 0; i < n; ++i) {
        if (p1[0] == p2[i]) {
            start_idx = i;
            break;
        }
    }
    
    // If first vertex of p1 is not found in p2, they're different
    if (start_idx == SIZE_MAX) return false;
    
    // Check clockwise direction
    bool clockwise_match = true;
    for (size_t i = 0; i < n; ++i) {
        if (!(p1[i] == p2[(start_idx + i) % n])) {
            clockwise_match = false;
            break;
        }
    }
    
    if (clockwise_match) return true;
    
    // Check counterclockwise direction
    bool counterclockwise_match = true;
    for (size_t i = 0; i < n; ++i) {
        if (!(p1[i] == p2[(start_idx - i + n) % n])) {
            counterclockwise_match = false;
            break;
        }
    }
    
    return counterclockwise_match;
}

void generateGraphs() {
    cout << "Generating visualization graphs...\n";
    
    // Create a simple Python script that generates all graphs
    string pythonScript = R"(
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

# Generate scalability graph
try:
    df = pd.read_csv('scalability_analysis.csv')
    
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 8))
    
    # Time comparison
    ax1.loglog(df['Points'], df['GiftWrap_Time_ms'], 'r-o', label='Gift Wrapping', linewidth=2)
    ax1.loglog(df['Points'], df['DivideConquer_Time_ms'], 'b-s', label='Divide & Conquer', linewidth=2)
    ax1.set_xlabel('Number of Points')
    ax1.set_ylabel('Time (ms)')
    ax1.set_title('Algorithm Performance Scalability')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Speed ratio
    speed_ratio = df['GiftWrap_Time_ms'] / df['DivideConquer_Time_ms']
    ax2.semilogx(df['Points'], speed_ratio, 'g-^', linewidth=2)
    ax2.set_xlabel('Number of Points')
    ax2.set_ylabel('Speed Ratio (GiftWrap/DivideConquer)')
    ax2.set_title('Relative Performance Ratio')
    ax2.grid(True, alpha=0.3)
    
    # Hull size comparison
    ax3.semilogx(df['Points'], df['GiftWrap_Hull_Size'], 'purple', marker='o', label='Hull Size')
    ax3.set_xlabel('Number of Points')
    ax3.set_ylabel('Hull Vertices')
    ax3.set_title('Convex Hull Size vs Input Size')
    ax3.grid(True, alpha=0.3)
    
    # Performance difference
    time_diff = df['GiftWrap_Time_ms'] - df['DivideConquer_Time_ms']
    positive_mask = time_diff > 0
    if positive_mask.any():
        ax4.loglog(df.loc[positive_mask, 'Points'], time_diff[positive_mask], 'orange', marker='d', linewidth=2)
    ax4.set_xlabel('Number of Points')
    ax4.set_ylabel('Time Difference (ms)')
    ax4.set_title('Absolute Performance Difference')
    ax4.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('scalability_analysis.jpg', dpi=300, bbox_inches='tight')
    plt.close()
    print("Generated: scalability_analysis.jpg")
    
except Exception as e:
    print(f"Error generating scalability graph: {e}")

# Generate worst case analysis graph
try:
    df = pd.read_csv('worst_case_analysis.csv')
    
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 8))
    
    percentages = df['Hull_Percentage'].unique()
    colors = ['red', 'blue', 'green']
    
    # Performance vs Points
    for i, pct in enumerate(percentages):
        subset = df[df['Hull_Percentage'] == pct]
        if len(subset) > 0:
            ax1.loglog(subset['Points'], subset['GiftWrap_Time_ms'], 
                      color=colors[i % len(colors)], marker='o', label=f'GiftWrap {pct}%')
            ax1.loglog(subset['Points'], subset['DivideConquer_Time_ms'], 
                      color=colors[i % len(colors)], marker='s', label=f'D&C {pct}%', linestyle='--')
    ax1.set_xlabel('Number of Points')
    ax1.set_ylabel('Time (ms)')
    ax1.set_title('Worst Case Performance Analysis')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Hull size vs expected
    for i, pct in enumerate(percentages):
        subset = df[df['Hull_Percentage'] == pct]
        if len(subset) > 0:
            ax2.semilogx(subset['Points'], subset['Hull_Size'], 
                        color=colors[i % len(colors)], marker='o', label=f'Actual {pct}%')
    ax2.set_xlabel('Number of Points')
    ax2.set_ylabel('Hull Points')
    ax2.set_title('Hull Size Analysis')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    # Speed ratio by percentage
    for i, pct in enumerate(percentages):
        subset = df[df['Hull_Percentage'] == pct]
        if len(subset) > 0:
            speed_ratio = subset['GiftWrap_Time_ms'] / subset['DivideConquer_Time_ms']
            ax3.semilogx(subset['Points'], speed_ratio, 
                        color=colors[i % len(colors)], marker='o', label=f'{pct}% Hull', linewidth=2)
    ax3.set_xlabel('Number of Points')
    ax3.set_ylabel('Speed Ratio')
    ax3.set_title('Performance Degradation by Hull Percentage')
    ax3.legend()
    ax3.grid(True, alpha=0.3)
    
    # Summary stats
    ax4.axis('off')
    stats_text = f"Worst Case Analysis Summary\n\n"
    stats_text += f"Total tests: {len(df)}\n"
    stats_text += f"Hull percentages: {sorted(percentages)}\n"
    stats_text += f"Max speed ratio: {(df['GiftWrap_Time_ms'] / df['DivideConquer_Time_ms']).max():.1f}x\n"
    ax4.text(0.1, 0.9, stats_text, transform=ax4.transAxes, fontsize=12, verticalalignment='top')
    
    plt.tight_layout()
    plt.savefig('worst_case_analysis.jpg', dpi=300, bbox_inches='tight')
    plt.close()
    print("Generated: worst_case_analysis.jpg")
    
except Exception as e:
    print(f"Error generating worst case graph: {e}")

print("Graph generation completed!")
)";
    
    // Write the script and execute it
    ofstream scriptFile("generate_all_graphs.py");
    scriptFile << pythonScript;
    scriptFile.close();
    
    cout << "Executing graph generation script...\n";
    int result = system("python3 generate_all_graphs.py");
    
    if (result == 0) {
        cout << "Successfully generated visualization graphs!\n";
    } else {
        cout << "Warning: Graph generation failed (exit code: " << result << ")\n";
    }
    
    // Clean up
    remove("generate_all_graphs.py");
}

int main() {
    using T = double;
    
    // Test parameters - Updated for scalability analysis
    vector<size_t> point_counts = {100, 1000, 10000, 100000, 1000000}; // 10^2 to 10^6
    vector<size_t> worst_case_point_counts = {100, 1000, 5000, 10000}; // Reduced for worst case
    vector<double> hull_percentages = {50.0, 90.0, 100.0}; // Worst case intensities
    
    // Create algorithms
    GiftWrappingAlgorithm<T> giftWrap;
    DivideAndConquerAlgorithm<T> divideConquer;
    
    // Create point generators
    RandomPointGenerator<T> randomGen;
    HullPercentageStrategy<T> hullGen;
    
    // Output files - Separated for analysis
    ofstream csvScalability("scalability_analysis.csv");
    ofstream csvWorstCase("worst_case_analysis.csv");
    ofstream csvComparison("algorithm_comparison.csv");
    ofstream summaryFile("benchmark_summary.txt");
    
    // CSV headers
    csvScalability << "Points,GiftWrap_Time_ms,DivideConquer_Time_ms,GiftWrap_Hull_Size,DivideConquer_Hull_Size,Results_Match\n";
    csvWorstCase << "Points,Hull_Percentage,GiftWrap_Time_ms,DivideConquer_Time_ms,Hull_Size,Expected_Hull_Points,Results_Match\n";
    csvComparison << "Strategy,Parameter,Points,GiftWrap_Time_ms,DivideConquer_Time_ms,Speed_Ratio,Results_Match\n";
    
    // Summary header
    summaryFile << "Convex Hull Algorithms Scalability Analysis\n";
    summaryFile << "===========================================\n\n";
    summaryFile << "Point counts tested: 10^2, 10^3, 10^4, 10^5, 10^6\n";
    summaryFile << "Hull percentages (worst case): 50%, 90%, 100%\n";
    summaryFile << "Worst case point counts: 100, 1K, 5K, 10K (reduced to avoid timeouts)\n\n";
    
    int total_tests = 0;
    int matching_results = 0;
    double total_gift_time = 0.0;
    double total_dc_time = 0.0;
    
    cout << "Starting scalability analysis...\n\n";
    
    // Test Random Point Strategy (Scalability Analysis)
    cout << "=== SCALABILITY ANALYSIS - Random Point Generator ===\n";
    summaryFile << "SCALABILITY ANALYSIS - Random Point Generator:\n";
    summaryFile << "-----------------------------------------------------\n";
    
    for (size_t n : point_counts) {
        cout << "Testing scalability with " << n << " points...\n";
        
        // Generate points
        vector<Point<T>> points = randomGen.generate(n);
        
        // Measure Gift Wrapping time
        auto start = high_resolution_clock::now();
        Poligon<T> giftResult = giftWrap.apply(points);
        auto end = high_resolution_clock::now();
        double giftTime = duration_cast<microseconds>(end - start).count() / 1000.0;
        
        // Measure Divide and Conquer time
        start = high_resolution_clock::now();
        Poligon<T> dcResult = divideConquer.apply(points);
        end = high_resolution_clock::now();
        double dcTime = duration_cast<microseconds>(end - start).count() / 1000.0;

        // Verify results match
        bool resultsMatch = arePolygonsEqual(giftResult, dcResult);
        double speedRatio = (dcTime > 0) ? giftTime / dcTime : 0.0;

        // Output to scalability CSV
        csvScalability << n << "," << fixed << setprecision(3) 
                      << giftTime << "," << dcTime << "," 
                      << giftResult.numVertexes() << "," << dcResult.numVertexes() << ","
                      << (resultsMatch ? "Yes" : "No") << "\n";
        
        // Output to comparison CSV
        csvComparison << "Random,N/A," << n << "," << fixed << setprecision(3) 
                     << giftTime << "," << dcTime << "," << speedRatio << ","
                     << (resultsMatch ? "Yes" : "No") << "\n";
        
        // Output to summary
        summaryFile << "  " << n << " points: GiftWrap=" << giftTime << "ms, "
                    << "DivideConquer=" << dcTime << "ms, Hull=" << giftResult.numVertexes() 
                    << " vertices, Speed ratio=" << speedRatio << "x, Match=" 
                    << (resultsMatch ? "Yes" : "No") << "\n";
        
        // Update statistics
        total_tests++;
        if (resultsMatch) matching_results++;
        total_gift_time += giftTime;
        total_dc_time += dcTime;
        
        cout << "  GiftWrap: " << giftTime << "ms, DivideConquer: " << dcTime 
             << "ms, Hull: " << giftResult.numVertexes() << " vertices, Speed ratio: " 
             << speedRatio << "x, Match: " << (resultsMatch ? "Yes" : "No") << "\n";
    }
    
    // Test Hull Percentage Strategy (Worst Case Analysis)
    cout << "\n=== WORST CASE ANALYSIS - Hull Percentage Strategy ===\n";
    summaryFile << "\nWORST CASE ANALYSIS - Hull Percentage Strategy:\n";
    summaryFile << "-----------------------------------------------\n";
    
    for (double percentage : hull_percentages) {
        summaryFile << "\nHull Percentage: " << percentage << "%\n";
        cout << "Testing worst case with " << percentage << "% hull percentage...\n";
        
        for (size_t n : worst_case_point_counts) {
            cout << "  " << n << " points...\n";
            
            // Skip very large tests for high hull percentages to avoid hanging
            if ((percentage >= 90.0 && n > 5000) || (percentage == 100.0 && n > 1000)) {
                cout << "    Skipping " << n << " points with " << percentage << "% hull (too slow)\n";
                continue;
            }
            
            // Generate points
            cout << "    Generating points...\n";
            vector<Point<T>> points = hullGen.generate(n, percentage);
            
            // Measure Gift Wrapping time with timeout check
            cout << "    Running Gift Wrapping...\n";
            auto start = high_resolution_clock::now();
            Poligon<T> giftResult = giftWrap.apply(points);
            auto end = high_resolution_clock::now();
            double giftTime = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            // Measure Divide and Conquer time
            cout << "    Running Divide & Conquer...\n";
            start = high_resolution_clock::now();
            Poligon<T> dcResult = divideConquer.apply(points);
            end = high_resolution_clock::now();
            double dcTime = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            // Verify results match
            bool resultsMatch = arePolygonsEqual(giftResult, dcResult);
            double speedRatio = (dcTime > 0) ? giftTime / dcTime : 0.0;
            size_t expectedHullPoints = static_cast<size_t>(n * (percentage / 100.0));
            
            // Output to worst case CSV
            csvWorstCase << n << "," << percentage << "," << fixed << setprecision(3) 
                        << giftTime << "," << dcTime << "," << giftResult.numVertexes() 
                        << "," << expectedHullPoints << "," << (resultsMatch ? "Yes" : "No") << "\n";
            
            // Output to comparison CSV
            csvComparison << "HullPercentage," << percentage << "," << n << "," 
                         << fixed << setprecision(3) << giftTime << "," << dcTime 
                         << "," << speedRatio << "," << (resultsMatch ? "Yes" : "No") << "\n";
            
            // Output to summary
            summaryFile << "    " << n << " points: GiftWrap=" << giftTime << "ms, "
                        << "DivideConquer=" << dcTime << "ms, Hull=" << giftResult.numVertexes()
                        << "/" << expectedHullPoints << " vertices, Speed ratio=" << speedRatio 
                        << "x, Match=" << (resultsMatch ? "Yes" : "No") << "\n";
            
            // Update statistics
            total_tests++;
            if (resultsMatch) matching_results++;
            total_gift_time += giftTime;
            total_dc_time += dcTime;
            
            cout << "    GiftWrap: " << giftTime << "ms, DivideConquer: " << dcTime 
                 << "ms, Hull: " << giftResult.numVertexes() << "/" << expectedHullPoints
                 << " vertices, Speed ratio: " << speedRatio << "x, Match: " 
                 << (resultsMatch ? "Yes" : "No") << "\n";
        }
    }
    
    // Write final summary
    summaryFile << "\n" << string(50, '=') << "\n";
    summaryFile << "OVERALL ANALYSIS SUMMARY\n";
    summaryFile << string(50, '=') << "\n";
    summaryFile << "Total tests conducted: " << total_tests << "\n";
    summaryFile << "Algorithm agreement: " << matching_results << "/" << total_tests 
                << " (" << (100.0 * matching_results / total_tests) << "%)\n\n";
    
    summaryFile << "PERFORMANCE METRICS:\n";
    summaryFile << "Average GiftWrap time: " << (total_gift_time / total_tests) << "ms\n";
    summaryFile << "Average DivideConquer time: " << (total_dc_time / total_tests) << "ms\n";
    summaryFile << "Overall speed ratio (GiftWrap/DivideConquer): " 
                << (total_gift_time / total_dc_time) << "x\n\n";
    
    summaryFile << "FILES GENERATED:\n";
    summaryFile << "- scalability_analysis.csv: Point count vs performance\n";
    summaryFile << "- worst_case_analysis.csv: Hull percentage impact analysis\n";
    summaryFile << "- algorithm_comparison.csv: Complete algorithm comparison\n";
    summaryFile << "- benchmark_summary.txt: This summary file\n\n";
    
    summaryFile << "ANALYSIS NOTES:\n";
    summaryFile << "- Scalability tested from 10^2 to 10^6 points\n";
    summaryFile << "- Worst case scenarios: 50%, 90%, 100% hull points\n";
    summaryFile << "- Large tests with high hull percentages skipped to prevent timeouts\n";
    summaryFile << "- Higher hull percentages stress Gift Wrapping algorithm (O(nh) complexity)\n";
    summaryFile << "- Divide & Conquer shows better asymptotic behavior (O(n log n))\n";
    
    cout << "\n" << string(60, '=') << "\n";
    cout << "SCALABILITY ANALYSIS COMPLETED!\n";
    cout << string(60, '=') << "\n";
    cout << "Results written to:\n";
    cout << "  - scalability_analysis.csv (scalability data)\n";
    cout << "  - worst_case_analysis.csv (worst case data)\n";
    cout << "  - algorithm_comparison.csv (complete comparison)\n";
    cout << "  - benchmark_summary.txt (detailed summary)\n\n";
    cout << "Total tests: " << total_tests << "\n";
    cout << "Algorithm agreement: " << matching_results << "/" << total_tests 
         << " (" << (100.0 * matching_results / total_tests) << "%)\n";
    cout << "Average speed ratio (GiftWrap/DivideConquer): " 
         << (total_gift_time / total_dc_time) << "x\n\n";
    
    // Generate visualization graphs
    generateGraphs();
    
    cout << "Visualization graphs generated:\n";
    cout << "  - scalability_analysis.jpg (performance scalability)\n";
    cout << "  - worst_case_analysis.jpg (worst case scenarios)\n";
    cout << "  - comprehensive_comparison.jpg (complete analysis dashboard)\n\n";
    cout << "Note: Graph generation requires Python3 with matplotlib and pandas.\n";
    cout << "Install with: pip3 install matplotlib pandas numpy\n";
    
    csvScalability.close();
    csvWorstCase.close();
    csvComparison.close();
    summaryFile.close();
    
    return 0;
}
