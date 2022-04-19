//
// Created by 乔嘉龙 on 2022/4/14.
//

#include<queue>

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
int current_line_infile = 0;
int line_count = 0;
int index = 0;
bool stop_tag_branch = false; //true is stop instruction fetch. false do nothing
bool is_satisfied = false;

int width;
int start_line;
int total_simulate_lines;
string file_name;
void remove(queue<Node*>& q, Node* node)
{
	if (q.empty()) {
		return;
	}
	// Helper queue to store the elements
	// temporarily.
	queue<Node*> ref;
	int s = q.size();
	int cnt = 0;

	// Finding the value to be removed
	while (q.front() != node && !q.empty()) {
		ref.push(q.front());
		q.pop();
		cnt++;
	}

	// If element is not found
	if (q.empty()) {
		cout << "element not found!!" << endl;
		while (!ref.empty()) {

			// Pushing all the elements back into q
			q.push(ref.front());
			ref.pop();
		}
	}

	// If element is found
	else {
		q.pop();
		while (!ref.empty()) {

			// Pushing all the elements back into q
			q.push(ref.front());
			ref.pop();
		}
		int k = s - cnt - 1;
		while (k--) {

			// Pushing elements from front of q to its back
			Node* p = q.front();
			q.pop();
			q.push(p);
		}
	}
}

bool check_data_dependence(queue<Node*> q, queue<Node*> after_wbq, Node* node) {

	/*Node* temp_node = wbq->head;
	while (temp_node != NULL) {
		cout << temp_node->instruction[0] << endl;
		temp_node = temp_node->next;
	}*/
	

	//遍历IFQueue,检查所需数据是否将要被更改
	//将要更改就waiting直到更改结束（过WB）
	//IFQueue一般不会太长 for循环嵌套应该没事

	/*Queue* temp = new Queue();
	temp = q;
	Queue* temp2 = new Queue();
	temp2 = wbq;*/
	queue<Node*> temp1;
	temp1 = q;
	queue<Node*> temp2;
	temp2 = after_wbq; 

	Node* temp_q = NULL;
	if (!q.empty()) {

		//size < 2 说明没有数据依赖
		temp_q = q.front();
	}
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

			temp_q = q.front();
		}

		//cout << "result: " << result << endl;
		//所有数据依赖在IFQueue里找到
		if (result == node->instruction.size() - 2) {
			return true;
		}

		//如果IFQueue里没有,遍历 过WBQueue    IFQueue里没有 只能从这里找
		//过WBQueue里储存着曾经完成WB的指令地址
		//可用多线程搜索
		if (after_wbq.empty())
			return false;

		Node* temp_wbq = after_wbq.front();
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
			temp_wbq = after_wbq.front();
		}

		//cout << "result: " << result << endl;
		//所有数据依赖在IFQueue和过WBQueue里找到
		if (result == node->instruction.size() - 2) {
			return true;
		}
		return false;
	}
//	cout << "111111111111111" << endl;

	return true;
}

void IF_stage(ifstream& infile,queue<Node*> IFQueue) {
	string content;
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

				//cout << "content: " << content << endl;
				//cout << ++index << endl;
				//分割字符串
				stringstream sstr(content);
				string token;
				Node* temp = new Node();
				while (getline(sstr, token, ',')) {
					//cout << token << endl;
					//IF 阶段 指令获取

					temp->instruction.push_back(token);
				}
				IFQueue.push(temp);

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

				//						IFQueue->size++;
				line_count++;
				current_line_infile++;

			}
		}
		//				cout << "after_WBQueue->size: " << after_WBQueue->size << endl;
		//				cout << "current line in file: " << current_line_infile << endl;
		//				cout << "IF: " << IFQueue->head->instruction[0] << endl; //ffff000008082890,3,ffff00000808288c
		//				//cout << stop_tag_branch << endl; 0, false
						//cout << "IFQueue->size: " << IFQueue->size << endl;
		circles++;
	}
}

