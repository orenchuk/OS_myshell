#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

boost::filesystem::path pwd() {
    return boost::filesystem::current_path();
}

int mypwd(vector<string> &args) {
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

int mycd(vector<string> &args) {
    string help = "[-h|--help] - change directory to <path>";
    if (args.size() == 1 && chdir(args[0].c_str()) == 0) {
        return 0;
    } else if (args.size() == 2 && (args[1] == "-h" || args[1] == "--help")) {
        cout<< help <<endl;
        return chdir(args[0].c_str());
    }
    else if(args.size() == 1 && (args[0] == "-h" || args[0] == "--help")){
        cout << help << endl;
    }
    return 1;
}

void myexit(vector<string> &args) {
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

void execute(vector<string> &args) {
    int err_code = 0;
    if (args.empty()) {
        exit(-2);
    }
    string cmd = args[0];
    
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
        pid_t pid = fork();
        if (pid == -1)
        {
            std::cerr << "Failed to fork()" << std::endl;
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            auto path_ptr = getenv("PATH");
            string path_var;
            if (path_ptr != nullptr) {
                path_var = path_ptr;
            } else {
                cout << "Error no such argumet(s), try \"help\" for more info" << endl;
            }
            path_var += ":.";
            setenv("PATH", path_var.c_str(), 1);
            vector<const char*> arg_for_c;
            for (auto s: args)
                arg_for_c.push_back(s.c_str());
            arg_for_c.push_back(nullptr);
            execvp(cmd.c_str(), const_cast<char* const*>(arg_for_c.data()));
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    string separator1("");      //dont let quoted arguments escape themselves
    string separator2(" ");     //split on spaces
    string separator3("\"\'");  //let it have quoted arguments
    
    while(true) {
        string cmd;
        vector<string> args;
        
        cout << "Computer:" << pwd().filename().string() << " user$ ";
        getline(cin, cmd);
        
        boost::escaped_list_separator<char> els(separator1,separator2,separator3);
        boost::tokenizer<boost::escaped_list_separator<char>> tok(cmd, els);
        
        for (auto &t : tok) {
            args.push_back(t);
        }
        
        execute(args);
    }
    return 0;
}
