#ifndef LEXICAL_HPP
#define LEXICAL_HPP

#include <set>
#include <map>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "../common.hpp"
#include "dfa.hpp"

using namespace std;


class Lexical {
public:
    CTE cte; // constantes (alfabeto e subconjuntos)
    vector<token_t> palavras_reservadas; // palavras reservadas da linguagem
    map<string,token_t> symbol_table; // tabela de símbolos

    int line; // contagem de linhas do arquivo
    int column; // contagem de colunas por linha do arquivo

    fstream fp; // ponteiro para leitura do arquivo fonte

    // inicializa analizador léxico
    // constrói alfabeto, tabela de símbolos e palavras reservadas
    // abre o arquivo fonte para leitura e inicializa a contagem de linhas e colunas
    Lexical(string file_path);

    // destrutor analizador léxico
    // fecha arquivo fonte
    ~Lexical();

    // inicializa as palavras reservadas da linguagem
    void initializeReservedWords();

    // adiciona as palavras reservadas na tabela de símbolos
    void initializeSymbolTable();

    // adiciona novo token à tabela de símbolos (chave: lexema)
    void insertSymbolTable(token_t token);

    // busca token na tabela de símbolos (chave: lexema)
    token_t querySymbolTable(string key);

    // atualiza entrada na tabela de símbolos (alterar tipo do token)
    void updateEntrySymbolTable(token_t token);

    // constrói e retorna o próximo token do arquivo
    token_t scanner();

    // exibe as entradas na tabela de símbolos
    void printSymbolTable();
};

#endif