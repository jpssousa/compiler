#include "main.hpp"


const string ALG_EXTENSION = string(".alg");
const string C_EXTENSION = string(".c");
const string DEFAULT_FILE = string("alg.c");

bool check_source_file_extension(string file_name) {
    size_t len = ALG_EXTENSION.length();
    if (file_name.length() > len) {
        return !file_name.compare(file_name.length() - len, len, ALG_EXTENSION);
    }
    return false;
}

string object_file_path(string file_name) {
    if (!file_name.compare(string(""))) {
        cout << "No object target file provided. Writing on '" << DEFAULT_FILE << "'\n" ;
        return DEFAULT_FILE;
    }
    size_t len = C_EXTENSION.length();
    if (!file_name.compare(file_name.length() - len, len, C_EXTENSION)) {
        cout << "Object file provided. Writing on '" << file_name << "'\n";
        return file_name;
    }
    string object_file = file_name + C_EXTENSION;
    cout << "Object file provided without extension. Writing on '" << object_file << "'\n";
    return object_file;
}

int main(int argc, char* argv[]) {
    ifstream fp;
    string source_file = string("");
    string output_file = string("");

    switch (argc) {
        case 2:
        case 3:
            source_file = argv[1];
            output_file = (argc == 3) ? argv[2] : "";
            if (!check_source_file_extension(source_file)) {
                cout << "Source file must end with '.alg' extension.\n";
                return 0;
            }
            output_file = object_file_path(output_file);
            break;
        default:
            cout << "Compiler run options:\n";
            cout << "$ ./main [REQUIRED] <file_name.alg> [OPTIONAL] <output_file>\n";
            return 0;
    }

    Lexical lex(source_file);
    token tkn;

    do {
        tkn = lex.scanner();
        tkn.print();
    } while (tkn.classe != Classe::Eof);

    lex.printSymbolTable();

    return 0;
}