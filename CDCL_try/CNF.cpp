#include "cnf.h"

#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <random>
#include <iostream>

using namespace std;

CNF::~CNF()
{
	delete assign;
	delete[] source;
	delete existence;
}

void CNF::input()
{
	int varNum;
	string clause;
	int sum;
	bool flag;

	//cout //<< "\nWhat's the largest variable name n in your cnf?(only numbers are valid variable names, you should rename your variables with 0, 1, 2, ..., n)" //<< endl;
	while (!(cin >> varNum) || (varNum < 0))
	{
		//cout //<< "Input error!(n needs to be a natural number)" //<< endl //<< endl;
		cin.clear();
		cin.sync();
	}
	//restricted variable name inputting saves memory
	existence = new vector< int >(size_t(varNum) + 1, 0);
	//cout //<< "\nPlease input the 3-cnf clause by clause and end with an 'E' in the last line.(the name of the propositional variables should be in N and no larger than " //<< varNum //<< ")" //<< endl
		//<< "e.g.:" //<< endl
		//<< "When your n is 4, you can input:" //<< endl
		//<< "1+2+0\n-4-1+3\nE\n";
	while (1)
	{
		cin >> clause;
		if ((clause == "E") || (clause == "e"))
			break;
		//use E to indicate the end of input
		sum = -1;
		//preventing nothing inputed for a variable
		cnf.push_back({});
		flag = 1;
		//initializing whether the variable is with a "not"
		for (int i = 0; i < int(clause.length()); ++i)
		{
			if (isdigit(clause[i]))
			//getting numbers
			{
				sum = (sum == -1) ? 0 : sum;
				sum = sum * 10 + clause[i] - '0';
			}
			else
				if ((clause[i] == '+') || (clause[i] == '-'))
					//which means it's time to add a new variable to the clause
				{
					if (sum > varNum)
						//overrange
					{
						//cout //<< "Input error!(variable name overrange)" //<< endl //<< endl;
						cnf.clear();
						input();
					}
					if (sum != -1)
						//numbers in
					{
						addNum(flag, sum);
					}
					if (!isdigit(clause[(i > 0) ? (i - 1) : 0]))
					{
						if (clause[i] == '-')
							flag = 0;
					}
					else
					{
						flag = bool(clause[i] - '-');
					}
					//change the "not" situation, allowing input like "+-" and "-+"
				}
				else
				{
					//cout //<< "Input error!(undefined character)" //<< endl //<< endl;
					cnf.clear();
					input();
				}
		}
		if (sum != -1)
		{
			addNum(flag, sum);
		}
	}
	//input and store the cnf
	if (!(cnf.size()))
	{
		//cout //<< "Input error!(nothing entered)" //<< endl //<< endl;
		cnf.clear();
		input();
		return;
	}
	assign = new vector< int >(size_t(varNum) + 1, 2);
	source = new vector< int >[size_t(varNum) + 1];
	//apply for space for truth assignments and the logical source of each assignment
	//cout //<< "\ncnf get!\n";
	for (int i = 0; i < int(cnf.size()); ++i)
	{
		//cout //<< '(' //<< i //<< "): ";
		for (int j = 0; j < int(cnf[i].size()); ++j)
		{
			//cout //<< ((cnf[i][j][0]) ? '+' : '-') //<< cnf[i][j][1];
		}
		//cout //<< endl;
	}
	//output the cnf stored
}

void CNF::gen(const int &varNum, const int &clauseNum)
{
	static default_random_engine engine(time(0));
	uniform_int_distribution< unsigned int >* randVar = new uniform_int_distribution< unsigned int >(0, varNum);
	uniform_int_distribution< unsigned int > randVarCount(0, clauseNum / 20);
	uniform_int_distribution< unsigned int >* randCls = new uniform_int_distribution< unsigned int >(0, clauseNum - 1);
	uniform_int_distribution< short int > randBool(0, 1);
	int cls, var;
	bool posi;
	
	existence = new vector< int >(size_t(varNum) + 1, 0);
	assign = new vector< int >(size_t(varNum) + 1, 2);
	source = new vector< int >[size_t(varNum) + 1]; 
	for (int i = 0; i < clauseNum; ++i)
	{
		cnf.push_back({});
		cnf[i].push_back({});
		cnf[i][0].push_back(posi = randBool(engine));
		cnf[i][0].push_back(var = (*randVar)(engine));
		if (!existence->at(var))
			++validVar;
		existence->at(var) = 1;
	}
	for (int i = 0; i < 2 * clauseNum /*- randVarCount(engine)*/; ++i)
	{
		while (cnf[cls = (*randCls)(engine)].size() == 3);
		cnf[cls].push_back({});
		cnf[cls][cnf[cls].size() - 1].push_back(posi = randBool(engine));
		cnf[cls][cnf[cls].size() - 1].push_back(var = (*randVar)(engine));
		if (!existence->at(var))
			++validVar;
		existence->at(var) = 1;
	}
	//randomly generate a 3-CNF
	//cout //<< "\ncnf generated!\n";
	for (int i = 0; i < int(cnf.size()); ++i)
	{
		//cout //<< '(' //<< i //<< "): ";
		for (int j = 0; j < int(cnf[i].size()); ++j)
		{
			//cout //<< ((cnf[i][j][0]) ? '+' : '-') //<< cnf[i][j][1];
		}
		//cout //<< endl;
	}
	//output the cnf generated
}

