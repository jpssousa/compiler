#ifndef SYNTACTIC_HPP
#define SYNTACTIC_HPP

#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <cstdio>
#include <stack>
#include <set>

#include "../common.hpp"
#include "../lexical/lexical.hpp"
#include "../semantic/semantic.hpp"

using namespace std;

enum Action {
    shift,
    reduce,
    accept,
    error
};

class Syntactic {
public:
    stack<int> st;
    int initalState, currState;
    vector<vector<pair<Action,int>>> actions;
    vector<map<string,int>> gotos;
    vector<rule_t> rules;
    // vector<pair<string, int>> rules;
    string source_file_path;
    string object_file_path;
    // Lexical *lex;
    // Semantic *sem;

    Syntactic(string source_file, string output_file);

    void analyze();

    vector<vector<pair<Action,int>>> buildActionTable();
    vector<map<string,int>> buildGotoTable();
    vector<rule_t> buildRules();
    // vector<pair<string, int>> buildRules();

    pair<Action, int> parseActionState(string s);
    vector<string> split (string s, string delimiter);
};

#endif
