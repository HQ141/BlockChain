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
	int (*func)(D, P &,int,int);
	int iMinners;
	vector<thread> Minners;
	size_t base_hash;
	int ifiles;
public:
	Corporation(int (*f)(D,P&,int,int),int n):iMinners(n),func(f){
		if (_mkdir("Config")) {
			fstream config;
			config.open("Config/Config.txt", ios::out);
			config << 0 << endl;
			config << hash<string>{}("");
			ifiles = 0;
			base_hash= hash<string>{}("");
			config.close();
		}
		else {
			fstream config;
			config.open("Config/Config.txt", ios::in);
			config >> ifiles;
			config >> base_hash;
			config.close();
		}
	}
	int start(json data) {
		srand(time(0));
		status curr;
		curr.iMinners = iMinners;
		stringstream obj;
		obj << data;
		string a = obj.str() + to_string(rand()%999+1);
		curr.hash = hash<string>{}(a);
		for (int i = 0; i < iMinners; i++) {
			Minners.push_back(thread(func, data, std::ref(curr), i,1000));
		}
		for (int i = 0; i < iMinners; i++) {
			Minners[i].join();
		}
		Minners.clear();
		fstream config;
		config.open("Config/Config.txt", ios::out);
		config << ++ifiles << endl;
		config << hash<string>{}("");
		base_hash = hash<string>{}("");
		config.close();
		return 1;
	}
};
