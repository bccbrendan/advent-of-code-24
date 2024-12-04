#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool is_a_centering_mas(std::vector<std::string> const& rows, size_t row_i, size_t col_i) {
    // search diagonals
    // no need to check bounds, since 'A' must be in the middle
    
    // check forwardslash
    bool forwardslash = rows[row_i+1][col_i-1] == 'M' && rows[row_i-1][col_i+1] == 'S';
    forwardslash |= rows[row_i+1][col_i-1] == 'S' && rows[row_i-1][col_i+1] == 'M';

    // check backslash
    bool backslash = rows[row_i-1][col_i-1] == 'M' && rows[row_i+1][col_i+1] == 'S';
    backslash |= rows[row_i-1][col_i-1] == 'S' && rows[row_i+1][col_i+1] == 'M';
        
    return forwardslash && backslash;
}

int main(int argc, char *argv[]) {
    std::cout << argv[0] << std::endl;
    if (argc < 2) {
        std::cout << "Usage: mull_it_over inputfile" << std::endl;
        return -1;
    }
    std::ifstream input_file(argv[1]);
    std::vector<std::string> rows;
    std::string line;
    size_t num_columns = 0;
    while (std::getline(input_file, line)) {
        rows.push_back(line);
    num_columns = line.size();
    }

    size_t num_xmases = 0;
    for (size_t row_i = 1; row_i < rows.size() - 1; ++row_i) {
    std::string const& row = rows[row_i];
    for (size_t column = 1; column < num_columns - 1; ++column) {
        if (row[column] == 'A') {
        std::cout << "A found at r,c " << row_i << ", " << column << "\n";
        num_xmases += (is_a_centering_mas(rows, row_i, column) ? 1 : 0);
        }
    }
    }
    std::cout << "XMASes found: " << num_xmases << std::endl;
    return 0;
}


 

