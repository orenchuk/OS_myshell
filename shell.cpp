#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string.h>

using namespace std;

void cwd() {
    char dir[100];
    getcwd(dir, 100);
    cout<<dir;
}
void cd(string path) {
    chdir(path.c_str());
}
vector<string> parse_args(string str) {
    vector<string> vector1;
    istringstream iss(str);
    while(iss) {
        string subs;
        iss>>subs;
        vector1.push_back(subs);
    }
    vector1.pop_back();
    return vector1;
}

void execute(vector<string> args) {
    
}

vector<string> read_args(string cmd) {
    vector<string> args;
    
    if (cmd == "exit") {
        exit(0);
    }
    args.push_back(cmd);
    return args;
}

int main() {
    string cmd;
    vector<string> args;
    
    while(cin >> cmd) {
        args = read_args(cmd);
        for (auto &a : args) {
            cout << a << endl;
        }
    }
    
    return 0;
}
