#ifndef SYNTATIC_HPP
#define SYNTATIC_HPP

#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <cstdio>
#include <stack>

#include "../common.hpp"
#include "../lexical/lexical.hpp"

using namespace std;

enum Action {
    shift,
    reduce,
    accept,
    error
};

typedef struct {
    string rule;
    string alpha;
    vector<string> beta;
    int rhs_size;
} rule_t;

class Syntactic {
public:
    stack<int> st;
    int initalState, currState;
    vector<vector<pair<Action,int>>> actions;
    vector<map<string,int>> gotos;
    vector<rule_t> rules;
    // vector<pair<string, int>> rules;
    string file_path;

    Syntactic(string source_file);

    void analyze();

    vector<vector<pair<Action,int>>> buildActionTable();
    vector<map<string,int>> buildGotoTable();
    vector<rule_t> buildRules();
    // vector<pair<string, int>> buildRules();

    pair<Action, int> parseActionState(string s);
    vector<string> split (string s, string delimiter);
};

#endif
