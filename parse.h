#ifndef _PARSE_H_
#define _PARSE_H_

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <random>
#include <ctime>
#include <vector>
#include <map>
#include <set>

#define sample_num 1000000

struct page{
    std::string term;
    int id;
    int in_cnt;
    std::vector<int> link_in;
    int out_cnt;
    std::vector<int> link_out;
    double rank = 0;
    page(std::string& term_, int id_):term(term_),id(id_),in_cnt(0), out_cnt(0){}
    page():term(""), id(0), in_cnt(0), out_cnt(0){}
};

int parse_id(const char *FILENAME);
void sampling(const char *FILENAME, int total_terms);
void parse_links(const char* FILENAME);
std::istream& operator>>(std::istream& is, page& p);
std::ostream& operator<<(std::ostream& os, const page& p);

#endif