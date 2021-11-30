#include<iostream>
#include<thread>
#include "json.hpp"
#include<fstream>
#include"Puzzle_data.h"
using namespace std;
using json = nlohmann::json;

template <class D,class P> 
class Corporation {
private:
	int (*func)(D, P &,int );
	int iMinners;
	vector<thread> Minners;
public:
	Corporation(int (*f)(D,P&,int),int n):iMinners(n),func(f){}
	int start(json data) {
		srand(time(0));
		status curr;
		curr.iMinners = iMinners;
		stringstream obj;
		obj << data;
		string a = obj.str() + to_string(rand()%199+1);
		curr.hash = hash<string>{}(a);
		for (int i = 0; i < iMinners; i++) {
			Minners.push_back(thread(func, data, std::ref(curr), i));
		}
		for (int i = 0; i < iMinners; i++) {
			Minners[i].join();
		}
		Minners.clear();
		return 1;
	
	}
};
