//
// Created by Gilass-09941213068 on 12/23/2022.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

void getFirstElement(string address){
    fstream newfile;
    newfile.open(address,ios::in);

    string line;

    if (newfile.is_open()){
        while(getline(newfile, line, '\n')) {
            char * input = &line[0];
            char* firstWord = strtok(input," ");
            cout << firstWord<<endl;
        }
        newfile.close();
    }
}

void getTenFirstLine(string address){
    fstream newfile;
    newfile.open(address,ios::in);

    string line;
    int count = 0;

    if (newfile.is_open()){
        while(getline(newfile, line, '\n')) {
            cout << line<<endl;
            count++;
            if(count == 10){
                break;
            }
        }
        newfile.close();
    }
}

void uncommentLines(string address){
    fstream newfile;
    newfile.open(address,ios::in);

    string line;
    if (newfile.is_open()){
        while(getline(newfile, line, '\n')) {
            if(line.rfind("#", 0)==-1)
                cout << line << endl;
        }
        newfile.close();
    }
}


int main() {
    getFirstElement("xyz.txt");
    getTenFirstLine("xyz.txt");
    uncommentLines("xyz.txt");

}