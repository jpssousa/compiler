#ifndef LEXICAL_HPP
#define LEXICAL_HPP

#include<string>
#include<vector>
#include<fstream>
#include<iostream>

using namespace std;

enum Classe {
    INICIO,
    VARINICIO,
    VARFIM,
    ESCREVA,
    LEIA,
    SE,
    ENTAO,
    FIMSE,
    REPITA,
    FIMREPITA,
    FIM,
    INTEIRO,
    LITERAL,
    REAL,
    NUM,
    LIT,
    ID,
    EOF_T,
    OPR,
    RCB,
    OPM,
    AB_P,
    FC_P,
    PT_V,
    VIR,
    ERRO
};

enum Tipo {
    INTEIRO_T,
    REAL_T,
    LITERAL_T,
    NULO_T
};

typedef struct token {
    Classe classe;
    string lexema;
    Tipo tipo;
} token_t;

class Lexical {
public:
    fstream fp;
    int line;
    int column;
    vector<char> alfabeto;

    Lexical(string file_path);

    ~Lexical();

    token_t Scanner();

    vector<char> gerar_alfabeto();
};

#endif