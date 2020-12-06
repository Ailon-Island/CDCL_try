#include "CNF.h"

#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;
//var[0] stores variable names; var[1] stores truth assignments

int getOption();

int main()
{
	clock_t clockStart, clockFinish;
	int opt;
	int genNum, varNum, clauseNum;
	CNF *cnf;
	unsigned int satCount, unsatCount;
	
	cout << "Welcome to CDCL SAT solver!\n";
	while (opt = getOption())
	{
		
		switch (opt)
		{
		case 1:
			cnf = new CNF;
			cnf->input();
			clockStart = clock();
			if (cnf->solve())
			{
				clockFinish = clock();
				cout << "\nSAT\n";
				cnf->outputAssignment();
			}
			else
			{
				clockFinish = clock();
				cout << "\nUNSAT\n";
			}
			delete cnf;
			cout << "CDCL ran for " << (double(clockFinish - clockStart) / CLOCKS_PER_SEC) << " second(s).\n";
			break;
		case 2:
			satCount = 0, unsatCount = 0;
			cout << "\nHow many CNFs would you like to generate?\n";
			cin >> genNum;
			cout << "\nWhat's the largest variable name n you'd like to be in your cnf?(all variables will have number names.)" << endl;
			while (!(cin >> varNum) || (varNum < 0))
			{
				cout << "Input error!(n needs to be a natural number)" << endl << endl;
				cin.clear();
				cin.sync();
				cout << "What's the largest variable name n you'd like to be in your cnf?(all variables will have number names.)" << endl;
			}
			cout << "\nHow many clauses would you like to have in each CNF?" << endl;
			while (!(cin >> clauseNum) || (clauseNum < 1))
			{
				cout << "Input error!(n needs to be a positive integer)" << endl << endl;
				cin.clear();
				cin.sync();
				cout << "How many clauses would you like to have in each CNF?" << endl;
			}
			for (int i = 0; i < genNum; ++i)
			{
				cnf = new CNF;
				cnf->gen(varNum, clauseNum);
				clockStart = clock();
				if (cnf->solve())
				{
					clockFinish = clock();
					cout << endl << setfill('0') << setw(8) << i + 1 << "\nSAT\n";
					++satCount;
					cnf->outputAssignment();
				}
				else
				{
					clockFinish = clock();
					cout << endl << setfill('0') << setw(8) << i + 1 << "\nUNSAT\n";
					++unsatCount;
				}
				delete cnf;
				cout << "CDCL ran for " << (double(clockFinish - clockStart) / CLOCKS_PER_SEC) << " second(s).\n";

			}
			cout << "\nSAT count: " << satCount << "\nUNSAT count: " << unsatCount << "\nTotal count: " << genNum << endl;
			break;
		}
	}
	return 0;
}

int getOption()
{
	int opt;

	cout << "\nWould like to have CNF generated themselves or input a CNF?\n"
		<< "\t0 for exit\n"
		<< "\t1 for input\n"
		<< "\t2 for generation\n";
	cin >> opt;
	return opt;
}