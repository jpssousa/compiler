#include "semantic.hpp"

Semantic::Semantic(string object_file, vector<rule_t> rules) {
    identation = 1;
    syntactic_rules = rules;
    object_file_path = object_file;
    tipo_aux = Tipo::NULO;

    object_file_text = "#include <stdio.h>\n\n";
    object_file_text += "typedef char literal[256];\n\n";
    object_file_text += "int main (void) {\n\t";
}

void Semantic::newLineObjectText() {
    object_file_text += "\n";
    for (int i = 0; i < identation; i++) {
        object_file_text += "\t";
    }
}

void Semantic::shiftToken(token_t tkn) {
    // tkn.print();
    token_stack.push(lookahead);
    lookahead = tkn;
    if (token_stack.top().classe == Classe::Inteiro) {
        tipo_aux = Tipo::INTEIRO;
    } else if (token_stack.top().classe == Classe::Real) {
        tipo_aux = Tipo::REAL;
    } else if (token_stack.top().classe == Classe::Literal) {
        tipo_aux = Tipo::LITERAL;
    }

    if (tkn.classe == Classe::Id) {
        localIdList.push_back(tkn.lexema);
        // printf("localId: %s\n", tkn.lexema.c_str());
        if (symbol_table.count(tkn.lexema)) {
            token_t t = querySymbolTable(tkn.lexema);
            if (t.tipo == Tipo::NULO && !id_declared[t.lexema]) {
                t.tipo = tipo_aux;
                updateEntrySymbolTable(t);
            }
        } else {
            tkn.tipo = tipo_aux;
            updateEntrySymbolTable(tkn);
        }
    }
}

