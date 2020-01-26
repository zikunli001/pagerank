#ifndef _PAGERANK_H_
#define _PAGERANK_H_

#include "parse.h"
#include <cmath>
#include <algorithm>

#define alpha 0.9
#define initializer 1

using namespace std;

 void read_data(const char* FILENAME, unordered_map<int, page>& hash_map, int page_cnt){
    ifstream fin(FILENAME);
    for(int i = 0; i < page_cnt; ++i){
        if(!(i % 1000000))
            cout << i << endl;
        page tmp;
        fin >> tmp;
        tmp.rank = initializer;
        hash_map[tmp.id] = tmp;
    }
}

//pagerank algorithm without random walk
void page_rank(const char *FILENAME, int page_cnt){
    unordered_map<int, page> hash_map;
    read_data(FILENAME, hash_map, page_cnt);
    double delta = 99999999;
    int cnt = 0;
    //if delta is larger than the threshold, the loop continues
    while(delta > 1e-1){
        cout << "in while" << endl;
        delta = 0;
        cnt++;
        int i = 0;
        for(auto it = hash_map.begin(); it != hash_map.end(); ++it){
            i++;
            if(!(i % 100000))
                cout << i << endl;
            double previous_rank = it->second.rank;
            double new_rank = 0;
            for(auto id : it->second.link_in){
                new_rank += hash_map[id].rank / hash_map[id].out_cnt;
            }

            delta = max(delta, fabs(new_rank - previous_rank));
            it->second.rank = new_rank;
        }
        cout << cnt << endl;
        cout << delta << endl;
    }
    vector<pair<string, double>> v;
    for(auto it = hash_map.begin(); it != hash_map.end(); ++it){
        v.push_back(pair<string, double>(it->second.term, it->second.rank));
    }

    sort(v.begin(), v.end(),
        [](pair<string, double>&a, pair<string, double>&b)->bool{
            return a.second > b.second;
        }
    );

    ofstream fout("result.txt");
    for(int i = 0; i < 10; ++i){
        cout << "top " << i << ": " << "pagerank = " << v[i].second << "\t";
        cout << v[i].first << endl;
    }

    for(auto page : v){
        fout << page.first << '\t' << page.second << endl;
    }
}

void pagerank_random_walk(const char* FILENAME, int page_cnt){
    unordered_map<int, page> hash_map;
    read_data(FILENAME, hash_map, page_cnt);
    double delta = 99999999;
    int cnt = 0;
    //if delta is larger than the threshold, the loop continues
    while(delta > 1e-1){
        delta = 0;
        cnt++;
        int i = 0;
        for(auto it = hash_map.begin(); it != hash_map.end(); ++it){
            i++;
            if(!(i % 1000000))
                cout << i << endl;
            double previous_rank = it->second.rank;
            double new_rank = 0;
            for(auto id : it->second.link_in){
                new_rank += hash_map[id].rank / hash_map[id].out_cnt;
            }
            //random walk
            new_rank = new_rank * alpha + (1 - alpha) / page_cnt;
            delta = max(delta, fabs(new_rank - previous_rank));
            it->second.rank = new_rank;
        }
        cout << cnt << endl;
        cout << delta << endl;
    }
    vector<pair<string, double>> v;
    for(auto it = hash_map.begin(); it != hash_map.end(); ++it){
        v.push_back(pair<string, double>(it->second.term, it->second.rank));
    }

    sort(v.begin(), v.end(),
        [](pair<string, double>&a, pair<string, double>&b)->bool{
            return a.second > b.second;
        }
    );

    ofstream fout("result.txt");
    for(int i = 0; i < 10; ++i){
        cout << "top " << i + 1 << ": " << "pagerank = " << v[i].second << "\t";
        cout << v[i].first << endl;
    }

    for(auto page : v){
        fout << page.first << '\t' << page.second << endl;
    }
}

void pagerank_distribution(const char *FILENAME, const char * RESULTS){
    ifstream fin(FILENAME);
    ofstream fout(RESULTS);

    int cnt[14] = {};
    double constants[] = {1000, 100, 10, 1, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9};

    string content;
    while(getline(fin, content)){
        auto pos1 = content.find('\t');
        string s(content, pos1, content.size() - pos1);
        double rank = stod(s);
        if(rank > constants[0])
            cnt[0]++;
        else if(rank < constants[12]){
            cnt[13]++;
        }
        else{
            for(int i = 1; i < 13; ++i){
                if(rank > constants[i] && rank < constants[i - 1]){
                    cnt[i]++;
                }
            }
        }
    }

    fout << "rank > 1000:\t\t" << cnt[0] << endl;
    for(int i = 1; i < 13; ++i){
        fout << constants[i - 1] << " < rank < " << constants[i] << ":\t" << cnt[i] << endl; 
    }
    fout << "rank < 1e-9:\t\t" << cnt[13] << endl;
}
#endif