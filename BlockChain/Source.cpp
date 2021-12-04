#include <iostream>
#include <iomanip>
#include<thread>
#include<chrono>
#include<fstream>
#include"Miner.h"
using namespace std;
using json = nlohmann::json;
int main() {
	Corporation<json, status> cc(Mine, 50);
	fstream a;
	Data data;
	string name;
	string id;
	a.open("Text.txt", ios::in|ios::end);
	while (!a.eof()) {
		a >> data;
		cout<<setw(4)<<data.getjson();
		if (a.peek()==EOF)
			break;
		cc.start(data.getjson());
	}
	//a >> data;
	//cc.start(data.getjson());
	/*a >> data;
	cc.start(data.getjson());
	a >> data;
	cc.start(data.getjson());*/

}