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

using namespace std;
#define clear() printf("\033[H\033[J")
#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
char inputString[MAXCOM], *parsedArgs[MAXLIST];
//char* parsedArgsPiped[MAXLIST];

// Function to take input
int takeInput(char *str);

void getFirstElement(string address);

void getTenFirstLine(string address);

void uncommentLines(string address);

void removeSpaces(string address);

void mostOccurring(string address);

void LineNumber(string address);

string showDir();

void init_shell();

int filesCommands(char **parsed);

void execArgs(char **parsed);

void parseSpace(char *str, char **parsed);

int ownCmdHandler(char **parsed);

int processString(char *str, char **parsed);

//---------------------------  Main Function --------------------------------------------------------
int main() {
    int execFlag = 0;
//    init_shell();
    while (1) {
        if (!takeInput(inputString))
            continue;
        // process
        execFlag = processString(inputString, parsedArgs);
        if (execFlag == 1)
            execArgs(parsedArgs);

//        if (execFlag == 2)
//            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    return 0;
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

string showDir() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    return (cwd);
//    cout << cwd << endl;
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

int takeInput(char *str) {
    char buf[MAXCOM];
    cout << endl << ">>>" << showDir() << " ";
    cin >> buf;
    if (strlen(buf) != 0) {
//         add_history(buf);
        strcpy(str, buf);
        return 1;
    } else {
        return 0;
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

int fileExistence(char *dir) {
    ifstream ifile;
    ifile.open(dir);
    if (ifile) {
        return 1;
    } else {
        return 0;
    }
}

int filesCommands(char **parsed)
{
    int NoOfFilesCommands = 6, i, switchOwnArg = 0;
    char *ListOfFilesCommands[NoOfFilesCommands];

    ListOfFilesCommands[0] = "a"; //first Element
    ListOfFilesCommands[1] = "b"; // most occurrence
    ListOfFilesCommands[2] = "c"; // remove empty spaces
    ListOfFilesCommands[3] = "d"; // uncommented files
    ListOfFilesCommands[4] = "e"; // number of lines
    ListOfFilesCommands[5] = "f"; // show first 10 line
    int detected = 0;
    for (i = 0; i < NoOfFilesCommands; i++) {
        if (strcmp(parsed[0], ListOfFilesCommands[i]) == 0) {
            switchOwnArg = i + 1;
            detected = 1;
            break;
        }
    }
    if (!detected) {
        return 0;
    }
    cout << "file: " << parsed[1];
    if (fileExistence(parsed[1])) {
        switch (switchOwnArg) {
            case 1:
                getFirstElement(parsed[1]);
                return 1;
            case 2:
                mostOccurring(parsed[1]);
                return 1;
            case 3:
                removeSpaces(parsed[1]);
                return 1;
            case 4:
                uncommentLines(parsed[1]);
                return 1;
            case 5:
                LineNumber(parsed[1]);
                return 1;
            case 6:
                getTenFirstLine(parsed[1]);
                return 1;
                return 0;
        }
    } else {
        cout << "File doesn't Found!" << endl;
    }
}

void execArgs(char **parsed) {
    // Forking a child
    pid_t pid = fork();

    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        cout << parsed[0];
        if (filesCommands(parsed)) {
//            cout << "sddvsd";
        } else if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        } else {
            cout << "command not found: " << parsed << endl;
        }
        exit(0);

    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}

void parseSpace(char *str, char **parsed) {
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}


int ownCmdHandler(char** parsed) {
    int NoOfOwnCmds = 4, i, switchOwnArg = 0;
    char *ListOfOwnCmds[NoOfOwnCmds];
    char *username;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg) {
        case 1:
            printf("\nGoodbye\n");
            exit(0);
        case 2:
            chdir(parsed[1]);
            return 1;
        case 3:
//            openHelp();
            return 1;
        case 4:
            username = getenv("USER");
            printf("\nHello %s.\nMind that this is "
                   "not a place to play around."
                   "\nUse help to know more..\n",
                   username);
            return 1;
        default:
            break;
    }
    return 0;
}

int processString(char *str, char **parsed) {

//    char* strpiped[2];
//    int piped = 0;

//    piped = parsePipe(str, strpiped);

//    if (piped) {
//        parseSpace(strpiped[0], parsed);
//        parseSpace(strpiped[1], parsedpipe);
//
//    }
//    else {

    parseSpace(str, parsed);
//    }

    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1;
}


