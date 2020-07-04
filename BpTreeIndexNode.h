#ifndef _BPTREEINDEXNODE_H_
#define _BPTREEINDEXNODE_H_

#include "BpTreeNode.h"

class BpTreeIndexNode : public BpTreeNode{
private:
	map <double, BpTreeNode*> mapIndex;
	
public:
	BpTreeIndexNode(){}
	~BpTreeIndexNode(){}

	virtual void insertDataMap(double avgGrade, vector<pair<int, StockData*> > m) { return; }
	virtual void insertIndexMap(double n, BpTreeNode* pN) { mapIndex.insert(make_pair(n, pN)); }
	virtual map <double, BpTreeNode*>* getIndexMap()	{ return &mapIndex; }
	virtual map< double, vector<pair<int, StockData*> > > *getDataMap() { return NULL; }
	virtual void setNext(BpTreeNode* pN) { return; }
	virtual void setPrev(BpTreeNode* pN) { return; }
	virtual BpTreeNode* getNext() { return NULL; }
	virtual BpTreeNode* getPrev() { return NULL; }
};

#endif