bool Semantic::applyRule(Lexical *lex, rule_t rule) {
    vector<token_t> rhsTokens;
    token_t aux;
    updateSemanticSymbolTable(lex->symbol_table);
    int rIdx = find(rule);
    bool flag = true;

    switch (rIdx) {
    // D -> TIPO L ptv
    case 5:
        for (int i = 0; i < localIdList.size(); i++) {
            if (!id_declared[localIdList[i]]) {
                id_declared[localIdList[i]] = true;
                if (i != 0) object_file_text += ", ";
                object_file_text += localIdList[i];
            }
            else {
                printf("Erro semântico: linha %d, coluna %ld\n", lex->line, lex->column - lookahead.lexema.size());
                printf("Variável [%s] já declarada.\n", localIdList[i].c_str());
                flag = false;
                break;
            }
        }
        localIdList.clear();
        object_file_text += ";";
        newLineObjectText();
        tipo_aux = Tipo::NULO;
        return flag;
    
    // TIPO -> inteiro
    case 8:
        object_file_text += "int ";
    break;

    // TIPO -> real
    case 9:
        object_file_text += "double ";
    break;

    // TIPO -> literal
    case 10:
        object_file_text += "literal ";
    break;

    // ES -> leia id ptv
    case 12:
        rhsTokens = getRhsTokens(rule.rhs_size);
        aux = querySymbolTable(rhsTokens[1].lexema);
        if (aux.tipo != Tipo::NULO) {
            string sc = "scanf(\"%";
            switch(aux.tipo) {
            case Tipo::INTEIRO:
                sc += "d\", &";
            break;
            case Tipo::REAL:
                sc += "lf\", &";
            break;
            case Tipo::LITERAL:
                sc += "s\", ";
            break;
            default:
                flag = false;
            }
            object_file_text += sc + string(aux.lexema) + ");";
        }
        else {
            printf("Erro semântico: linha %d, coluna %ld\n", lex->line, lex->column - lookahead.lexema.size());
            printf("Variável [%s] não declarada.\n", rhsTokens[1].lexema.c_str());
            flag = false;
        }
        newLineObjectText();
    break;
    
    // ES -> escreva ARG ptv
    case 13:
        object_file_text += "printf(" + ARG + ");";
        newLineObjectText();
    break;

    // ARG -> lit
    case 14:
        rhsTokens = getRhsTokens(1);
        ARG = rhsTokens[0].lexema;
    break;

    // ARG -> num
    case 15:
        rhsTokens = getRhsTokens(1);
        ARG = rhsTokens[0].lexema;
    break;
    
    // ARG -> id
    case 16:
        rhsTokens = getRhsTokens(1);
        aux = querySymbolTable(rhsTokens[0].lexema);
        if (aux.tipo != Tipo::NULO) {
            switch(aux.tipo) {
            case Tipo::INTEIRO:
                ARG = "\"%d\", " + aux.lexema;
            break;
            case Tipo::REAL:
                ARG = "\"%lf\", " + aux.lexema;
            break;
            case Tipo::LITERAL:
                ARG = "\"%s\", " + aux.lexema;
            break;
            default:
                flag = false;
            }
        }
        else {
            printf("Erro semântico: linha %d, coluna %ld\n", lex->line, lex->column - lookahead.lexema.size());
            printf("Variável [%s] não declarada.\n", rhsTokens.back().lexema.c_str());
            flag = false;
        }
    break;

    // CMD -> id rcb LD ptv
    case 18:
        rhsTokens = getRhsTokens(3 + LD.size());
        aux = querySymbolTable(rhsTokens[0].lexema);
        if (aux.tipo != Tipo::NULO) {
            if (aux.tipo == LD[0].tipo || (aux.tipo == Tipo::REAL && LD[0].tipo == Tipo::INTEIRO)) {
                object_file_text += aux.lexema + " =";
                for (int i = 0; i < LD.size(); i++) {
                    if (LD[i].lexema == "<>") {
                        object_file_text += " !=";
                    } else {
                        object_file_text += " " + LD[i].lexema;
                    }
                }
                object_file_text += ";";
                newLineObjectText();
            } else {
                printf("Erro semântico: linha %d, coluna %ld\n", lex->line, lex->column - lookahead.lexema.size());
                printf("Tipos diferentes para atribuição.\n");
                flag = false;
            }
        }
        else {
            printf("Erro semântico: linha %d, coluna %ld\n", lex->line, lex->column - lookahead.lexema.size());
            printf("Variável [%s] não declarada.\n", rhsTokens[0].lexema.c_str());
            flag = false;
        }
        LD.clear();
    break;

    // LD -> OPRD opm OPRD
    case 19:
        rhsTokens = getRhsTokens(3);
        if (rhsTokens[0].tipo == rhsTokens[2].tipo &&
        rhsTokens[0].tipo != Tipo::LITERAL) {
            LD = rhsTokens;
        } else {
            printf("Erro semântico: linha %d, coluna %ld\n", lex->line, lex->column - lookahead.lexema.size());
            printf("Operando de tipos incompatíveis.\n");
            flag = false;
        }
        OPRD.clear();
    break;

    // LD -> OPRD
    case 20:
        LD = OPRD;
        OPRD.clear();
        // LD[0].print();
    break;

    // OPRD -> id
    case 21:
        rhsTokens = getRhsTokens(1);
        aux = querySymbolTable(rhsTokens[0].lexema);
        if (aux.tipo != Tipo::NULO) {
            OPRD.push_back(rhsTokens[0]);
        }
        else {
            printf("Erro semântico: linha %d, coluna %ld\n", lex->line, lex->column - lookahead.lexema.size());
            printf("Variável [%s] não declarada.\n", rhsTokens.back().lexema.c_str());
            flag = false;
        }
    break;

    // OPRD -> num
    case 22:
        rhsTokens = getRhsTokens(1);
        // rhsTokens[0].print();
        OPRD.push_back(rhsTokens[0]);
    break;

    // COND -> CAB CP
    case 24:
        identation--;
        newLineObjectText();
        object_file_text += "}";
        newLineObjectText();
    break;
    

    // CAB -> se abp EXP_R fcp entao
    case 25:
        object_file_text += "if (";
        for (int i = 0; i < EXP_R.size(); i++) {
            if (EXP_R[i].lexema == "=") {
                object_file_text += " ==";
            }
            else {
                object_file_text += " " + EXP_R[i].lexema;
            }
        }
        object_file_text += " ) {";
        identation++;
        newLineObjectText();
        EXP_R.clear();
    break;


    // EXP_R -> OPRD opr OPRD
    case 26:
        if (OPRD.size() == 2) {
            if (OPRD[0].tipo == OPRD[1].tipo) {
                rhsTokens = getRhsTokens(rule.rhs_size);
                EXP_R = rhsTokens;
                OPRD.clear();
            }
            else {
                printf("Erro semântico: linha %d, coluna %ld\n", lex->line, lex->column - lookahead.lexema.size());
                printf("Operando de tipos incompatíveis.\n");
                flag = false;
            }
        }
    break;
    
    // R -> CABR CPR
    case 32:
        identation--;
        newLineObjectText();
        object_file_text += "}";
        newLineObjectText();
    break;

    // CABR -> repita abp EXP_R fcp entao
    case 33:
        object_file_text += "while (";
        for (int i = 0; i < EXP_R.size(); i++) {
            if (EXP_R[i].lexema == "=") {
                object_file_text += " ==";
            }
            else {
                object_file_text += " " + EXP_R[i].lexema;
            }
        }
        object_file_text += " ) {";
        identation++;
        newLineObjectText();
        EXP_R.clear();
    break;

    case 38:
        newLineObjectText();
        object_file_text += "return 0;";
        identation--;
        newLineObjectText();
        object_file_text += "}\n";
    break;

    default:
        return flag;
        // printf("No rule to be applied.\n");
    }
    return true;
}

