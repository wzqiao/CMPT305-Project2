//
// Created by 乔嘉龙 on 2022/4/14.
//

#ifndef QUEUE
#include "Queue.h"
#define QUEUE
#endif // !QUEUE

#ifndef GLOBAL_VALUES
#include "all_global_values.h"
#define GLOBAL_VALUES
#endif

#include <fstream>
#include <vector>
#include <sstream>
//using namespace std;

//create 5 stages queue
//Queue* IFQueue = new Queue();
//Queue* IDQueue = new Queue();
//Queue* EXQueue = new Queue();
//Queue* MEMQueue = new Queue();
//Queue* WBQueue = new Queue();

bool check_data_dependence(Queue* q,Queue* wbq, Node* node) {
	//遍历IFQueue,检查所需数据是否将要被更改
	//将要更改就waiting直到更改结束（过WB）
	//IFQueue一般不会太长 for循环嵌套应该没事
	int result = 0;
	Queue* temp = q;
	//size < 2 说明没有数据依赖
	if (node->instruction.size() > 2) {
		for (int j = 2; j < node->instruction.size(); j++) {
			for (int i = 0; i < q->size; i++) {
				if (node->instruction[j] == temp->head->instruction[0]) {
					result++;
				}
			}
		}

		//所有数据依赖在IFQueue里找到
		if (result == node->instruction.size() - 2) {
			return true;
		}

		//如果IFQueue里没有,遍历 过WBQueue    IFQueue里没有 只能从这里找
	//过WBQueue里储存着曾经完成WB的指令地址
	//可用多线程搜索
		
		//所有数据依赖在IFQueue和过WBQueue里找到
		if(result == node->instruction.size() - 2) {
			return true;
		}
	}
	

	return false;
}

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


	Queue* IFQueue = new Queue();
	IFQueue->size = 0;
	Queue* IDQueue = new Queue();
	Queue* EXQueue = new Queue();
	Queue* MEMQueue = new Queue();
	Queue* WBQueue = new Queue();
	//是否可以用WBQueue代替
	Queue* after_WBQueue = new Queue();

	ifstream infile;
	infile.open(file_name, ifstream::in);
	if (infile.is_open()) {
		//vector<string> vStr;
		string content;
		
		//
		start_line--;
		int current_line_infile = 0;
		int line_count = 0;

		bool stop_tag_branch = false; //true is stop instruction fetch. false do nothing
		while (line_count < total_simulate_lines)
		{
			//line_count == 0 说明已经开始操作了，外面这个getline会导致多读一行
			if (line_count == 0) {
				getline(infile, content);
			}

			//start push instruction into pipeline
			if (current_line_infile >= start_line && line_count < total_simulate_lines) {
				if (!stop_tag_branch) {
					//创建w个node， 一次读取w个指令
					for (int i = 0; i < width; i++) {
						getline(infile, content);
						//vStr.push_back(content);

						//cout << content << endl;

						//分割字符串
						stringstream sstr(content);
						string token;
						Node* temp = new Node();
						while (getline(sstr, token, ',')) {
							//cout << token << endl;
							//IF 阶段 指令获取

							temp->instruction.push_back(token);
						}
						push_back(IFQueue, temp);
						if (temp->instruction[1] == "3") {
							stop_tag_branch = true;
						}
						IFQueue->size++;
						line_count++;


					}
				}
				current_line_infile++;
				cout << "current line in file: " << current_line_infile << endl;

				
				
				//IF to ID
				for (int i = 0; i < width; i++) {
					
					//如果指令是分支指令停止IF指令的获取直到该指令过EX
					/*if (IFQueue->head->instruction[1] != "3") {
						push(IDQueue, IFQueue->head);
						continue;
					}*/
					//有分支指令 不停止IF to ID 停止IF 停止其他指令ID to EX 等该指令通过EX
					if (IFQueue->head->instruction[1] == "3") {
						stop_tag_branch = true;
						push(IDQueue, IFQueue->head);
					}
					if (!stop_tag_branch) {
						push(IDQueue, IFQueue->head);
					}
					//break;
				}

				//ID 阶段 指令解码和读取操作数 
				// An instruction cannot go to EX until all its data dependences are satisfied
				//一条指令在满足其所有数据相关性之前不能进入 EX
				
				//检查是否满足数据相关性
				bool is_satisfied = false;
				is_satisfied = check_data_dependence(IDQueue, WBQueue, IDQueue->head);
				//满足数据相关性后
				if (is_satisfied) {
					//ID to EX 
					//这里少判断条件
					push(EXQueue, IDQueue->head);
				}
				//只有一个整数ALU的话，好像不管w是多少第一个使用整数ALU的指令push进EXQueue
				// 都会使后面的使用整数ALU的指令等待
				//EX 阶段指令发出和执行


				//
			}
		}
		//cout << IFQueue->head->instruction[1] << endl;
		//cout << IFQueue->head->next->instruction[1] << endl;
		/*cout << "line count: " << line_count << endl;
		for (auto line : vStr)
		{
			cout << line << endl;
		}*/
	}
	infile.close();


}

int main(int argc, char* argv[]) {
	string filename = "srv_0";
	//string filename = "test.txt";

	int width = 1;
	int start_line = 2;
	int total_line = 2;


	cout << "start simulation" << endl;
	simulation(filename, width, start_line, total_line);


	//system("pause");
	return 0;
}
