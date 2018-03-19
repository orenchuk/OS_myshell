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

string pwd() {
    return filesystem::current_path().string();
}

int mypwd(vector<string> args = {}) {
    int erno = 0;
    string help = "[-h|--help] - returns current directory path";
    string error = "Error: there are no such options, use -h or --help to get more info";
    
    if (args.size() == 0) {
        auto cwd = pwd();
        cout << cwd << endl;
    } else if ((args.size() == 1) && ((args[0] == "-h") || (args[0] == "--help"))) {
        cout << help << endl;
    } else {
        erno = -1;
        cout << error << endl;
    }
    
    return erno;
}

int mycd(vector<string> args) {
    for (auto &a : args) {
        cout << "test: " << a << endl;
    }
    
    if (args.size() == 1 && chdir(args[0].c_str()) == 0) {
        return 0;
    } else if(args.size() > 1 || (args[1] == "-h" && args[1] == "--help")) {
        chdir(args[0].c_str());
        return 0;
    } else {
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
    
    //    for (auto &a : args) {
    //        cout << "test: " << a << endl;
    //    }
    
    if (cmd == "merrno") {
        cout<<err_code;
    } else if (cmd == "mpwd") {
        err_code = mypwd(args);
    } else if (cmd == "mcd") {
        err_code = mycd(args);
    } else if (cmd == "mexit") {
        myexit(args);
    }
}

int main() {
    string cmd;
    vector<string> args;
    while(true) {
        cout << pwd() << "$ ";
        getline(cin, cmd);
        split(args, cmd, is_any_of(" "), token_compress_on);
        execute(args);
        //        for (auto &a : args) {
        //            cout << "test: " << a << endl;
        //        }
    }
    return 0;
}
