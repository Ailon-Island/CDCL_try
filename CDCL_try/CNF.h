#pragma once
#ifndef _CNF_H
#define _CNF_H
#include <vector>

class CNF
{
private:
	std::vector< std::vector< std::vector< int > > > cnf;
	std::vector< int > assignSeq;
	std::vector< int >* assign;
	//False = 0 True = 1 Unassigned = 2
	std::vector< int >* source;
	std::vector < int >* existence;
	int pickBegin = 0, validVar = 0;
	int UnitPro();
	//0 for unpropagatable, 1 for normal, 2 for empty conflict clause
	void Pick();
	void ConflictClauseGen(const int &target);
	void Unpick();
	int Propa();
	//0 for normal(regular conflict or successful propagatation), 1 for solution, 2 for empty conflict clause
	void sourcer(const int &, const int &);
	void addNum(const bool&, int&);
public:
	void gen(const int&, const int&);
	void input();
	void outputAssignment() const;
	bool solve();
	~CNF();
};

#endif // !_CNF_H
