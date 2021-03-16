#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "tokens.h"
#include "gettoken.h"
#include "parse.h"
#include "tree.h"
using namespace std;

int val = 0;
istream *us1 = &cin;
ifstream inFile;

int main(int argc, const char *argv[]) {
    for (int i = 1; i < argc; i++) {
        string vari(argv[i]);
        if (i == argc - 1) {

            inFile.open(vari);

            if (inFile.is_open() == false) {
                cerr << "COULD NOT OPEN " << vari << endl;
                return -1;
            }
            us1 = &inFile;
        }
        else {
            cerr << "TOO MANY FILENAMES" << endl;
            return -1;
        }
    }

    ParseTree *mainTree = Prog(*us1, val);

    if (mainTree != 0)
    {
        try {
            mainTree->Eval();
        }
        catch (std::logic_error &e) {
            cout << "RUNTIME ERROR " << e.what() << endl;
        }

        if (mainTree->NodeCount() != 0) {
            cout << "NODE COUNT: " << mainTree->NodeCount() << endl;
        }

        if ((mainTree->NodeCount() - mainTree->leaves()) != 0) {
            cout << "INTERIOR COUNT: " << (mainTree->NodeCount() - mainTree->leaves()) << endl;
        }

        if (mainTree->ops() != 0) {
            cout << "OPS COUNT: " << mainTree->ops() << endl;
        }

        if (mainTree->strings() != 0) {
            cout << "STRING COUNT: " << mainTree->strings() << endl;
        }

        if (mainTree->maxdepth() != 0) {
            cout << "MAX DEPTH: " << mainTree->maxdepth() << endl;
        }
    }

    return 0;
}
