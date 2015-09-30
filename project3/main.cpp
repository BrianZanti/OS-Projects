#include <pthread.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct threadArguments {
	int* nums;
	int index;
	bool* error;
};

void *checkRow(void *args)
{	
	bool check[9] = {false,false,false,false,false,false,false,false,false};
	struct threadArguments *tArgs = (threadArguments*)args;

	int * row = tArgs->nums;
	int j = 0;

	for(j; j < 9; j++)
	{
		check[row[j]-1] = true;
	}
	j = 0;
	vector<int> missingNums;
	for(j; j < 9;j++)
	{
		if(!check[j])		
		{
			missingNums.push_back(j+1);
		}
	}
	if(missingNums.size())
	{
		*(tArgs->error) = true;
		cout << "Missing the number";
		if(missingNums.size() > 1)
		{
			cout << "s";
		}
		int i = 0;
		for(i; i < missingNums.size(); i++)
		{
			cout << " " <<  missingNums[i];
		}
		cout << " from row " << tArgs->index+1 << endl;
	}
	pthread_exit(NULL);
}

void *checkColumn(void *args)
{	
	bool check[9] = {false,false,false,false,false,false,false,false,false};
	struct threadArguments *tArgs = (threadArguments*)args;

	int * column = tArgs->nums;
	int j = 0;
	for(j; j < 9; j++)
	{
		check[column[j*9]-1] = true;
	}
	j = 0;
	vector<int> missingNums;
	for(j; j < 9;j++)
	{
		if(!check[j])		
		{
			missingNums.push_back(j+1);
		}
	}
	if(missingNums.size())
	{
		*(tArgs->error) = true;
		cout << "Missing the number";
		if(missingNums.size() > 1)
		{
			cout << "s";
		}
		int i = 0;
		for(i; i < missingNums.size(); i++)
		{
			cout << " " <<  missingNums[i];
		}
		cout << " from column " << tArgs->index+1 << endl;
	}
	pthread_exit(NULL);
}

void *checkSubgrid(void *args)
{	
	bool check[9] = {false,false,false,false,false,false,false,false,false};
	struct threadArguments *tArgs = (threadArguments*)args;

	int * subgrid = tArgs->nums;
	int j = 0;
	for(j; j < 9; j++)
	{
		check[subgrid[(j/3)*9+j%3]-1] = true;
	}
	j = 0;
	vector<int> missingNums;
	for(j; j < 9;j++)
	{
		if(!check[j])		
		{
			missingNums.push_back(j+1);
		}
	}
	if(missingNums.size())
	{
		*(tArgs->error) = true;
		cout << "Missing the number";
		if(missingNums.size() > 1)
		{
			cout << "s";
		}
		int i = 0;
		for(i; i < missingNums.size(); i++)
		{
			cout << " " <<  missingNums[i];
		}
		int x = tArgs->index;
		cout << " from the subgrid containing rows " << (x/3)*3 +1 << " " << (x/3)*3 +2 << " " <<  (x/3)*3 +3;
		cout << " and the columns " << (x%3)*3+1<< " " <<  (x%3)*3+2<<  " " << (x%3)*3+3 << endl;
	}
	pthread_exit(NULL);
}


int main()
{
	ifstream solution("sudoku.txt");
	string nums;
	getline(solution, nums);
	int i = 0;
	int grid[9][9];
	for(i; i < 9; i++)
	{
		int j = 0;
		for(j; j < 9; j++)
		{
			grid[i][j] = nums[(i*9+j)*2]-'0';
		}
	}
	
	pthread_t ids[27];
	struct threadArguments args[27];
	i = 0;
	bool x = false;
	bool* error = &x;	
	for(i; i < 9; i++)
	{
		args[i].nums = &grid[i][0];	
		args[i].index = i;
		args[i].error = error;
		pthread_create(&ids[i], NULL, checkRow, &args[i]);
	}
	for(i; i < 18; i++)
	{
		args[i].nums = &grid[0][i-9];
		args[i].index = i-9;
		args[i].error = error;
		pthread_create(&ids[i], NULL, checkColumn, &args[i]);
	}
	for(i; i < 27; i++)
	{
		args[i].nums = &grid[((i-18)/3)*3][((i-18)%3)*3];
		args[i].index = i-18;
		args[i].error = error;
		pthread_create(&ids[i], NULL, checkSubgrid, &args[i]);
	}

	i=0;
	for(i; i < 27; i++)
	{
		pthread_join(ids[i], NULL);
	}
	if(!*error)
	{
		cout << "Valid Solution!" << endl;
	}
	return 0;
}
