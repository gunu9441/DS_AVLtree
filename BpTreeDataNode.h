#ifndef _BPTREEDATANODE_H_
#define _BPTREEDATANODE_H_

#include "BpTreeNode.h"

class BpTreeDataNode : public BpTreeNode
{
private:
	map <double, vector<pair<int, StockData*> > > mapData;
	BpTreeNode* pNext;
	BpTreeNode* pPrev;

public:
	BpTreeDataNode(){
		pNext = NULL;
		pPrev = NULL;
	}

	virtual void insertDataMap(double avgGrade, vector<pair<int, StockData*> > m) {
		mapData.insert(make_pair(avgGrade, m));
	}

	virtual void insertIndexMap(double n, BpTreeNode* pN) { return; }
	virtual map <double, BpTreeNode*>* getIndexMap()  { return NULL; }
	virtual map< double, vector<pair<int, StockData*> > > *getDataMap() { return &mapData; }
	virtual void setNext(BpTreeNode* pN)	{ pNext = pN; }
	virtual void setPrev(BpTreeNode* pN)	{ pPrev = pN; }
	virtual BpTreeNode* getNext()				{ return pNext; }
	virtual BpTreeNode* getPrev()				{ return pPrev; }	
};

#endif
