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


bool check_data_dependence(Queue* q, Queue* wbq, Node* node) {

	/*Node* temp_node = wbq->head;
	while (temp_node != NULL) {
		cout << temp_node->instruction[0] << endl;
		temp_node = temp_node->next;
	}*/


	//遍历IFQueue,检查所需数据是否将要被更改
	//将要更改就waiting直到更改结束（过WB）
	//IFQueue一般不会太长 for循环嵌套应该没事

	Queue* temp = new Queue();
	temp = q;
	Queue* temp2 = new Queue();
	temp2 = wbq;
	temp->first = temp->head;
	temp2->first = temp2->head;

	//size < 2 说明没有数据依赖
	Node* temp_q = q->head;
	int result = 0;
	if (node->instruction.size() > 2) {
		for (int j = 2; j < node->instruction.size(); j++) {

			/*for (int i = 0; i < q->size; i++) {
				if (node->instruction[j] == temp->head->instruction[0]) {
					result++;
				}
				temp->first = temp->first->next;
			}
			temp->first = temp->head;*/


			while (temp_q != NULL) {
				if (node->instruction[j] == temp_q->instruction[0]) {
					result++;
					break;
				}
				temp_q = temp_q->next;
			}

			temp_q = q->head;
		}

//		cout << "result: " << result << endl;
		//所有数据依赖在IFQueue里找到
		if (result == node->instruction.size() - 2) {
			return true;
		}

		//如果IFQueue里没有,遍历 过WBQueue    IFQueue里没有 只能从这里找
	//过WBQueue里储存着曾经完成WB的指令地址
	//可用多线程搜索
		Node* temp_wbq = wbq->head;
		for (int j = 2; j < node->instruction.size(); j++) {
			/*for (int i = 0; i < wbq->size; i++) {
				if (node->instruction[j] == temp2->head->instruction[0]) {
					result++;
				}
				temp2->first = temp2->first->next;
			}
			temp2->first = temp2->head;*/

			while (temp_wbq != NULL) {
				if (node->instruction[j] == temp_wbq->instruction[0]) {
					result++;
					break;
				}
				temp_wbq = temp_wbq->next;
			}
			temp_wbq = wbq->head;
		}

//		cout << "result: " << result << endl;
		//所有数据依赖在IFQueue和过WBQueue里找到
		if (result == node->instruction.size() - 2) {
			return true;
		}
		return false;
	}


	return true;
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
		bool is_satisfied = false;
		while (line_count < total_simulate_lines)
		{
			//line_count == 0 说明已经开始操作了，外面这个getline会导致多读一行
			if (start_line != 0) {
				if (line_count == 0) {
					getline(infile, content);
					current_line_infile++;
				}
			}
			//cout << 1 << endl;
			//cout << "current line in file: " << current_line_infile << endl;
			//start push instruction into pipeline
			if (current_line_infile >= start_line && line_count < total_simulate_lines) {
				if (!stop_tag_branch) {
					//cout << 2 << endl;
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

						/*if (temp->instruction[1] == "3") {
							stop_tag_branch = true;
						}*/

						switch (stoi(temp->instruction[1])) {
						case 1:
							type_count[0]++;
							break;
						case 2:
							type_count[1]++;
							break;
						case 3:
							type_count[2]++;
							break;
						case 4:
							type_count[3]++;
							break;
						case 5:
							type_count[4]++;
							break;
						}

						IFQueue->size++;
						line_count++;
						current_line_infile++;

					}
				}
//				cout << "after_WBQueue->size: " << after_WBQueue->size << endl;
				cout << "current line in file: " << current_line_infile << endl;
//				cout << "IF: " << IFQueue->head->instruction[0] << endl; //ffff000008082890,3,ffff00000808288c
//				//cout << stop_tag_branch << endl; 0, false
				//cout << "IFQueue->size: " << IFQueue->size << endl;


				IFQueue->first = IFQueue->head;
				//IF to ID
				for (int i = 0; i < width; i++) {
					
					//如果指令是分支指令停止IF指令的获取直到该指令过EX
					//有分支指令 不停止IF to ID? 停止IF 停止其他指令ID to EX 等该指令通过EX
					/*if (IFQueue->head == NULL) {
						push(IDQueue, IFQueue->first);
						pop(IFQueue);
					}*/
					//普通指令进入ID
					
					if (IFQueue->first != NULL) {
						if (IFQueue->first->instruction[1] == "3") {
							stop_tag_branch = true;
							push(IDQueue, IFQueue->first);
							//pop(IFQueue);
							remove(IFQueue, IFQueue->first);
							break;
						}
					}
					push(IDQueue, IFQueue->head);
					pop(IFQueue);
				}
				IDQueue->first = IDQueue->head;

				cout << "ID: " << IDQueue->head->instruction[0] << endl;
				//ID 阶段 指令解码和读取操作数 
				// An instruction cannot go to EX until all its data dependences are satisfied
				//一条指令在满足其所有数据相关性之前不能进入 EX
				
				//检查是否满足数据相关性
				for (int i = 0; i < width; i++) {
					is_satisfied = check_data_dependence(IDQueue, after_WBQueue, IDQueue->first);
					//满足数据相关性后
					if (is_satisfied) {
						//ID to EX 
						// 
						//这里少判断条件?
						//cout << IDQueue->head->instruction[0] << endl;
						push(EXQueue, IDQueue->first);
						//pop(IDQueue);
						remove(IDQueue, IDQueue->first);
					}
					if (IDQueue->first->next != NULL) {
						IDQueue->first = IDQueue->first->next;
					}
				}

				
				//只有一个整数ALU的话，好像不管w是多少第一个使用整数ALU的指令push进EXQueue
				// 都会使后面的使用整数ALU的指令等待
				//EX 阶段指令发出和执行
				//stop_tag_branch在过EX后变为false
				
				if (EXQueue->head != NULL) {
					cout << "EX: " << EXQueue->head->instruction[0] << endl;
					if (EXQueue->head->next != NULL) {
						EXQueue->first = EXQueue->head->next;
					}
				}

				for (int i = 0; i < width; i++) {
					if (EXQueue->head != NULL) {
						if (EXQueue->head->instruction[1] == "3") {
							push(MEMQueue, EXQueue->head);
							pop(EXQueue);
							stop_tag_branch = false;
							break;
						}

						//同时用一个功能单元
						if (EXQueue->first != NULL) {
							if (EXQueue->head->instruction[1] == EXQueue->first->instruction[1]) {
								push(MEMQueue, EXQueue->head);
								pop(EXQueue);
								break;
							}
						}
					}
					if (EXQueue->head == NULL) {
						break;
					}
					//不共用一个功能单元，正常输出
					push(MEMQueue, EXQueue->head);
					pop(EXQueue);
					if (EXQueue->head == NULL) {
						break;
					}
					if (EXQueue->head->next != NULL) {
						EXQueue->first = EXQueue->head->next;
					}
					
				}
				
				
				//cout << "1111111" << endl;
				//MEM
				if (MEMQueue->head != NULL) {
//					cout << "MEM: " << MEMQueue->head->instruction[0] << endl;
					if (MEMQueue->head->next != NULL) {
						MEMQueue->first = MEMQueue->head->next;
					}
				}
				for (int i = 0; i < width; i++) {
					//同时用一个功能单元
					if (MEMQueue->first != NULL) {
						if (MEMQueue->head->instruction[1] == MEMQueue->first->instruction[1]) {
							push(WBQueue, MEMQueue->head);
							pop(MEMQueue);
							break;
						}
					}
					if (MEMQueue->head == NULL) {
						break;
					}
					push(WBQueue, MEMQueue->head);
					pop(MEMQueue);
					if (MEMQueue->head == NULL) {
						break;
					}
					if (MEMQueue->head->next != NULL) {
						MEMQueue->first = MEMQueue->head->next;
					}
				}
				
				
				
				//WB
				if (WBQueue->head != NULL) {
//					cout << "WB: " << WBQueue->head->instruction[0] << endl << endl;
					if (WBQueue->head->next != NULL) {
						WBQueue->first = WBQueue->head->next;
					}
				}
				for (int i = 0; i < width; i++) {
					//同时用一个功能单元
					if (WBQueue->first != NULL) {
						if (WBQueue->head->instruction[1] == WBQueue->first->instruction[1]) {
							push(after_WBQueue, WBQueue->head);
							pop(WBQueue);
							break;
						}
					}
					if (WBQueue->head == NULL) {
						break;
					}
					//cout << WBQueue->head->instruction[0] << endl;
					push(after_WBQueue, WBQueue->head);
					after_WBQueue->size++;
					pop(WBQueue);
					if (WBQueue->head == NULL) {
						break;
					}
					if (WBQueue->head->next != NULL) {
						WBQueue->first = WBQueue->head->next;
					}
				}

				
				
				//cout << after_WBQueue->tail->instruction[0] << endl;
			}
			circles++;
		}
		//cout << IFQueue->head->instruction[1] << endl;
		//cout << IFQueue->head->next->instruction[1] << endl;
//		cout << "line count: " << line_count << endl;
//		cout << "Total line: " << total_simulate_lines << endl;
		cout << circles << endl;
		/*for (auto line : vStr)
		{
			cout << line << endl;
		}*/
	}
	infile.close();


}

int main(int argc, char* argv[]) {
	string filename = "srv_0";
	//string filename = "test.txt";

	int width = 2;
	int start_line = 1;
	int total_line = 1000;


	cout << "start simulation" << endl;
	simulation(filename, width, start_line, total_line);


	for (int i = 0; i < 5; i++)
		cout << (double)type_count[i] / circles << endl;


	cout << "end simulation" << endl;

	system("pause");
	return 0;
}
