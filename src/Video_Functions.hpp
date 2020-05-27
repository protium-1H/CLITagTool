#ifndef VIDEO_FUNCTIONS_HPP
#define VIDEO_FUNCTIONS_HPP
#include <string>
#include <sstream>
#include <iostream>
class Video_Functions {
public:
    static int get_video_timestamp(std::string filename);
    static std::string get_video_timestamp_str(std::string filename);
    static int get_upper_time(std::string time);
    static int get_lower_time(std::string time);
};
#endif