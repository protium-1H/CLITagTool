#include "Video_Functions.hpp"
int Video_Functions::get_video_timestamp(std::string filename) {
    int final = 0, temp;
    std::string str = get_video_timestamp_str(filename);
    if (str[0] == '\0') {
        return 0;
    } else {
        int index = str.find(':');
        std::stringstream stream(str.substr(0, index));
        stream >> temp;
        final += temp * 3600;
        stream = std::stringstream(str.substr(index+1, index));
        stream >> temp;
        final += temp * 60;
        stream = std::stringstream(str.substr(index+4));
        stream >> temp;
        final += temp;
    }
    return final;
}
std::string Video_Functions::get_video_timestamp_str(std::string filename) {
    std::string str = "ffmpeg -i \"" + filename + "\" 2>&1 | grep Duration | cut -d ' ' -f 4 | sed s/,//";
    FILE *ls = popen(str.c_str(), "r");
    char buf[16];
    while (fgets(buf, sizeof(buf), ls) != 0);
    pclose(ls);
    return std::string(buf).substr(0,8);
}
int Video_Functions::get_lower_time(std::string time) { 
    std::stringstream stream;
    int temp, index = time.size(), lower = 0;
    if (index >= 2) {
        index -= 2;
        std::stringstream stream(time.substr(index, 2));
        stream >> temp;
        if (temp >= 60) {
            return -1;
        }
        lower += temp;     
    } 
    if( index >= 2) {
        index -= 2;
        std::stringstream stream(time.substr(index, 2));
        stream >> temp;
        if (temp >= 60) {
            return -1;
        }
        lower += temp * 60;
    }
    if (index > 0) {
        std::stringstream stream(time.substr(0, index));
        stream >> temp;
        lower += temp * 3600;
    }
    return lower;
}
int Video_Functions::get_upper_time(std::string time) {
    std::stringstream stream;
    int temp, index = time.size(), upper = 0;
    if (index >= 2) {
        index -= 2;
        std::stringstream stream(time.substr(index, 2));
        stream >> temp;
        if (temp >= 60) {
            return -1;
        }
        upper += temp;     
    } 
    if( index >= 2) {
        index -= 2;
        std::stringstream stream(time.substr(index, 2));
        stream >> temp;
        if (temp >= 60) {
            return -1;
        }
        upper += temp * 60;
    }
    if (index > 1) {
        std::stringstream stream(time.substr(1, index-1));
        stream >> temp;
        upper += temp * 3600;
    }
    return upper;
}
