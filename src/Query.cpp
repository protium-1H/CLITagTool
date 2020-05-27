#include "Query.hpp"
Query::Query() {
    this->search_type = Query::NIL;
    this->search_term = "";
}
Query::Query(Query::Query_Type type, std::string term) {
    this->search_type = type;
    this->search_term = term;
}
void Query::setType(Query::Query_Type type) {
    this->search_type = type;
}
void Query::setTerm(std::string term) {
    this->search_term = term;
}
Query::Query_Type Query::getType() {
    return this->search_type;
}
std::string Query::getTerm() {
    return this->search_term;
}
