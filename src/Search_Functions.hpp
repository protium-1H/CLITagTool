#ifndef SEARCH_FUNCTIONS_HPP
#define SEARCH_FUNCTIONS_HPP
#include <vector>
#include <map>
#include <vector>
#include <climits>
#include "Query.hpp"
#include "File_Functions.hpp"
#include "Video_Functions.hpp"
class Search_Functions {
private:
    static int recur_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> parent_map, std::map<std::string, std::vector<std::string>> master_map, std::vector<Query>);
    static void and_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, Query query);
    static void or_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, Query query);
    static void exclude_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, Query query);
    static int date_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, Query query);
    static int time_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, Query query);
    static int group_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> parent_map, std::map<std::string, std::vector<std::string>> master_map, std::vector<Query> queries, int *index);
    static int generate_queries(std::string query_str, std::vector<Query> *queries);
    static void term_check(Query *query);
public:
    static int search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, std::string query_str);
};
#endif