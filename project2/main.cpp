#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

vector<string> tokenize(string input, string delim){
	vector<string> token_groups;			
	int pos = 0;
	while(1){
		pos = input.find(delim, 0);
		if(pos == -1) {
			token_groups.push_back(input.substr(0, input.size()));
			break;
		}		
		token_groups.push_back(input.substr(0,pos));		
		input = input.substr(pos+delim.size(), input.size());
	}
	return token_groups;
}

bool validate_token(string token_group) {
	int i = 0;
	for(i; i < token_group.size(); i++) {
		if(! isalnum(token_group[i])) {
			if(token_group[i] == '.'
				|| token_group[i] == '-' 
				|| token_group[i] == '/' 
				|| token_group[i] == '_') {
			}
			else{ 
				return false;
			}
		}
		
	}
	return true;
}

void handle_redirects(vector<string> commands) {
	int in, out;
	int i = 0;
	string inputfile, outputfile;
	char * arg0 = &commands[0][0];
	char ** args;
	int numArgs = 0;	
	bool commandFinished = false;
	for(i; i < commands.size(); i++) {
		if(commands[i].compare("<") == 0){
			inputfile = commands[i+1];
			commandFinished = true;
		}
		else if(commands[i].compare(">") == 0){
			outputfile = commands[i+1];
			commandFinished = true;
		}
		else if(!commandFinished) {
			args[i] = &commands[i][0];
			numArgs++;
		}
	}
	args[numArgs] = NULL;
	pid_t pid = fork();
	if(pid == 0){
			if(in = open(&inputfile[0], O_RDONLY))
			{
				dup2(in, 0);
				close(in);
			}
			else
			{
				cout << "Error opening file" << endl;
				return;
			}
			if(out = open(&outputfile[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR))
			{
				dup2(out, 1);		
				close(out);
			}
			else
			{
				cout << "Error opening file" << endl;
				return;
			}
			execvp(arg0, args);
	}
	else {
		int returnStatus;
		waitpid(pid, &returnStatus, 0);	
		if(returnStatus != 0)
		{		
			cout << "Command did not exit normally." << endl;
		}
	}
}

void handle_pipes(vector<vector<string> > commands) {
	int pipefd[commands.size()-1][2];
	int i = 0;
	for(i; i < commands.size()-1; i++)
	{
		pipe(pipefd[i]);
	}
	vector<char**> commands_as_cstrings;
	pid_t driverPid = fork();
	if(driverPid == 0)
	{
		i = commands.size() - 1;
		for(i; i >= 0; i--)
		{
			cout << "i = " << i << endl;
			char* args[commands[i].size()];
			int j = 0;
			int numArgs = 0;
			for(j; j < commands[i].size(); j++)
			{
				args[j] = &commands[i][j][0];
				numArgs++;
			}
			args[numArgs] = NULL;

			pid_t pid;

			
			if(i == 0) 
			{
				pid = 1;
			}
			else
			{
				pid = fork();
			}
			if(pid != 0)
			{	

				int x = 0;
				for(x; x < commands.size()-1; x++)
				{
					if(x != i && x != i-1){
						close(pipefd[x][0]);
						close(pipefd[x][1]);
						//cout << "closing pipe " << x << endl;
					}				
				}

				if(i != 0)  //handle redirection input
				{
					//cout << "Replacing standard input with pipe " << i - 1 <<"'s input" <<endl;
					//cout << "Closing output of pipe " << i - 1 << endl;			
					dup2(pipefd[i-1][0], 0);	
					close(pipefd[i-1][1]);
				}

				if(i != commands.size()-1) //handle redirecting output
				{
					//cout <<"replalcing standard output with pipe "<<i << "'s output" << endl;
					//cout << "Closing input of pipe " << i  << endl;
					//cout << "===============================" << endl;		

					dup2(pipefd[i][1], 1);
					if(i == 0){cout << "here" << endl;}
					close(pipefd[i][0]);

				}

				cout << "executing " << args[0] << endl;
				execvp(args[0],args);
			}

		}
	}
	else
	{
		int returnStatus;
		cout << "driver waiting..." << endl;
		waitpid(driverPid, &returnStatus, 0);	
		cout << "driver resuming" << endl;
		if(returnStatus != 0)
		{		
			cout << "Command did not exit normally." << endl;
		}
	}
}

int main(int argc, char** argv){
	string input;	
	string token_groups[25];
	bool testing = false;
	if(argv[1] != NULL){
		testing = string(argv[1]).compare("-t") == 0;		
	}	
	while(1){		
		if(!testing){
			cout << ">";
		}	
		getline(cin, input);
		if(input.size() > 100)
		{
			cout << "Error: cannot read lines of greater than 100 characters." << endl;
		}
		else{
			if(input.compare("exit") == 0 || input.compare("") == 0)
			{
				break;
			}
			vector<string> token_groups = tokenize(input, " | ");
			vector<vector<string> > commands;			
			int i = 0;
			bool valid = true;
			bool hasInputRedirect = false;
			bool hasOutputRedirect = false;

			for(i; i < token_groups.size(); i++)
			{				
				vector<string> command = tokenize(token_groups[i], " ");
				commands.push_back(command);
				int j = 0;
				for(j; j < command.size(); j++)
				{			
					if( !(validate_token(command[j]))){						
						if(token_groups.size() != 1 || (command[j].compare(">") != 0 && command[j].compare("<") != 0))
						{
							valid = false;
						}
						if(command[j].compare("<") == 0 )
						{
							if(hasInputRedirect || hasOutputRedirect || j == 0 || j != command.size() - 2)
							{
								if(j+2 < command.size()){
									if(command[j+2].compare(">") != 0)
									{
										valid = false;
									}
								}

							}
							else 
							{
								hasInputRedirect = true;
							}							
						}
						else if(command[j].compare(">") == 0)
						{
							if(hasOutputRedirect || j == 0 || j != command.size() - 2)
							{		
								valid = false;									
							}
							else
							{
								hasOutputRedirect = true;
							}
						}
					}					
				}
			}

			if(valid) {
				if(testing){			
					cout << input << " is a valid statement" << endl;	

				}
				if(commands.size() > 1){
					handle_pipes(commands);
				}
				else{
					handle_redirects(commands[0]);
				}
			}
			else {
				cout << "Invalid command" << endl;	
			}
			

		}
	}
	return 0;
}
