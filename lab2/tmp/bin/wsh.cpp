#include "wsh.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;



// used to convert numeric file modes to string representation

char*	fmodes[] = { "---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx" };

// -----------------------------------------------------------------------------------



wsh::wsh()
{
	char*	commands[] = { "-place holder-", "exit", "copy", "cd", "list", 
		"view", "del", "ren", "mkdir", "rmdir" };

	for (unsigned i = 1; i < sizeof(commands)/sizeof(char*); i++)
		command_table[commands[i]] = i;

	system("clear");
	getcwd(cwd, PATH_MAX);
}


int wsh::next_command()
{
	// LAB 1: place assignment code here.
	// Note that the program will not run until this part is completed.
	// define a command buffer and a place for the current working directory string
	static char cmd[PATH_MAX];
	char *token;
	// display current working directory
	cout << cwd;
	cout << "=>";
	// get a command and arguments from the user, count and parse into argv[]
	cin.getline(cmd, sizeof(cmd));
	// reset argc
	argc = 0;
	// while tokens available, assign them to argv using argc as index
	token = strtok(cmd, " \t");
	while (token != NULL) {
		argv[argc++] = token;
		token = strtok(NULL, " \t"); 
	}
	// if there were no tokens, return NONE else return token from ENUM
	if (argv[0] == NULL)
		return NONE;
	else
		return command_table[argv[0]];
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

			case CD :
				cd();
				break;

			case DEL :
				del();
				break;

			case REN :
				ren();
				break;

			case LIST :
				list();
				cout << endl;
				break;

			case VIEW :
				view();
				break;

			case MKDIR :
				makedir();
				break;

			case RMDIR :
				removedir();
				break;

			case NONE :
				break;

			default:
				for (int i = 0; i < argc && argv[i] != NULL; i++)
					cout << argv[i] << endl;
			      break;
		}
	}
}

void wsh::list()
{
	system("ls -al");
}

void wsh::cd()
{
	chdir(argv[1]);
	getcwd(cwd, PATH_MAX);
}

void wsh::del()
{
	if(argc == 2)
	{	
		if( remove( argv[1] ) != 0 )
			perror( "Error deleting file" );
		else
			puts( "File successfully deleted" );
		return;
	}
	else
	{
		cout << "Invalid argument count" << endl;
		return;
	}
}

void wsh::ren()
{
	if(argc == 3)
	{
		int result;
		result= rename( argv[1] , argv[2] );
		if ( result == 0 )
			puts ( "File successfully renamed\n" );
		else
			perror( "Error renaming file\n" );
		return;
	}
	else
	{
		cout << "Invalid argument count" << endl;
		return;
	}
}

void wsh::view()
{
	if (argc == 2) {
		
		std::ifstream is (argv[1], std::ifstream::binary);
		if (is) {
			// get length of file:
			is.seekg (0, is.end);
			int length = is.tellg();
			is.seekg (0, is.beg);

			char * buffer = new char [length];
			// read data as a block:
			is.read (buffer,length);

			if (!is)
				std::cout << "error: only " << is.gcount() << " could be read" << endl;
			is.close();

			// ...buffer contains the entire file...
			cout << buffer << endl;
			delete[] buffer;
		}

		return;
	}
	else
	{
		cout << "Invalid argument count" << endl;
		return;
	}
}

void wsh::makedir()
{
	if (argc == 2) {

		int mk = mkdir(argv[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if(mk == -1)
			cout << "File exists" << endl;
		return;
	}
	else
	{
		cout << "Invalid argument count" << endl;
		return;
	}

}

void wsh::removedir()
{
	// verify 2 or 3 arguments only
	if (argc != 2 && argc != 3) {
		cout << "Invalid argument count" << endl;
		return;
	}

	// 3 args means rmdir -s DIR: verify the -s and recursively delete everything
	if (argc == 3) {
		if (strcmp(argv[1], "-s")) {
			cout << "Invalid argument count" << endl;
			return;
		}
		// call a recursive algorithm to delete everything inside a dir plus the dir itself
		if (!removedir(argv[2])) {
			cout << "Falure deleting " << argv[2] << endl;
		}
		return;
	}
	
	// -s not specified, simply remove the directory
	if (rmdir(argv[1])) {
		perror(argv[1]);
	}
}

bool wsh::removedir(char *dirName)
{
	DIR *dir;
	struct dirent *dirEntry;
	bool success;
	int rc;

	cout << "processing subdirectory " << dirName << endl;
	if (!(dir = opendir(dirName))) {
		perror(dirName);
		return false;
	}
	// chdir into directory to process its entries
	cout << "chdir into " << dirName << endl;
	chdir(dirName);
	while ((dirEntry = readdir(dir))) {
		// if a directory found, recurse to delete everything in it
		if (dirEntry->d_type & DT_DIR) {
			// skip "." and ".."
			if (!strcmp(dirEntry->d_name, "."))
				continue;
			if (!strcmp(dirEntry->d_name, ".."))
				continue;
			cout << "found subdirectory " << dirEntry->d_name << endl;
			success = removedir(dirEntry->d_name);
			if (!success) {
				return false;
			}
		} else {
			// otherwise delete the entry
			cout << "deleting " << dirEntry->d_name << endl;
			// success = true;
			rc = remove(dirEntry->d_name);
			if (rc) {
				perror(dirEntry->d_name);
				return false;
			} 
		}
	}
	// director is now empty -- chdir out of it and delete it
	cout << "chdir out of " << dirName << endl;
	chdir("..");
	cout << "deleting " << dirName << endl;
	// success = true;
	rc = rmdir(dirName);
	if (rc) {
		perror(dirName);
		return false;
	}
	return true;
}

void wsh::copy()
{
	if(argc == 3)
	{
		if(std::ifstream(argv[1]) && !std::ifstream(argv[2]))
		{
			ifstream inFile(argv[1], ios::binary);
			ofstream outFile(argv[2], ios::binary);

			outFile << inFile.rdbuf();

			inFile.close();
			outFile.close();

			return;
		}
		else
		{
			perror("file error");
			return;
		}
	}
	else
	{
		cout << "Invalid argument count" << endl;
		return;
	}
}

// bool wsh::copy(char* src, char* dest)
// {
// 	if(!std::ifstream(argv[2])
// 	{
// 		cout << "destination file already exhists" << endl;
// 		return false;
// 	}

// 	DIR *dir;
// 	struct dirent *entry;
// 	struct stat info;

// 	dir = opendir(src);
// 	if(!dir)
// 	{
// 		ifstream inFile(src);
// 		ofstream outFile(dest);

// 		outFile << inFile.rdbuf();

// 		inFile.close();
// 		outFile.close();
// 		return true;
// 	}

// 	while((entry=readdir(src)!=NULL))
// 	{
// 		if(entry->d_name[0]!='.')
// 		{
// 			string path = string(dirName)+"/"+string(entry->d_name);
// 			cout << path << endl;
// 			stat(entry,&info);
// 			if(S_ISDIR(info.st_mode))
// 			{
// 				copy(path);
// 			}
// 		}
// 	}

// 	closedir(src);

// 	return true;
// }

int main()		// execution startup.
{
	wsh	cli;

	cli.interpret();

	return 0;
}

