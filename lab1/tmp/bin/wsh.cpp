#include "wsh.h"
#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <unistd.h>
#include <stdlib.h>
using namespace std;



wsh::wsh()
{
	// recognized commands
	char*	commands[] = { "-place holder-", "exit", "copy" };


	// builds command name hash map
	for (unsigned i = 1; i < sizeof(commands)/sizeof(char*); i++)
		command_table[commands[i]] = i;

	system("clear");
	// LAB 1: initialize the cwd member variable here.
	getcwd(cwd,sizeof(cwd));
}



int wsh::next_command()
{
	// LAB 1: place assignment code here.
	// Note that the program will not run until this part is completed.
	// define a command buffer and a place for the current working directory string

	cout << cwd << "=>";
	static char cmd[PATH_MAX];
	cin.getline(cmd,sizeof(cmd));
	
	char * token;
	token = strtok(cmd," \t");

	if(token == NULL) return NONE;
	
	argc=0;

	while(token != NULL)
	{
		argv[argc++] = token;
		token = strtok(NULL," \t");
	}
	
	if(strcmp(argv[0], "exit")==0) return EXIT;
	if(strcmp(argv[0],"copy")==0) return COPY;
	else
	{
		cout << "argc is " << argc;
		for(int i=0; i < argc; i++)
		{
			cout << "; argv[" << i << "] is " << argv[i];
		}
		cout << endl;
	}
	return 0;
}



void wsh::interpret()		// loops executing commands from the command line.
{
	bool	running = true;

	while (running)
	{
		int	c = next_command();

		switch(c)
		{
			case EXIT :
				running = false;
				break;

			case COPY :
				copy();
				break;

			case NONE :		// no command entered
				break;

			default:		// tests command line parsing
				for (int i = 0; i < argc; i++)
					cout << argv[i] << endl;
			      break;
		}
	}
}

void wsh::copy()
{
        char *copier = strrchr(argv[1],'/');

        int result = strcmp(argv[2],".");

        if(result == 0){
                if(copier != NULL){
			cout << argv[0] << " " << argv[1] << " " << cwd << copier << endl;
		}else{
			cout << argv[0] << " " << argv[1] << " " << cwd << "/" << argv[1] << endl;
		}
        }else{
		result = strcmp(argv[1],argv[2]); 

        	if(result == 0){
                	cout << argv[0] << " " << argv[1] << " " << argv[2] << endl;
        	}else{
                	cout << argv[0] << " " << argv[1] << " " << argv[2] << copier << endl;
        	}
	}
}

int main()		// execution startup.
{
	wsh	cli;

	cli.interpret();

	return 0;
}

