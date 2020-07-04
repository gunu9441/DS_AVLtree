#include "Manager.h"

Manager::Manager(int bpOrder) {
	fout.open("log.txt", ios::app);
	avl = new AVLTree(&fout);
	bp = new BpTree(&fout, bpOrder);
}

Manager::~Manager() {
	delete avl;
	delete bp;
	fout.close();
}

void Manager::run(const char* command_txt) {
	ifstream fin;													//declare stream
	fin.open(command_txt);											//open a file,'command_txt'
	int	load = -1;													//identifier of the number of LOAD functions implementation
	int IsWork;														//identifier of working or not working

	if (!fin)														//stream is not opened.
	{
		cout << "[ERROR] command file open error!" << endl;
		return;
	}

	char*	str = NULL;
	char	buf[128] = { 0 };
	char	buf2[128] = { 0 };

	while (fin.getline(buf, 64)) {
		strcpy(buf2, buf);
		str = strtok(buf, " \n");

		if (strcmp(str, "LOAD") == 0) {								//LOAD
			IsWork = LOAD();
			if (load == 1)											//When Load is implemeting twice, print error code 100.
			{
				printErrorCode(100);
			}
			else if ((str = strtok(NULL, " \n")) != NULL || !IsWork)//When adding a strange word in "Load" or not working, Load() returns false and print error code.
			{
				printErrorCode(100);
			}
			else if (IsWork)										//Working successfully.
				fout << "======== LOAD ========" << endl << "Success" << endl << "=====================" << endl;
			load = 1;
		}
		else if (strcmp(str, "SEARCH_AVL") == 0) {					//SEARCH_AVL
			char *name;

			name = strtok(NULL, " ");
			str = strtok(NULL, "\n");

			if (name == NULL || str != NULL || !SEARCH_AVL(name))//When adding a strange word in "SEARCH_AVL" or not working, SEARCH_AVL returns false and print error code.
				printErrorCode(300);
		}
		else if (strcmp(str, "SEARCH_BP") == 0) {					//SEARCH_BP
			char *anotherstr;

			str = strtok(NULL, " ");
			anotherstr = strtok(NULL, "\n");

			if (str == NULL || anotherstr == NULL)					//Exception Handling
				printErrorCode(200);
			else if (!SEARCH_BP(atof(str), atof(anotherstr)))
				printErrorCode(200);
		}
		else if (strcmp(str, "RANK") == 0) {						//RANK
			if ((str = strtok(NULL, "\n")) != NULL || !RANK())
				printErrorCode(400);
		}
		else if (strcmp(str, "PRINT_AVL") == 0) {					//PRINT_AVL
			if ((str = strtok(NULL, "\n")) != NULL || !PRINT_AVL())
				printErrorCode(600);
		}
		else if (strcmp(str, "PRINT_BP") == 0) {					//PRINT_BP
			if ((str = strtok(NULL, "\n")) != NULL || !PRINT_BP())
				printErrorCode(500);
		}
		else if (strcmp(str, "EXIT") == 0) {						//EXIT
			fout << "==================== EXIT ======================" << endl << "Success" << endl << "===============================================" << endl;
			break;
		}
		fout << endl;
	}
	fin.close();
}

bool Manager::LOAD() {												//LOAD
	char*	str = NULL;
	char	buf[128] = { 0 };
	char	buf2[128] = { 0 };

	ifstream fin;													//declare stream
	fin.open("stock_list.txt");										//open a file,'stock_list.txt'

	if (!fin) {														//stream is not opened.
		return false;
	}

	while (fin.getline(buf, 64)) {
		StockData* pSto;
		strcpy(buf2, buf);

		pSto = new StockData();

		str = strtok(buf2, " ");
		pSto->setStockID(atoi(str));								//get stockID
		str = strtok(NULL, " ");
		pSto->setName(str);											//get Name
		int open = atoi(strtok(NULL, " "));
		pSto->setOpening_Price(open);								//get opening price
		int close = atoi(strtok(NULL, " "));
		pSto->setClosing_Price(close);								//get closing price
		int volume = atoi(strtok(NULL, " "));
		pSto->setVolume(volume);									//get volume

		pSto->setEarning(open, close);								//set Earning

		avl->Insert(pSto);											//Insert pSto to make AVL tree
		bp->Insert(pSto);											//Insert pSto to make B+ tree
	}
	return true;
}

bool Manager::SEARCH_AVL(char* name) {								//SEARCH_AVL
	if (avl->getRoot() == NULL)//avl is not established.
		return false;
	else if (!avl->Search(name))//not found
		return false;
	else				   //found
		return true;
}

bool Manager::SEARCH_BP(double a, double b) {						//SEARCH_BP
	if (bp->getRoot() == NULL)
		return false;
	else if (!bp->Search(a, b))
		return false;
	else
		return true;
}

bool Manager::RANK() {												//RANK
	if (!avl->Rank())		return false;
	else					return true;
}

bool Manager::PRINT_AVL() {											//PRINT_AVL
	if (avl->getRoot() == NULL)
		return false;
	else if (!avl->Print())	return false;
	else					return true;
}

bool Manager::PRINT_BP() {											//PRINT_BP
	if(bp->getRoot()==NULL) return false;
	else if (!bp->Print())		return false;
	else					return true;
}

void Manager::printErrorCode(int n) {								//print errorcode
	fout << "======== ERROR ========" << endl;
	fout << n << endl;
	fout << "======================" << endl;
}
