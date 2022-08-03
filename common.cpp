#include "common.hpp"

char const *classes_str[] = {
    "inicio",
    "varinicio",
    "varfim",
    "escreva",
    "leia",
    "se",
    "entao",
    "fimse",
    "repita",
    "fimrepita",
    "fim",
    "inteiro",
    "real",
    "literal",
    "NUM",
    "LIT",
    "id",
    "OPR",
    "RCB",
    "OPM",
    "AB_P",
    "FC_P",
    "PT_V",
    "VIR",
    "ERRO",
    "EOF"
};

char const *tipos_str[] = {
    "inicio",
    "varinicio",
    "varfim",
    "escreva",
    "leia",
    "se",
    "entao",
    "fimse",
    "repita",
    "fimrepita",
    "fim",
    "inteiro",
    "real",
    "literal",
    "NULO",
    "EOF"
};

char const * classeToStr(Classe cl) {
    return classes_str[(int) cl];
}

char const * tipoToStr(Tipo tp) {
    return tipos_str[(int) tp];
}

void token::print() {
    printf("Classe: %s | Lexema: %s | Tipo: %s\n",
        classeToStr(classe), (*this).lexema.c_str(), tipoToStr(tipo));
}

CTE::CTE() {
    alphabet = {EOF};
    whitespace = {' ','\t','\n'};
    for (char c = '0'; c <= '9'; c++) {
        digits.insert(c);
    }
    for (char c = 'A', d = 'a'; c <= 'Z'; c++, d++) {
        letters.insert(c);
        letters.insert(d);
    }
    misc = {',',';',':','.','!','?','\\','(',')','[',']','{','}','<','>','=','\'','\"',};
    opm = {'+','-','*','/'};

    alphabet.insert(whitespace.begin(), whitespace.end());
    alphabet.insert(digits.begin(), digits.end());
    alphabet.insert(letters.begin(), letters.end());
    alphabet.insert(misc.begin(), misc.end());
    alphabet.insert(opm.begin(), opm.end());
}