void IF_to_ID_stage(ifstream& infile, queue<Node*> IFQueue, queue<Node*> IDQueue) {
	Node* first_IF = NULL;
	if (!IFQueue.empty())
		first_IF = IFQueue.front();


	//IF to ID
	for (int i = 0; i < width; i++) {
		//					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " << i << endl;

							//如果指令是分支指令停止IF指令的获取直到该指令过EX
							//有分支指令 不停止IF to ID? 停止IF 停止其他指令ID to EX 等该指令通过EX
							/*if (IFQueue->head == NULL) {
								push(IDQueue, IFQueue->first);
								pop(IFQueue);
							}*/
							//普通指令进入ID
		if (IDQueue.empty())
			break;

		if (first_IF != NULL) {
			if (first_IF->instruction[1] == "3") {
				stop_tag_branch = true;
				IDQueue.push(IFQueue.front());
				//pop(IFQueue);
				remove(IFQueue, first_IF);
				break;
			}
		}
		if (!IDQueue.empty()) {
			Node* temp = new Node();
			temp = IDQueue.front();
			while (temp->next != NULL) {
				//							cout << "IDQueue instruction@@@@@@@" << temp->instruction[0] << endl;
				//							cout << "IDQueue instruction next" << temp->next->instruction[0] << endl;
				temp = temp->next;

			}
		}
		IDQueue.push(IFQueue.front());
		IFQueue.pop();

	}
}

void ID_stage(ifstream& infile, queue<Node*> IFQueue, queue<Node*> IDQueue, queue<Node*>EXQueue,queue<Node*> after_WBQueue) {
	IF_stage(infile, IFQueue);
	Node* first_ID = NULL;
	if (!IDQueue.empty())
		first_ID = IDQueue.front();

	//检查是否满足数据相关性
	for (int i = 0; i < width; i++) {

		if (IDQueue.empty()) {
			break;
		}

		//cout << "IDQueue first instruction: " << IDQueue->first->instruction[0] << endl;
		is_satisfied = check_data_dependence(IDQueue, after_WBQueue, first_ID);

		//满足数据相关性后
		if (is_satisfied) {
			//						cout << "is_satisfied2: " << is_satisfied << endl;
									//ID to EX 
									// 
									//这里少判断条件?
									//cout << IDQueue->head->instruction[0] << endl;
			//						cout << "IDQueue first instruction3: " << IDQueue->first->instruction[0] << endl;
			EXQueue.push(first_ID);
			//pop(IDQueue);
			remove(IDQueue, first_ID);
		}
		if (first_ID->next != NULL) {
			first_ID = first_ID->next;
		}
	}
	//circles++;
}

void EX_stage(ifstream& infile, queue<Node*> IFQueue, queue<Node*> IDQueue, queue<Node*>EXQueue, queue<Node*> MEMQueue,queue<Node*> after_WBQueue) {
	IF_stage(infile, IFQueue);
	IF_to_ID_stage(infile, IFQueue, IDQueue);
	ID_stage(infile, IFQueue, IDQueue, EXQueue, after_WBQueue);
	Node* first_EX = NULL;
	if (!EXQueue.empty()) {
		//					cout << "EX: " << EXQueue->head->instruction[0] << endl;
		if (EXQueue.front()->next != NULL) {
			first_EX = EXQueue.front()->next;
		}
	}

	for (int i = 0; i < width; i++) {

		if (!EXQueue.empty()) {
			if (EXQueue.front()->instruction[1] == "3") {
				MEMQueue.push(EXQueue.front());
				EXQueue.pop();
				circles++;
				cout << "1111111111" << endl;
				stop_tag_branch = false;
				break;
			}


			if (first_EX == NULL) {
				break;
			}

			//同时用一个功能单元
			if (first_EX != NULL) {
				if (EXQueue.front()->instruction[1] == first_EX->instruction[1]) {
					MEMQueue.push(EXQueue.front());
					EXQueue.pop();
					circles++;
					break;
				}
			}
		}
		if (EXQueue.empty()) {
			break;
		}
		//不共用一个功能单元，正常输出
		MEMQueue.push(EXQueue.front());
		EXQueue.pop();

		if (EXQueue.empty()) {
			break;
		}

		if (EXQueue.front()->next != NULL) {
			first_EX = EXQueue.front()->next;
		}

	}

	//circles++;
}

