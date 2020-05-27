#include "Search_Functions.hpp"
int Search_Functions::search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, std::string query_str) {
    std::map<std::string, std::vector<std::string>>parent_map;
    std::vector<Query> queries;
    master_map.insert(map->begin(), map->end());
    int error_num = generate_queries(query_str, &queries);
    if (error_num) {
        return error_num;
    }
    return recur_search(map, parent_map, master_map, queries);
}
int Search_Functions::recur_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> parent_map,std::map<std::string, std::vector<std::string>> master_map, std::vector<Query> queries) {
    parent_map.insert(map->begin(), map->end());
    for(int i = 0; i < queries.size(); i++) {
        switch (queries[i].getType()) {
            case Query::AND: {
                and_search(map, master_map, queries[i]);
                break;
            }
            case Query::OR: {
                or_search(map, master_map, queries[i]);
                break;
            }
            case Query::EXCLUDE: {
                exclude_search(map, master_map, queries[i]);
                break;
            }
            case Query::DATE: {
                int error_num = date_search(map, master_map, queries[i]);
                if (error_num) {
                    return error_num;
                }
                break;
            }
            case Query::TIME: {
                int error_num = time_search(map, master_map, queries[i]);
                if (error_num) {
                    return error_num;
                }
                break;
            }
            case Query::GROUP_START: {
                int error_num = group_search(map, parent_map, master_map, queries, &i);
                if (error_num) {
                    return error_num;
                }
                break;
            }
            default:;
        }
    }
    return 0;
}
void Search_Functions::and_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, Query query) {
    std::map<std::string, std::vector<std::string>>::iterator m_it = map->begin();
    while (m_it != map->end()) {
        bool found = false;
        for (std::vector<std::string>::iterator v_it = m_it->second.begin(); v_it != m_it->second.end(); v_it++) {
            if (*v_it == query.getTerm()) {
                found = true;
                break;
            }
        }
        if (found) {
            m_it++; 
        } else {
            m_it = map->erase(m_it);
        }
    }
}
void Search_Functions::or_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, Query query) {
    std::map<std::string, std::vector<std::string>>::iterator m_it = master_map.begin();
    while (m_it != master_map.end()) {
        bool found = false;
        for (std::vector<std::string>::iterator v_it = m_it->second.begin(); v_it != m_it->second.end(); v_it++) {
            if (*v_it == query.getTerm()) {
                found = true;
                break;
            }
        }
        if (found) {
            map->insert(std::pair<std::string, std::vector<std::string>>(m_it->first, m_it->second));
        }
        m_it++;
    }
}
void Search_Functions::exclude_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, Query query) {
    std::map<std::string, std::vector<std::string>>::iterator m_it = map->begin();
    while (m_it != map->end()) {
        bool found = false;
        for (std::vector<std::string>::iterator v_it = m_it->second.begin(); v_it != m_it->second.end(); v_it++) {
            if (*v_it == query.getTerm()) {
                found = true;
                break;
            }
        }
        if (!found) {
            m_it++; 
        } else {
            m_it = map->erase(m_it);
        }
    }
}
int Search_Functions::date_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, Query query) {
    std::string term = query.getTerm();
    int lower = 0, upper = INT_MAX;
    if (term.size() == 9 && term[0] == '>' && isdigit(term[1]) && isdigit(term[2]) && isdigit(term[3]) && isdigit(term[4]) && isdigit(term[5]) && isdigit(term[6]) && isdigit(term[7]) && isdigit(term[8])) {
        std::stringstream stream(term.substr(1));
        stream >> upper;
    }
    else if (term.size() == 8 && isdigit(term[0]) && isdigit(term[1]) && isdigit(term[2]) && isdigit(term[3]) && isdigit(term[4]) && isdigit(term[5]) && isdigit(term[6]) && isdigit(term[7])) {
        std::stringstream stream(term);
        stream >> lower;
    }
    else if (term.size() == 17 && isdigit(term[0]) && isdigit(term[1]) && isdigit(term[2]) && isdigit(term[3]) && isdigit(term[4]) && isdigit(term[5]) && isdigit(term[6]) && isdigit(term[7]) && term[8] == '>' && isdigit(term[9]) && isdigit(term[10]) && isdigit(term[11]) && isdigit(term[12]) && isdigit(term[13]) && isdigit(term[14]) && isdigit(term[15]) && isdigit(term[16])) {
        std::stringstream stream(term.substr(0,8));
        stream >> lower;
        stream = std::stringstream(term.substr(9));
        stream >> upper;
    }
    else {
        return 5;
    }
    std::map<std::string, std::vector<std::string>>::iterator it = map->begin();
    while (it != map->end()) {
        int file_date = File_Functions::get_date(it->first);
        std:: cout << "Lower: " << lower << "\nFile Date: " << file_date << "\nUpper: " << upper << std::endl;
        if (lower < file_date && upper > file_date) {
            it++;
        } else {
            it = map->erase(it);
        }
    }
    return 0;
}
int Search_Functions::time_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> master_map, Query query) {
    std::string term = query.getTerm();
    int lower, upper;
    for (int i = 0; i < term.size(); i++) {
        if (!isdigit(term[i]) && term[i] != '>') {
            return 6;
        }
    }
    int index = term.find('>');
    if (index == std::string::npos) {
        lower = Video_Functions::get_lower_time(term);
        upper = INT_MAX;
    } else if (index == 0) {
        lower = 0;
        upper = Video_Functions::get_upper_time(term);
    }
    else {
        lower = Video_Functions::get_lower_time(term.substr(0, index));
        upper = Video_Functions::get_upper_time(term.substr(index));
    }
    if (lower == -1 | upper == -1) {
        return 6;
    }
    std:: cout << "LLL:" << lower << " UUU:" << upper << std::endl;
    std::map<std::string, std::vector<std::string>>::iterator it = map->begin();
    while (it != map->end()) {
        int file_time = Video_Functions::get_video_timestamp(it->first);
        if (lower < file_time && upper > file_time) {
            it++;
        } else {
            it = map->erase(it);
        }
    }
    return 0;
}
int Search_Functions::group_search(std::map<std::string, std::vector<std::string>> *map, std::map<std::string, std::vector<std::string>> parent_map, std::map<std::string, std::vector<std::string>> master_map, std::vector<Query> queries, int *index) {
    int i = *index;
    std::vector<Query> group;
    int group_open = 1, group_close = 0;
    for(i++; i < queries.size() && group_open - group_close != 0; i++) {
        if (queries[i].getType() == Query::GROUP_START) {
            group_open++;
        }
        if (queries[i].getType() == Query::GROUP_END) {
            group_close++;
        }
        group.push_back(queries[i]);
    }
    if (group_open - group_close != 0) {
        return 9;
    } else {
        i--;
    }
    recur_search(&parent_map, parent_map, master_map, group);
    if (queries[*index].getTerm() == "and") {
        std::map<std::string, std::vector<std::string>>::iterator it = map->begin();
        while (it != map->end()) {
            if (!parent_map.count(it->first)) {
                it = map->erase(it);
            } else  {
                it++; 
            }
        }
    } else if (queries[*index].getTerm() == "or") {
        std::map<std::string, std::vector<std::string>>::iterator it = parent_map.begin();
        while(it != parent_map.end()) {
            map->insert(std::pair<std::string, std::vector<std::string>>(it->first, it->second));
            it++;
        }

    } else if (queries[*index].getTerm() == "exclude") {
        std::map<std::string, std::vector<std::string>>::iterator it = map->begin();
        std::cout << map->size() << " " << parent_map.size() << std::endl;
        while (it != map->end()) {
            if (parent_map.count(it->first)) {
                it = map->erase(it);
            } else  {
                it++; 
            }
        }
        std::cout << map->size() << " " << parent_map.size() << std::endl;
    }
    *index = i;
    return 0;
}

