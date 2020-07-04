#include "BpTree.h"
#include <iomanip>

BpTree::BpTree(ofstream* fout, int order){
	root = NULL;
	this->order = order;
	this->fout = fout;
}

BpTree::~BpTree(){

}

void BpTree::Insert(StockData* pSto)
{
	double key = pSto->getEarning();							//key = Earning
	vector<pair<int, StockData*> > value;						//value = map< id, stock data* >
	value.push_back( make_pair(pSto->getStockID(), pSto) );		//push the compoenent to value

	if(root == NULL)											// empty B+ tree
	{ 
		BpTreeNode* pDataNode = new BpTreeDataNode;				//make BpTreeDatanode
		pDataNode->insertDataMap(key, value);					//insert the pair of key and value data to map of pDataNode.
		root = pDataNode;										//root = pDataNode
		return;													//end
	}
		
	BpTreeNode* pDataNode = searchDataNode(key);				// seach data node with key or search the location that key will be inserted.
	
	map<double, vector<pair<int, StockData*> > > *m = pDataNode->getDataMap();				//m is a map of pDataNode
	map<double, vector<pair<int, StockData*> >>::iterator it;								//declare iterator it
	
	if (m->find(key) == m->end())															//no data found
		m->insert(pair<double, vector < pair<int, StockData*>>>(key, value));				//insert data

	else                                                                                    // data found
	{
		it = m->find(key);																	//iterator it points the key component in the map
		it->second.push_back(make_pair(pSto->getStockID(),pSto));							//insert the pair that composed of stockID and stockdata into the vector
		sort(it->second.begin(), it->second.end());								//sort the vector with the condition, 'compare_Bp'
		return;
	}

	if (exceedDataNode(pDataNode))															//if the size of pDataNode exceeds order
	{
		splitDataNode(pDataNode);															//splitDataNode
		do {
			pDataNode = pDataNode->getParent();												//up to parents
			if (pDataNode == NULL)															
				return;
			if (exceedIndexNode(pDataNode))													//if the size of pDataNode exceeds order
				splitIndexNode(pDataNode);													//splitDataNode
		} while (pDataNode!=NULL);															//untill meet the root
	}
	return;
}

BpTreeNode* BpTree::searchDataNode(double n){
	BpTreeNode* pCur = root;
	map<double, BpTreeNode*>::iterator itIndex;												//declare iterator itindex

	while(pCur->getMostLeftChild() != NULL){												//untill pCur points the BpTreedataNode			
		itIndex = pCur->getIndexMap()->begin();												//itIndex is the begin of the map that pCur points

		if(n < itIndex->first){																//if n is smaller than itIndex->first
			pCur = pCur->getMostLeftChild();												//pCur moves to its MostLeftChild
		}else{
			while(true){																	//endless loop
				itIndex++;																	//ItIndex moves to next component in the map
				if(itIndex == pCur->getIndexMap()->end() || n < itIndex->first){			//if itIndex is back of the last componenet of the map that pCur points or n is smaller than itIndex->first
					itIndex--;																//itIndex moves to previous component
					pCur = itIndex->second;													//pCur is itIdex->second
					break;																	//go out of loop
				}
			}
		}
	}
	return pCur;																			//return pCur
}

void BpTree::splitDataNode(BpTreeNode* pDataNode){
	int loop = 1;
	int splitPosition_data = ceil((order - 1) / 2.0) + 1;									//splitPosition_data is the location of splitting.

	BpTreeNode* pnewDataNode = new BpTreeDataNode;											//pnewDataNode is the new dataNode.
	BpTreeNode* pnewIndexNode = new BpTreeIndexNode;										//pnewIndexNode is the newIndexNode.

	map<double, vector<pair<int, StockData*> > >::iterator it;
	it = pDataNode->getDataMap()->begin();

	while (loop < splitPosition_data)														//move iterator it to splitPosition
	{
		it++;
		loop++;
	}

	if (pDataNode == root)																	//initial condition
	{
		pnewIndexNode->setMostLeftChild(pDataNode);	
		pDataNode->setParent(pnewIndexNode);												
		pnewDataNode->setParent(pnewIndexNode);												
		pnewDataNode->setPrev(pDataNode);													
		pDataNode->setNext(pnewDataNode);													
		root = pnewIndexNode;																
	}
	else                                                                                    //after initial condtion
	{
		pnewIndexNode = pDataNode->getParent();
		pnewDataNode->setParent(pnewIndexNode);
		if (pDataNode->getNext() != NULL)
		{
			pnewDataNode->setNext(pDataNode->getNext());
			pDataNode->getNext()->setPrev(pnewDataNode);
		}
		pnewDataNode->setPrev(pDataNode);
		pDataNode->setNext(pnewDataNode);
	}

	pnewIndexNode->insertIndexMap(it->first, pnewDataNode);								    //insert data to map in new indexnode

	while (loop <= order)
	{
		pnewDataNode->insertDataMap(it->first, it->second);									//insert data to map in new datanode
		it++;
		loop++;
	}

	loop = 1;
	it = pDataNode->getDataMap()->begin();													//iterator it is used to erase the components

	while (loop < splitPosition_data)														//move iterator it to splitPostion
	{
		it++;
		loop++;
	}

	pDataNode->getDataMap()->erase(it, pDataNode->getDataMap()->end());						//erase unneccesary components
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode){
	int loop = 1;
	int splitPosition_index = ceil(order / 2.0);											//splitPosition_index is the location of splitting.

	BpTreeNode* pnewparentIndexNode = new BpTreeIndexNode;
	BpTreeNode* pnewchildIndexNode = new BpTreeIndexNode;

	map<double, BpTreeNode*>::iterator it;
	it = pIndexNode->getIndexMap()->begin();												

	while (loop < splitPosition_index)														//move iterator it to splitPosition
	{
		it++;
		loop++;
	}
	
	if (root == pIndexNode)																	//initial condition
	{
		pnewparentIndexNode->setMostLeftChild(pIndexNode);			  
		pIndexNode->setParent(pnewparentIndexNode);					  
		pnewchildIndexNode->setParent(pnewparentIndexNode);	          
		root = pnewparentIndexNode;                                   

	}
	else                                                                                    //after initial condition
	{
		pnewparentIndexNode = pIndexNode->getParent();
		pnewchildIndexNode->setParent(pnewparentIndexNode);
	}
				   
	pnewparentIndexNode->insertIndexMap(it->first,pnewchildIndexNode);						//insert pnewparentIndexNode into Map
	pnewchildIndexNode->setMostLeftChild(it->second);										//it->second is theMostLeftChild of pnewchildIndexNode
	it->second->setParent(pnewchildIndexNode);												//pnewchildIndexNode is the parent of it->second

	it++;
	loop++;

	while (loop<=order)
	{
		pnewchildIndexNode->insertIndexMap(it->first, it->second);							//insert data into indexmap
		it->second->setParent(pnewchildIndexNode);
		it++;
		loop++;
	}

	loop = 1;
	it = pIndexNode->getIndexMap()->begin();												//iterator it is used to erase the components

	while (loop < splitPosition_index)														//move iterator it to splitPostion
	{
		it++;
		loop++;
	}
	pIndexNode->getIndexMap()->erase(it, pIndexNode->getIndexMap()->end());					//erase unneccesary components
}

