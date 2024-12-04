#include <regex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

 

int main(int argc, char *argv[]) {
    std::cout << argv[0] << std::endl;
    if (argc < 2) {
        std::cout << "Usage: mull_it_over inputfile" << std::endl;
        return -1;
    }
    std::ifstream memory_file(argv[1]);
    std::stringstream memory_stream;
    memory_stream << memory_file.rdbuf();
    std::string memory_contents = memory_stream.str();

    std::regex mul_regex("mul\\((\\d+),(\\d+)\\)");
    auto mul_begin = std::sregex_iterator(memory_contents.begin(), memory_contents.end(), mul_regex);
    auto mul_end = std::sregex_iterator();

    int sum_of_muls = std::accumulate(mul_begin, mul_end, 0, [](int sum, const auto& match) {
        return sum + std::stoi(match.str(1)) * std::stoi(match.str(2));
    });
    std::cout << "result: " << sum_of_muls << "\n";

    return 0;
}
