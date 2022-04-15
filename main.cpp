//
// Created by 乔嘉龙 on 2022/4/14.
//

#ifndef QUEUE
#include "Queue.h"
#define QUEUE
#endif // !QUEUE

#include <fstream>
#include <vector>
//using namespace std;

//create 5 stages queue
//Queue* IFQueue = new Queue();
//Queue* IDQueue = new Queue();
//Queue* EXQueue = new Queue();
//Queue* MEMQueue = new Queue();
//Queue* WBQueue = new Queue();


void simulation(string file_name, int width, int start_line, int total_simulate_lines) {
	
	//ifstream read_file(file_name);
	//string line;
	////go to specific line
	//int current_line = 0;
	//while(current_line < start_line){
	//	getline(read_file,line);
	//	cout<< line <<endl;
	//	current_line++;
	//}

	ifstream infile;
	infile.open(file_name, ifstream::in);
	if (infile.is_open()) {
		vector<string> vStr;
		string content;

		int current_line_infile = 0;
		int line_count = 0;
		while (line_count<total_simulate_lines)
		{
			getline(infile, content);
			
			if (current_line_infile >= start_line && line_count<total_simulate_lines) {
				vStr.push_back(content);
				line_count++;
			}

			current_line_infile++;

			
			cout << "current line in file: " << current_line_infile << endl;
		}

		cout << "line count: " << line_count << endl;
		for (auto line : vStr)
		{
			cout << line << endl;
		}
	}
	infile.close();


}

int main(int argc, char* argv[]) {
	string filename = "srv_0";
	//string filename = "test.txt";

	int width = 1;
	int from_line = 50;
	int total_line = 100;


	cout << "start simulation" << endl;
	simulation(filename, width, from_line, total_line);


	system("pause");
	return 0;
}
