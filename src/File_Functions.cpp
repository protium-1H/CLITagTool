#include "File_Functions.hpp"
std::string File_Functions::change_directory(char *dir, int *error_num) {
    struct stat info;
    if (stat(dir, &info) != 0 || !S_ISDIR(info.st_mode)) {
        return std::string(dir);
        *error_num = 4;
    }
    chdir(dir);
    *error_num = 0;
    return "";
}
int File_Functions::get_date(std::string filename) {
    struct stat info;
    char date[16];
    int int_date;
    stat(filename.c_str(), &info);
    struct tm *timeinfo = localtime(&info.st_ctime);
    std::strftime(date, sizeof(date), "%Y%m%d", timeinfo);
    std::stringstream stream(date);
    stream >> int_date;
    return int_date;
}
std::string File_Functions::get_date_str(std::string filename) {
    struct stat info;
    char date[16];
    stat(filename.c_str(), &info);
    struct tm * timeinfo = localtime(&info.st_ctime);
    std::strftime(date, sizeof(date), "%Y-%m-%d", timeinfo);
    return std::string(date);
}
void File_Functions::recur_files(std::string path, std::vector<std::string> *list) {
    DIR *dirp = opendir(path.c_str());
    struct dirent *dp;
    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_name[0] == '.') {
            continue;
        }
        if (dp->d_type == DT_DIR) {
            recur_files(path + dp->d_name + "/", list);
        }
        if (dp->d_type == DT_REG) {
            list->push_back(path + dp->d_name);
        }
    }
}