vector<token_t> Semantic::getRhsTokens(int n) {
    vector<token_t> rhsTokens(n);
    for (int i = n-1; i >= 0; i--) {
        token_t t = token_stack.top();
        if (t.classe == Classe::Id) {
            rhsTokens[i] = querySymbolTable(t.lexema);
        }
        else {
            rhsTokens[i] = t;
        }

        token_stack.pop();
    }

    for (int i = 0; i < n; i++) {
        token_stack.push(rhsTokens[i]);
    }

    return rhsTokens;
}


int Semantic::find(rule_t rule) {
    for (int i = 0; i < syntactic_rules.size(); i++) {
        if (syntactic_rules[i] == rule) return i;
    }
    return -1;
}

void Semantic::createObjectFile() {
    fstream fp;
    fp.open(object_file_path, fstream::out);
    fp.write(object_file_text.c_str(), object_file_text.size());
    // string text = codeGen("printf(\"Hello world.\\n\");");
    // fp.write(text.c_str(), text.size());

    fp.close();
}

void Semantic::insertSymbolTable(token_t token) {
    if (token.classe != Classe::Erro) {
        symbol_table[token.lexema] = token;
    }
}

token_t Semantic::querySymbolTable(string key) {
    if (symbol_table.count(key)) {
        return symbol_table[key];
    }
    else {
        return {Classe::Id, key, Tipo::NULO};
    }
}

void Semantic::updateEntrySymbolTable(token_t token) {
    insertSymbolTable(token);
    // symbol_table[token.lexema] = token;
}

void Semantic::printSymbolTable() {
    printf("\n\t Tabela de símbolos:\n");
    for (pair<string, token_t> entry: symbol_table) {
        printf("key[%s]: ", entry.first.c_str());
        entry.second.print();
    }
}

void Semantic::updateSemanticSymbolTable(map<string, token_t> st) {
    for (pair<string, token_t> entry: st) {
        if (entry.second.classe != Classe::Id) {
            symbol_table[entry.first] = entry.second;
        } else {
            if (symbol_table.count(entry.first) == 0) {
                symbol_table[entry.first] = entry.second;
            }
        }
    }
}
