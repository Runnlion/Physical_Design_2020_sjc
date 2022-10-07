// Generate_BFS_Data.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <algorithm>
#include "time.h"
using namespace std;

int main(int argc, char* argv[])
{
    int vertex_number, data_number, starting_point = 0;
    string filename;
    if (argc > 3) {
        vertex_number = stoi(argv[1]);
        data_number = stoi(argv[2]);
        starting_point = stoi(argv[3]);
        filename = string(argv[4]);
    }
    else {
        return -1;
    }
    ofstream output_File;
    output_File.open(argv[4]);
    output_File << ".number_of_vertex " << vertex_number << endl;
    output_File << ".source " << starting_point << endl;

    for (int i = 0; i < data_number; i++) {
        output_File << ".edge " << (rand() % vertex_number) << " " << (rand() % vertex_number ) << endl;
    }
    output_File.close();

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
