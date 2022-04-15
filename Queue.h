#pragma once
#include<iostream>
#include <string>
using namespace std;


class Node {
public:
	string instruction;
	Node* next;

	Node(string instruction);
};

class Queue {
public:
	Node* head;
	Node* tail;

	Queue();
};

void push(Queue* q, Node* node);
Node* pop(Queue* q);