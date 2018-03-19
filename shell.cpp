#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string>
#include <libgen.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

boost::filesystem::path pwd() {
    return boost::filesystem::current_path();
}

int mypwd(vector<string> args = {}) {
    int erno = 0;
    string help = "[-h|--help] - returns current directory path";
    string error = "Error: there are no such options, use -h or --help to get more info";
    
    if (args.size() == 0) {
        auto cwd = pwd().string();
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
    if (args.size() == 1 && chdir(args[0].c_str()) == 0) {
        return 0;
    } else if (args.size() == 2 && (args[1] == "-h" && args[1] == "--help")) {
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
    
    if (cmd == "merrno") {
        cout << err_code << endl;
    } else if (cmd == "mpwd") {
        err_code = mypwd(args);
    } else if (cmd == "mcd") {
        err_code = mycd(args);
    } else if (cmd == "mexit") {
        myexit(args);
    } else if (cmd == "help") {
        cout << "Program MyShell. version 1.0 beta release\n" << endl;
        cout << "merrno [-h|--help] \t returns exit status of the command" << endl;
        cout << "mpwd [-h|--help] \t returns current directory path" << endl;
        cout << "mcd <path> [-h|--help] \t change dir to <path>" << endl;
        cout << "mexit [exit code] [-h|--help] \t exit myshell\n" << endl;
    } else {
        err_code = -1;
        cout << "Error: no such command, type `help` to get more info" << endl;
    }
}

int main() {
    string cmd;
    vector<string> args;
    while(true) {
        cout <<"Computer:" << pwd().filename().string() << " user$ ";
        getline(cin, cmd);
        boost::split(args, cmd, boost::is_any_of(" "), boost::token_compress_on);
        execute(args);
    }
    return 0;
}
