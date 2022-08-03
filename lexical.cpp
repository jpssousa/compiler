#include "lexical.hpp"


void Lexical::initializeReservedWords() {
    for (int i = Classe::Inicio; i <= Classe::Fim; i++) {
        palavras_reservadas.push_back({(Classe)i,classeToStr((Classe) i),(Tipo)i});
    }
    palavras_reservadas.push_back({Classe::Inteiro,"inteiro",Tipo::NULO});
    palavras_reservadas.push_back({Classe::Real,"real",Tipo::NULO});
    palavras_reservadas.push_back({Classe::Literal,"literal",Tipo::NULO});
}

void Lexical::initializeSymbolTable() {
    for (int i = 0; i < palavras_reservadas.size(); i++) {
        insertSymbolTable(palavras_reservadas[i]);
    }
}

void Lexical::insertSymbolTable(token_t token) {
    if (token.classe != Classe::Erro) {
        symbol_table[token.lexema] = token;
    }
}

token_t Lexical::querySymbolTable(string key) {
    return symbol_table[key];
}

void Lexical::updateEntrySymbolTable(token_t token) {
    insertSymbolTable(token);
}

Lexical::Lexical(string file_path) {
    cte = CTE();
    initializeReservedWords();
    initializeSymbolTable();

    fp.open(file_path, fstream::in);
    line = 1;
    column = 1;
}

Lexical::~Lexical() {
    fp.close();
}

token_t Lexical::scanner() {
    DFA dfa(cte);
    token_t token;
    char c = fp.peek();

    if (c != EOF) {
        bool tst = dfa.isPossibleTransition(c);
        while ((tst && dfa.current_state != DFA_State::Accepted) && c != EOF) {
            c = fp.get();
            column++;
            if (c == '\n') {
                column = 1;
                line++;
            }
            dfa.current_state = dfa.transition(c);
            c = fp.peek();
            if (dfa.current_state == DFA_State::Accepted) {
                token = dfa.token;
                return token;
            }
            tst = dfa.isPossibleTransition(c);
        }
        switch (dfa.current_state) {
            case DFA_State::Initial:
                if (!dfa.isCharValid(c)) {
                    printf("Linha: %d, coluna: %d -> ", line, column);
                    printf("Erro léxico (caracter inválido).\n");
                    token.classe = Classe::Erro;
                    token.tipo = Tipo::NULO;
                    token.lexema += fp.get();
                    column++;
                    return token;
                }
                break;

            case DFA_State::Id_state:
                for (token_t pr: palavras_reservadas) {
                    if (dfa.token.lexema == pr.lexema) {
                        return querySymbolTable(dfa.token.lexema);
                    }
                }
                if (symbol_table.count(dfa.token.lexema)) {
                    updateEntrySymbolTable(dfa.token);
                    return querySymbolTable(dfa.token.lexema);
                }
                insertSymbolTable(dfa.token);
                return querySymbolTable(dfa.token.lexema);

            case DFA_State::Num_1:
            case DFA_State::Num_4:
                token.lexema = dfa.token.lexema;
                token.classe = Classe::Num;
                token.tipo = Tipo::INTEIRO;
                return token;

            case DFA_State::Num_6:
            case DFA_State::Num_9:
                token.lexema = dfa.token.lexema;
                token.classe = Classe::Num;
                token.tipo = Tipo::REAL;
                return token;

            default:
                token = dfa.token;
                if (token.lexema == "") return {Classe::Eof, string("EOF"), Tipo::NULO};
                return token;
        }
    } else {
        token = {Classe::Eof, string("EOF"), Tipo::NULO};
        return token;
    }
    token = dfa.token;
    if (token.lexema == "") return {Classe::Eof, string("EOF"), Tipo::NULO};
    return token;
}

void Lexical::printSymbolTable() {
    printf("\n\t Tabela de símbolos:\n");
    for (pair<string, token_t> entry: symbol_table) {
        printf("key[%s]: ", entry.first.c_str());
        entry.second.print();
    }
}
