#include <regex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <utility>
#include <vector>


constexpr char VISITED = 'X';
constexpr char OBSTRUCTED = '#';

enum Direction {
    N = '^',
    E = '>',
    S = 'v',
    W = '<',
    Unknown = '\0'
};

struct position_t {
	size_t row = 0;
	size_t col = 0;
	Direction dir = Unknown;

	position_t() {};
	position_t(size_t row, size_t col, Direction dir) :
		row(row),
		col(col),
		dir(dir)
	{}
};



using maze_t = std::vector<std::string>;

auto parse_map(std::string const& filename) {
    std::ifstream input_file(filename);
    std::string line;
    maze_t maze;
    while (std::getline(input_file, line)) {
        maze.push_back(line);
    }
    return maze;
}

position_t find_starting_location(maze_t const& maze) {
    for (size_t row_i = 0; row_i < maze.size(); ++row_i) {
	const auto &row = maze[row_i];
	for (const char c : "^>v<") {
            if (auto delim = row.find(c); delim != std::string::npos) {
	       return {row_i, delim, Direction(c)};
	    }
	}
    }
    return {0, 0, Direction::Unknown};
}


void print_maze(maze_t const& maze) {
    for(const auto& row : maze) {
	    std::cout << row << "\n";
    }
}

bool about_to_exit(position_t const& pos, maze_t const& maze) {
	switch (pos.dir) {
		case Direction::N:
			return pos.row == 0;
		case Direction::E:
			return pos.col + 1 >= maze[0].size();
		case Direction::S:
			return pos.row + 1 >= maze.size();
		case Direction::W:
			return pos.col == 0;
	}
	throw std::runtime_error("moving with unknown direction!");
}

position_t turn_right(position_t pos) {
	switch (pos.dir) {
		case Direction::N:
			return {pos.row, pos.col, Direction::E};
		case Direction::E:
			return {pos.row, pos.col, Direction::S};
		case Direction::S:
			return {pos.row, pos.col, Direction::W};
		case Direction::W:
			return {pos.row, pos.col, Direction::N};
	}
	throw std::runtime_error("turning with unknown direction!");
}

bool move_forward_and_check_exit(position_t &pos, maze_t &maze) {
        do {
		if (about_to_exit(pos, maze)) {
			maze[pos.row][pos.col] = VISITED;
			std::cout << "Exiting " << std::string(1, pos.dir) << " at [" << pos.row << "," << pos.col << "]\n";
			return true;
		}
		position_t next_pos;
		switch (pos.dir) {
			case Direction::N:
				next_pos = {pos.row - 1, pos.col, pos.dir};
				break;
			case Direction::E:
				next_pos = {pos.row, pos.col + 1, pos.dir};
				break;
			case Direction::S:
				next_pos = {pos.row + 1, pos.col, pos.dir};
				break;
			case Direction::W:
				next_pos = {pos.row, pos.col - 1, pos.dir};
				break;
			default:
				throw std::runtime_error("next direction unknown");
		}
		if (maze[next_pos.row][next_pos.col] == OBSTRUCTED) {
			return false;
		}
		maze[pos.row][pos.col] = VISITED;
		pos = next_pos;
		maze[pos.row][pos.col] = pos.dir;
	} while (true);
}

size_t count_visited(maze_t const& maze) {
	return std::accumulate(std::cbegin(maze), std::cend(maze), 0, [](size_t sum, const auto& row) {
			return sum + std::count(std::cbegin(row), std::cend(row), VISITED);
			});
}

int main(int argc, char *argv[]) {
    std::cout << argv[0] << std::endl;
    if (argc < 2) {
        std::cout << "Usage: mull_it_over inputfile" << std::endl;
        return -1;
    }
    auto maze = parse_map(argv[1]);
    auto position = find_starting_location(maze);

    std::cout << "row " << position.row << " col " << position.col << " " << std::string(1, position.dir) << "\n";
//    print_maze(maze);
    maze[position.row][position.col] = VISITED;
    while(!move_forward_and_check_exit(position, maze)) {
	    std::cout << "\n"; 
	    position = turn_right(position);
	    print_maze(maze);
    }
 //   print_maze(maze);
    size_t num_visited = count_visited(maze);
    std::cout << "visited " << num_visited << "\n";
    return 0;
}
