#include"Queue.h"
Node::Node() {
	this->next = NULL;
}

Node::Node(string instruction_PC, string instruction_type, string instruction_depends) {
	/*this->instruction_PC = instruction;
	this->instruction_type = instruction_type;
	this->instruction_depends = instruction_depends;*/
	this->instruction[0] = instruction_PC;
	this->instruction[1] = instruction_type;//type是个int值不知道有没有影响
	this->instruction.push_back(instruction_depends);
	this->next = NULL;
}

Queue::Queue() {
	this->head = NULL;
	this->tail = NULL;
	this->first = NULL;
	this->size = 0;
}

void push(Queue* q, Node* node) {
	if (q->head == NULL) {
		q->head = node;
		q->tail = node;
	}
	else {
		q->tail->next = node;
		q->tail = q->tail->next;
	}
}

void push_back(Queue* q, Node* node) {
	if (q->head == NULL) {
		//queue is empty
		q->head = node;
		q->tail = node;
	}
	else {
		//queue is not empty
		q->tail->next = node;
		q->tail = node;
	}
}

Node* pop(Queue* q) {

	if (q->head == NULL) {
		return NULL;
	}
	/*if (q->head->next == NULL) {
		Node* temp = q->head;
		q->head = NULL;
		return temp;
	}*/

	else {
		Node* temp = q->head;
		q->head = q->head->next;
		q->size--;
		return temp;
	}
}

void remove(Queue* q, Node* node) {
	if (q->head == NULL) {
		return;
	}
	if (q->head == node) {
		q->head = node->next;
		return;
	}
	Queue* temp = new Queue();
	temp = q;
	while (temp->head->next != node) {
		//
		if (temp->head->next == NULL) {
			return;
		}
		temp->head = temp->head->next;
	}
	temp->head->next = node->next;
}