bool BpTree::exceedDataNode(BpTreeNode* pDataNode){
	map <double, vector<pair<int, StockData*> > > *m = pDataNode->getDataMap();				//m is the map of pDataNode.

	if(m->size() > order-1)				return true;										//if the size of map is bigger than order-1
	else								return false;
}

bool BpTree::exceedIndexNode(BpTreeNode* pIndexNode){
	map <double, BpTreeNode*>* m = pIndexNode->getIndexMap();								//m is the map of pIndexNode.
	
	if (m == NULL)							return false;

	else if(m->size() > order-1)			return true;									//if the size of map is bigger than order-1
	else									return false;
}



bool BpTree::Print(){																		//PRINT_BP
	BpTreeNode* start = root;
	BpTreeNode* travel;

	while (start->getMostLeftChild()!=NULL)													//'start' move to the MostLeftChild until meet node that does not have any MostLeftChild.
		start = start->getMostLeftChild();
	while (start->getNext() != NULL)														//'start' move to the nextnode until meet node that does not have any nextnode.
		start = start->getNext();
	travel = start;																			//travel is traveling node that moving the tree.

	*fout << "====================" << " " << "PRINT" << " =====================";

	map<double, vector<pair<int, StockData*> >>::iterator end_it;							//declare iterator end_it
	end_it = travel->getDataMap()->end();													//end_it is the most right map component in the most right node.
	end_it--;

	while (1)
	{
		while (travel->getDataMap()->begin() != end_it)
		{
			printDecreasingOrder(end_it->second);											//print
			end_it--;
		}
		printDecreasingOrder(end_it->second);												//print
		if (travel->getPrev() == NULL)														//the end
		{
			*fout << "===============================================" << endl;
			return true;
		}
		travel = travel->getPrev();
		end_it = travel->getDataMap()->end();
		end_it--;
	}
}
bool BpTree::Search(double a, double b)														//Search
{
	stack<vector<pair<int, StockData*>>> s;
	vector<pair<int, StockData*>> accept;													//declare vector
	
	map<double, vector<pair<int, StockData*> >>::iterator m_it;
	BpTreeNode* start = root;

	while (start->getMostLeftChild() != NULL)
		start = start->getMostLeftChild();

	m_it = start->getDataMap()->begin();													//m_it is begin of datamap in 'start' node.
	while (1)
	{
		if (m_it->first <= b && a <= m_it->first)
			s.push(m_it->second);
		m_it++;
		if (m_it == start->getDataMap()->end())
		{
			start = start->getNext();
			if (start == NULL)
				break;
			m_it = start->getDataMap()->begin();
		}
	}
	if (s.empty())																			//if stack is empty
		return false;

	*fout << "========" << " SEARCH " << "=======";
	while (!s.empty())
	{
		accept = s.top();
		printDecreasingOrder(accept);
		s.pop();
	}
	*fout << "======================" << endl;
	return true;
}

void BpTree::printDecreasingOrder(vector<pair<int, StockData*>> a)							//Print
{
	vector<pair<int, StockData*>>::iterator it = a.end() - 1;
	*fout << fixed << setprecision(2);
	for (it; it != a.begin(); it--)
	{
		*fout << endl << it->first << " " << it->second->getName() << " " << it->second->getEarning() << endl;
		*fout << "시가: " << it->second->getOpening_Price() << endl;
		*fout << "종가: " << it->second->getClosing_Price() << endl;
		*fout << "거래량: " << it->second->getVolume() << endl;
		*fout << "수익률: " << it->second->getEarning() << endl;
	}
	*fout << endl << it->first << " " << it->second->getName() << " " << it->second->getEarning() << endl;
	*fout << "시가: " << it->second->getOpening_Price() << endl;
	*fout << "종가: " << it->second->getClosing_Price() << endl;
	*fout << "거래량: " << it->second->getVolume() << endl;
	*fout << "수익률: " << it->second->getEarning() << endl;
	return;
}
