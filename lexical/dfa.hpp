#ifndef DFA_HPP
#define DFA_HPP

#include "../common.hpp"

enum DFA_State {
    Initial, // estado inicial aceita qualquer símbolo do alfabeto da linguagem
    Comment, // entra no estado após ler '{' e sai com '}'
    Num_1, // aceitação para D+ (inteiro ex: 12345)
    Num_2, // intermediário D+(E|e)
    Num_3, // intermediário D+(E|e)\+
    Num_4, // aceitação para D+(E|e)\+D+ (inteiro ex: 10e3)
    Num_5, // intermediário para D+.
    Num_6, // aceitação para D+.D+ (real ex: 12.34)
    Num_7, // intermediário para D+.D+(E|e)
    Num_8, // intermediário para D+.D+(E|e)(+|-) ou D+(E|e)-
    Num_9, // aceitação para real (1e-3) ou (1.2E-3) ou (1.2E+3)
    Id_state, // possível Id/palavra reservada, começa com letra
    Lit_state, // possível literal, começa com \"
    R_LesT, // Lido '<': <- | <= | <> | <
    R_GrtT, // lido '>': >= | >
    Accepted, // aceitaçao genérico
    Rejected // rejeição genérica
};


class DFA {
public:
    CTE cte; // constantes (alfabeto e subconjuntos)
    token_t token; // token em construção
    DFA_State current_state; // estado corrente do DFA

    // inicializa DFA para reconhecimento do próximo token
    DFA(CTE cte);

    // se o caracter pertence ao alfabeto
    bool isCharValid(char c);

    // se existe transição para o próximo
    // caracter a partir do estado corrente do DFA
    bool isPossibleTransition(char c);
    
    // realiza transição para o próximo caracter
    DFA_State transition(char c);
};

#endif