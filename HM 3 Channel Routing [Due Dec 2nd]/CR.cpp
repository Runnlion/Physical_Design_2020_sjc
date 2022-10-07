#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include "time.h"
#include <cmath> 
#include <stdlib.h>

#define ERR_LOADFILE -1
#define ERR_DATA_SIZE -2
#define ERR_UNKNOWN -3
#define ERR_HC_CHECK -4
#define ERR_VC_CHECK -5
#define UPDATE_GRID_ERROR -6
#define NOT_OCCUPIED -7
#define PIN_NOT_FOUND -8

#define SUCCESS 1
#define OCCUPIED 3
#define HOMO_NETWORK 4
#define PIN_BOTTOM 5
#define PIN_TOP 6

#define LAYER_METAL_1 7
#define LAYER_METAL_2 8

#define VCG_IGNORED 9

#define ROUTEABLE 10
#define UNROUTEABLE 11

#define DOGLEG_ENABLED 12
#define DOGLEG_DISABLED 13
#define DOGLEG_NOT_ASSIGNED 14
#define SA_SIZE 5000
using namespace std;

int Dogleg = DOGLEG_NOT_ASSIGNED;
int input_file_location = 0;
int output_file_location = 0;
int signed net_number = 0;
int signed channel_length = 0;
int SA_Size = 5000;
bool Output_Log = false;

vector<pair<vector<int>,vector<int>>> Nets; //Top,Bottom
vector<pair<int,int>> TBN; //Top Bottom network number graph (TOP,BOTTOM)
vector<pair<int,int>> VCG;
vector<pair<int,int>> NETWORK_IO_NUM;
vector<pair<int,int>> HCG;



vector<int> Net_Index;

vector<string> Program_Log;
vector<pair<int,int>> Net_Grid; //Point nunbe, network number (Metal1, Metal2)
void print_Network(vector<pair<int,int>> TBN);
int parse_raw_data(vector<string> Raw_Data);
int find_pin(int Pin_Type,int current_pin);

struct wire{
    int num;
    bool rise;//1 stands for rise 0 stands for fall
    int status;//start 2 end 1 mid 0
    wire(){num = 0;rise = false;status = 0;}
};

struct re{
    wire** h;
    wire** v;
};

re extend_track(wire** h,wire** v,int track,int wide,bool tb);

int main(int argc , char** argv)

