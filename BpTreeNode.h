#ifndef _BpTreeNode_H_
#define _BpTreeNode_H_

#include "StockData.h"

class BpTreeNode{
private:
	BpTreeNode* pParent;
	BpTreeNode* pMostLeftChild;

public:
	BpTreeNode(){
		pParent = NULL;
		pMostLeftChild = NULL;
	}

	void setMostLeftChild(BpTreeNode* pN)	{ pMostLeftChild = pN; }
	void setParent(BpTreeNode* pN)				{ pParent = pN; }
	
	BpTreeNode* getParent()						{ return pParent;	}
	BpTreeNode* getMostLeftChild()				{ return pMostLeftChild; }


	virtual void insertDataMap(double avgGrade, vector<pair<int, StockData*> > m)=0;
	virtual void insertIndexMap(double n, BpTreeNode* pN)=0;
	virtual map <double, BpTreeNode*>* getIndexMap()=0;
	virtual map< double, vector<pair<int, StockData*> > > *getDataMap()=0;
	virtual void setNext(BpTreeNode* pN)=0;
	virtual void setPrev(BpTreeNode* pN)=0;
	virtual BpTreeNode* getNext()=0;
	virtual BpTreeNode* getPrev()=0;
};

#endif
