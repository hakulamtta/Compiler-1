#pragma once
#include "Nfa.h"
//////////////////////////////
// data structure : 
#include <set>
typedef struct QNode {
	int label;
	std::set<Node*> subStates; // ����������Щ�ڵ�
}QNode;
