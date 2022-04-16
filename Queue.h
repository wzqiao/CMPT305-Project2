#pragma once
#include<iostream>
#include <string>
#include <vector>
using namespace std;


class Node {
public:
	//原来的储存方法
	/*string instruction_PC;
	string instruction_type;
	string instruction_depends;*/

	//改进后 用vector储存 instruction_depends有不定长度的string
	//所以改成vector 
	//vector[0] = instruction_PC
	//vector[1] = instruction_type
	//vector[2 to end] = instruction_depends
	vector<string> instruction;
	Node* next;
	Node();
	Node(string instruction, string instruction_type, string instruction_depends);
};

class Queue {
public:
	Node* head;
	Node* tail;
	int size;
	Queue();
};

void push(Queue* q, Node* node);
void push_back(Queue* q, Node* node);
Node* pop(Queue* q);