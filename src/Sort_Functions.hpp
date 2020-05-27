#ifndef SORT_FUNCTIONS_HPP
#define SORT_FUNCTIONS_HPP
#include <vector>
#include <string>
#include <map>
#include <functional>
#include "Video_Functions.hpp"
#include "File_Functions.hpp"
class Sort_Functions {
public:
static std::vector<std::string> sort_name(std::map<std::string, std::vector<std::string>> map, bool reverse);
static std::vector<std::string> sort_date(std::map<std::string, std::vector<std::string>> map, bool reverse);
static std::vector<std::string> sort_time(std::map<std::string, std::vector<std::string>> map, bool reverse);
private:
static int comp_date(std::string one, int two);
static int comp_time(std::string one, int two);
static int rev_comp_date(std::string one, int two);
static int rev_comp_time(std::string one, int two);
static void q_sort(std::vector<std::string> *array, int low, int high, std::function<int(std::string)> piv, std::function<int(std::string, int)>  comp);
static int q_part(std::vector<std::string> *array, int low, int high, std::function<int(std::string)> piv, std::function<int(std::string, int)> comp);
};
#endif