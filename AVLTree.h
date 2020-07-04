#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "AVLNode.h"

class AVLTree
{
private:
	AVLNode*	root;
	vector < pair< pair<double, int>, StockData* > > vHeap;
	ofstream*	fout;

public:
	AVLTree(ofstream* fout);
	~AVLTree();

	void		Insert(StockData* pStu);
	bool		Print(); 
	bool		Search(char* name);
	bool		Rank();
	void		printSearch_single(StockData* pStockData);
	void		printMultiple(StockData* pStockData);
	AVLNode*		getRoot() { return root; }
};
#endif
