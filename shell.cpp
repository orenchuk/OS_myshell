
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;
using namespace boost;

void mpwd() {
    char dir[100];
    getcwd(dir, 100);
    cout<<dir;
}
void mcd(string path) {
    chdir(path.c_str());
}

void execute(vector<string> args) {

}

vector<string> read_args(string cmd) {
    vector<string> args;
    args.push_back(cmd);
    return args;
}

int myexit(vector<string> args){
    int exit_num = 0;
    string help = "help was made";
    if(args.size() == 3){
        if(args[2] == "-h" && args[2] == "--help"){
            cout<<help;
            string num = args[3];
            exit_num = stoi(num);
        }
        else{
            string num = args[2];
            exit_num = stoi(num);
            cout<<help;
        }
    }
    else{
        if (args[2] == "-h" && args[2] == "--help"){
            cout<<help;
        }
        else{
            string num = args[2];
            exit_num = stoi(num);
        }
    }
    return exit_num;
}


int main() {
    string cmd;
    vector<string> args;
    while(true){
        getline(cin, cmd);
        split(args, cmd, is_any_of("\t "), token_compress_on);
        if (args[0] == "mexit"){
            return myexit(args);
            }
        if(args[0] == "mpwd"){
            mpwd();
        }

        }
    return 0;
}