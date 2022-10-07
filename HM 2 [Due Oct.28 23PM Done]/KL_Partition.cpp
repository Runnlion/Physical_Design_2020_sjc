// KL_Partition.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include "time.h"
#include <cmath> 
#include "KL_Partition.h"

using namespace std;
void print_computation_table(vector<vector<int>> table) {
    for (size_t i = 0; i < table.size(); i++)
    {
        printf("Cell %d\t", i + 1);
        for (size_t j = 0; j < table.size(); j++)
        {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }
}
void print_Partition(vector<int> partition) {
    printf("\nPartition Size: %d\nInit:", partition.size());
    for (size_t i = 0; i < partition.size(); i++)
    {
        printf("%d", partition[i]);
    }
    printf("\n");
}
void set_Init_Partition(vector<int> &partition,int &max_cell,bool is_Random) {
    bool is_even = !(max_cell % 2);
    srand((int)time(0));
    int first_part = ceil(max_cell / 2.0);
    if (!is_Random) {
            for (int i = 0; i < first_part; i++) {
                partition.push_back(0);
            }
            for (int i = 0; i < max_cell - first_part; i++) {
                partition.push_back(1);
            }
    }
    else {

        for (size_t i = 0; i < max_cell; i++)
            partition.push_back(0);
        vector<int> rand_index;
        
        for (size_t i = 0; i < first_part; i++)
        {
            int rand_num = (rand() % max_cell);
            if (partition[rand_num] == 0 ){
                partition[rand_num] = 1;
            }
            else
                i = i - 1;
        }
    }

    if (!is_even) {
        partition.push_back(0);
        max_cell += 1;
    }

}
void balance_partition(vector<int>& partition, int max_cell) {

}
int calculate_cost(vector<int> partition, vector<vector<int>> table) {
    int cost = 0;
    for (size_t i = 0; i < partition.size(); i++)
    {
        for (size_t j = 0; j < i+1; j++)
        {
            if (partition[i] != partition[j]) 
                cost = cost + table[i][j];
        }
    }
    return cost;
}
int calculate_cost_two_element(vector<int> partition, vector<vector<int>> table, int cell_1, int cell_2) {
    return table[cell_1][cell_2];
}
vector<int> calculate_Diff(vector<int> partition, vector<vector<int>> table) {
    vector<int> D;
    for (size_t i = 0; i < partition.size(); i++) {
        int Ei = 0, Ii = 0;
        //vector<int>::iterator it = find(ignore.begin(), ignore.end(), i);
            //No ignore term
            for (size_t j = 0; j < partition.size(); j++)
            {
               // vector<int>::iterator it1 = find(ignore.begin(), ignore.end(), i);
                
                    if (partition[i] != partition[j])
                        Ei = Ei + table[i][j];
                    else
                        Ii = Ii + table[i][j];

            }

            D.push_back(Ei - Ii);
    }
    return D;
}
void update_Diff(vector<int> &Diff, vector<vector<int>> table,vector<int> sim_P_table,int A,int B) {
    Diff[A] = -1000000;
    Diff[B] = -1000000;
    for (size_t i = 0; i < Diff.size(); i++)
    {   
        if ((i != A) & (i != B)) {
            if (sim_P_table[i] == 0) {
                Diff[i] = Diff[i] + 2 * table[i][A] - 2 * table[i][B];
            }
            else {
                Diff[i] = Diff[i] + 2 * table[i][B] - 2 * table[i][A];
            }
        }
        
    }
}


int main(int argc, char* argv[])
{   
    /// <summary>
    /// Main Program of K-L Partition
    /// </summary>
    /// <param name="argc"></param>
    /// <param name="argv"></param>
    /// <returns></returns>
    bool show_table = false;
    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-h") {
            //help meun
        }
        if (string(argv[i]) == "-st") {
            //help meun
            show_table = true;
        }
    }
    //Read Data Steram
    ifstream File;
    string line;//Temp variavle for reading
    vector<string> data_in;//Container to store input data
    vector<pair<int, vector<int>>> cells;

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

    //Before we store data in to some structure
    //we have to convert string into data
    for (int iter = 0; iter < data_in.size(); iter++) {
        int current_net;
        vector<int> temp_cell;
        if (data_in[iter].substr(0, 5).compare("NET n") == 0) {
            if (data_in[iter].substr(6, 1).compare(" ") == 0) {
                current_net = stoi(data_in[iter].substr(5, 1));
                data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 7);
            }
            else if (data_in[iter].substr(7, 1).compare(" ") == 0) {
                current_net = stoi(data_in[iter].substr(5, 2));
                data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 8);
            }
            else if (data_in[iter].substr(8, 1).compare(" ") == 0) {
                current_net = stoi(data_in[iter].substr(5, 3));
                data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 9);
            }
            else if (data_in[iter].substr(9, 1).compare(" ") == 0) {
                current_net = stoi(data_in[iter].substr(5, 4));
                data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 10);
            }
            else if (data_in[iter].substr(10, 1).compare(" ") == 0) {
                current_net = stoi(data_in[iter].substr(5, 5));
                data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 11);
            }
            else if (data_in[iter].substr(11, 1).compare(" ") == 0) {
                current_net = stoi(data_in[iter].substr(5, 6));
                data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 12);
            }
            else {
                printf("Net Too large!");
                return 0;
            }
        }
        while (data_in[iter].substr(0, 1).compare(";") != 0) {
            if (data_in[iter].substr(0, 1).compare("c") == 0) {
                if (data_in[iter].substr(2, 1).compare(" ") == 0) {
                    temp_cell.push_back(stoi(data_in[iter].substr(1, 1)));
                    data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 3);
                }
                else if (data_in[iter].substr(3, 1).compare(" ") == 0) {
                    temp_cell.push_back(stoi(data_in[iter].substr(1, 2)));
                    data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 4);
                }
                else if (data_in[iter].substr(4, 1).compare(" ") == 0) {
                    temp_cell.push_back(stoi(data_in[iter].substr(1, 3)));
                    data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 5);
                }
                else if (data_in[iter].substr(5, 1).compare(" ") == 0) {
                    temp_cell.push_back(stoi(data_in[iter].substr(1, 4)));
                    data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 6);
                }
                else if (data_in[iter].substr(6, 1).compare(" ") == 0) {
                    temp_cell.push_back(stoi(data_in[iter].substr(1, 5)));
                    data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 7);
                }
                else if (data_in[iter].substr(7, 1).compare(" ") == 0) {
                    temp_cell.push_back(stoi(data_in[iter].substr(1, 6)));
                    data_in[iter].erase(data_in[iter].begin(), data_in[iter].begin() + 8);
                }
                else {
                    printf("Cells Too large!");
                    return 0;
                }
        }

        }
        cells.push_back(make_pair(current_net, temp_cell));
    }

    //We have to define some data structure to store data
    //First easy method: create a N*M size of array to represent 0 and 1
    //which means not connected and connected
    int max_cell=0;
    for (int i = 0; i < cells.size(); i++) {
        //printf("Net:%d Cells:", cells[i].first);
        for (int j = 0; j < cells[i].second.size(); j++) {
            //printf("%d ", cells[i].second[j]);
            if (cells[i].second[j] > max_cell)
                max_cell = cells[i].second[j];
        }
        //printf("\n");
    }
    cout << "Max:" << max_cell << endl;

    //Define storage unit
    vector<int> partition;
    vector<int> G1;
    vector<int> G2;
    vector<vector<int>> computation_table;

    //Set Partition init. state
    set_Init_Partition(partition, max_cell,true);
    balance_partition(partition, max_cell);
    //Convert cell index into G1 and G2
    for (size_t i = 0; i < partition.size(); i++)
    {
        if (partition[i] == 0)
            G1.push_back(i);
        else
            G2.push_back(i);
    }
    //Init Computation table
    for (int i = 0; i < max_cell; i++) {
        vector<int> temp_cell;
        for (int j = 0; j < max_cell; j++) {
            temp_cell.push_back(0);
        }
        computation_table.push_back(temp_cell);
    }
    for (int iter = 0; iter < cells.size(); iter++) {
        for (int j = 0; j < cells[iter].second.size() - 1; j++) {
            computation_table[cells[iter].second[j] - 1][cells[iter].second[j + 1] - 1] = 1;
            computation_table[cells[iter].second[j + 1] - 1][cells[iter].second[j] - 1] = 1;
        }
    }
	clock_t start, end;
	start = clock();
	for (size_t i = 0; i < 10; i++)
	{
		calculate_cost(partition,computation_table);

	}
	end = clock();
        double time_taken1 = double(end - double(start)) / double(CLOCKS_PER_SEC);
	cout << "Time taken by program is : " << fixed << time_taken1/10.0 << " sec " << endl;
    /// <summary>
    /// START OF K_L Algorithm
    /// </summary>
    /// <param name="argc"></param>
    /// <param name="argv"></param>
    /// <returns></returns>
    /// 
    vector<int> ignored_table;
    printf("Cost Before Change = %d\n", calculate_cost(partition, computation_table));
        bool Exit_flag = false;
        int counter = 0;
        vector<int> simulated_Partition;
        vector<pair<int, int>> cell_pairs;
        vector<int> gains;
        vector<int> D = calculate_Diff(partition, computation_table);
        copy(partition.begin(),partition.end(), back_inserter(simulated_Partition)); // The simulated_Prartition reperesnt the situatuon 
        
        //init two time_t variable to calculate the time
        //Strat timer
        start = clock();

        while (!Exit_flag) {
            //for (size_t i = 0; i < D.size(); i++)
            //{
            //    printf("%d\n", D[i]);
            //}

            int gain = -1000000; //G1:Partition = 0 G2 =1 
            int G1_Index, G2_Index;
            //Find max gain and index
            for (size_t i = 0; i < G1.size(); i++)
            {   //G1 layer(Left)
                for (size_t j = 0; j < G2.size(); j++)
                {   
                    //G2 layer(Right)
                    
                    int _gain = D[G1[i]] + D[G2[j]] - 2 * computation_table[G1[i]][G2[j]];
                    //printf("Inner Index 1: %d Index 2: %d Gain=%d\n", G1[i], G2[j], _gain);
                    if (_gain > gain) {
                        gain = _gain;
                        G1_Index = G1[i];
                        G2_Index = G2[j];
                    }
                    else {
                        
                    }
                }
            }
            if(gain<=0)
                break;

            //Print whole cells network graph
            //print_computation_table(computation_table);
            printf("Index 1: %d Index 2: %d Gain = %d\n", G1_Index, G2_Index, gain);

            //record the locked pair 
            cell_pairs.push_back(make_pair(G1_Index, G2_Index));
            gains.push_back(gain);

            //Add these two index into ignored table (locked)
            ignored_table.push_back(G1_Index);
            ignored_table.push_back(G2_Index);

            //Change the order by changing the simulated order 
            if (simulated_Partition[G1_Index] == 0)
                simulated_Partition[G1_Index] = 1;
            else
                simulated_Partition[G1_Index] = 0;
            if (simulated_Partition[G2_Index] == 0)
                simulated_Partition[G2_Index] = 1;
            else
                simulated_Partition[G2_Index] = 0;


            //Recompute(update) Diff
            update_Diff(D, computation_table, simulated_Partition, G1_Index, G2_Index);
            counter += 2; //update the counter 
            if (counter == partition.size())
                Exit_flag = true;
        }
        //we have a series of pairs and total gain
        // we need to determind whether we change or not using given gain
        //the pairs should store in the vector<pair<int,int>> which each element reperesnt the index of the partition
        
        for (size_t i = 0; i < cell_pairs.size(); i++)
        {
            //change partition table order 
            int temp = partition[cell_pairs[i].first];
            partition[cell_pairs[i].first] = partition[cell_pairs[i].second];
            partition[cell_pairs[i].second] = temp;
        }
        end = clock();
        printf("Cost After Change = %d\n", calculate_cost(partition, computation_table));
        double time_taken = double(end - double(start)) / double(CLOCKS_PER_SEC);
        cout << "Time taken by program is : " << fixed << time_taken << " sec " << endl;

        if (show_table) {
            print_computation_table(computation_table);
        }

        ofstream output_File;
        string filename;
        if (argc >= 2) {
            filename = argv[2];
        }
        else {
            filename = string(argv[1])+".out";
        }

        ifstream check_File(filename);

        if (check_File) {
            string Decision;
            string G1, G2;
            int counter_G1=0, counter_G2=0;
        Decision:;
            cout << "File Exists, Override? [Y/N]";
            getline(cin, Decision);
            if ((Decision.substr(0, 1).compare("Y") == 0) | (Decision.substr(0, 1).compare("y") == 0)) {
                output_File.open(filename);

                //C1
                for (size_t i = 0; i < partition.size(); i++)
                {
                    if (partition[i] == 0) {
                        G1 = G1 + "c" + to_string(i)+" ";
                        counter_G1 += 1;
                    }
                    else {
                        G2 = G2 + "c" + to_string(i) + " ";
                        counter_G2 += 1;
                    }
;                }
                G1 += ";\n";
                G2 += ";\n";
                output_File << "Cutsize = " << calculate_cost(partition, computation_table) << endl;
                output_File << "G1 " << counter_G1 << endl;
                output_File << G1 << endl;
                output_File << "G2 " << counter_G2 << endl;
                output_File << G2 << endl;
                cout << "Saved:" << filename << endl;
                output_File.close();
            }
            else if ((Decision.substr(0, 1).compare("N") == 0) | (Decision.substr(0, 1).compare("n") == 0)) {
                cout << "Abort!" << endl;
                output_File.close();
                return 0;
            }
            else {
                goto Decision;
            }
        }
        else {
            string G1, G2;
            int counter_G1 = 0, counter_G2 = 0;
            output_File.open(filename);
            for (size_t i = 0; i < partition.size(); i++)
            {
                if (partition[i] == 0) {
                    G1 = G1 + "c" + to_string(i) + " ";
                    counter_G1 += 1;
                }
                else {
                    G2 = G2 + "c" + to_string(i) + " ";
                    counter_G2 += 1;
                }
            }

            G1 += ";\n";
            G2 += ";\n";
            output_File << "Cutsize = " << calculate_cost(partition, computation_table) << endl;
            output_File << "G1 " << counter_G1 << endl;
            output_File << G1 << endl;
            output_File << "G2 " << counter_G2 << endl;
            output_File << G2 << endl;
            cout << "Saved:" << filename << endl;
            output_File.close();
        }


}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
