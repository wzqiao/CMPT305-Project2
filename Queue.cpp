#include"Queue.h"

Node::Node(string instruction) {
	this->instruction = instruction;
	this->next = NULL;
}

Queue::Queue() {
	this->head = NULL;
	this->tail = NULL;
}

void push(Queue* q, Node* node) {
	if (q->head == NULL) {
		q->head = node;
		q->tail = node;
	}
	else {
		q->tail = node;
		q->tail->next = NULL;
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
		return temp;
	}
}