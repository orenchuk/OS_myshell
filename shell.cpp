//
// Created by Rostyk Popov on 3/18/18.
//
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string.h>

using namespace std;

void cwd(){
    char dir[100];
    getcwd(dir, 100);
    cout<<dir;
}
void cd(char* path){
    chdir(path);
}
vector<string>parse_args(string str){
    vector<string> vector1;
    istringstream iss(str);
    while(iss){
        string subs;
        iss>>subs;
        vector1.push_back(subs);
    }
    vector1.pop_back();
    return vector1;
}
int main(){
    string cmd;
    while(1){
        cin>>cmd;
        if(cmd.compare("exit") == 0){
            exit(EXIT_SUCCESS);
        }
        else{
            vector<string> vector1 = parse_args(cmd);
            for (int i = 0; i < vector1.size(); ++i) {
                cout<<vector1[i]<<" ";
            }
        }
    }
    return 0;
}