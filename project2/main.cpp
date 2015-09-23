#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sys/types.h>
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


	for(i; i < commands.size(); i++) {
		if(commands[i].compare("<") == 0){
			inputfile = commands[i+1];
			cout << inputfile << "is input file" << endl;

		}
		if(commands[i].compare(">") == 0){
			outputfile = commands[i+1];
			cout << outputfile << "is output file" << endl;

		}
	}
	in = open(&inputfile[0], O_RDONLY);
	out = open(&outputfile[0], O_WRONLY);

	dup2(in, 0);

	dup2(out, 1);

	close(in);
	close(out);

	char * arg0 = &commands[0][0];
	char ** args;
	i = 0;
	for(i;i < commands.size(); i++)
	{
		args[i] = &commands[i][0];
	}
	args[commands.size()] = NULL;	

	execvp(arg0, args);

}

void handle_pipes(vector<vector<string> > commands) {

}

int main(int argc, char **argv){
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
				if(testing){	
					cout << input << " is an invalid statement" << endl;	
				}
			}
			

		}
	}
	return 0;
}
