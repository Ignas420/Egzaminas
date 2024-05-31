#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>

using namespace std;

string salintiSkyryba(const string& word);
vector<string> Atrinkimas(const string& eil);

int main(){
    string eilute;
    vector<string> eilutes;
    unordered_map<string, int> zodzio_kiekis;
    unordered_map<string, vector<int>> zodzio_pozicija;
    ifstream fd("text.txt");

// Failo skaitymas
    if(!fd.is_open()){
        throw runtime_error("Skaitymo failas neatidaromas!");
    }

    int eil_nr=0;

    while(getline(fd, eilute)){
        eilutes.push_back(eilute);
        istringstream iss(eilute);
        string zodis;
        while(iss >> zodis){
            string newEil = salintiSkyryba(zodis);
            if(!newEil.empty()){
                zodzio_kiekis[newEil]++;
                zodzio_pozicija[newEil].push_back(eil_nr);
            }
        }
        eil_nr++;
    }
    fd.close();

// Pasikartojusiu zodziu isvedimas
    ofstream fr("pasikartojimai.txt");
    if(!fr.is_open()){
        throw runtime_error("Isvedimo failas neatidaromas!");
    }

    fr << "Zodziai, kurie pasikartoja daugiau nei viena karta: " << endl;
    for(auto i : zodzio_kiekis){
        if(i.second > 1){
            fr << i.first << ": " << i.second << endl;
        }
    }
    fr.close();

// Cross-reference lenteles isvedimas
    ofstream fr1("lentele.txt");
    if(!fr1.is_open()){
        throw runtime_error("Isvedimo failas neatidaromas!");
    }

    fr1 << "Cross-reference lentele: " << endl;
    for(auto i : zodzio_pozicija){
        if(i.second.size() > 1){
            fr1 << i.first << ": ";
            for(int pozicija : i.second){
                fr1 << pozicija << " ";
            }
            fr1 << endl;
        }
    }
    fr1.close();

// URL atrinkimas
    ofstream fr2("URL.txt");
    if(!fr2.is_open()){
        throw runtime_error("Isvedimo failas neatidaromas!");
    }

    fr2 << "Atrinkti URL: " << endl;
    for(string eil : eilutes){
        vector<string> URL = Atrinkimas(eil);
        for(string url : URL){
            fr2 << url << endl;
        }
    }
    fr2.close();

    return 0;
}
string salintiSkyryba(const string& word) {
    string newEil;
    for (char c : word) {
        if (isalpha(c) || isdigit(c)) {
            newEil += tolower(c);
        }
    }
    return newEil;
}
vector<string> Atrinkimas(const string& eil){
    vector<string> urls;
    size_t pozicija = 0;

    while(pozicija < eil.size()){
        size_t pradzia = eil.find("http://", pozicija);
        if(pradzia == string::npos){
            pradzia = eil.find("http://", pozicija);
        }
        if(pradzia == string::npos){
            pradzia = eil.find("www.", pozicija);
        }
        if(pradzia == string::npos){
            break;
        }
        
        size_t pabaiga = eil.find_first_of(" \t\n", pradzia);
        if(pabaiga == string::npos){
            pabaiga = eil.size();
        }

        string url = eil.substr(pradzia, pabaiga - pradzia);
        urls.push_back(url);

        pozicija = pabaiga;
    }

    return urls;
}