#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>

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

bool validate_token_group(string token_group) {
	return true;
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
			char *args[] = {"ls",NULL};
			execvp("ls",token_groups);
			int i = 0;
			bool valid = true;
			for(i; i < token_groups.size(); i++)
			{
				
				if( !(validate_token_group(token_groups[i])) ){
					valid = false;
				}
			}
			if(valid) {
				if(testing){			
					cout << input << " is a valid statement" << endl;	
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
