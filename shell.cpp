#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string.h>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

void cwd() {
    char dir[100];
    getcwd(dir, 100);
    cout<<dir;
}
void cd(string path) {
    chdir(path.c_str());
}

int myexit(string first, string second) {
    int exit_num;
    if(first == "-h" && first == "--help") {
        exit_num = stoi(second);
    }
    else {
        exit_num = stoi(first);
    }
    return exit_num;
}

void execute(vector<string> args) {
    
}

int main() {
    string cmd;
    vector<string> args;
    while(true) {
        getline(cin, cmd);
        split(args, cmd, is_any_of(" "), token_compress_on);
        execute(args);
    }
    return 0;
}
