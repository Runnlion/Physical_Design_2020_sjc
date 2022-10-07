#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <algorithm>
#include "time.h"
using namespace std;
//Define some global flags
bool show_Warning = false;
bool mute_Result = false;
void parse_Data(string str, vector<pair<int, int>> &parsed_data,vector<bool> &is_Traversed) {
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
	parsed_data.push_back(make_pair(num1, num2));
	is_Traversed.push_back(false);
}

void log_histoty(int current_vertex, int next_vertex, vector<pair<int, int>>& out) {
	for (int i = 0; i < out.size(); i++) {
		if (out[i].second == next_vertex) {
			//a same vertex has been found
			if(show_Warning)
				printf("A same vertex has been found (%d,%d), existing(%d,%d)\n", current_vertex, next_vertex, out[i].first, out[i].second);
			return;
		}
	}
	out.push_back(make_pair(current_vertex, next_vertex));
}

int main(int argc, char* argv[]) {
	
	for (int i = 0; i < argc;i++) {
		if ((string(argv[i]) == "-h") | (string(argv[i]) == "-help")) {
			printf("\nHelp Information:\n");
			printf("This Program will compute the BFS using given data\n\n");
			printf("[Command Structure]\n");
			printf("  BFS  <input_file.in>  <output_file.out> <other command>\n");
			printf("  -h -help Help Information\n");
			printf("  -w Enable repeated warning [default = false]\n");
			printf("  -m Disable result [deafault = true]\n");
			return 0;
		}
		if ((string(argv[i]) == "-w")) {
			show_Warning = true;
		}
		if ((string(argv[i]) == "-m")) {
			mute_Result = true;
		}
	}

		
	//Init some parameters
	ifstream File;
	string line;
	
	//Init essential computation parameters
	int vertex_num = 0;
	int start_point = 0;
	vector<string> data_in;
	vector<pair<int, int>> parsed_data;
	vector<bool> is_Traversed;
	vector<pair<int, int>> out;
	vector<int> queue;
	//
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

	cout << "Data_size =\t\t" << data_in.size() << endl;
	vertex_num = stoi(data_in[0].substr(18, data_in[0].length() - 18), &sz);
	cout << "Vertex number =\t\t" << vertex_num << endl;

	start_point = stoi(data_in[1].substr(8, data_in[1].length() - 8), &sz);
	cout << "Strating Point =\t" << start_point << endl;
	



	//push back start point to queue[0]
	queue.push_back(start_point);

	//parse data into a vector which contains the pure integer
	for (int i = 0; i < data_in.size(); i++) {
		if (data_in[i].substr(0,5).compare(".edge")==0) {
			parse_Data(data_in[i].substr(6, data_in[i].length() - 6), parsed_data, is_Traversed);
		}
	}



	//The total interarion should be exactly the edges in .in files
	//The max iteration number should equal to number of edges
	int total_process_num = 0;
	int total_edges = parsed_data.size();

	cout << "total_edges=\t\t" << total_edges << endl;

	//Init Processing flag
	bool show_Process = false;
	if(total_edges>5000)
		show_Process = true;
	//Strat timer
	start = clock();
	
	///
	///Algorithm: From start point which read form header of file, find whether there have some inverted data
	/// for example, we start from 0. So, zero is the strating point which need to spread outside. If there has 
	/// a data like 1,0 (from 1 to 0), we need to invert it and set the is Traversed flag as true. If we do the 
	/// same things, we don't need to change the order when the flag is true, cause it has been computed already.
	/// 
	/// 
	while (true) {
		/// More general function
		/// change order if the left side of the data contains searching point
		
		for (int i = 0; i < parsed_data.size(); i++)
		{
			if ((parsed_data[i].second == queue[0])&(is_Traversed[i]==false)) {
				//change order if equals
				int temp = parsed_data[i].second;
				parsed_data[i].second = parsed_data[i].first;
				parsed_data[i].first = temp;
				is_Traversed[i] = true;
			}
		}



		//find BFS firstly from x == start_point 
		vector<int> temp; // record the all related edges 

		for (int i = 0; i < parsed_data.size(); i++)
		{
			if (parsed_data[i].first == queue[0]) {
				temp.push_back(parsed_data[i].second);
				is_Traversed[i] = true;
			}
		}
		//The temp.size() will return the all edges found which related to strating / relating point.
		total_process_num += temp.size();

		sort(temp.begin(), temp.end()); //Sort temp value ascendingly. This is a VC 11 feature, i'am not very sure. 
		
		for (int i = 0; i < temp.size(); i++) {
			// Check whether the vertex has been added into queue
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

		// Record the traverse history. Of course, if the temp[i] is repeated, throw it.
		for (int i = 0; i < temp.size(); i++) {
			log_histoty(queue[0], temp[i],out);
		}

		if (total_process_num == total_edges) {
			break;
		}
		//dequeue
		queue.erase(queue.begin());
		
		if(show_Process){
			printf("\r%i/%i",total_process_num,total_edges);	
		}
		
	}
	cout<<endl;
	//End timer
	end = clock();

	//Print out the result in the screen 
	if (!mute_Result) {
		for (int i = 0; i < out.size(); i++) {
			cout << out[i].first << " " << out[i].second << endl;
		}	
	}

	
	double time_taken = double(end - double(start)) / double(CLOCKS_PER_SEC); 
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
		if((Decision.substr(0,1).compare("Y")==0)|(Decision.substr(0,1).compare("y")==0)){
			output_File.open(argv[2]);
			for (const auto& i : out)
				output_File << ".edge " << i.first << " " << i.second << endl;
			cout<<"Saved:"<<argv[2]<<endl;
			output_File.close();
		}else if((Decision.substr(0,1).compare("N")==0)|(Decision.substr(0,1).compare("n")==0)){
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











