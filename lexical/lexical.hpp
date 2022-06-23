#include<string>
#include<vector>

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
    INTEIRO,
    REAL,
    LITERAL,
    NULO
};

typedef struct token {
    Classe classe;
    string lexema;
    Tipo tipo;
} token_t;

class Lexical {

};