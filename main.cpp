#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <string>
#include <algorithm>
#include <sstream>
#include <map>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define clear() printf("\033[H\033[J")
using namespace std;

void getFirstElement(string address);

void getTenFirstLine(string address);

void uncommentLines(string address);

void removeSpaces(string address);

void mostOccurring(string address);

void LineNumber(string address);

void showDir();

void init_shell();

//---------------------------  Main Function --------------------------------------------------------
int main() {
    init_shell();
    while (true) {
        showDir();
//         sleep(2);

        //---------- Ali Methods: ----------
//         removeSpaces("text.txt");
//         mostOccurring("text.txt");
//         LineNumber("text.txt");

        //---------- Pouria Methods: ----------
//        getFirstElement("xyz.txt");
//        getTenFirstLine("text.txt");
//        uncommentLines("text.txt");

        break;
    }
}


void init_shell() {
    clear();
    cout << "\n\n****************************************";
    cout << "*\n*\n*\n*\t\tPOAL SHELL\t\t";
    cout << "\n*\n*\n*\n*****************************************";
    char *username = getenv("USER");
    cout << "\n\n\nUSER is: " << username;
    cout << "\n";
    sleep(2);
    clear();
}

void showDir() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cout << "➜ " << cwd << endl;
}

void removeSpaces(string address) {
    fstream myF;
    myF.open(address, ios::in);
    string allLines;
    string line;
    if (myF.is_open()) {
        while (getline(myF, line)) {
            allLines += line;
            allLines.push_back('\n');
        }
        myF.close();

        allLines.erase(remove_if(allLines.begin(), allLines.end(),
                                 [](char c) {
                                     return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f');
                                 }),
                       allLines.end());
        // cout << allLines;
        myF.open(address, ios::out);
        myF << allLines;
        myF.close();
        cout << "allLines: " << allLines << endl;
        cout << "Done!" << endl;
    } else {
        cout << "Can't open " << address << endl;
    }
}

void mostOccurring(string address) {
    fstream myF;
    myF.open(address, ios::in);
    string allLines;
    string line;
    if (myF.is_open()) {
        while (getline(myF, line)) {
            allLines += line;
            allLines.push_back('\n');
        }
        myF.close();

        istringstream input(allLines);
        map<string, int> count;
        string word;
        decltype(count)::const_iterator most_occurring;
        while (input >> word) {

            auto itr = count.emplace(word, 0).first;
            ++itr->second;
            if (count.size() == 1 || itr->second > most_occurring->second)
                most_occurring = itr;
        }
        cout << "The Word '";
        cout << most_occurring->first << "' is repeated ";
        cout << most_occurring->second << " times." << endl;

    } else {
        cout << "Can't open " << address << endl;
    }
}

void LineNumber(string address) {
    int linec = 0;
    ifstream myF(address);
    string line;
    if (myF.is_open()) {
        while (!myF.eof()) {
            getline(myF, line);
            // cout << line << endl;
            linec++;
        }
        myF.close();
    }

    cout << "number of lines: " << linec << endl;
}

void getFirstElement(string address) {
    fstream newfile;
    newfile.open(address, ios::in);

    string line;

    if (newfile.is_open()) {
        while (getline(newfile, line, '\n')) {
            char *input = &line[0];
            char *firstWord = strtok(input, " ");
            cout << firstWord << endl;
        }
        newfile.close();
    }
}

void getTenFirstLine(string address) {
    fstream newfile;
    newfile.open(address, ios::in);

    string line;
    int count = 0;

    if (newfile.is_open()) {
        while (getline(newfile, line, '\n')) {
            cout << line << endl;
            count++;
            if (count == 10) {
                break;
            }
        }
        newfile.close();
    }
}

void uncommentLines(string address) {
    fstream newfile;
    newfile.open(address, ios::in);

    string line;
    if (newfile.is_open()) {
        while (getline(newfile, line, '\n')) {
            if (line.rfind("#", 0) == -1)
                cout << line << endl;
        }
        newfile.close();
    }
}