#ifndef QUERY_HPP
#define QUERY_HPP
#include <string>

class Query {
public:
    enum Query_Type {
        NIL, INVALID, AND, OR, EXCLUDE, DATE, TIME, GROUP_START, GROUP_END
    };
    Query();
    Query(Query::Query_Type type, std::string term = "");
    void setType(Query::Query_Type type);
    void setTerm(std::string term);
    Query::Query_Type getType();
    std::string getTerm();
private:
    Query::Query_Type search_type;
    std::string search_term;
};
#endif
