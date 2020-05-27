#include "Sort_Functions.hpp"
std::vector<std::string> Sort_Functions::sort_name(std::map<std::string, std::vector<std::string>> map, bool reverse) {
    std::vector<std::string> sorted;
    if (!reverse) {
        for (std::map<std::string, std::vector<std::string>>::iterator it = map.begin(); it != map.end(); ++it) {
            sorted.push_back(it->first);
        }
    } else {
        for (std::map<std::string, std::vector<std::string>>::reverse_iterator it = map.rbegin(); it != map.rend(); ++it) {
            sorted.push_back(it->first);
        }
    }
    return sorted;
}
std::vector<std::string> Sort_Functions::sort_date(std::map<std::string, std::vector<std::string>> map, bool reverse) {
    std::vector<std::string> sorted;
    for (std::map<std::string, std::vector<std::string>>::iterator it = map.begin(); it != map.end(); ++it) {
            sorted.push_back(it->first);
        }
    if (!reverse) {
        q_sort(&sorted, 0, sorted.size()-1, File_Functions::get_date, comp_date);
    } else {
        q_sort(&sorted, 0, sorted.size()-1, File_Functions::get_date, rev_comp_date);
    }
    return sorted;
}
std::vector<std::string> Sort_Functions::sort_time(std::map<std::string, std::vector<std::string>> map, bool reverse) {
    std::vector<std::string> sorted;
    for (std::map<std::string, std::vector<std::string>>::iterator it = map.begin(); it != map.end(); ++it) {
            sorted.push_back(it->first);
        }
    if (!reverse) {
        q_sort(&sorted, 0, sorted.size()-1, Video_Functions::get_video_timestamp, comp_time);
    } else {
        q_sort(&sorted, 0, sorted.size()-1, Video_Functions::get_video_timestamp, rev_comp_time);
    }
    return sorted;
}
int Sort_Functions::comp_date(std::string one, int two) {
    return File_Functions::get_date(one) - two;
}
int Sort_Functions::comp_time(std::string one, int two) {
    return Video_Functions::get_video_timestamp(one) - two;
}
int Sort_Functions::rev_comp_date(std::string one, int two) {
    return -1 * comp_date(one, two);
}
int Sort_Functions::rev_comp_time(std::string one, int two) {
    return -1 * comp_time(one, two);
}
void Sort_Functions::q_sort(std::vector<std::string> *array, int low, int high, std::function<int(std::string)> piv, std::function<int(std::string, int)> comp) {
    if (low < high) {
        int pivot = q_part(array, low, high, piv, comp);
        q_sort(array, low, pivot, piv, comp);
        q_sort(array, pivot + 1, high, piv, comp);
    }
}
int Sort_Functions::q_part(std::vector<std::string> *array, int low, int high, std::function<int(std::string)> piv, std::function<int(std::string, int)> comp) {
    int pivot = piv(array->at(low));
    int lower = low - 1;
    int upper = high + 1;
    while (true) {
        do {
            lower++;
        } while (comp(array->at(lower), pivot) < 0);
        do {
            upper--;
        } while (comp(array->at(upper), pivot) > 0);
        if (lower >= upper) {
            return upper;
        }
        std::string temp = array->at(lower);
        array->at(lower) = array->at(upper);
        array->at(upper) = temp;
    }
}