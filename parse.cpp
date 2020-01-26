#include "parse.h"

using namespace std;

//overriding the << operator to output the struct page
ostream& operator<<(ostream& os, const page& p){
    os << p.term << endl;
    os << p.id << endl;
    os << p.in_cnt << endl;
    for(auto i : p.link_in){
        os << i << " ";
    }
    os << endl << p.out_cnt << endl;
    for(auto i : p.link_out){
        os << i << " ";
    } 
    os << endl;
    return os;
}

//overriding the >> operator to input the struct page
istream& operator>>(istream& is, page& p){
    getline(is, p.term);
    is >> p.id;
    is >> p.in_cnt;
    int tmp;
    for(int i = 0;i < p.in_cnt; ++i){
        is >> tmp;
        p.link_in.push_back(tmp);
    }
    is >> p.out_cnt;
    for(int i = 0; i < p.out_cnt; ++i){
        is >> tmp;
        p.link_out.push_back(tmp);
    }
    is.get();
    is.get();
    return is;
}

//parsing all the name-id pairs
//result saved in "title_id.txt"
int parse_id(const char *FILENAME){
    ofstream fout("title_id.txt");
    ifstream fin(FILENAME);
    string s;
    int cnt = 0;
    while(getline(fin, s)){
        auto pos1 = s.find("<title>");
        if(pos1 == string::npos)
            continue;

        auto pos2 = s.find("</title>");
        string title(s, pos1 + 7, pos2 - pos1 - 7);
        fout << title << endl;
        while(getline(fin, s)){
            auto pos1 = s.find("<id>");
            if(pos1 == string::npos)
                continue;

            auto pos2 = s.find("</id>");
            string id(s, pos1 + 4, pos2 - pos1 - 4);
            fout << id << endl;
            if(!(cnt % 100000)){
                cout << cnt << endl;
            }
            cnt++;
            break;
        }
    }
    cout << cnt << endl;
    return cnt;
}

//sampling the terms
//sample_num is set to 1000000
//result saved in "sample_title_id.txt"
void sampling(const char *FILENAME, int total_terms){
    ifstream fin(FILENAME);
    ofstream fout("sample_title_id.txt");

    string term;
    int id;
    vector<pair<int, string>> v;
    while(true){
        if(fin.eof())
            break;
        getline(fin, term);
        (fin >> id).get();
        v.emplace_back(id, term);
    }

    set<int> s;
    set<string> s_string;
    srand((unsigned int)time(NULL));
    int sample_idx;
    int cnt = 0;
    while(cnt < sample_num){
        sample_idx = ((int64_t)rand() * rand()) % total_terms;
        auto tmp_0 = s.insert(sample_idx);
        if(tmp_0.second){
            auto tmp_1 = s_string.insert(v[sample_idx].second);
            if(tmp_1.second)
                cnt++;
            else
                continue;
        }
        else
            continue;
    }

    for(auto s_it = s.begin(); s_it != s.end(); s_it++){
        fout << v[*s_it].second << endl;
        fout << v[*s_it].first << endl;
    }
}

//parsing the links between given pages
//result saved in "links_info.txt" or "sample_links_info.txt"
void parse_links(const char *FILENAME){
    ifstream terms_in(FILENAME);
    ifstream data_in("enwiki-20190920-pages-articles-multistream.xml");
    ofstream fout;
    const char* SAMPLE_RESULTS = "sample_links_info.txt";
    const char* RESULTS = "links_info.txt";
    if(FILENAME[0] == 's'){
        fout.open(SAMPLE_RESULTS);
    }
    else{
        fout.open(RESULTS);
    }

    //keep the name-id pairs in a hash map
    string term;
    int id;
    int cnt = 0;
    unordered_map<string, page*> hm;
    while(getline(terms_in, term)){
        terms_in >> id;
        terms_in.get();
        hm[term] = new page(term, id);
    }

    //traverse the data and parse the links
    string content;
    while(getline(data_in, content)){
        auto pos1 = content.find("<title>");
        if(pos1 == string::npos)
            continue;

        auto pos2 = content.find("</title>");
        string title(content, pos1 + 7, pos2 - pos1 - 7);

        if(hm.find(title) == hm.end())
            continue;

        cnt++;
        if(!(cnt % 100000))
            cout << cnt << endl;
        set<string> terms;
        while(getline(data_in, content)){
            pos1 = content.find("</page>");
            if(pos1 != string::npos)
                break;
            
            pos2 = 0;
            while((pos1 = content.find("[[", pos2)) != string::npos){
                pos2 = content.find_first_of("]|", pos1);
                string linked_term(content, pos1 + 2, pos2 - pos1 - 2);

                if(hm.find(linked_term) == hm.end())
                    continue;
                else{
                    terms.insert(linked_term);
                }
            }
        }
        int id_ = hm[title]->id;
        for(auto it = terms.begin(); it != terms.end(); ++it){
            hm[title]->link_out.push_back(hm[*it]->id); 
            hm[title]->out_cnt++;
            hm[*it]->link_in.push_back(hm[title]->id);
            hm[*it]->in_cnt++; 
        }
    }
    
    for(auto it = hm.begin(); it != hm.end(); it++){
        fout << *(it->second);
    }
}

