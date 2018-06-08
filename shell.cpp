#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <sys/wait.h>
#include <fstream>
#include <sstream>
#include <map>
#include <errno.h>
#include <fcntl.h>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

boost::filesystem::path pwd() {
    return boost::filesystem::current_path();
}

int mypwd(vector<string>& args) {
    args.erase(args.begin());
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
        std::cerr << error << endl;
    }
    
    return erno;
}

int mycd(vector<string>& args) {
    args.erase(args.begin());
    string help = "[-h|--help] - change directory to <path>";
    if (args.size() == 1 && chdir(args[0].c_str()) == 0) {
        return 0;
    } else if (args.size() == 2 && (args[1] == "-h" || args[1] == "--help")) {
        cout<< help <<endl;
        return chdir(args[0].c_str());
    }
    else if(args.size() == 1 && (args[0] == "-h" || args[0] == "--help")) {
        cout << help << endl;
    }
    return 1;
}

void myexit(vector<string>& args) {
    args.erase(args.begin());
    int exit_num = 0;
    string help = "help was made";
    if(args.size() == 2) {
        if(args[2] == "-h" && args[1] == "--help") {
            cout << help << endl;
            string num = args[1];
            exit_num = stoi(num);
        } else {
            string num = args[0];
            exit_num = stoi(num);
            cout << help << endl;
        }
    } else if (args.size() == 1) {
        if (args[0] == "-h" && args[0] == "--help") {
            cout << help << endl;
        } else {
            string num = args[0];
            exit_num = stoi(num);
        }
    } else {
        exit(exit_num);
    }
    exit(exit_num);
}

vector<string> replace(vector<string>& args, std::map<string, string>& vars) {
    vector<string> new_args;
    for (auto& arg : args) {
        if (arg[0] == '$') {
            arg.erase(arg.begin());
            if (vars.count(arg) > 0) {
                new_args.push_back(vars[arg]);
            } else if (getenv(arg.c_str()) != NULL) {
                new_args.push_back(getenv(arg.c_str()));
            } else {
                std::cerr << "Error: there is no such variable" << endl;
            }
        } else {
            new_args.push_back(arg);
        }
    }
    return new_args;
}

void setvars(std::map<string, string>& vars, const string& line) {
    vector<string> res;
    boost::split(res, line, boost::is_any_of("="));
    if (res.back().front() == '`') {
        res.back().erase(res.back().begin());
    }
    if (res.back().back() == '`') {
        res.back().erase(res.back().end());
    }
    vars[res.front()] = res.back();
    cout << res.front() << " is localy set to " << vars[res.front()] << endl;
}

void mysetenv(string line, std::map<string, string>& vars) {
    if (boost::contains(line, "=")) {
        vector<string> res;
        boost::split(res, line, boost::is_any_of("="));
        if (res.back().front() == '`') {
            res.back().erase(res.back().begin());
        }
        if (res.back().back() == '`') {
            res.back().erase(res.back().end());
        }
        setenv(res.front().c_str(), res.back().c_str(), 1);
        cout << res.front() << " is exportly set to " << getenv(res.front().c_str()) << endl;
    } else if (boost::contains(line, "$")) {
        line.erase(line.begin());
        if (vars.count(line) > 0) {
            setenv(line.c_str(), vars[line].c_str(), 0);
        }
        cout << "variable " << line << " is exportly set to " << getenv(line.c_str()) << endl;
    }
}

void myexec(const vector<string>& args, int from, int to, bool back = false, bool err = false) {
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork()" << std::endl;
        exit(EXIT_FAILURE);
        
    } else if (!back && pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        
    } else {
        
        if (back) {
            close(0);
            close(1);
            close(2);
        }
        
        if (err) {
            if (from != -1) {
                dup2(from, STDERR_FILENO);
            }
            
            if (to != -1) {
                dup2(to, STDERR_FILENO);
            }
        } else {
            if (from != -1) {
                dup2(from, STDIN_FILENO);
            }
            
            if (to != -1) {
                dup2(to, STDOUT_FILENO);
            }
        }
        
        auto path_ptr = getenv("PATH");
        string path_var;
        if (path_ptr != nullptr) {
            path_var = path_ptr;
        } else {
            std::cerr << "Error no such argumet(s), try \"help\" for more info" << endl;
        }
        path_var += ":.";
        setenv("PATH", path_var.c_str(), 1);
        vector<const char*> arg_for_c;
        for (auto s: args)
            arg_for_c.push_back(s.c_str());
        arg_for_c.push_back(nullptr);
        execvp(args.front().c_str(), const_cast<char* const*>(arg_for_c.data()));
        std::cerr << "Error: there is no such argument(s)" << endl;
        exit(EXIT_FAILURE);
    }
}

void myforkexec(const vector<string>& args) {
    myexec(args, -1, -1);
}

