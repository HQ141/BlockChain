#include <iostream>
using namespace std;
#include <thread>
using namespace std;
void nQueen(int n);
bool track(int**, int, int, int);
bool safe(int** arr, int);
bool track(int** arr, int i, int j, int s) {
	if (i >= s || j >= s)
		return false;
	arr[i][j] = 1;
	track(arr, i + 1, 0, s);
	if (safe(arr, s))
		return true;
	arr[i][j] = 0;
	return track(arr, i, j + 1, s);
}
bool safe(int** arr, int s) {

	int a[4];
	for (int i = 0; i < s; i++)
		for (int j = 0; j < s; j++) {
			if (arr[i][j])
				a[i] = j;
		}
	for (int i = 0; i < s-1; i++) {
		for (int j = i+1; j < s; j++) {
			if (a[i] == a[j])
				return 0;
			if (abs(i - j) == abs(a[i] - a[j]))
				return 0;
		}
	}
}
void nQueen(int n) {
	int s = 4;
	int** arr = new int* [s];
	for (int i = 0; i < s; i++) {
		arr[i] = new int[s];
		for (int j = 0; j < s; j++)
			arr[i][j] = 0;
	}
	track(arr, 0, 0, s);
	if (safe(arr, s))
		cout << "solved" << endl;
		
}
int main()
{
	std::thread first(nQueen,1); 
	std::thread second(nQueen,2);
	first.join();     
	second.join();
	return 0;
}