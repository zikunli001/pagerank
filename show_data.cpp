#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

const char* PATH = "enwiki-20190920-pages-articles-multistream.xml";

int main(){
    ifstream fin(PATH);
    ofstream fout("4000_page_links.txt");
    if(fin.fail()){
        cout << "fin failed" << endl;
        exit(1);
    }
    char text[10000];
    int cnt = 4000;
    while(cnt--){
        memset(text, 0, sizeof(text));
        fin.getline(text, 10000);
        fout << (string)text << endl;
        if(fin.fail()){
            cout << "\n\n\nerror\n\n\n" << endl;
        }
    }
    return 0;
}