bool CNF::solve()
{
	switch (Propa())
	{
	case 1:
		return 1;
		//SAT
		break;
	case 2:
		return 0;
		//UNSAT
		break;
	}
	//if (int(assignSeq.size()) == (validVar)) return 1;
	Pick();
	return(solve());
}

void CNF::Pick()
{
	//cout //<< "\nPick:\n\t";
	for (int i = pickBegin; i < int(assign->size()); ++i)
		if ((assign->at(i) == 2) && existence->at(i))
		{
			//cout //<< i //<< "|->T \n";
			assign->at(i) = 1;
			assignSeq.push_back(i);
			source[i].push_back(i);
			pickBegin = i + 1;
			return;
		}
}

void CNF::Unpick()
{
	int count = 0;

	for (int i = int(assignSeq.size() - 1); i >= 0; --i)
	{
		if (existence->at(assignSeq[i]))
		{
			for (int j = 0; j < int(cnf[cnf.size() - 1].size()); ++j)
			{
				if (assignSeq[i] == cnf[cnf.size() - 1][j][1])
					if (++count == 2)
						return;
				
			}
			//cout //<< "\nUnpick " //<< assignSeq[i];
			assign->at(assignSeq[i]) = 2;
			source[assignSeq[i]].clear();
			if (pickBegin > assignSeq[i])
				pickBegin = assignSeq[i];
			//relocate where to start a pick
			assignSeq.pop_back();
		}
	}
	//cout //<< endl;
	return;
}

int CNF::UnitPro()
{
	int mark = -1, count, i, j, q;
	bool done = 0;

	i = 0;
	//iterate clause by clause for potential propagation
	count = 0;
	//the count of unassigned variables in clause i
	for (j = 0; j < cnf[i].size(); ++j)
	{
		//////cout //<< i //<< j //<< endl;
		if ((assign->at(cnf[i][j][1])) == 2)
		{
			//unassigned variable exists
			if (!count)
				//no previous different unassigned variable
			{
				//////cout //<< "marked!\n";
				if (mark != cnf[i][j][1])
				{
					++count;
					mark = cnf[i][j][1];
					q = j;
				}
			}
			//mark the unassigned variable
			//add to the count of unassigned variable
			else
			//already one unassigned variable
			{
				if (i == (cnf.size() - 1))
				{
					count = 0;
					mark = -1;
					done = 1;
					break;
				}
				count = 0;
				mark = -1;
				++i;
				j = -1;
			}
			//skip the clause because it cannot help propagation
		}
		else
			//the current variable has been assigned
			if (!(cnf[i][j][0] ^ assign->at(cnf[i][j][1])))
			//a certain variable in the clause makes the clause true
			{
				if ((i == (cnf.size() - 1)))
				{
					count = 0;
					mark = -1;
					done = 1;
					//tell the program that the unit propagation has been done
					break;
				}
				count = 0;
				mark = -1;
				++i;
				j = -1;
			}
			//skip the clause because it cannot help propagation
	}
	if (count)
		//only one unassigned variable and no other variables in the clause making the clause true
	{
		assign->at(mark) = cnf[i][q][0];
		assignSeq.push_back(mark);
		//cout //<< '\t' //<< cnf[i][q][1] //<< "|->" //<< ((assign->at(mark)) ? 'T' : 'F') //<< " from(" //<< i //<< ")\n";
		//assign the propagated truth value and record the sequence
		sourcer(mark, i);
		//record the logical source of the truth value of the newly assigned variable
		if (UnitPro() == 2)
			return 2;
		return 1;
		//unit propagate again and exit the current function
	}
	//execute propagation
	else if (!((i == int(cnf.size() - 1)) && done))
	//a clause without any possibly true variable
	{
		//cout //<< "\tconflict occurred in clause\n\t(" //<< i //<< "): ";
		for (int j = 0; j < int(cnf[i].size()); ++j)
		{
			//cout //<< ((cnf[i][j][0]) ? '+' : '-') //<< cnf[i][j][1];
		}
		//cout //<< endl;
		ConflictClauseGen(i);
		if (!cnf[cnf.size() - 1].size())
			return 2;
		if (Propa() == 2)
			return 2;
	}
	return 0;
}

