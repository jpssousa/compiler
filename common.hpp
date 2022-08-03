#ifndef COMMON_HPP
#define COMMON_HPP

#include <set>
#include <string>

using namespace std;

enum Classe {
    Inicio,
    VarInicio,
    VarFim,
    Escreva,
    Leia,
    Se,
    Entao,
    FimSe,
    Repita,
    FimRepita,
    Fim,
    Inteiro,
    Real,
    Literal,
    Num,
    Lit,
    Id,
    Opr,
    Rcb,
    Opm,
    Ab_p,
    Fc_p,
    Pt_v,
    Vir,
    Erro,
    Eof
};

enum Tipo {
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
    REAL,
    LITERAL,
    NULO,
    EoF,
};

// função para mapear Enum Classe para string (saída)
char const * classeToStr(Classe cl);

// função para mapear Enum Tipo para string (saída)
char const * tipoToStr(Tipo tp);

// estrutura token
typedef struct token {
    Classe classe;
    string lexema;
    Tipo tipo;
    void print(); // imprimir token em formato string na tela
} token_t;

class CTE {
public:
    CTE();
    set<char> whitespace; // espaço, \n e \t
    set<char> digits; // dígitos
    set<char> letters; // letras maiúsculas e minúsculas
    set<char> misc; // outros caracteres
    set<char> opm; // operadores matemáticos
    set<char> alphabet; // união dos subconjuntos + EOF
};

#endif
