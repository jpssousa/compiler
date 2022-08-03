#include "lexical.hpp"


vector<char> Lexical::gerar_alfabeto() {
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

Lexical::Lexical(string file_path) {
    fp.open(file_path, fstream::in);
    line = 0;
    column = 0;
    alfabeto = gerar_alfabeto();
}

Lexical::~Lexical() {
    fp.close();
}

token_t Lexical::Scanner() {
    char c;
    if (fp.is_open()) {
        cout << "Arquivo aberto. Realizando leitura.\n\n";

        while ((c = fp.get()) && c != EOF) {
            cout << c;
        }
        cout << "\nFim de arquivo.\n";
    }
    else {
        cout << "Erro ao abrir o arquivo.\n";
    }
}
