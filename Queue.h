#pragma once
#include<iostream>
#include <string>
#include <vector>
using namespace std;


class Node {
public:
	//ԭ���Ĵ��淽��
	/*string instruction_PC;
	string instruction_type;
	string instruction_depends;*/

	//�Ľ��� ��vector���� instruction_depends�в������ȵ�string
	//���Ըĳ�vector 
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