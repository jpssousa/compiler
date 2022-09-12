#include "syntactic.hpp"

string ACTIONS_FILE_NAME = "./files/actions.csv";
string GOTO_FILE_NAME = "./files/gotos.csv";
string RULES_FILE_NAME = "./files/rules.txt";

Syntactic::Syntactic(string source_file, string output_file) {
    initalState = 0;
    st.push(initalState);
    source_file_path = source_file;
    object_file_path = output_file;
    actions = buildActionTable();
    gotos = buildGotoTable();
    rules = buildRules();
}

vector<vector<pair<Action,int>>> Syntactic::buildActionTable() {
    vector<vector<pair<Action,int>>> actions;
    int line_count = 77;
    ifstream fp;
    fp.open(ACTIONS_FILE_NAME);
    string line;

    for (int i = 0 ; i < line_count; i++) {
        string del = ",";
        getline(fp, line);
        vector<string> words = split(line, del);
        vector<pair<Action,int>> state_actions;

        // printf("S %2d:", i);
        for (int j = 1; j < words.size(); j++) {
            state_actions.push_back(parseActionState(words[j]));
            // printf("%2d ", state_actions[j-1].second);
        }
        // printf("\n");
        actions.push_back(state_actions);
    }

    fp.close();
    return actions;
}

vector<map<string,int>> Syntactic::buildGotoTable() {
    vector<map<string,int>> gotos;
    int line_count = 78;
    ifstream fp;
    fp.open(GOTO_FILE_NAME);
    string line, del = ",";
    vector<string> variables;

    getline(fp, line);
    variables = split(line, del);
    variables.back().pop_back();

    for (int i = 1; i < line_count; i++) {
        // printf("S %2d:", i-1);
        getline(fp, line);
        vector<string> words = split(line, del);
        map<string,int> state_gotos;

        for (int j = 1; j < words.size(); j++) {
            int state;
            sscanf(words[j].c_str(), "%d", &state);
            // printf(" (%s %2d) ", variables[j].c_str(), state);
            state_gotos[variables[j]] = state;
            // state_gotos.push_back(make_pair(variables[j], state));
        }
        gotos.push_back(state_gotos);
        // printf("\n");
    }
    // printf("S 42 [CPR]: %d\n", gotos[42]["CPR"]);

    return gotos;
}

vector<rule_t> Syntactic::buildRules() {
    vector<rule_t> rules;
    int line_count = 39;
    ifstream fp;
    fp.open(RULES_FILE_NAME);
    string line, del = " -> ";

    for (int i = 0; i < line_count; i++) {
        rule_t rule;
        vector<string> words;
        getline(fp, line);
        words = split(line, del);

        rule.rule = line;
        // printf("%s\n", rule.rule.c_str());
        rule.alpha = words[0];
        rule.beta = split(words[1], " ");
        rule.rhs_size = rule.beta.size();
        // printf("alpha: %s | rhs_size: %d\n", rule.alpha.c_str(), rule.rhs_size);
        rules.push_back(rule);
    }
    fp.close();

    return rules;
}

pair<Action, int> Syntactic::parseActionState(string s) {
    pair<Action, int> action_state;
    int state = -1;
    char c;
    switch (s[0]) {
        case 's':
            // printf("sht");
            sscanf(s.c_str(), "%c%d", &c, &state);
            action_state = make_pair(Action::shift, state);
            break;
        case 'r':
            // printf("red");
            sscanf(s.c_str(), "%c%d", &c, &state);
            action_state = make_pair(Action::reduce, state);
            break;
        case 'a':
            // printf("acc");
            action_state = make_pair(Action::accept, state);
            break;
        case 'e':
            // printf("err");
            action_state = make_pair(Action::error, state);
            break;
        default:
            action_state = make_pair(Action::error, state);
    }
    return action_state;
}

vector<string> Syntactic::split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


void Syntactic::analyze() {
    bool found_errors = false;
    stack<token_t> error_stack;

    Lexical lex(source_file_path);
    Semantic sem(object_file_path, rules);
    token tkn;
    // token tkn = {Classe::Id, "A", Tipo::LITERAL};
    // lex.updateEntrySymbolTable(tkn);
    // sem.createObjectFile();

    tkn = lex.scanner();
    sem.lookahead = tkn;
    // sem.shiftToken(tkn);
    // tkn.print();
    while (1) {
        currState = st.top();
        // printf("curState: %d ", currState);

        pair<Action, int> currAction = actions[currState][tkn.classe];
        if (currAction.first == Action::shift) {
            st.push(currAction.second);
            if (error_stack.empty()) {
                do {
                    tkn = lex.scanner();
                    if (tkn.classe == Classe::Erro) {
                        found_errors = true;
                    }
                } while (tkn.classe == Classe::Erro);
                sem.shiftToken(tkn);
            } else {
                tkn = error_stack.top();
                error_stack.pop();
            }
            // tkn.print();
            continue;
        }
        else if (currAction.first == Action::reduce) {
            printf("%s\n", rules[currAction.second].rule.c_str());
            for (int i = 0; i < rules[currAction.second].rhs_size; i++) {
                st.pop();
            }
            int t = st.top();
            st.push(gotos[t][rules[currAction.second].alpha]);
            if (!sem.applyRule(&lex, rules[currAction.second])) {
                found_errors = true;
            }
        }
        else if (currAction.first == Action::accept) {
            printf("\nFim de análise.\n");
            break;
        } else {
            found_errors = true;
            printf("Erro sintático: linha %d, coluna %ld.\n",
                lex.line, (long long)(lex.column - tkn.lexema.size()) < 0 ? 0 : lex.column - tkn.lexema.size());
            printf("Token lido: ");
            tkn.print();
            printf("Token(s) esperado(s):");
            set<Classe> tokenToRead;
            for (int i = Classe::Inicio; i <= Classe::Eof; i++) {
                if (actions[currState][i].first == Action::shift ||
                    actions[currState][i].first == Action::reduce) {
                    tokenToRead.insert((Classe) i);
                    printf(" [%s]", classeToStr((Classe) i));
                }
            }
            printf("\n");

            if (tokenToRead.count(Classe::Opr)) {
                error_stack.push(tkn);
                tkn = {Classe::Opr, "", Tipo::NULO};
            } else if (tokenToRead.count(Classe::Opm)) {
                error_stack.push(tkn);
                tkn = {Classe::Opm, "", Tipo::NULO};
            } else if (tokenToRead.count(Classe::Rcb)) {
                error_stack.push(tkn);
                tkn = {Classe::Rcb, "<-", Tipo::NULO};
            } else if (tokenToRead.count(Classe::Pt_v)) {
                error_stack.push(tkn);
                tkn = {Classe::Pt_v, ";", Tipo::NULO};
            } else {
                do {
                    tkn = lex.scanner();
                } while (tkn.classe != Classe::Eof && !tokenToRead.count(tkn.classe));
                sem.shiftToken(tkn);

                if (tkn.classe == Classe::Eof) {
                    printf("\nFim de análise.\n");
                    printf("Erro(s) detectado(s).\n");
                    break;
                }
            }
        }
    }

    sem.printSymbolTable();

    printf("\n\n%s\n", sem.object_file_text.c_str());
}
