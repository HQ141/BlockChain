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
	void (*func)(D, P &,size_t&,int,int);
	int iMinners;
	vector<thread> Minners;
	size_t base_hash;
	int ifiles;
	int mod = 500;
public:
	Corporation(void (*f)(D,P&,size_t&,int,int),int n):iMinners(n),func(f){
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
		string a = obj.str() + to_string(rand()%(mod-1));
		curr.hash = hash<string>{}(a);
		size_t data_hash;
		for (int i = 0; i < iMinners; i++) {
			Minners.push_back(thread(func, data, std::ref(curr),ref(data_hash), i,mod-1));
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
		update_tree(data_hash);
		return 1;
	}
	void update_tree(size_t hash) {
		fstream config;
		uint64_t h = hash;
		config.open("Config/Merkel_Tree.txt", ios::app|ios::binary);
		config.write((const char*)&h, 8);
		config.close();
	}
};
