#include "AVLTree.h"
#include <iomanip>

AVLTree::AVLTree(ofstream* fout) {
	root = NULL;
	make_heap(vHeap.begin(), vHeap.end());
	this->fout = fout;
}

AVLTree::~AVLTree() {
}

void AVLTree::Insert(StockData* pSto) {
	AVLNode* pNew = new AVLNode;						//pNew is the name of inserting node.
	pNew->setSD(pSto);								
	char *pName;										//pName is the name of p.
	char *insert_Name = pSto->getName();
	int	rotation = 0;

	if (root == NULL) {									//if AVLTree is not existed...
		root = pNew;
		return;
	}
		
	AVLNode *a  = root,									 //most recent node with bf = +1 or bf = -1.
			*pa = NULL,									 //paraent of a.
			*p  = root,									 //p moves through the tree.
			*pp = NULL,									 //parent of p.
			*rootSub = NULL;							 //subtree of main tree.

	while (p != NULL)
	{
		pName = p->getSD()->getName();					 //Redefine p.

		if (p->getBF() != 0) 
		{ a = p; pa = pp; }
		if (strcmp(insert_Name, pName) < 0)				 //take left branch
		{
			pp = p; p = p->getLeft();
		}
		else if (strcmp(insert_Name, pName) > 0)		 //take right branch
		{
			pp = p; p = p->getRight();
		}
		else                                             //pName is already in tree.
			return;
	}
	if (strcmp(insert_Name, pp->getSD()->getName()) < 0) //pName is the name of previous p.
		pp->setLeft(pNew);
	else
		pp->setRight(pNew);

	int det;
	AVLNode *de_a,										 //child of a
			*de_de_a;									 //child of de_a

	if (strcmp(insert_Name, a->getSD()->getName()) > 0)	 
	{
		de_a = p = a->getRight(); 
		det = -1;
	}
	else
	{
		de_a = p = a->getLeft();
		det = 1;
	}

	while (p != pNew)
	{
		if (strcmp(insert_Name, p->getSD()->getName())>0)//height of right increases by 1
		{
			p->setBF(-1);
			p = p->getRight();
		}
		else                                             //height of left increases by 1
		{
			p->setBF(1);
			p = p->getLeft();
		}
	}

	if (a->getBF() == 0 || a->getBF() + det == 0)		 //balance factor is stable.
	{
		a->setBF(a->getBF()+det);
		return;
	}

	/*tree unbalanced*/
	if (det == 1)										
	{
		if (de_a->getBF() == 1)							 //rotation type LL
		{
			a->setLeft(de_a->getRight());
			de_a->setRight(a);
			a->setBF(0);
			de_a->setBF(0);
			rootSub = de_a;								 //de_a is the new root of the subtree.
		}
		else                                             //rotation type LR
		{
			de_de_a = de_a->getRight();
			de_a->setRight(de_de_a->getLeft());
			a->setLeft(de_de_a->getRight());
			de_de_a->setLeft(de_a);
			de_de_a->setRight(a);
			switch (de_de_a->getBF())
			{
			case 0:
				de_a->setBF(0);
				a->setBF(0);
				break;
			case 1:
				a->setBF(-1);
				de_a->setBF(0);
				break;
			case -1:
				de_a->setBF(1);
				a->setBF(0);
				break;
			}
			de_de_a->setBF(0);
			rootSub = de_de_a;							 //c is the new root of the subtree.
		}
	}
	else                                                 
	{
		if (de_a->getBF() == -1)						 //rotation type RR
		{
			a->setRight(de_a->getLeft());
			de_a->setLeft(a);
			a->setBF(0);
			de_a->setBF(0);
			rootSub = de_a;								 //b is the new root of the subtree.
		}
		else                                             //rotation type RL
		{
			de_de_a = de_a->getLeft();
			de_a->setLeft(de_de_a->getRight());
			a->setRight(de_de_a->getLeft());
			de_de_a->setRight(de_a);
			de_de_a->setLeft(a);
			switch (de_de_a->getBF())
			{
			case 0:
				de_a->setBF(0);
				a->setBF(0);
				break;
			case 1:
				de_a->setBF(-1);
				a->setBF(0);
				break;
			case -1:
				a->setBF(1);
				de_a->setBF(0);
				break;
			}
			de_de_a->setBF(0);
			rootSub = de_de_a;							 //de_de_a is the new root of the subtree.
		}												 //end of RL
	}
	/*subtree with root de_a has been rebalanced*/
	if (pa == NULL)
		root = rootSub;
	else if (a == pa->getLeft())
		pa->setLeft(rootSub);
	else
		pa->setRight(rootSub);
	return;
}

bool AVLTree::Print() {									 //using inorder
	stack<AVLNode*> s;									 //using stack.
	AVLNode* currentNode = root;						 //currentNode is traveling node.
	*fout << "====================" << " " << "PRINT" << " ======================";
	while (1)
	{
		while (currentNode)
		{
			s.push(currentNode);
			currentNode = currentNode->getLeft();
		}
		if (s.empty())break;							 //If stack is empty, break.
		currentNode = s.top();							 //currentNode is top of the stack.
		s.pop();										 //pop
		printMultiple(currentNode->getSD());

		currentNode = currentNode->getRight();
	}
	*fout << "===============================================" << endl;
	return true;
}

bool AVLTree::Search(char* name) {
	AVLNode *pCur = root;																//pCur is a traveling node.

	while (pCur != NULL)																//untill pCur is not NULL
	{
		if (strcmp(pCur->getSD()->getName(), name) < 0)									//take right branch
			pCur = pCur->getRight();
		else if (strcmp(pCur->getSD()->getName(), name) > 0)							//take left branch
			pCur = pCur->getLeft();
		else																			//Found
			break;
	}

	if (pCur == NULL)																	//if root is NULL, return false. if node was not found, return false.
		return false;

	printSearch_single(pCur->getSD());
	StockData* pSD = pCur->getSD();														//pCur is pointing the node that has name.
	vHeap.push_back(make_pair(make_pair(pSD->getEarning(), pSD->getStockID()), pSD));	//make the pair with two data, the pair of earning of pSD and stockID of pSD, pSD.
	push_heap(vHeap.begin(), vHeap.end(),compare);										//implementing sorting.

	return true;
}

bool AVLTree::Rank() {																	//RANK
	if (vHeap.empty())		return false;												//if vHeap is empty, return false.

	StockData* pSto;
	*fout << "====================" << " " << "RANK" << " =====================";
	while (!vHeap.empty()) {
		pSto = vHeap.front().second;													//pSto is the second componenet of the top of vHeap.
		pop_heap(vHeap.begin(), vHeap.end(),compare);									//pop
		vHeap.pop_back();																//pop
		printMultiple(pSto);															//print data
	}
	*fout << "===============================================" << endl;
	return true;
}

void AVLTree::printSearch_single(StockData* pStockData)									//print function
{
	*fout << "========" << " SEARCH " << "=======";
	printMultiple(pStockData);
	*fout << "======================" << endl;
}

void AVLTree::printMultiple(StockData* pStockData)										//print multiple data
{
	*fout << fixed << setprecision(2);
	*fout << endl << pStockData->getStockID() << " " << pStockData->getName() << " " << pStockData->getEarning() << endl;
	*fout << "시가: " << pStockData->getOpening_Price() << endl;
	*fout << "종가: " << pStockData->getClosing_Price() << endl;
	*fout << "거래량: " << pStockData->getVolume() << endl;
	*fout << "수익률: " << pStockData->getEarning() << endl;
}


