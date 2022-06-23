#include "lexical.hpp"

vector<char> gerar_alfabeto() {
    vector<char> alfabeto;
    vector<char> misc = {',',';',':','.','!','?','\\','*','+','-','/','(',')','{','}','[',']','<','>','=','\'','\"'};
    for (char c = '0'; c <= '9'; c++) {
        alfabeto.push_back(c);
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        alfabeto.push_back(c);
    }
    for (char c = 'a'; c <= 'z'; c++) {
        alfabeto.push_back(c);
    }
    alfabeto.insert(alfabeto.end(), misc.begin(), misc.end());
    return alfabeto;
}