int Search_Functions::generate_queries(std::string query_str, std::vector<Query> *queries) {
    Query query;
    std::string term;
    for (int index = 0; index < query_str.size(); index++) {
        if (query_str[index] == ' ') {
            query.setTerm(term);
            term = "";
            term_check(&query);
            Query::Query_Type type;
            if ((type = query.getType()) != Query::INVALID) {
                if (query.getTerm().size() != 0) {
                    queries->push_back(query);
                    query = Query();
                }
            } else if (query.getTerm().size() == 0) {

            } else {
                return 8;
            }
        } else if (query_str[index] == '&') {
            query.setType(Query::AND);
        } else if (query_str[index] == '|') {
            query.setType(Query::OR);
        } else if (query_str[index] == '-') {
            query.setType(Query::EXCLUDE);
        } else if (query_str[index] == '@') {
            query.setType(Query::DATE);
        } else if (query_str[index] == '#') {
            query.setType(Query::TIME);
        } else if (query_str[index] == '(') {
            Query::Query_Type type = query.getType();
            query.setTerm(term);
            term = "";
            term_check(&query);
            if (query.getTerm().size() == 0) {
                if (type == Query::NIL && query.getType() == Query::INVALID) {
                    query.setType(Query::AND);
                }
                switch (query.getType()) {
                    case Query::AND:
                        query.setTerm("and");
                        break;
                    case Query::OR:
                        query.setTerm("or");
                        break;
                    case Query::EXCLUDE:
                        query.setTerm("exclude");
                        break;
                    case Query::INVALID:
                        return 8;
                        break;
                    default:;
                }
            } else if (query.getType() != Query::INVALID) {
                queries->push_back(query);
                query.setTerm("a");
            }
            query.setType(Query::GROUP_START);
            queries->push_back(query);
            query = Query();
            
            query = Query();
        } else if (query_str[index] == ')') {
            query.setTerm(term);
            term = "";
            term_check(&query);
            if (query.getType() != Query::INVALID) {
                queries->push_back(query);
                queries->push_back(Query(Query::GROUP_END));
                query = Query();
            } else if (query.getTerm().size() == 0) {
                queries->push_back(Query(Query::GROUP_END));
                query = Query();
            } else {
                return 8;
            }
        } else {
            term += query_str[index];
        }
    }
return 0;
}
void Search_Functions::term_check(Query *query) {
    std::string op_check = query->getTerm();
    if (op_check == "and") {
        query->setType(Query::AND);
        query->setTerm("");
    } else if (op_check == "or") {
        query->setType(Query::OR);
        query->setTerm("");
    } else if (op_check == "not") {
        query->setType(Query::EXCLUDE);
        query->setTerm("");
    } else if (op_check == "date") {
        query->setType(Query::DATE);
        query->setTerm("");
    } else if (op_check == "time") {
        query->setType(Query::TIME);
        query->setTerm("");
    } else if (query->getType() == Query::NIL) {
        if (query->getTerm().size() != 0) {
            query->setType(Query::AND);
        } else {
            query->setType(Query::INVALID);
        }
    }
}