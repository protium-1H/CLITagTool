#define TAG_OPEN '['
#define TAG_CLOSE ']'
#ifdef __linux__
    #define SEPERATOR '/'
    #define EXECUTOR "xdg-open"
#elif __APPLE__
    #define SEPERATOR '/'
    #define EXECUTOR "open"
#elif _WIN32
    #define SEPERATOR '\\'
    #define EXCUTOR "start"
#else
    error "Unknown Compiler"
#endif
#include "Search_Functions.hpp"
#include "Sort_Functions.hpp"
enum sort_type {name, date, length};
sort_type sort = name;
bool reverse = false;
// Functions
std::string arg_handler(int argc, char *argv[]);
void list_tags();
std::vector<std::string> display_matches(std::map<std::string, std::vector<std::string>> map);
void execute(std::string filename);
// Function Helpers
std::map<std::string, std::vector<std::string>> createTagPathMap();
std::vector<std::string> createTags(std::string path);
void print_error(int number, std::string supplement = "");

int main(int argc, char *argv[]) {
    std::string query_str;
    std::map<std::string, std::vector<std::string>> map, master_map;
    int error_num;
    if (argc == 1) {
        print_error(1);
    }
    query_str = arg_handler(argc, argv);
    map = createTagPathMap();
    error_num = Search_Functions::search(&map, master_map, query_str);
    if (error_num != 0) {
        print_error(error_num);
    }
    display_matches(map);    
    return 0;
}
std::string arg_handler(int argc, char *argv[]) {
    std::string query_str;
    bool l_flag = false;
    char f_flag = 0, s_flag = 0;
    for (int argv_index = 1; argv_index < argc; argv_index++) {
        if (argv[argv_index][0] == '-') {
            for (int string_index = 1; string_index < strlen(argv[argv_index]); string_index++) {
                switch (argv[argv_index][string_index]) {
                    case 'f': {
                        if (f_flag == 2) {
                            print_error(2);
                        }
                        else {
                            f_flag = 1;
                        }
                        break;
                    }
                    case 'l': {
                        l_flag = true;
                        break;
                    }
                    case 'n': {
                        sort = name;
                        break;
                        break;
                    }
                    case 'd': {
                        sort = date;
                        break;
                    }
                    case 't': {
                        sort = length;
                        break;
                    }
                    case 'r':
                        reverse = true;
                        break;
                    default: {
                        exit(3);
                    }
                }
            }
        }
        if (f_flag == 1) {
            int error_num;
            if (s_flag == 1) {
                print_error(2);
            }
            std::string error_str = File_Functions::change_directory(argv[++argv_index], &error_num);
            
            if (error_num) {
                print_error(error_num, error_str);
            }
            f_flag = 2;
        } else {
            std::string lower_arg = argv[argv_index];
            for (int i = 0; i < lower_arg.size(); i++) {
                lower_arg[i] = tolower(lower_arg[i]);
            }
            query_str = query_str + lower_arg + " ";
        }
    }
    if (l_flag) {
        list_tags();
        exit(0);
    }
    return query_str;
}
void list_tags() {
    std::map<std::string, std::vector<std::string>> map;
    std::vector<std::string> paths;
    File_Functions::recur_files("./", &paths);
    for (int i = 0; i < paths.size(); i++) {
        std::vector<std::string> tags = createTags(paths[i]);
        for (int j = 0; j < tags.size(); j++) {
            if (map.count(tags[j])) {
                map.at(tags[j]).push_back(paths[i]);
            }
            else {
                std::vector<std::string> tag_path;
                tag_path.push_back(paths[i]);
                map.insert(std::pair<std::string,std::vector<std::string>>(tags[j], tag_path));
            }
        }
    }
    std::cout << "Tags Found:" << std::endl;
    for (std::map<std::string, std::vector<std::string>>::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << TAG_OPEN << it->first << TAG_CLOSE;
        if (it != --map.end()) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

}
std::vector<std::string> display_matches(std::map<std::string, std::vector<std::string>> map) {
    std::vector<std::string> matches;
    if (sort == name) {
        matches = Sort_Functions::sort_name(map, reverse);
    } else if (sort == date) {
        matches = Sort_Functions::sort_date(map, reverse);
    } else if (sort == length) {
        matches = Sort_Functions::sort_time(map, reverse);
    }
    for (int i = 0; i < matches.size(); i++) {
        std::string time = Video_Functions::get_video_timestamp_str(matches[i]);
        if (time == "") {
            time = "N/A";
        }
        std::cout << "Match " << i+1 << ":" << std::endl <<
        "    Name: " << matches[i].substr(matches[i].rfind(SEPERATOR, matches[i].length())+1) << std::endl <<
        "    Date: " << File_Functions::get_date_str(matches[i]) << std::endl <<
        "    Time: " << time << std::endl <<
        "Location: " << matches[i] << std::endl;
    }
    return matches;
}
void execute(std::string filename) {
    // for (std::map<std::string, std::vector<std::string>>::iterator it = map.begin(); it != map.end(); ++it) {
    //     list_map.insert(std::pair<int, std::string>(index++, it->first));
    //     matches.push_back(it->first);
    // }
}
std::map<std::string, std::vector<std::string>> createTagPathMap() {
    std::map<std::string, std::vector<std::string>> map;
    std::vector<std::string> paths;
    File_Functions::recur_files("./", &paths);
    for (int i = 0; i < paths.size(); i++) {
        std::vector<std::string> tags = createTags(paths[i]);
        map.insert(std::pair<std::string, std::vector<std::string>>(paths[i], tags));
    }
    return map;
}
std::vector<std::string> createTags(std::string path) {
    std::vector<std::string> tags;
    std::string name;
    int index = path.rfind(SEPERATOR, path.length());
    if (index != std::string::npos) {
        name = path.substr(index+1, path.length() - index);
    }
    bool active = false;
    std::string tag;
    for (int i = 0; i < name.length(); i++) {
        if (name[i] == TAG_OPEN) {
            if (active) {
                print_error(7, path);
            }
            active = true;
            tag = "";
            continue;
        }
        else if(name[i] == TAG_CLOSE) {
            if(!active) {
                print_error(7, path);
            }
            active = false;
            tags.push_back(tag);
            continue;
        }
        else if (active) {
            tag += tolower(name[i]);
        }
    }
    if (active) {
        print_error(7, path);
    }
    return tags;
}
void print_error(int number, std::string supplement) {
    std::string error;
    switch (number) {
        case 1:
            error = "No options entered.";
            break;
        case 2:
            error = "-f option may not be used twice.";
            break;
        case 3:
            error =  "Invalid Option:";
            break;
        case 4:
            error = "Can not access:";
            break;
        case 5:
            error = "Invalid date format.";
            break;
        case 6:
            error = "Invalid time format";
            break;
        case 7:
            error = "Invalid tag format on file:";
            break;
        case 8:
            error = "Invalid search query.";
            break;
        case 9:
            error = "Grouping mismatch.";
            break;
        default:
            error = "Unknown Error.";
    }
    std::cerr << error << supplement << " (Refer to documentation or man page for help.)" << std::endl;
    exit(number);
}
