#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

vector<string> tokenize(string input, string delim){
	vector<string> token_groups;			
	int endItr = 0;
	int lastPos = 0;
	int pos = 0;
	int i = 0;
	while(1){
		pos = input.find(delim, 0);
		i++;
		if(pos == -1) {
			token_groups.push_back(input.substr(0, input.size()));
			break;
		}		
		token_groups.push_back(input.substr(0,pos));		
		input = input.substr(pos+delim.size(), input.size());
	}
	return token_groups;
}

int main(){
	string input;	
	string token_groups[25];
	while(1){
		cout << ">";
		getline(cin, input);
		if(input.size() > 100)
		{
			cout << "Error: cannot read lines of greater than 100 characters." << endl;
		}
		else{
			vector<string> token_groups = tokenize(input, " | ");
			int i = 0;
			for(i; i < token_groups.size(); i++)
			{
				cout << token_groups[i] << endl;
			}


		}
	}
	return 0;
}
