#include <iostream>
#include <unistd.h>
#include <regex>
#include <string>
#include <set>
#include <cstdio>
#include <limits>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

using namespace std;

const set<string> interns = { "cd", "exit", "pwd" };

int change_dir(char* path)
{
    return chdir(path);
}

void exit_cmd()
{
    exit(0);
}

int get_cwd()
{
    cout << get_current_dir_name() << endl;
    return 0;
}

struct Command
{

    bool intern = false;
    array<char*, 20> argv;

    void read()
    {
        string s;
        int i = 0;

        while(cin.peek() != '\n')
        {
            cin >> s;
            argv[i] = strdup(s.c_str());
            i++;
        }


        if(i > 0 && interns.count(argv[0]))
            intern = true;

        argv[i] = NULL;
    }

    int execute()
    {
        if(argv[0] == NULL) return 0;

        if(intern)
        {
            if(!strcmp("cd", argv[0]))
                return change_dir(argv[1]);

            if(!strcmp("exit", argv[0]))
                exit_cmd();

            if(!strcmp("pwd", argv[0]))
                get_cwd();

        }

        else if(!fork())
            return execvp(argv[0], argv.data());

        else
            wait(0);
    }
};


int main()
{
    Command cmd;
    string user = getpwuid(getuid())->pw_name;
    user += "@";

    while(1)
    {
        cout << user << basename(get_current_dir_name()) << ":~$ ";
        cmd.read();
        cin.ignore();
        if(cmd.execute() < 0)
            cout << strerror(errno) << endl;
    }

    return 0;
}
