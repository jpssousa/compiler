#include "semantic.hpp"

Semantic::Semantic(string object_file, vector<rule_t> rules) {
    identation = 1;
    syntactic_rules = rules;
    object_file_path = object_file;
    tipo_aux = Tipo::NULO;

    object_file_text = "#include <stdio.h>\n\n";
    object_file_text += "typedef char literal[256];\n\n";
    object_file_text += "void main (void) {\n\t";
}

string Semantic::codeGen(string text) {
    string lib = "#include <stdio.h>\n\n";
    string tpdf = "typedef char literal[256];\n\n";
    string mainf = "void main (void) {\n\t";

    string endB = "\n}\n";
    return lib + tpdf + mainf + text + endB;
}

void Semantic::newLineObjectText() {
    object_file_text += "\n";
    for (int i = 0; i < identation; i++) {
        object_file_text += "\t";
    }
}

void Semantic::shiftToken(token_t tkn) {
    tkn.print();
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
        printf("localId: %s\n", tkn.lexema.c_str());
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
    updateSemanticSymbolTable(lex->symbol_table);
    vector<token_t> rhsTokens(rule.rhs_size);
    // for (int i = 0; i < rule.rhs_size; i++) {
    //     rhsTokens[i] = token_stack.top();
    //     token_stack.pop();
    // }
    token_stack.push({Classe::Erro, "reduction placeholder", Tipo::NULO});

    int rIdx = find(rule);
    bool flag = true;

    switch (rIdx) {
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
            printf("\n\n rule 5\n");
            printSymbolTable();
            return flag;



            // for (int i = 0; i < localIdList.size(); i++) {
            //     if (symbol_table.count(localIdList[i])) {

            //     }
            // }


            // printf("before\n");
            // printSymbolTable();
            // for (int i = 0; i < localIdList.size(); i++) {
            //     if (symbol_table.count(localIdList[i])) {
            //         if (querySymbolTable(localIdList[i]).tipo == Tipo::NULO) {
            //             updateEntrySymbolTable({Classe::Id, localIdList[i], tipo_aux});
            //             if (i != 0) object_file_text += ", ";
            //             object_file_text += localIdList[i];
            //         } else {
            //             printf("Erro semântico: linha %d, coluna %d\n", lex->line, lex->column);
            //             printf("Variável já declarada.\n");
            //             return false;
            //         }
            //     } else {
            //         printf("else\n");
            //     }
            // }
            // object_file_text += ";\n";
            // localIdList.clear();
            // printf("after\n");
            // printSymbolTable();


            // for (int i = 0; i < localIdList.size(); i++) {
            //     if (symbol_table.count(localIdList[i]) &&
            //     symbol_table[localIdList[i]].tipo != Tipo::NULO) {
            //         printf("Erro semântico: linha %d, coluna %d\n", lex->line, lex->column);
            //         printf("Variável já declarada.\n");
            //         return false;
            //     }
            // }
            // object_file_text += localIdList[0];
            // updateEntrySymbolTable({Classe::Id, localIdList[0], tipo_aux});
            // for (int i = 1; i < localIdList.size(); i++) {
            //     object_file_text += ", " + localIdList[i];
            //     updateEntrySymbolTable({Classe::Id, localIdList[i], tipo_aux});
            // }
            // object_file_text += ";\n";
            // localIdList.clear();
            break;

        case 6:
            // for (int i = 0; i < localIdList.size(); i++) {
            //     if (symbol_table.count(localIdList[i]) &&
            //     symbol_table[localIdList[i]].tipo != Tipo::NULO) {
            //         printf("Erro semântico: linha %d, coluna %d\n", lex->line, lex->column);
            //         printf("Variável já declarada.\n");
            //         return false;
            //     }
            // }
            break;
        case 7:
            // for (int i = 0; i < localIdList.size(); i++) {
            //     if (symbol_table.count(localIdList[i]) &&
            //     symbol_table[localIdList[i]].tipo != Tipo::NULO) {
            //         printf("Erro semântico: linha %d, coluna %d\n", lex->line, lex->column);
            //         printf("Variável já declarada.\n");
            //         return false;
            //     }
            // }
            break;
        case 8:
            object_file_text += "int ";
            break;
        case 9:
            object_file_text += "double ";
            break;
        case 10:
            object_file_text += "literal ";
            break;
        default:
            printf("No rule to be applied.\n");
    }
    return true;
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
    string text = codeGen("printf(\"Hello world.\\n\");");
    fp.write(text.c_str(), text.size());

    fp.close();
}

void Semantic::insertSymbolTable(token_t token) {
    if (token.classe != Classe::Erro) {
        symbol_table[token.lexema] = token;
    }
}

token_t Semantic::querySymbolTable(string key) {
    return symbol_table[key];
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