void MEM_stage(ifstream& infile, queue<Node*> IFQueue, queue<Node*> IDQueue, queue<Node*>EXQueue, queue<Node*> MEMQueue,queue<Node*> WBQueue ,queue<Node*> after_WBQueue) {
	IF_stage(infile, IFQueue);
	IF_to_ID_stage(infile, IFQueue, IDQueue);
	ID_stage(infile, IFQueue, IDQueue, EXQueue, after_WBQueue);
	EX_stage(infile, IFQueue, IDQueue, EXQueue, MEMQueue, after_WBQueue);
	Node* first_MEM = NULL;
	//MEM
	if (!MEMQueue.empty()) {
		//					cout << "MEM: " << MEMQueue->head->instruction[0] << endl;
		if (MEMQueue.front()->next != NULL) {
			first_MEM = MEMQueue.front()->next;
		}
	}
	for (int i = 0; i < width; i++) {
		//同时用一个功能单元
		if (first_MEM != NULL) {
			if (MEMQueue.front()->instruction[1] == first_MEM->instruction[1]) {
				WBQueue.push(MEMQueue.front());
				MEMQueue.pop();
				circles++;
				break;
			}
		}
		if (MEMQueue.empty()) {
			break;
		}
		WBQueue.push(MEMQueue.front());
		MEMQueue.pop();
		if (MEMQueue.empty()) {
			break;
		}
		if (MEMQueue.front()->next != NULL) {
			first_MEM = MEMQueue.front()->next;
		}
	}
	circles++;
}

