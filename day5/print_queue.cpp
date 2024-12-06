#include <regex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <sstream>
#include <utility>
#include <vector>

using page_ordering_rules_t = std::unordered_map<int, std::unordered_set<int>>;
using updates_list_t = std::vector<std::vector<int>>;

std::pair<page_ordering_rules_t, updates_list_t> parse_input(std::string const& filename) {
    std::ifstream infile(filename);
    std::string line;

    page_ordering_rules_t rules;
    // exactly one blank line separates rules from updates
    while(std::getline(infile, line) && line != "") {
        auto delim = line.find('|');
        int before = std::stoi(line.substr(0, delim));
        int after = std::stoi(line.substr(delim + 1, line.size()));
        rules[before].insert(after);
    }

    // now parse the updates lists
    updates_list_t updates;
    while(std::getline(infile, line)) {
        std::stringstream ss{ line };
        std::string page;
        std::vector<int> pages_in_update;
        while(std::getline(ss, page, ',')) {
            pages_in_update.push_back(std::stoi(page));
            // std::cout << "update " << updates.size() << " has page " << page << "\n";
        }
        updates.push_back(std::move(pages_in_update));
    }
    return {rules, updates};
}


bool does_violate_rule(int before, int after, std::vector<int> const& update, page_ordering_rules_t& rules) {
    if (before == after) { return false; }
    // return true if 'after' page occurs in the before-than relation to before
    // e.g. if rules[after].find(before) != rules[after].end(), or if any of rules[after]s elements violate this rule
    std::queue<int> to_check;
    to_check.push(after);
    while (!to_check.empty()) {
        int page = to_check.front();
        // std::cout << page << " should come before ";
        for (int after_page : rules[page]) {
            if (std::find(std::cbegin(update), std::cend(update), after_page) == std::cend(update)) {
                // if the 'after' page of a rule isn't in the given update, it doesn't apply
                continue;
            }
            // std::cout << after_page << ", ";
            if (after_page == before) {
                // we found the putative "before" page in our putative "after" page's 'after' tree.
                return true;
            }
            to_check.push(after_page);
        }
        to_check.pop();
        // std::cout << "\n";
    }
    return false;
}


bool is_update_valid(std::vector<int> const& update, page_ordering_rules_t& rules) {
    std::vector<int> earlier_pages;
    // check against all the earlier pages to make sure that this page shouldn't occur before them, then add this to the earlier pages list
    //for (int p : update) {
    //    std::cout << p << ", ";
    //}
    // std::cout << "\n";
    for (int i = 0; i < update.size(); ++i) {
        int page = update[i];
        bool page_out_of_order = std::any_of(std::cbegin(update), std::cbegin(update) + i,
                                             [page, &update, &rules](int before){
                                                return does_violate_rule(before, page, update, rules);
                                              });
        if (page_out_of_order) { return false; }
    }
    return true;
}


int get_middle_page_if_valid(std::vector<int> const& update, page_ordering_rules_t& rules) {
    return is_update_valid(update, rules) ? update[update.size() / 2] : 0;
}

int get_middle_page_of_corrected_update(std::vector<int>& update, page_ordering_rules_t& rules) {
    if (is_update_valid(update, rules)) {
        return 0;
    }
    std::sort(update.begin(), update.end(), [&rules](int a, int b) {
        // return true if a can come before b
        return rules[b].find(a) == rules[b].end();
    });
    return update[update.size() / 2];
}

int main(int argc, char *argv[]) {
    std::cout << argv[0] << std::endl;
    if (argc < 2) {
        std::cout << "Usage: mull_it_over inputfile" << std::endl;
        return -1;
    }
    auto [rules, updates] = parse_input(argv[1]);
    std::cout << "I count " << rules.size() << " rules and " << updates.size() << " updates\n";
    // part1: int sum = std::accumulate(std::cbegin(updates), std::cend(updates), 0, [&rules](int accum, const auto& update){ return accum + get_middle_page_if_valid(update, rules); });
    // part2
    int sum = std::accumulate(std::begin(updates), std::end(updates), 0, [&rules](int accum, auto& update){ return accum + get_middle_page_of_corrected_update(update, rules); });
    std::cout << "sum " << sum << "\n";
    return 0;
}
 
