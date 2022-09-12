#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP

#include <map>
#include <list>
#include <stack>
#include <vector>
#include <string>
#include <fstream>

#include "../lexical/lexical.hpp"
#include "../common.hpp"

using namespace std;

class Semantic {
public:
    // contador de identações para impressão em programa.c
    int identation;

    // texto do código objeto '.c'
    string object_file_text;

    // tipo auxiliar para atribuição de tipos na declaração de variáveis
    Tipo tipo_aux;

    // lista de variáveis declaradas em um contexto
    vector<string> localIdList;

    // nome do arquivo objeto 'programa.c'
    string object_file_path;

    // regras sintáticas
    vector <rule_t> syntactic_rules;

    // pilha de tokens lidos
    stack <token_t> token_stack;

    // token lookaead (abstração)
    token_t lookahead;

    // tabela de símbolos
    map<string, token_t> symbol_table;

    // mapa de identificadores declarados (tipo não pode ser alterado)
    map<string, bool> id_declared;

    // construtor
    Semantic(string object_file, vector<rule_t> rules);

    // empilha token lido
    void shiftToken(token_t);

    // aplica regra semântica
    bool applyRule(Lexical *lex, rule_t rule);

    string codeGen(string text);

    void newLineObjectText();

    void createObjectFile();

    // retorna índice da regra aplicada
    int find(rule_t rule);

    // adiciona novo token à tabela de símbolos (chave: lexema)
    void insertSymbolTable(token_t token);

    // busca token na tabela de símbolos (chave: lexema)
    token_t querySymbolTable(string key);

    // atualiza entrada na tabela de símbolos (alterar tipo do token)
    void updateEntrySymbolTable(token_t token);

    // exibe as entradas na tabela de símbolos
    void printSymbolTable();

    // atualização da tabela de símbolos do semântico
    void updateSemanticSymbolTable(map<string, token_t> st);
};

#endif