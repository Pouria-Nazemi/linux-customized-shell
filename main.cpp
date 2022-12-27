#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include <fstream>
#include <string>
#include <cstring>
#include <string>
#include <algorithm>
#include <sstream>
#include <map>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCOM 1000
#define MAXLIST 100

#define clear() printf("\033[H\033[J")
using namespace std;

//void getFirstElement(string address);

//void getTenFirstLine(string address);
//
//void uncommentLines(string address);
//
//void removeSpaces(string address);
//
//void mostOccurring(string address);
//
//void lineNumber(string address);
//
//void showDir();
//
//void init_shell();

//---------------------------  Main Function --------------------------------------------------------


void init_shell() {
    ofstream myF;
    myF.open("History.txt", ios::out);
    myF.close();
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
    cout << "\n➜ " << cwd << endl;
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
        cout << "Can't open '" << address << "'" << endl;
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
        cout << "Can't open '" << address << "'" << endl;
    }
}

void lineNumber(string address) {
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
    } else {
        cout << "Can't open '" << address << "'" << endl;

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

int takeInput(char *str) {
    string buff;
    char buf[MAXCOM];
//    cout << typeid(buff).name() << endl;

    cout << ">>> ";
    getline(cin, buff);
    strcpy(buf, buff.c_str());
//    cout << typeid(buf).name() << endl;

    if (strlen(buf) != 0) {
        ofstream myF;
        myF.open("History.txt", ios::app);
        myF << buf << endl;
        myF.close();
//        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}

void execArgs(char **parsed) {
    // Forking a child
    pid_t pid = fork();

    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}

void execArgsPiped(char **parsed, char **parsedpipe) {
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }

    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();

        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}

int ownCmdHandler(char **parsed) {
    int NoOfOwnCmds = 7, i, switchOwnArg = 0;
    char *ListOfOwnCmds[NoOfOwnCmds];
    char *username;

    ListOfOwnCmds[0] = "a";
    ListOfOwnCmds[1] = "b";
    ListOfOwnCmds[2] = "c";
    ListOfOwnCmds[3] = "d";
    ListOfOwnCmds[4] = "e";
    ListOfOwnCmds[5] = "f";
    ListOfOwnCmds[6] = "cd";
    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg) {
        case 1:
            getFirstElement(parsed[1]);
            return 1;
        case 2:
            cout << parsed[1] << endl;
            mostOccurring(parsed[1]);
            return 1;
        case 3:
            removeSpaces(parsed[1]);
            return 1;
        case 4:
            uncommentLines(parsed[1]);
            return 1;
        case 5:
            lineNumber(parsed[1]);
            return 1;
        case 6:
            getTenFirstLine(parsed[1]);
            return 1;
        case 7:
            int chdir_Status;
            chdir_Status = chdir(parsed[1]);
            if (chdir_Status == -1) {
                if (parsed[1] != NULL) {
                    string varr = parsed[1];
                    cout << "cd: no such file or directory: " << parsed[1] << endl;
                } else{
                    cout << "cd: no such file or directory" << endl;
                }
            }

            return 1;
        default:
            break;
    }

    return 0;
}

int parsePipe(char *str, char **strpiped) {
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}

void parseSpace(char *str, char **parsed) {
    int i;
    string st = str;
    if (st.rfind("cd", 0) == 0) {
        parsed[0] = strsep(&str, " ");
        parsed[1] = str;
//        cout << "the command in cd" << endl;
//        cout << parsed[0] << endl << parsed[1] << endl;
    } else {
        for (i = 0; i < MAXLIST; i++) {
            parsed[i] = strsep(&str, " ");
            if (parsed[i] == NULL)
                break;
            if (strlen(parsed[i]) == 0)
                i--;
        }
    }

}

int processString(char *str, char **parsed, char **parsedpipe) {

    char *strpiped[2];
    int piped = 0;

    piped = parsePipe(str, strpiped);

    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);

    } else {

        parseSpace(str, parsed);
    }

    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1 + piped;
}


char inputString[MAXCOM], *parsedArgs[MAXLIST];
char *parsedArgsPiped[MAXLIST];

int main() {

    int execFlag = 0;
    init_shell();
    while (true) {
        showDir();
        if (takeInput(inputString))
            continue;

        execFlag = processString(inputString, parsedArgs, parsedArgsPiped);

        if (execFlag == 1)
            execArgs(parsedArgs);

        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);

    }
    return 0;
}