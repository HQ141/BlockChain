#include <iostream>
#include <iomanip>
#include<thread>
#include<chrono>
#include<fstream>
#include"Miner.h"
using namespace std;
using json = nlohmann::json;
int main() {
	Corporation<json, status> cc(Mine, 3);
	fstream a;
	Data data;
	string name;
	string id;
	a.open("Text.txt", ios::in);
	while (!a.eof()) {
		a >> name;
		a >> id;
		data.set(name, id);
		cc.start(data.getjson());
	}

	
}