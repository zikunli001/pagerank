#include "parse.h"
#include "pagerank.h"

using namespace std;

int main(){
    int cnt = parse_id("enwiki-20190920-pages-articles-multistream.xml");

    /*
     * sampling 1000000 pages
     */
    sampling("title_id.txt", cnt);
    parse_links("sample_title_id.txt");
    page_rank("links_info.txt", sample_num); //sample_num = 1000000,defined in parse.h
    pagerank_random_walk("sample_links_info.txt", sample_num);

    /*
     * all pages
     */
    parse_links("title_id.txt");
    page_rank("links_info.txt", cnt);
    pagerank_random_walk("links_info.txt", cnt);

    pagerank_distribution("global_results.txt","global_rank_distribution.txt");
    pagerank_distribution("sample_results.txt", "sample_rank_distribution.txt");
}