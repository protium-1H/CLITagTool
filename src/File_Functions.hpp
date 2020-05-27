#ifndef FILE_FUNCTIONS_HPP
#define FILE_FUNCTIONS_HPP
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
extern "C" {
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
}
class File_Functions {
public:
    static std::string change_directory(char *dir, int *error_num);
    static int get_date(std::string filename);
    static std::string get_date_str(std::string filename);
    static void recur_files(std::string path, std::vector<std::string> *list);
};
#endif