void WB_stage(ifstream& infile, queue<Node*> IFQueue, queue<Node*> IDQueue, queue<Node*>EXQueue, queue<Node*> MEMQueue, queue<Node*> WBQueue, queue<Node*> after_WBQueue) {
	IF_stage(infile, IFQueue);
	IF_to_ID_stage(infile, IFQueue, IDQueue);
	ID_stage(infile, IFQueue, IDQueue, EXQueue, after_WBQueue);
	EX_stage(infile, IFQueue, IDQueue, EXQueue, MEMQueue, after_WBQueue);
	MEM_stage(infile, IFQueue, IDQueue, EXQueue, MEMQueue, WBQueue, after_WBQueue);
	Node* first_WB = NULL;
	//WB
	if (!WBQueue.empty()) {
		//					cout << "WB: " << WBQueue->head->instruction[0] << endl << endl;
		if (WBQueue.front()->next != NULL) {
			first_WB = WBQueue.front()->next;
		}
	}
	for (int i = 0; i < width; i++) {
		//同时用一个功能单元
		if (first_WB != NULL) {
			if (WBQueue.front()->instruction[1] == first_WB->instruction[1]) {
				after_WBQueue.push(WBQueue.front());
				WBQueue.pop();
				//cout << "1111111111" << endl;
				circles++;
				break;
			}
		}
		if (WBQueue.empty()) {
			break;
		}
		//cout << WBQueue->head->instruction[0] << endl;
		after_WBQueue.push(WBQueue.front());
		//					after_WBQueue->size++;

		WBQueue.pop();
		if (WBQueue.empty()) {
			break;
		}

		if (WBQueue.front()->next != NULL) {
			first_WB = WBQueue.front()->next;
		}
	}
	circles++;
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


	//Queue* IFQueue = new Queue();

	//Queue* IDQueue = new Queue();
	//Queue* EXQueue = new Queue();
	//Queue* MEMQueue = new Queue();
	//Queue* WBQueue = new Queue();
	////是否可以用WBQueue代替
	//Queue* after_WBQueue = new Queue();

	queue<Node*> IFQueue;
	queue<Node*> IDQueue;
	queue<Node*> EXQueue;
	queue<Node*> MEMQueue;
	queue<Node*> WBQueue;
	//是否可以用WBQueue代替
	queue<Node*> after_WBQueue;


	
	ifstream infile;
	infile.open(file_name, ifstream::in);
	if (infile.is_open()) {
		//vector<string> vStr;
		
		
		//
		start_line--;
		
		while (line_count < total_simulate_lines)
		{
			IF_stage(infile, IFQueue);
//			//line_count == 0 说明已经开始操作了，外面这个getline会导致多读一行
//			if (start_line != 0) {
//				if (line_count == 0) {
//					getline(infile, content);
//					current_line_infile++;
//				}
//			}
//			//cout << 1 << endl;
//			//cout << "current line in file: " << current_line_infile << endl;
//			//start push instruction into pipeline
//
//			if (current_line_infile >= start_line && line_count < total_simulate_lines) {
//				if (!stop_tag_branch) {
//					//cout << 2 << endl;
//					//创建w个node， 一次读取w个指令
//					for (int i = 0; i < width; i++) {
//						getline(infile, content);
//						//vStr.push_back(content);
//
//						//cout << "content: " << content << endl;
//						//cout << ++index << endl;
//						//分割字符串
//						stringstream sstr(content);
//						string token;
//						Node* temp = new Node();
//						while (getline(sstr, token, ',')) {
//							//cout << token << endl;
//							//IF 阶段 指令获取
//
//							temp->instruction.push_back(token);
//						}
//						IFQueue.push(temp);
//
//						/*if (temp->instruction[1] == "3") {
//							stop_tag_branch = true;
//						}*/
//
//						
//						switch (stoi(temp->instruction[1])) {
//
//						case 1:
//							type_count[0]++;
//							break;
//						case 2:
//							type_count[1]++;
//							break;
//						case 3:
//							type_count[2]++;
//							break;
//						case 4:
//							type_count[3]++;
//							break;
//						case 5:
//							type_count[4]++;
//							break;
//						}
//
////						IFQueue->size++;
//						line_count++;
//						current_line_infile++;
//
//					}
//				}
////				cout << "after_WBQueue->size: " << after_WBQueue->size << endl;
////				cout << "current line in file: " << current_line_infile << endl;
////				cout << "IF: " << IFQueue->head->instruction[0] << endl; //ffff000008082890,3,ffff00000808288c
////				//cout << stop_tag_branch << endl; 0, false
//				//cout << "IFQueue->size: " << IFQueue->size << endl;
//				circles++;

//				Node* first_IF = NULL;
//				if(!IFQueue.empty())
//					first_IF = IFQueue.front();
//
//
//				//IF to ID
//				for (int i = 0; i < width; i++) {
////					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " << i << endl;
//
//					//如果指令是分支指令停止IF指令的获取直到该指令过EX
//					//有分支指令 不停止IF to ID? 停止IF 停止其他指令ID to EX 等该指令通过EX
//					/*if (IFQueue->head == NULL) {
//						push(IDQueue, IFQueue->first);
//						pop(IFQueue);
//					}*/
//					//普通指令进入ID
//					if (IDQueue.empty())
//						break;
//					
//					if (first_IF != NULL) {
//						if (first_IF->instruction[1] == "3") {
//							stop_tag_branch = true;
//							IDQueue.push(IFQueue.front());
//							//pop(IFQueue);
//							remove(IFQueue, first_IF);
//							break;
//						}
//					}
//					if (!IDQueue.empty()) {
//						Node* temp = new Node();
//						temp = IDQueue.front();
//						while (temp->next != NULL) {
////							cout << "IDQueue instruction@@@@@@@" << temp->instruction[0] << endl;
////							cout << "IDQueue instruction next" << temp->next->instruction[0] << endl;
//							temp = temp->next;
//							
//						}
//					}
//					IDQueue.push(IFQueue.front());
//					IFQueue.pop();
//
//				}
			IF_to_ID_stage(infile, IFQueue, IDQueue);
//				cout << "ID: " << IDQueue->head->instruction[0] << endl;
				//ID 阶段 指令解码和读取操作数 
				// An instruction cannot go to EX until all its data dependences are satisfied
				//一条指令在满足其所有数据相关性之前不能进入 EX
				
//				Node* first_ID = NULL;
//				if(!IDQueue.empty())
//					first_ID = IDQueue.front();
//
//				//检查是否满足数据相关性
//				for (int i = 0; i < width; i++) {
//
//					if (IDQueue.empty()) {
//						break;
//					}
//
//					//cout << "IDQueue first instruction: " << IDQueue->first->instruction[0] << endl;
//					is_satisfied = check_data_dependence(IDQueue, after_WBQueue, first_ID);
//					
//					//满足数据相关性后
//					if (is_satisfied) {
////						cout << "is_satisfied2: " << is_satisfied << endl;
//						//ID to EX 
//						// 
//						//这里少判断条件?
//						//cout << IDQueue->head->instruction[0] << endl;
////						cout << "IDQueue first instruction3: " << IDQueue->first->instruction[0] << endl;
//						EXQueue.push(first_ID);
//						//pop(IDQueue);
//						remove(IDQueue, first_ID);
//					}
//					if (first_ID->next != NULL) {
//						first_ID = first_ID->next;
//					}
//				}
//				circles++;
			ID_stage(infile, IFQueue, IDQueue, EXQueue, after_WBQueue);
				//只有一个整数ALU的话，好像不管w是多少第一个使用整数ALU的指令push进EXQueue
				// 都会使后面的使用整数ALU的指令等待
				//EX 阶段指令发出和执行
				//stop_tag_branch在过EX后变为false
//				Node* first_EX = NULL;
//				if (!EXQueue.empty()) {
////					cout << "EX: " << EXQueue->head->instruction[0] << endl;
//					if (EXQueue.front()->next != NULL) {
//						first_EX = EXQueue.front()->next;
//					}
//				}
//
//				for (int i = 0; i < width; i++) {
//
//					if (!EXQueue.empty()) {
//						if (EXQueue.front()->instruction[1] == "3") {
//							MEMQueue.push(EXQueue.front());
//							EXQueue.pop();
//							stop_tag_branch = false;
//							break;
//						}
//
//
//						if (first_EX == NULL) {
//							break;
//						}
//
//						//同时用一个功能单元
//						if (first_EX != NULL) {
//							if (EXQueue.front()->instruction[1] == first_EX->instruction[1]) {
//								MEMQueue.push(EXQueue.front());
//								EXQueue.pop();
//								break;
//							}
//						}
//					}
//					if (EXQueue.empty()) {
//						break;
//					}
//					//不共用一个功能单元，正常输出
//					MEMQueue.push(EXQueue.front());
//					EXQueue.pop();
//
//					if (EXQueue.empty()) {
//						break;
//					}
//
//					if (EXQueue.front()->next != NULL) {
//						first_EX = EXQueue.front()->next;
//					}
//					
//				}
//
//				circles++;
			
			EX_stage(infile, IFQueue, IDQueue, EXQueue, MEMQueue, after_WBQueue);
//				Node* first_MEM = NULL;
//				//MEM
//				if (!MEMQueue.empty()) {
////					cout << "MEM: " << MEMQueue->head->instruction[0] << endl;
//					if (MEMQueue.front()->next != NULL) {
//						first_MEM = MEMQueue.front()->next;
//					}
//				}
//				for (int i = 0; i < width; i++) {
//					//同时用一个功能单元
//					if (first_MEM != NULL) {
//						if (MEMQueue.front()->instruction[1] == first_MEM->instruction[1]) {
//							WBQueue.push(MEMQueue.front());
//							MEMQueue.pop();
//							break;
//						}
//					}
//					if (MEMQueue.empty()) {
//						break;
//					}
//					WBQueue.push(MEMQueue.front());
//					MEMQueue.pop();
//					if (MEMQueue.empty()) {
//						break;
//					}
//					if (MEMQueue.front()->next != NULL) {
//						first_MEM = MEMQueue.front()->next;
//					}
//				}
//				circles++;
				
			MEM_stage(infile, IFQueue, IDQueue, EXQueue, MEMQueue, WBQueue, after_WBQueue);
//				Node* first_WB = NULL;
//				//WB
//				if (!WBQueue.empty()) {
////					cout << "WB: " << WBQueue->head->instruction[0] << endl << endl;
//					if (WBQueue.front()->next != NULL) {
//						first_WB = WBQueue.front()->next;
//					}
//				}
//				for (int i = 0; i < width; i++) {
//					//同时用一个功能单元
//					if (first_WB != NULL) {
//						if (WBQueue.front()->instruction[1] == first_WB->instruction[1]) {
//							after_WBQueue.push(WBQueue.front());
//							WBQueue.pop();
//							break;
//						}
//					}
//					if (WBQueue.empty()) {
//						break;
//					}
//					//cout << WBQueue->head->instruction[0] << endl;
//					after_WBQueue.push(WBQueue.front());
////					after_WBQueue->size++;
//
//					WBQueue.pop();
//					if (WBQueue.empty()) {
//						break;
//					}
//
//					if (WBQueue.front()->next != NULL) {
//						first_WB = WBQueue.front()->next;
//					}
//				}
//				circles++;
				
			WB_stage(infile, IFQueue, IDQueue, EXQueue, MEMQueue, WBQueue, after_WBQueue);
				//cout << after_WBQueue->tail->instruction[0] << endl;
			}
			
		}
		//cout << IFQueue->head->instruction[1] << endl;
		//cout << IFQueue->head->next->instruction[1] << endl;
//		cout << "line count: " << line_count << endl;
//		cout << "Total line: " << total_simulate_lines << endl;
		cout << "circles: " << circles << endl;
		/*for (auto line : vStr)
		{
			cout << line << endl;
		}*/
	
	infile.close();


}

int main(int argc, char* argv[]) {
	file_name = "compute_int_0";

	width = 2;
	start_line = 1;
	total_simulate_lines = 1000000;


	cout << "start simulation" << endl;
	simulation(file_name, width, start_line, total_simulate_lines);


	for (int i = 0; i < 5; i++)
		cout << (double)type_count[i] / total_simulate_lines << endl;


	cout << "end simulation" << endl;

	system("pause");
	return 0;
}
