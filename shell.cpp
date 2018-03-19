
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

void execute(vector<string> args) {

}

vector<string> read_args(string cmd) {
    vector<string> args;
    args.push_back(cmd);
    return args;
}

void myexit(string fisrt, string second){
    int exit_num;
    if(first == "-h" && first == "--help"){
        exit_num = stoi(second);
    }
    else{
        exit_num = stoi(fisrt);
    }
    return exit_num;
}


int main() {
    string cmd;
    vector<string> args;
    while(true){
        getline(cin, cmd);
        split(args, cmd, is_any_of("\t "), token_compress_on);
        if (args[0] == "exit"){
            myexit(args[1], args[2]);
        }
    }
    return 0;
}