void CNF::ConflictClauseGen(const int &target)
{
	vector< int > toBeSearched;
	int size;
	bool flag;

	cnf.push_back({});
	//prepare for the new clause
	
	size = cnf.size() - 1;
	for (int i = 0; i < cnf[target].size(); ++i)
	{
		for (int j = 0; j < source[i].size(); ++j)
		{
			flag = 1;
			for (int k = 0; k < cnf[size].size(); ++k)
				if (source[i][j] == cnf[size][k][1])
				{
					flag = 0;
					break;
				}
			if (flag)
			{
				cnf[size].push_back({});
				cnf[size][cnf[size].size() - 1].push_back(!assign->at(source[i][j]));
				cnf[size][cnf[size].size() - 1].push_back(source[i][j]);
				
			}
		}
	}
	
	/*for (int i = 0; i < cnf[target].size(); ++i)
		toBeSearched.push_back(cnf[target][i][1]);*/
	//cout //<< "\nConflictClauseGeneration:\n";
	if (!cnf[size].size())
	{
		//cout //<< "\tThe generated clause is Empty!\n";
		return;
	}
	//cout //<< "\t(" //<< size //<< "): ";
	for (int j = 0; j < cnf[size].size(); ++j)
	{
		//cout //<< ((cnf[size][j][0]) ? '+' : '-') //<< cnf[cnf.size() - 1][j][1];
	}
	//cout //<< endl;
	Unpick();
	return;
}

void CNF::sourcer(const int &mark, const int &i)
{
	for (int j = 0; j < cnf[i].size(); ++j)
	{
		if (cnf[i][j][1] != mark)
		{
			for (int k = 0; k < source[cnf[i][j][1]].size(); ++k)
			{
				if (find(source[mark].begin(), source[mark].end(), source[cnf[i][j][1]][k]) == source[mark].end())
				{
					source[mark].push_back(source[cnf[i][j][1]][k]);
				}
			}
		}
	}
	/*bool flag = 1;

	for (int i = 0; i < sources.size(); ++i)
	{
		if ((source[sources[i]].size()))
			if (source[sources[i]][0] == sources[i])
			{
				for (int j = 0; j < cnf[cnf.size() - 1].size(); ++j)
				{
					if (sources[i] == cnf[cnf.size() - 1][j][1])
						flag = 0;
				}
				if (flag)
				{
					cnf[cnf.size() - 1].push_back({});
					cnf[cnf.size() - 1][cnf[cnf.size() - 1].size() - 1].push_back(!(assign->at(sources[i])));
					cnf[cnf.size() - 1][cnf[cnf.size() - 1].size() - 1].push_back(sources[i]);
				}
			}
			else
				OriginAndGen(source[sources[i]]);
	}*/
}

void CNF::outputAssignment() const
{

	int killSpace = 0;

	//cout //<< "The truth assignment can be:";
	//cout //<< endl //<< '[';
	for (int i = 0; i < assign->size(); ++i)
	{
		//if (existence->at(i))
			//cout //<< ((killSpace++) ? (' ') : ('\0')) //<< i //<< "|->" //<< ((assign->at(i)) ? 'T' : 'F');
	}
	//cout //<< "]\n";
}

void CNF::addNum(const bool &flag, int &sum)
{
	cnf[cnf.size() - 1].push_back({});
	cnf[cnf.size() - 1][cnf[cnf.size() - 1].size() - 1].push_back(flag);
	cnf[cnf.size() - 1][cnf[cnf.size() - 1].size() - 1].push_back(sum);
	if (!existence->at(sum))
		++validVar;
	existence->at(sum) = 1;
	sum = -1;
}

int CNF::Propa()
{
	size_t cnfSize_current;
	int UnitProRes;

	cnfSize_current = cnf.size();
	//cout //<< "\nPropagation:\n";
	switch (UnitPro())
	{
	case 0:
		//cout //<< "\tThe recent assignments cannot be propagated.\n";
	case 1:
		if (assignSeq.size() == validVar)
			return 1;
		break;
	case 2:
		return 2;
	}
	return 0;
}