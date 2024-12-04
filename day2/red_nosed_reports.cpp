#include <fstream>
#include <iostream>
#include <numeric>
//#include <optional>
#include <sstream>
//#include <string_view>
#include <vector>


using level_t = int;
using report_t = std::vector<level_t>;

constexpr int MIN_DELTA = 1;
constexpr int MAX_DELTA = 3;

std::vector<report_t> parse_reports(std::string const& filename) {
    std::ifstream report_file(filename);
    // if (!report_file) { throw std::runtime_error("unable to open input file");}
    std::vector<report_t> reports;
    std::string line;
    while(std::getline(report_file, line)) {
        reports.push_back({});
        std::stringstream ss(line);
        std::string level_str;
        while(std::getline(ss, level_str, ' ')) {
            reports.back().push_back(std::stoi(level_str));
        }
    }
    return reports;
}

bool is_report_safe_strict(report_t const& report) {
    if (report.size() < 2) { return true; }
    bool is_level_increasing = false;

    for(int i = 1; i < report.size(); ++i) {
        int delta = report[i] - report[i-1];
        if (std::abs(delta) < MIN_DELTA || std::abs(delta) > MAX_DELTA) {
            std::cout << "delta " << delta << " between " << report[i-1] << " and " << report[i] << " is out of bounts [1,3]" << std::endl;
            return false;
        }
        bool increase = delta > 0;
        if (i == 1) {
            is_level_increasing = increase;
        } else if (increase != is_level_increasing) {
            std::cout << (increase ? "increasing where we were decreasing" : "decreasing where we were increasing") << std::endl;
            return false;
        }
    }
    std::cout << "Safe!" << std::endl;
    return true;
}

bool is_report_safe(report_t const& report) {
    if (report.size() < 2) { return true; }
    bool is_level_increasing = false;
    for(int i = 1; i < report.size(); ++i) {
        bool problem_at_i = false;
        int delta = report[i] - report[i-1];
        if (std::abs(delta) < MIN_DELTA || std::abs(delta) > MAX_DELTA) {
            std::cout << "delta " << delta << " between " << report[i-1] << " and " << report[i] << " is out of bounts [1,3]" << std::endl;
            problem_at_i = true;
        }
        bool increase = delta > 0;
        if (i == 1) {
            is_level_increasing = increase;
        } else if (increase != is_level_increasing) {
            std::cout << (increase ? "increasing where we were decreasing" : "decreasing where we were increasing") << std::endl;
            problem_at_i = true;
        }
        if (problem_at_i) {
            report_t report_without_i = report;
            report_without_i.erase(std::begin(report_without_i) + i);
            report_t report_without_i_minus1 = report;
            report_without_i_minus1.erase(std::begin(report_without_i_minus1) + i - 1);
            bool safe =  is_report_safe_strict(report_without_i) || is_report_safe_strict(report_without_i_minus1);
            if (!safe && i >= 2) {
                report_t report_without_i_minus2 = report;
                report_without_i_minus2.erase(std::begin(report_without_i_minus2) + i - 2);
                safe =  is_report_safe_strict(report_without_i_minus2);
            }
            if (!safe) {
                std::cout << "unsafe report: ";
                for (int level : report) {
                    std::cout << level << " ";
                }
                std::cout << "\n" << std::endl;
            }
            return safe;
        }
    }
    return true;
}



int main(int argc, char *argv[]) {
    std::cout << argv[0] << std::endl;
    if (argc < 2) {
        std::cout << "Usage: red_nosed_reports inputfile" << std::endl;
        return -1;
    }
    std::cout << "Reading reports from " << argv[1] << std::endl;
    std::vector<report_t> reports = parse_reports(argv[1]);
    int num_safe_reports = std::accumulate(std::begin(reports), std::end(reports), 0, [](int safe, report_t const& report) {
        return safe + (is_report_safe(report) ? 1 : 0);
    });

    std::cout << "Safe reports: " << num_safe_reports << std::endl;
    return num_safe_reports;
}
