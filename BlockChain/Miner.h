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
	int Hash_Space;
	int l_base;
	int Occ_hash;
	int mod = 500;
public:
	Corporation(void (*f)(D,P&,size_t&,int,int),int n):iMinners(n),func(f){
		if (!_mkdir("Config")) {
			fstream config;
			config.open("Config/Config.txt", ios::out);
			config << 0 << endl;
			config << hash<string>{}("")<<endl;
			Hash_Space = 0;
			Occ_hash = 0;
			ifiles = 0;
			base_hash= hash<string>{}("");
			config.close();
			config.open("Config/Merkel_Config.txt", ios::out);
			config << 0 << endl;
			config << 0 << endl;
			config << 0;
			l_base = 0;
			config.close();
			config.open("Config/Merkel_Tree.txt", ios::out);
			config.close();
		}
		else {
			fstream config;
			config.open("Config/Config.txt", ios::in);
			config >> ifiles;
			config >> base_hash;
			config.close();
			config.open("Config/Merkel_Config.txt", ios::in);
			config >> Hash_Space;
			config >> Occ_hash;
			config >> l_base;
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
	void update_tree(size_t ha) {
		fstream config;
		uint64_t h = ha;
		uint64_t prev_hash;
		if (Occ_hash == Hash_Space) {
			uint64_t t0 = 0;
			uint64_t t1 = 1;
			config.open("Config/Merkel_Tree.txt", ios::app | ios::binary);
			for (int i = 0; i < Hash_Space;i++) 
					config.write((const char*)&t0, 8);
			config.write((const char*)&t0, 8);
			Hash_Space = Hash_Space * 2 + 1;
			config.close();
		}
		config.open("Config/Merkel_Tree.txt", ios::out | ios::in | ios::binary);
		if (ifiles == 1) {
			config.seekg(0, ios::beg);
			config.write((const char*)&h, 8);
			Occ_hash++;
			l_base = 1;
		}
		else if (ifiles % 2 == 0) {
			config.seekg(l_base*8, ios::beg);
			config.write((const char*)&h, 8);
			Occ_hash++;
			l_base++;
			int temp = ifiles;
			int up = 1;
			int down = 0;
			while (temp % 2 == 0) {
				Occ_hash++;
				config.seekg(-16*up, ios::cur);
				config.read((char*)&prev_hash, 8);
				config.seekg(8*(down+1), ios::cur);
				h = std::hash<uint64_t>{}(prev_hash + h);
				config.write((const char*)&h, 8);
				temp /= 2;
				up++; down++;
			}
			
		}
		else {
			int temp = ifiles-1;
			int pos=0;
			while (temp % 2 == 0) {
				pos++;
				temp /= 2;
			}
			config.seekg((l_base+pos)* 8, ios::beg);
			config.write((const char*)&h, 8);
			Occ_hash++;
			l_base+=pos+1;
			
		}
		/*else {
			int temp = ifiles
				int a = 2;
				while (temp / 2!=0) {
					config.seekg(-8*a, ios::end);
					config.read((char*)&prev_hash, 8);
					config.seekg(-8, ios::end);
					config.write((const char*)&h, 8);
					h = hash<uint64_t>{}(prev_hash + h);
					config.write((const char*)&h, 8);
					config.seekg(16, ios::beg);
					config.read((char*)&prev_hash, 8);
					config.seekg(0, ios::end);
					h = hash<uint64_t>{}(prev_hash + h);
					config.write((const char*)&h, 8);
			}
		}
		else if (ifiles == 2) {
			
			config.read((char*)&prev_hash, 8);
			config.write((const char*)&h, 8);
			h = hash<uint64_t>{}(prev_hash + h);
			config.write((const char*)&h, 8);
		}
		else if (ifiles == 3) {
			config.seekg(-8, ios::end);
			config.read((char*)&prev_hash, 8);
			config.write((const char*)&h, 8);
			h = hash<uint64_t>{}(prev_hash + h);
			config.write((const char*)&h, 8);
		}
		else if (ifiles == 4) {
			config.seekg(-16, ios::end);
			config.read((char*)&prev_hash, 8);
			config.seekg(-8, ios::end);
			config.write((const char*)&h, 8);
			h = hash<uint64_t>{}(prev_hash + h);
			config.write((const char*)&h, 8);
			config.seekg(16, ios::beg);
			config.read((char*)&prev_hash, 8);
			config.seekg(0, ios::end);
			h = hash<uint64_t>{}(prev_hash + h);
			config.write((const char*)&h, 8);
		}*/
		config.close();
		config.open("Config/Merkel_Config.txt", ios::out);
		config << Hash_Space << endl;
		config << Occ_hash << endl;
		config << l_base;
	}
};
