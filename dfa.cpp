#include "dfa.hpp"

DFA::DFA(CTE cte) {
    (*this).cte = cte;
    token = {Classe::Erro, string(""), Tipo::NULO};
    current_state = DFA_State::Initial;
}

bool DFA::isCharValid(char c) {
    return cte.alphabet.count(c);
}

bool DFA::isPossibleTransition(char c) {
    // printf("state: %d isPossibleTransition: %c: ", (int)current_state, c);
    if (!isCharValid(c)) {
        return false;
    }
    switch (current_state) {
        case DFA_State::Initial: return true;
        case DFA_State::Comment: return true;
        case DFA_State::Num_1:
            return (cte.digits.count(c) || c == 'e' || c == 'E' || c == '.');
        case DFA_State::Num_2:
            return (cte.digits.count(c) || c == '+' || c == '-');
        case DFA_State::Num_3:
            return (cte.digits.count(c));
        case DFA_State::Num_4:
            return (cte.digits.count(c));
        case DFA_State::Num_5:
            return (cte.digits.count(c));
        case DFA_State::Num_6:
            return (cte.digits.count(c) || c == 'e' || c == 'E');
        case DFA_State::Num_7:
            return (cte.digits.count(c) || c == '+' || c == '-');
        case DFA_State::Num_8:
            return (cte.digits.count(c));
        case DFA_State::Num_9:
            return (cte.digits.count(c));
        case DFA_State::Id_state:
            return (cte.letters.count(c) || cte.digits.count(c) || c == '_');
        case DFA_State::Lit_state: return true;
        case DFA_State::R_LesT: return (c == '-' || c == '=' || c == '>');
        case DFA_State::R_GrtT: return (c == '=');
        default:
            return false;
    }
}

DFA_State DFA::transition(char c) {
    // printf("estado: %d\n", (int)current_state);
    // token.print();
    if (current_state == DFA_State::Initial) {
        if (cte.whitespace.count(c)) return DFA_State::Initial;
        if (c == '{') return DFA_State::Comment;
        if (c == EOF) {
            token.classe = Classe::Eof;
            token.lexema = string("EOF");
            return DFA_State::Accepted;
        }
    }
    else if (current_state == DFA_State::Comment) {
        if (c == '}') return DFA_State::Initial;
        return DFA_State::Comment;
    }

    token.lexema += c;
    // printf("lexema: %s\n", token.lexema.c_str());
    switch (current_state) {
    case DFA_State::Initial:
        if (cte.digits.count(c)) return DFA_State::Num_1;
        else if (cte.letters.count(c)) {
            token.classe = Classe::Id;
            return DFA_State::Id_state;
        }
        else if (cte.opm.count(c)) {
            token.classe = Classe::Opm;
            return DFA_State::Accepted;
        }
        else if (c == '\"') {
            token.classe = Classe::Lit;
            return DFA_State::Lit_state;
        }
        else if (c == '<') {
            token.classe = Classe::Opr;
            return DFA_State::R_LesT;
        }
        else if (c == '>') {
            token.classe = Classe::Opr;
            return DFA_State::R_GrtT;
        }
        else if (c == '=') {
            token.classe = Classe::Opr;
            return DFA_State::Accepted;
        }
        else if (c == '(') {
            token.classe = Classe::Ab_p;
            return DFA_State::Accepted;
        }
        else if (c == ')') {
            token.classe = Classe::Fc_p;
            return DFA_State::Accepted;
        }
        else if (c == ';') {
            token.classe = Classe::Pt_v;
            return DFA_State::Accepted;
        }
        else if (c == ',') {
            token.classe = Classe::Vir;
            return DFA_State::Accepted;
        }
        return DFA_State::Rejected;

    case DFA_State::Comment:
        if (c == '}') return DFA_State::Initial;
        return DFA_State::Comment;

    case DFA_State::Num_1:
        token.classe = Classe::Num;
        token.tipo = Tipo::INTEIRO;
        if (cte.digits.count(c)) return DFA_State::Num_1;
        if (c == 'E' || c == 'e') return DFA_State::Num_2;
        if (c == '.') {
            token.classe = Classe::Erro;
            token.tipo = Tipo::NULO;
            return DFA_State::Num_5;
        }
        return DFA_State::Rejected;

    case DFA_State::Num_2:
        token.classe = Classe::Erro;
        token.tipo = Tipo::NULO;
        if (cte.digits.count(c)) return DFA_State::Num_4;
        if (c == '+') return DFA_State::Num_3;
        if (c == '-') return DFA_State::Num_8;
        return DFA_State::Rejected;

    case DFA_State::Num_3:
        token.classe = Classe::Erro;
        token.tipo = Tipo::NULO;
        if (cte.digits.count(c)) return DFA_State::Num_4;
        return DFA_State::Rejected;

    case DFA_State::Num_4:
        token.classe = Classe::Inteiro;
        token.tipo = Tipo::INTEIRO;
        if (cte.digits.count(c)) return DFA_State::Num_4;
        return DFA_State::Rejected;
    
    case DFA_State::Num_5:
        token.classe = Classe::Erro;
        token.tipo = Tipo::NULO;
        if (cte.digits.count(c)) return DFA_State::Num_6;
        return DFA_State::Rejected;

    case DFA_State::Num_6:
        token.classe = Classe::Num;
        token.tipo = Tipo::REAL;
        if (cte.digits.count(c)) return DFA_State::Num_6;
        if (c == 'E' || c == 'e') return DFA_State::Num_7;
        return DFA_State::Rejected;

    case DFA_State::Num_7:
        token.classe = Classe::Erro;
        token.tipo = Tipo::NULO;
        if (cte.digits.count(c)) return DFA_State::Num_9;
        if (c == '+' || c == '-') return DFA_State::Num_8;
        return DFA_State::Rejected;

    case DFA_State::Num_8:
        token.classe = Classe::Erro;
        token.tipo = Tipo::NULO;
        if (cte.digits.count(c)) return DFA_State::Num_9;
        return DFA_State::Rejected;

    case DFA_State::Num_9:
        token.classe = Classe::Num;
        token.tipo = Tipo::REAL;
        if (cte.digits.count(c)) return DFA_State::Num_9;
        return DFA_State::Rejected;

    case DFA_State::Id_state:
        token.classe = Classe::Id;
        token.tipo = Tipo::NULO;
        if (cte.letters.count(c) || c == '_') return DFA_State::Id_state;
        return DFA_State::Rejected;

    case DFA_State::Lit_state:
        token.classe = Classe::Lit;
        if (c == '\"') return DFA_State::Accepted;
        return DFA_State::Lit_state;

    case DFA_State::R_LesT:
        token.classe = Classe::Opr;
        token.tipo = Tipo::NULO;
        if (c == '=' || c == '>') return DFA_State::Accepted;
        if (c == '-') {
            token.classe = Classe::Rcb;
            return DFA_State::Accepted;
        }
        return DFA_State::Rejected;

    case DFA_State::R_GrtT:
        token.classe = Classe::Opr;
        token.tipo = Tipo::NULO;
        if (c == '=') return DFA_State::Accepted;
        return DFA_State::Rejected;
    default:
        return DFA_State::Rejected;
    }
}
