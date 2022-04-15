//
// Created by 乔嘉龙 on 2022/4/14.
//

#include "Queue.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//create 5 stages queue
//Queue* IFQueue = new Queue();
//Queue* IDQueue = new Queue();
//Queue* EXQueue = new Queue();
//Queue* MEMQueue = new Queue();
//Queue* WBQueue = new Queue();


void simulation(string file_name, int wide, int start_line, int total_simulate_lines){
    //
    ifstream read_file(file_name);
    string line;
    //go to specific line
    int current_line = 0;
    while(current_line < start_line){
        getline(read_file,line);
        cout<< line <<endl;
        current_line++;
    }
    //start pipeline


}

int main(){
    cout << "start" << endl;
    simulation("srv_0",1,0,3);
    return 0;
}
