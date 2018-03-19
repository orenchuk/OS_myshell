#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>

using namespace std;
using namespace boost;

int mypwd(vector<string> args) {
    auto cwd = filesystem::current_path().string();
    cout << cwd << endl;
    return chdir(cwd.c_str());
}

int mycd(vector<string> args) {
    if (args.size() == 1 || chdir(args[0].c_str()) == 0){
        return 0;
    }
    else if(args.size() > 1 || (args[1] == "-h" && args[1] == "--help"){
        return 0;
    }
    else{
        return 1;
    }
}

void myexit(vector<string> args) {
    int exit_num = 0;
    string help = "help was made";
    if(args.size() == 2) {
        if(args[2] == "-h" && args[1] == "--help") {
            cout<<help<<endl;
            string num = args[1];
            exit_num = stoi(num);
        }
        else {
            string num = args[0];
            exit_num = stoi(num);
            cout<<help<<endl;
        }
    }
    else if (args.size() == 1) {
        if (args[0] == "-h" && args[0] == "--help") {
            cout<<help<<endl;
        }
        else {
            string num = args[0];
            exit_num = stoi(num);
        }
    }
    else {
        exit(exit_num);
    }
    exit(exit_num);
}

void execute(vector<string> args) {
    int err_code = 0;
    string cmd = args[0];
    args.erase(args.begin());
    if (cmd == "merrno") {
        cout<<err_code;
    } else if (cmd == "mpwd") {
        err_code = mypwd();
    } else if (cmd == "mcd") {
        mycd("../");
    } else if (cmd == "mexit") {
        err_code = myexit(args);
    }
}

int main() {
    string cmd;
    vector<string> args;
    while(true) {
        getline(cin, cmd);
        split(args, cmd, is_any_of(" "), token_compress_on);
        execute(args);
        //        for (auto &a : args) {
        //            cout << "test: " << a << endl;
        //        }
    }
    return 0;
}
