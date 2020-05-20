#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> table;
vector<string> first_table;

vector<string> first() {	
	for (int i=0; i<table.size(); i++) {
		string temp = table[i];
		string symbol_str;
		cout << "ok" << endl;
		for (int j=0; j<temp.size(); j++) {
			char symbol = temp[j];
			if (symbol == '<' || symbol == '=' || symbol == ' ') continue;
			symbol_str += symbol;
		}
		if(!first_table.empty()) {
			temp = first_table.back();
			if(temp[0] == table[i][0]) {
				first_table.pop_back();
				symbol_str.substr(1);
				symbol_str = temp + symbol_str;
			}
		}
		first_table.push_back(symbol_str);
	}
}

int main() {
	string str = "";
	while(1) {
		cin.getline(str,256);
		cout << str << endl;
		if(str == "0") break;
		table.push_back(str);
	}
	
	first();
	for (int i=0; i<first_table.size(); i++) {
		string temp = first_table[i];
		for (int j=0; j<temp.size(); j++) {
			if(j==0) cout << temp[j] << ' ' << "<==" ;
			cout << temp[j] << ' ';
		}
		cout << '\n';
	}
	
	
}
