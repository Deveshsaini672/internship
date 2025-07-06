#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

// Simple Run-Length Encoding (RLE) compression
std::string compressRLE(const std::string& data) {
    std::string result;
    int n = data.length();

    for (int i = 0; i < n;) {
        char ch = data[i];
        int count = 1;
        while (i + count < n && data[i + count] == ch) {
            count++;
        }
        result += ch + std::to_string(count);
        i += count;
    }

    return result;
}

// Thread function to compress a chunk
void compressChunk(const std::string& chunk, std::string& output) {
    output = compressRLE(chunk);
}

int main() {
    std::ifstream inFile("input.txt");
    if (!inFile) {
        std::cerr << "❌ Failed to open input.txt\n";
        return 1;
    }

    // Read the full file into a string
    std::string fullData((std::istreambuf_iterator<char>(inFile)),
                          std::istreambuf_iterator<char>());
    inFile.close();

    if (fullData.empty()) {
        std::cerr << "❌ input.txt is empty.\n";
        return 1;
    }

    std::cout << "✅ Read input.txt (" << fullData.size() << " characters)\n";

    // ==========================
    // 🧪 Single-threaded RLE
    // ==========================
    auto start1 = std::chrono::high_resolution_clock::now();
    std::string compressedSingle = compressRLE(fullData);
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();

    // ==========================
    // ⚙️ Multi-threaded RLE
    // ==========================
    int numThreads = 4;
    int chunkSize = fullData.size() / numThreads;
    std::vector<std::thread> threads;
    std::vector<std::string> results(numThreads);

    auto start2 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? fullData.size() : (i + 1) * chunkSize;
        std::string chunk = fullData.substr(start, end - start);

        threads.emplace_back(compressChunk, chunk, std::ref(results[i]));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::string compressedMulti;
    for (const auto& part : results) {
        compressedMulti += part;
    }

    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();

    // ==========================
    // 📤 Output Result
    // ==========================
    std::ofstream outFile("compressed.txt");
    if (!outFile) {
        std::cerr << "❌ Failed to create compressed.txt\n";
        return 1;
    }
    outFile << compressedMulti;
    outFile.close();

    // ==========================
    // 📊 Performance Summary
    // ==========================
    std::cout << "\n✅ Compression complete!\n";
    std::cout << "📄 Output file: compressed.txt\n\n";
    std::cout << "⏱️  Single-threaded time: " << duration1 << " ms\n";
    std::cout << "⚡ Multi-threaded time:   " << duration2 << " ms\n";

    return 0;
}