{   //PART 1: Init argument command
    for(int iter = 0;iter<argc;iter++){
        //printf("Iter:%d\tCMD:",iter);
        //cout<<string(argv[iter])<<endl;
        if(string(argv[iter])=="-h"){
            //show help info and return
            printf("This is a test help text!\n");
        }
        //We assume the related file should behind the I/O argument command
        if(string(argv[iter])=="-i"){
            input_file_location = iter+1;
        }
        if(string(argv[iter])=="-o"){
            output_file_location = iter+1;
        }
        if(string(argv[iter])=="-l"){
            Output_Log = true;
        }
        if(string(argv[iter])=="-dl"){
            Dogleg = DOGLEG_ENABLED;
        }
        if(string(argv[iter])=="-ndl"){
            Dogleg = DOGLEG_DISABLED;
        }
        if(string(argv[iter])=="-SA"){
            SA_Size = stoi(argv[iter + 1]);
        }

    }

    //PART 2: Read file stream and parse Raw data 
    ifstream File;
    vector<string> Raw_Data;//Container to store input data
    File.open(argv[input_file_location]);
    if(!File){
        printf("Error in Loading input file!");
        //LOG OUTPUT FUCTION
        return ERR_LOADFILE;
    }
    else {
        string line;
        while (getline(File, line)) {
        Raw_Data.push_back(line);
        }
        if(Raw_Data.size()<2){
            return ERR_DATA_SIZE;
        }
    }

    parse_raw_data(Raw_Data);

    print_Network(TBN);



    char temp[256];
    string path = "./Data.txt";
    ifstream ifs(path);
	int index;
	for(index=0;index<256;index++){
		if(path[index] == '.'){
			break;
		}
		else{
			temp[index]=path[index]; 
		}
	}
	temp[index++] = '.';
	temp[index++] = 'o';
	temp[index++] = 'u';
	temp[index++] = 't';
    printf("1\n");
    ofstream ofs("Data.out");
    vector <int> all,record,record1;
    wire **h,**v;
    re temp_re;
    int temp_i,track(2),wide,index_column(0),wire_num(2),temp_i1,index_row;
    bool temp_t,temp_b;
        printf("2\n");

    cout<<record1.size()<<endl;
    while(!ifs.eof()){
        temp_b = true;
        ifs>>temp_i;
        cout<<temp_i<<endl;
        all.push_back(temp_i);
        cout<<record1.size()<<endl;
        for(int i=0;i<record1.size();i++){
            if(record1[i] == temp_i)
                temp_b = false;
        }
        if(temp_b){
            record1.push_back(temp_i);
        }
    }

    wide = all.size()/2;
    h = new wire*[track];
    v = new wire*[track];
    for(int i=0;i<track;i++){
        h[i] = new wire[wide+record1.size()];
        v[i] = new wire[wide+record1.size()];
    }
    for(int i=0;i<wide;i++){
        h[0][i].num = all[i];
        h[1][i].num = all[i+wide];
        v[0][i].num = all[i];
        v[1][i].num = all[i+wide];
    }
    for(int i=0;i<wide;i++){
        temp_t = false; temp_b = false;
        for(int k=0;k<record.size();k++){
            if(h[0][i].num==record[k]){temp_t = true;}
            if(h[1][i].num==record[k]){temp_b = true;}
        }
        if(!temp_t){record.push_back(h[0][i].num);h[0][i].status = 2;}
        if(!temp_b){record.push_back(h[1][i].num);h[1][i].status = 2;}
        for(int j=i+1;j<wide && h[0][i].num;j++){
            if(h[0][i].num == h[0][j].num){
                h[0][i].rise = 1;
                break;
            }
            else if(h[0][i].num == h[1][j].num){
                h[0][i].rise = 0;
                break;
            }
            else if(j==wide-1){h[0][i].status = 1;}
        }
        for(int j=i+1;j<wide && h[1][i].num;j++){
            if(h[1][i].num == h[0][j].num){
                h[1][i].rise = 1;
                break;
            }
            else if(h[1][i].num == h[1][j].num){
                h[1][i].rise = 0;
                break;
            }
            else if(j==wide-1){h[1][i].status = 1;}
        }
        h[0][wide-1].status = 1;
        h[1][wide-1].status = 1;
    }
    wide+=record1.size();

    //main algorithm////////////////////////////////////////////////////////////////////////
    while(index_column != wide){
        if(h[0][index_column].num){
            if(h[0][index_column].status == 2){
                if(track == wire_num){
                    temp_re = extend_track(h,v,track,wide,1);
                    track++; wire_num++;
                    h = temp_re.h; v = temp_re.v;
                    h[1][index_column] = h[0][index_column];
                    v[1][index_column] = v[0][index_column];
                }
                else{
                    wire_num++;
                    for(int i=0;i<track;i++){
                        if(!h[i][index_column].num){
                            h[i][index_column] = h[0][index_column];
                            for(int j=0;j<=i;j++){
                                v[j][index_column] = v[0][index_column];
                            }
                            break;
                        }
                    }
                }
            }
            else{
                for(int i=1;i<track;i++){
                    if(h[i][index_column].num == h[0][index_column].num){
                        h[i][index_column] = h[0][index_column];
                        for(int j=0;j<=i;j++){
                            v[j][index_column] = v[0][index_column];
                        }
                        break;
                    }
                }
                //wm+=h[track-1][index_column].status;
            }
        }

        if(h[track-1][index_column].num){
            if(h[track-1][index_column].status == 2){
                if(track == wire_num){
                    temp_re = extend_track(h,v,track,wide,0);
                    track++; wire_num++;
                    h = temp_re.h; v = temp_re.v;
                    h[track-2][index_column] = h[track-1][index_column];
                    v[track-2][index_column] = v[track-1][index_column];
                }
                else{
                    wire_num++;
                    for(int i=track-1;i>=0;i--){
                        if(!h[i][index_column].num){
                            h[i][index_column] = h[track-1][index_column];
                            for(int j=track-1;j>=i;j--){
                                v[j][index_column] = v[track-1][index_column];
                            }
                            break;
                        }
                    }
                }
            }
            else{
                temp_b = true;
                for(int i=track-2;i>=0;i--){
                    if(h[i][index_column].num == h[track-1][index_column].num){
                        temp_i = i;
                        for(int j=track-2;j>=i;j--){
                            if(v[j][index_column].num){
                                temp_b = false;
                            }
                        }
                        break;
                    }
                }
                if(temp_b){
                    h[temp_i][index_column] = h[track-1][index_column];
                    //wm+=h[track-1][index_column].status;
                    for(int i=track-2;i>=temp_i;i--){
                        v[i][index_column] = v[track-1][index_column];
                    }
                }
                else if(track == wire_num){
                    temp_re = extend_track(h,v,track,wide,0);
                    track++; wire_num++;
                    h = temp_re.h; v = temp_re.v;
                    h[track-2][index_column] = h[track-1][index_column];
                    v[track-2][index_column] = v[track-1][index_column];
                }
                else{
                    temp_t = true;
                    for(int i=track-2;i>=0;i--){
                        if(!h[i][index_column].num){
                            temp_i1 = i;
                            for(int j=track-2;j>=i;j--){
                                if(v[j][index_column].num){
                                    temp_t = false;
                                }
                            }
                            break;
                        }
                    }
                    if(temp_t){
                        for(int i=track-2;i>=temp_i1;i--){
                            v[i][index_column] = v[track-1][index_column];
                        }
                        h[temp_i1][index_column] = h[track-1][index_column];
                    }
                    else{
                        temp_re = extend_track(h,v,track,wide,0);
                        track++; wire_num++;
                        h = temp_re.h; v = temp_re.v;
                        h[track-2][index_column] = h[track-1][index_column];
                        v[track-2][index_column] = v[track-1][index_column];
                    }
                }
            }
        }

        for(int i=1;i<track-1;i++){
            temp_b = true;
            if(h[i][index_column].num){
                if(h[i][index_column].status == 1){
                    for(int j=i+1;j<track-1;j++){
                        if(h[j][index_column].num == h[i][index_column].num){
                            for(int k=i;k<=j;k++){
                                if(v[k][index_column].num && v[k][index_column].num!=h[i][index_column].num){
                                    temp_b = false;
                                }
                            }
                            if(temp_b){
                                for(int k=j;k>=i;k--){
                                    v[k][index_column] = v[i][index_column];
                                }
                            }
                            else{
                                h[i][index_column+1] = h[i][index_column];
                            }
                            break;
                        }
                    }
                    for(int j=0;j<i;j++){
                        if(h[j][index_column].num == h[i][index_column].num){
                            for(int k=i;k>=j;k--){
                                if(v[k][index_column].num && v[k][index_column].num!=h[i][index_column].num){
                                    temp_b = false;
                                }
                            }
                            if(temp_b){
                                for(int k=j;k>=i;k++){
                                    v[k][index_column] = v[i][index_column];
                                }
                            }
                            else{
                                h[i][index_column+1] = h[i][index_column];
                            }
                            break;
                        }
                    }
                }
                else{
                    if(h[i][index_column].rise){
                        h[i][index_column+1] = h[i][index_column];
                        for(int j=1;j<i;j++){
                            if(!h[j][index_column].num || h[j][index_column].num == h[i][index_column].num){
                                for(int k=j;k<=i;k++){
                                    if(v[k][index_column].num && v[k][index_column].num!=h[k][index_column].num){
                                        temp_b = false;
                                    }
                                }
                                if(temp_b){
                                    for(int k=j;k<=i;k++){
                                        v[k][index_column] = h[i][index_column];
                                    }
                                    h[i][index_column+1].num = 0;
                                    h[i][index_column+1].status = 0;
                                    h[i][index_column+1].rise = 0;
                                    //h[j][index_column] = h[i][index_column];
                                    //if(h[j][index_column].status!=1)h[j][index_column+1] = h[j][index_column];
                                }
                                break;
                            }
                        }
                    }
                    else{
                        h[i][index_column+1] = h[i][index_column];
                        for(int j=track-2;j>i;j--){
                            if(!h[j][index_column].num || h[j][index_column].num == h[i][index_column].num){
                                for(int k=j;k>=i;k--){
                                    if(v[k][index_column].num && v[k][index_column].num!=h[i][index_column].num){
                                        temp_b = false;
                                    }
                                }
                                if(temp_b){
                                    for(int k=j;k>=i;k--){
                                        v[k][index_column] = h[i][index_column];
                                    }
                                    h[i][index_column+1].num = 0;
                                    h[i][index_column+1].status = 0;
                                    h[i][index_column+1].rise = 0;
                                    //h[j][index_column].rise = h[i][index_column];
                                    //if(h[j][index_column].status!=1)h[j][index_column+1] = h[j][index_column];
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
        index_column++;
    }
    //output//////////////////////////////////////////////////////////////////////////////////////////////////
    record.clear();
    for(int i=0;i<wide;i++){
        for(int j=0;j<track/2;j++){
            swap(h[j][i],h[track-1-j][i]);
            swap(v[j][i],v[track-1-j][i]);
        }
    }
    /*cout<<"h"<<endl;
    for(int i=0;i<track;i++){
        for(int j=0;j<wide;j++){
            cout<<h[i][j].num<<" ";
        }
        cout<<endl;
    }
    cout<<"v"<<endl;
    for(int i=0;i<track;i++){
        for(int j=0;j<wide;j++){
            cout<<v[i][j].num<<" ";
        }
        cout<<endl;
    }*/

    for(int i=0;i<wide;i++){
        temp_b = h[0][i].num;
        for(int j=0;j<record.size();j++){
            if(record[j] == h[0][i].num){temp_b = false;}
        }
        if(temp_b){
            ofs<<".begin"<<h[0][i].num<<endl;
            for(int j=0;j<wide;j++){
                for(int k=track-1;k>=0;k--){
                    if(v[k][j].num == h[0][i].num){
                        ofs<<".V "<<j<<" "<<k;
                        while(k>0 && v[k-1][j].num == h[0][i].num){k--;}
                        ofs<<" "<<k<<endl;
                    }
                }
            }
            for(int k=0;k<track;k++){
                for(int j=0;j<wide;j++){
                    if(h[k][j].num == h[0][i].num && h[k][j+1].num == h[0][i].num){
                        ofs<<".H "<<j<<" "<<k;
                        while(j<wide && h[k][j+1].num == h[0][i].num){j++;}
                        ofs<<" "<<j<<endl;
                    }
                }
            }
            record.push_back(h[0][i].num);
            ofs<<".end"<<endl;
        }
    }

    for(int i=0;i<wide;i++){
        temp_b = h[track-1][i].num;
        for(int j=0;j<record.size();j++){
            if(record[j] == h[track-1][i].num){temp_b = false;}
        }
        if(temp_b){
            ofs<<".begin"<<h[track-1][i].num<<endl;
            for(int j=0;j<wide;j++){
                for(int k=track-1;k>=0;k--){
                    if(v[k][j].num == h[track-1][i].num){
                        ofs<<".V "<<j<<" "<<k;
                        while(k>0 && v[k-1][j].num == h[track-1][i].num){k--;}
                        ofs<<" "<<k<<endl;
                    }
                }
            }
            for(int k=0;k<track;k++){
                for(int j=0;j<wide;j++){
                    if(h[k][j].num == h[track-1][i].num && h[track-1][j+1].num == h[track-1][i].num){
                        ofs<<".H "<<j<<" "<<k;
                        while(j<wide && h[k][j+1].num == h[track-1][i].num){j++;}
                        ofs<<" "<<j<<endl;
                    }
                }
            }
            record.push_back(h[track-1][i].num);
            ofs<<".end"<<endl;
        }
    }


    return 0;
}

re extend_track(wire** h,wire** v,int track,int wide,bool tb){
    wire **th,**tv;
    re R;
    th = new wire*[track+1];
    tv = new wire*[track+1];
    for(int i=0;i<track+1;i++){
        th[i] = new wire[wide];
        tv[i] = new wire[wide];
    }
    if(tb){
        for(int i=0;i<wide;i++){
            th[0][i] = h[0][i];
            tv[0][i] = v[0][i];
            if(track>2)tv[1][i] = v[1][i];
        }
        for(int j=1;j<track;j++){
            for(int i=0;i<wide;i++){
                th[j+1][i] = h[j][i];
                tv[j+1][i] = v[j][i];
            }
        }
    }
    else{
        for(int i=0;i<wide;i++){
            th[track][i] = h[track-1][i];
            tv[track][i] = v[track-1][i];
            if(track>2)tv[track-1][i] = v[track-2][i];
        }
        for(int j=0;j<track-1;j++){
            for(int i=0;i<wide;i++){
                th[j][i] = h[j][i];
                tv[j][i] = v[j][i];
            }
        }
    }
    R.h = th;
    R.v = tv;

    return R;
}
int parse_raw_data(vector<string> Raw_Data){
    if(Raw_Data[0].substr(0,7).compare("NumNet ") == 0){
        net_number = signed(stoi(Raw_Data[0].substr(6,Raw_Data[0].size()-6)));
    }
    if(Raw_Data[1].substr(0,14).compare("ChannelLength ") == 0){
        channel_length = stoi(Raw_Data[1].substr(13,Raw_Data[1].size()-13));
    }
    cout<<channel_length<<endl;
    for (size_t i = 2; i < Raw_Data.size(); i++)
    {

        /* code */
        if(Raw_Data[i].substr(0,5).compare("NET n") == 0){
            string net_index = "";
            size_t k;
            for (k = 5; k < Raw_Data.size(); k++)
            {
                /* code */
                if(Raw_Data[i].at(k) == ' '){
                    break;
                }else{
                    net_index += Raw_Data[i].at(k);
                }
            }
            vector<int>::iterator it = find(Net_Index.begin(),Net_Index.end(),stoi(net_index));
            if(it!=Net_Index.end()){
            }
            
            Net_Index.push_back(stoi(net_index));

            bool is_Top = false;
            bool is_Bottom = false;
            string temp_pin = "";
            vector<int> top;
            vector<int> bottom;
            for (size_t j = k+1; j < Raw_Data[i].size(); j++)
            {
                if(Raw_Data[i].at(j)==' '){
                    if(is_Top){
                        top.push_back(stoi(temp_pin));
                    }else if(is_Bottom){
                        bottom.push_back(stoi(temp_pin));
                    }
                    temp_pin = "";
                }else if(Raw_Data[i].at(j)==';'){
                    break;
                }else{
                    if(Raw_Data[i].at(j)=='t'){
                        is_Top = true;
                        is_Bottom = false;
                    }else if(Raw_Data[i].at(j)=='b'){
                        is_Top = false;
                        is_Bottom = true;
                    }else{
                        //data
                        temp_pin += Raw_Data[i].at(j);
                    }

                }
            }
            if(top.size() + bottom.size()>=2)
                Nets.push_back(make_pair(top,bottom));
            
        }
        else{
            if(Raw_Data[i]!="\n"){
            cout<<"Unknown Input type. (Might cause by char '\\n'). Ignored!"<<endl;
            }
        }
    }
    if(signed(Nets.size())!=signed(net_number)){
    }
    //Fill TBN grah
    for (int i = 0; i < channel_length; i++)
    {
        /* code */
        TBN.push_back(make_pair(find_pin(PIN_TOP,i),find_pin(PIN_BOTTOM,i)));
    }
    
    return SUCCESS;
}
void print_Network(vector<pair<int,int>> TBN){

    ofstream output_File;
    string filename = "Data.txt";
    output_File.open(filename);

        for (int i = 0; i < TBN.size(); i++)
    {
        /* code */
        if(TBN[i].first == PIN_NOT_FOUND)
            output_File<<to_string(0)+" ";
        else
        {
            output_File<<to_string(TBN[i].first+1)+" ";
        }
        
    }
    output_File<<endl;
        for (int i = 0; i < TBN.size(); i++)
    {
        /* code */
        if(TBN[i].second == PIN_NOT_FOUND)
            output_File<<to_string(0)+" ";
        else
        {
            output_File<<to_string(TBN[i].second+1)+" ";
        }
        
    }
        output_File<<endl;
        output_File.close();
}
int find_pin(int Pin_Type,int current_pin){
        signed int network_number = PIN_NOT_FOUND;
    /*CODE HERE*/
    //find top or bottom related
        for (size_t i = 0; i < Nets.size(); i++)
        {   
            if(Pin_Type == PIN_TOP){
                vector<int>::iterator iter = find(Nets[i].first.begin(),Nets[i].first.end(),current_pin);
                if(iter!=Nets[i].first.end()){
                    //We assume that each pin could solely exists in ONE network
                    return i;
                    
                }
            
            }else if(Pin_Type == PIN_BOTTOM){
                vector<int>::iterator iter = find(Nets[i].second.begin(),Nets[i].second.end(),current_pin);
                if(iter!=Nets[i].second.end()){
                    return i;
                }
            }
        }
    
    return network_number;
}