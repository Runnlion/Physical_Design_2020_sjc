#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <algorithm>
#include "time.h"
using namespace std;

void prase_Data(string str, vector<pair<int, int>> &prased_data) {
	string v1, v2;
	bool is_v1_end = false;
	for (auto x : str)
	{
		if (x == ' ')
		{
			is_v1_end = true;
		}
		else
		{
			if (!is_v1_end) {
				v1 = v1 + x;
			}else{
				v2 = v2 + x;
			}
			
		}
	}
	int num1 = stoi(v1);
	int num2 = stoi(v2);
	prased_data.push_back(make_pair(num1, num2));
}

void log_histoty(int current_vertex, int next_vertex, vector<pair<int, int>>& out) {
	for (int i = 0; i < out.size(); i++) {
		if (out[i].second == next_vertex) {
			//a same vertex has been found
			printf("A same vertex has been found (%d,%d), existing(%d,%d)\n", current_vertex, next_vertex, out[i].first, out[i].second);
			return;
		}
	}
	out.push_back(make_pair(current_vertex, next_vertex));
}
int main(int argc, char* argv[]) {

	
	

	//Init some parameters

	ifstream File;
	string line;

	//Init essential computation parameters
	int vertex_num = 0;
	int start_point = 0;
	vector<string> data_in;
	vector<pair<int, int>> prased_data;
	vector<pair<int, int>> out;
	vector<int> queue;

	//init two time_t variable to calculate the time
	clock_t start, end; 
	

	File.open(argv[1]);
	if (!File) {

		cout << "Input invaild\n";
		return 0;
	}
	else {

		while (getline(File, line)) {
			data_in.push_back(line);
		}


	}
	std::string::size_type sz;

	cout << "Data_size " << data_in.size() << endl;
	vertex_num = stoi(data_in[0].substr(18, data_in[0].length() - 18), &sz);
	cout << "Vertex number =" << vertex_num << endl;

	start_point = stoi(data_in[1].substr(8, data_in[1].length() - 8), &sz);
	cout << "Strating Point =" << start_point << endl;
	
	//push back start point to queue[0]
	queue.push_back(start_point);

	for (int i = 0; i < data_in.size(); i++) {
		if (data_in[i].substr(0,5).compare(".edge")==0) {
			prase_Data(data_in[i].substr(6, data_in[i].length() - 6), prased_data);
		}
	}




	//The total interarion should be exactly the edges in .in files
	int total_process_num = 0;
	int total_edges = prased_data.size();

	cout << "total_edges=" << total_edges << endl;

	//Strat timer
	start = clock();

	while (true) {
		//Alpha fuction: change the order if I choose another source
		vector<pair<int, int>> temp_prased_data;
	    	for (int i=0; i<prased_data.size(); i++) 
			temp_prased_data.push_back(prased_data[i]); 

		for(int i=0;i<prased_data.size();i++){
			//change order if the data is not perfect		
			if(temp_prased_data[i].second == queue[0]){
				temp_prased_data[i].second = temp_prased_data[i].first;
				temp_prased_data[i].first = queue[0];
			}	
		}
		//find BFS for x == start_point 
		vector<int> temp;
		for (int i = 0; i < prased_data.size(); i++)
		{
			if (prased_data[i].first == queue[0]) {
				temp.push_back(prased_data[i].second);
			}
		}
		total_process_num += temp.size();

		sort(temp.begin(), temp.end());
		for (int i = 0; i < temp.size(); i++) {
			bool is_repeated = false;
			for (int j = 0; j < queue.size(); j++) {
				if (temp[i] == queue[j]) {
					is_repeated = true;
					break;
				}
			}
			if (!is_repeated) {
				queue.push_back(temp[i]);
			}

		}


		for (int i = 0; i < temp.size(); i++) {
			log_histoty(queue[0], temp[i],out);
		}

		if (total_process_num == total_edges) {
			break;
		}
		//dequeue
		queue.erase(queue.begin());
		temp_prased_data.clear();
		
	}
	//End timer
	end = clock();

	//Print out the result in the screen 
	for (int i = 0; i < out.size(); i++) {
		cout << out[i].first << " " << out[i].second << endl;
	}
	
	double time_taken = double(end - start) / double(CLOCKS_PER_SEC); 
	cout << "Time taken by program is : " << fixed << time_taken << " sec " << endl; 

	//Prepare some variables to save result
	ofstream output_File;
	ifstream check_File(argv[2]);

	//Checking whether the file already existed in given folder
	if(check_File){
		string Decision;
		Decision: ;
		cout<<"File Exists, Override? [Y/N]";
		getline(cin,Decision);
		if(Decision.substr(0,1).compare("Y")==0|Decision.substr(0,1).compare("y")==0){
			output_File.open(argv[2]);
			for (const auto& i : out)
				output_File << ".edge " << i.first << " " << i.second << endl;
			cout<<"Saved:"<<argv[2]<<endl;
			output_File.close();
		}else if(Decision.substr(0,1).compare("N")==0|Decision.substr(0,1).compare("n")==0){
			cout<<"Abort!"<<endl;
			output_File.close();
			return 0;
		}else{
			goto Decision;
		}	
	}
	else{
	output_File.open(argv[2]);
	for (const auto& i : out)
		output_File << ".edge " << i.first << " " << i.second << endl;
	cout<<"Saved:"<<argv[2]<<endl;
	output_File.close();
	}

	
}