void execute_script(std::map<string, string>& vars, const vector<string>& args) {
    for (auto& a : args) {
        std::ifstream input(a);
        if (!input.is_open()) {
            std::cerr << "Error: there is no such file" << endl;
        }

        string line;
        while (std::getline(input, line)) {
            vector<string> lines;
            std::istringstream sstream(line);
            string word;
            while (sstream >> word) {
                if (word[0] == '#') {
                    break;
                } else if (word[0] == '$') {
                    word.erase(word.begin());
                    if (vars.count(word) > 0) {
                        auto variable = vars[word];
                        lines.push_back(variable);
                    } else if (getenv(word.c_str()) != NULL) {
                        lines.push_back(getenv(word.c_str()));
                    } else {
                        std::cerr << "Error: there is no such var" << endl;
                    }
                } else if (word == "mexport") {
                    sstream >> word;
                    if (boost::contains(word, "=")) {
                        setvars(vars, word);
                    }
                    mysetenv(word, vars);
                    continue;
                } else if (word == "mecho") {
                    lines.push_back("echo");
                } else if (boost::contains(word, "=")) {
                    setvars(vars, word);
                    continue;
                } else {
                    lines.push_back(word);
                }
            }
            myforkexec(lines);
        }
    }
    
}

void conveyor(vector<string>& args, std::map<string, string>& vars) {
    vector<vector<string>> parts;
    vector<string> temp_args;
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] != "|") {
            temp_args.push_back(args[i]);
        } else {
            parts.push_back(replace(temp_args, vars));
            temp_args.clear();
        }
    }
    
    parts.push_back(replace(temp_args, vars));
    temp_args.clear();
    
    int pipfd[args.size()-1][2];
    
    for (size_t i = 0; i < parts.size() - 1; ++i) {
        if (pipe(pipfd[i]) < 0) {
            std::cerr << "Error: pipe init failed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i == 0) {
            myexec(parts[i], -1, pipfd[i][1]);
            close(pipfd[i][1]);
        } else  if (i == parts.size() - 1) {
            myexec(parts[i], pipfd[i-1][0], -1);
            close(pipfd[i-1][0]);
        } else {
            myexec(parts[i], pipfd[i-1][0], pipfd[i][1]);
            close(pipfd[i-1][0]);
            close(pipfd[i][1]);
        }
    }
    
}

void redirect(const string& from_file, const string& to_file, bool reversed = false, bool err = false) {
    
    int fd;
    
    if ((fd = open(to_file.c_str(), O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
        std::cerr << "Can't open file" << endl;
        exit(1);
    }
    
    vector<string> arg;
    arg.push_back(from_file);
    
    if (!reversed) {
        myexec(arg, -1, fd, false, err);
    } else {
        myexec(arg, fd, -1, false, err);
    }
    
    close(fd);
}

void background(vector<string>& args, std::map<string, string>& vars) {
    auto new_args = replace(args, vars);
    myexec(new_args, -1, -1, true);
}

void execute(std::map<string, string>& vars, vector<string>& args) {
    int err_code = 0;
    if (args.empty()) {
        exit(-2);
    }
    string cmd = args[0];
    
    if (cmd == "merrno") {
        cout << err_code << endl;
    } else if (cmd == "mpwd") {
        auto new_args = replace(args, vars);
        err_code = mypwd(new_args);
    } else if (cmd == "mcd") {
        auto new_args = replace(args, vars);
        err_code = mycd(new_args);
    } else if (cmd == "mexit") {
        auto new_args = replace(args, vars);
        myexit(new_args);
    } else if (cmd == "mecho") {
        args[0] = "echo";
        auto new_args = replace(args, vars);
        myforkexec(new_args);
    } else if (cmd == "mexport") {
        args.erase(args.begin());
        for (auto& a : args) {
            if (boost::contains(a, "=")) {
                setvars(vars, a);
            }
            mysetenv(a, vars);
        }
    } else if (args.size() > 2) {
        for (size_t i = 1; i < args.size() - 1; ++i) {
            if (args[i] == ">") {
                redirect(args[i-1], args[i+1]);
            } else if (args[i] == "2>") {
                redirect(args[i-1], args[i+1], false, true);
                break;
            } else if (args[i] == "<") {
                redirect(args[i-1], args[i+1], true);
                break;
            } else if (args[i] == "2>&1") {
                redirect(args[0], args[2], false, true);
                redirect(args[0], args[2]);
                break;
            } else if (args[i] == "|") {
                conveyor(args, vars);
                break;
            }
        }
    } else if (boost::contains(cmd, "=")) {
        setvars(vars, cmd);
    } else if (cmd == "help") {
        cout << "Program MyShell. version 1.0 beta release\n" << endl;
        cout << "merrno [-h|--help] \t returns exit status of the command" << endl;
        cout << "mpwd [-h|--help] \t returns current directory path" << endl;
        cout << "mcd <path> [-h|--help] \t change dir to <path>" << endl;
        cout << "mexit [exit code] [-h|--help] \t exit myshell\n" << endl;
    } else {
        if (cmd[0] == '.') {
            args.front().erase(args.front().begin());
            execute_script(vars, args);
        } else if (args.back() == "&") {
            args.pop_back();
            background(args, vars);
        } else {
            myforkexec(replace(args, vars));
        }
    }
}

int main(int argc, const char * argv[]) {
    
    std::map<string, string> variables;
    
    string separator1("");      //dont let quoted arguments escape themselves
    string separator2(" ");     //split on spaces
    string separator3("\"\'");  //let it have quoted arguments
    
    if (argc > 1) {
        vector<string> vect;
        for (size_t i = 1; i < argc; ++i) {
            vect.push_back(argv[i]);
        }
        
        execute_script(variables, vect);
        
        exit(EXIT_SUCCESS);
    }
    
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
        
        execute(variables, args);
    }
    
    return 0;
}
