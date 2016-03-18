#include <pthread.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

vector<int> nums;

struct Barrier{
	pthread_cond_t cond;
	int running;
	pthread_mutex_t mutex;
}barrier;

struct ThreadArguments{
	int index;
	int length;
};

void *sort(void *args){
	struct ThreadArguments tArgs = *((ThreadArguments*)args);
	int i = 0;
	int leftIndex = tArgs.index;
	int rightIndex = (tArgs.index*2+tArgs.length)/2;
	int leftEnd = rightIndex;
	int rightEnd = tArgs.index+tArgs.length;
	int tempArray[tArgs.length];
	for(i; i < tArgs.length; i++){
		if((nums[leftIndex] < nums[rightIndex] || rightIndex == rightEnd) && leftIndex != leftEnd){
			tempArray[i] = nums[leftIndex];		
			leftIndex++;
		}
		else{
			tempArray[i] = nums[rightIndex];
			rightIndex++;
		}
	}
	i = tArgs.index;
	for(i; i < tArgs.index + tArgs.length; i++){
		nums[i] = tempArray[i - tArgs.index];
	}
	pthread_mutex_lock(&barrier.mutex); 
	barrier.running--;
	while(barrier.running){
		pthread_cond_wait(&barrier.cond,&barrier.mutex);
	}
	pthread_cond_broadcast(&barrier.cond);
	pthread_mutex_unlock(&barrier.mutex);
}

int main(){
	ifstream array("indata.txt");
	string allNums;
	getline(array, allNums);
	while(1){		
		int end = 0;
		while(1){
			if(allNums[end] == ' ' || allNums[end] == NULL){
				break;
			}
			end++;
		}
		int x;
		stringstream ss(allNums.substr(0,end));
		ss >> x;		
		nums.push_back(x);
		if(allNums[end] == NULL){
			break;
		}
		allNums = allNums.substr(end+1, allNums.length());
	}

	int j = 0;
	int total = nums.size();
	int iterations = log2(total);
	for(j; j < iterations; j++){
		int length = pow(2,j+1);
		int i = 0;
		pthread_mutex_init(barrier.mutex, NULL);
		pthread_cond_init(barrier.cond, NULL);
		barrier.running  = total/length;
		struct ThreadArguments args[total/length];	
		for(i; i < total; i+=length)
		{
			pthread_t id;
			args[i/length].index = i;
			args[i/length].length = length;
			if(i >= 508){
				int x = 1;
			}
			pthread_create(&id, NULL, sort, &args[i/length]);
		}
		pthread_mutex_lock(&barrier.mutex);
		while(barrier.running){
			pthread_cond_wait(&barrier.cond,&barrier.mutex);
		}
		pthread_mutex_unlock(&barrier.mutex);

	}
	int i = 0;
	for(i; i < total; i++){
		cout << nums[i];
		if(i != total - 1){
			cout << " ";
		}
	
	}
	cout << endl;
	return 0;
}
