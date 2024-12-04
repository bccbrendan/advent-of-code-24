#include <fstream>
#include <iostream>
#include <string>
#include <vector>

size_t count_xmases_starting_from_x(std::vector<std::string> const& rows, size_t row_i, size_t col_i) {
    size_t count = 0;
    // search 8 cardinal directions
    // N
    if (row_i >= 3
        && rows[row_i - 1][col_i] == 'M'
        && rows[row_i - 2][col_i] == 'A'
        && rows[row_i - 3][col_i] == 'S') {
        count++;
    }
    // NE
    if (row_i >= 3 && col_i + 3 < rows[row_i].size()
        && rows[row_i - 1][col_i + 1] == 'M'
        && rows[row_i - 2][col_i + 2] == 'A'
        && rows[row_i - 3][col_i + 3] == 'S') {
        count++;
    }
    // E
    if (col_i + 3 < rows[row_i].size()
        && rows[row_i][col_i + 1] == 'M'
        && rows[row_i][col_i + 2] == 'A'
        && rows[row_i][col_i + 3] == 'S') {
        count++;
    }
    // SE
    if (row_i + 3 < rows.size() && col_i + 3 < rows[row_i].size()
        && rows[row_i + 1][col_i + 1] == 'M'
        && rows[row_i + 2][col_i + 2] == 'A'
        && rows[row_i + 3][col_i + 3] == 'S') {
        count++;
    }
    // S
    if (row_i + 3 < rows.size()
        && rows[row_i + 1][col_i] == 'M'
        && rows[row_i + 2][col_i] == 'A'
        && rows[row_i + 3][col_i] == 'S') {
        count++;
    }
    // SW
    if (row_i + 3 < rows.size() && col_i >= 3
        && rows[row_i + 1][col_i - 1] == 'M'
        && rows[row_i + 2][col_i - 2] == 'A'
        && rows[row_i + 3][col_i - 3] == 'S') {
        count++;
    }
    // W
    if (col_i >= 3
        && rows[row_i][col_i - 1] == 'M'
        && rows[row_i][col_i - 2] == 'A'
        && rows[row_i][col_i - 3] == 'S') {
        count++;
    }
    // NW
    if (row_i >= 3 && col_i >= 3
        && rows[row_i - 1][col_i - 1] == 'M'
        && rows[row_i - 2][col_i - 2] == 'A'
        && rows[row_i - 3][col_i - 3] == 'S') {
        count++;
    }
        
    return count;
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
    for (size_t row_i = 0; row_i < rows.size(); ++row_i) {
    std::string const& row = rows[row_i];
    for (size_t column = 0; column < num_columns; ++column) {
        if (row[column] == 'X') {
        std::cout << "X found at r,c " << row_i << ", " << column << "\n";
        num_xmases += count_xmases_starting_from_x(rows, row_i, column);
        }
    }
    }
    std::cout << "XMASes found: " << num_xmases << std::endl;
    return 0;
}


 

