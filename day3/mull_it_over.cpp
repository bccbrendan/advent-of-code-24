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

    std::regex conditional_mul_regex("do\\(\\)|don't\\(\\)|mul\\((\\d+),(\\d+)\\)");
    auto mul_begin = std::sregex_iterator(memory_contents.begin(), memory_contents.end(), conditional_mul_regex);
    auto mul_end = std::sregex_iterator();

    bool enabled = true;
    int sum = 0;
    for (auto& i = mul_begin; i != mul_end; ++i) {
        std::smatch match = *i;
        if (match.str() == "do()") {
            enabled = true;
        } else if (match.str() == "don't()") {
            enabled = false;
        } else if (enabled) {
            sum += std::stoi(match.str(1)) * std::stoi(match.str(2));
        }
    }
        
    std::cout << "result: " << sum << "\n";

    return 0;
}
