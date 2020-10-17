// 201610674 이영훈
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cstring>
using namespace std;
#define EPSILON '_'
#define MAX 100


char lh[MAX], s, t[100], nt[100], tnt[100], temp_read[MAX][100], temp, templ[MAX], temp2[5], slr[MAX][MAX][6];
string rh[MAX], tempr[MAX];
int p,n = 1, ns = 0, rr = 0,n_tnt, tn = 0, n_t, n_nt;

struct state{
	char lh[MAX];
	string rh[MAX];
	int n, last;
} I[MAX];


int fine_old(struct state s1,struct state s2) 
{ 
	int t; 
	if(s1.n != s2.n) 
		return 0; 
	if(strcmp(s1.lh,s2.lh) != 0) 
		return 0; 
	for(t = 0; t < s1.n; t++) 
		if(s1.rh[t] != s2.rh[t]) 
			return 0; 
	return 1; 
} 

int indexOf(char ip, char symbol[]) {
	if (ip == '$') return strlen(symbol);
	for(int i = 0; i < strlen(symbol); i++)
		if(symbol[i] == ip)
			return i;
	return -1;
}

void closure() { 
	int r,s,t,l1=0,rr1=0; 
	string read1[MAX]; 
	for(r = 0; r < I[ns].n; r++) { 
		t = I[ns].rh[l1].find('.');
		if(t + 1 == I[ns].rh[l1].length())  // A->aBb.
		{ 
			l1++; 
			continue; 
		} 
		temp = I[ns].rh[r][t+1]; 
		l1++; // l1 is useless it should be r
		for(s = 0; s < rr1; s++) 
			if( temp == read1[s][0] ) 
				break; 
		if(s == rr1) { 
			read1[rr1][0] = temp; 
			rr1++; 
		} 
		else 
			continue; 
		for(s = 0; s < n; s++) { 
			if(lh[s] == temp) {
				I[ns].rh[I[ns].n] = "."; 
				I[ns].rh[I[ns].n][1] = '\0'; 
				I[ns].rh[I[ns].n] = I[ns].rh[I[ns].n] + rh[s];
				I[ns].lh[I[ns].n] = lh[s]; 
				I[ns].lh[I[ns].n + 1] = '\0'; 
				I[ns].n++; 
			} 
		} 
	} 
} 


void newstate(int l, ostream &AG) {
	int t1; 
	char read1[MAX][10],rr1 = 0;
	char* ptr;
	for(int i = 0; i < I[l].n; i++) { // ith rule of state Il
		
		temp2[0] = '.'; // Here temp2 is "."
		t1 = I[l].rh[i].find('.'); // no of chars before '.'
		if(t1 + 1 == I[l].rh[i].length()) // A->aBb.
			continue;
			
		temp2[1] = I[l].rh[i][t1+1]; // Here temp2 is ".B"
		temp2[2] = '\0'; // Here temp2 is ".B\0"
		
		int j;
		for(j = 0; j < rr1; j++) 
			if(strcmp(temp2,read1[j]) == 0) 
				break; 
		if(j == rr1) {
			strcpy(read1[rr1],temp2); 
			read1[rr1][2] = '\0'; 
			rr1++; 
		} 
		else 
			continue; 
		
		for(j = 0; j < I[0].n; j++) {
			const char* o2 = I[l].rh[j].c_str();
			char* ptr = strstr(o2,temp2);
			//size_t o2 = I[l].rh[j].find(temp2);
			//if(o2 != string::npos) { 
			if(ptr) {
				templ[tn] = I[l].lh[j]; 
				templ[tn + 1] = '\0'; 
				tempr[tn] = I[l].rh[j]; 
				tn++; 
			}
		}
		for(j = 0; j < tn; j++) {//I1
			p = tempr[j].find('.');
			swap(tempr[j][p], tempr[j][p+1]); // Shift dot one position
			I[ns].lh[I[ns].n] = templ[j]; 
			I[ns].lh[I[ns].n + 1] = '\0'; 
			I[ns].rh[I[ns].n] = tempr[j]; 
			I[ns].n++; 
		} 
		
		closure(); // got closure of I1
		int k;
		for(j = 0; j < ns; j++) {
			if(fine_old(I[ns],I[j]) == 1) {	// If state is old
				I[ns].lh[0] = '\0'; // Clear the state
				for(k = 0; k < I[ns].n; k++) 
				I[ns].rh[k][0] = '\0'; 
				I[ns].n = 0; 
				k = indexOf(temp2[1],t);
				if(k == -1){ // goto if non-terminal
					k = indexOf(temp2[1],tnt);
					strcpy(slr[l][k],"G");
					sprintf(templ,"%d",j);
					strcat(slr[l][k],templ);
				}
				// else shift
				
				else{
					strcpy(slr[l][k],"S");
					sprintf(templ,"%d",j);
					strcat(slr[l][k],templ);
				}
				break; 
			} 
		} 
		if(j < ns) {// I got old state in the previous loop
			tn = 0; // free temp
			for(j = 0; j < 15; j++) {
				templ[j] = '\0'; 
				tempr[j][0] = '\0';
			} 
			continue; 
		} 
		

		k = indexOf(temp2[1],t);
		if(k == -1){ // goto if non-terminal
			k = indexOf(temp2[1],tnt);
			
			strcpy(slr[l][k],"G");
			sprintf(templ,"%d",j);
			strcat(slr[l][k],templ);
		}
		// else shift
		
		else{
			strcpy(slr[l][k],"S");
			sprintf(templ,"%d",j);
			strcat(slr[l][k],templ);
		}

		AG << "\n\nI" << ns << ": "; 
		for(j = 0; j < I[ns].n; j++)  {
			AG << "\n\t" << I[ns].lh[j] << " ==> ";
			for (int k=0; k<I[ns].rh[j].length(); k++) {
				if(I[ns].rh[j][k] == 'i') 
					AG << "id";
				else
					AG << I[ns].rh[j][k];
			}
			AG << '\n';
		}
		ns++; 
		tn = 0; 
		for(j = 0; j < 15; j++) { 
			templ[j] = '\0'; 
			tempr[j][0] = '\0'; 
		} 
	} 
}


int main(){
	ifstream File;
	int l,t1;
	File.open("input_grammar.txt");
	string ch;
	if(File.fail()) {
		return 1;
	}
	for(int i = 0; i < MAX; i++) { 
		I[i].n = 0; 
		I[i].last = -1;
		I[i].lh[0] = '\0'; 
		I[i].rh[0][0] = '\0'; 
	}
	for(int i = 0; i < MAX; i++)
		for(int j = 0; j < MAX; j++)
			slr[i][j][0] = '\0'; 
	

	// Start Symbol
	
	// Productions
	while(!File.eof()) {
		string str, temp_str;
		getline(File,str);
		bool flag = false;
		for (int i=0; i<str.length(); i++) {
			if(str[i] == ' ' || str[i] == '\t' || str[i] == '\v') { // tab exception
				str = str.substr(0,i) + str.substr(i+1);
			}
		}
		for (int i=4; i<str.length(); i++) {
			if(str[i] == 'i' && str[i+1] == 'd') { // id -> i
				str = str.substr(0,i+1) + str.substr(i+2);
				i++;
			}
			else if(str[i] == '|') { // || separator
				temp_str = str;
				str = temp_str[0];
				str += "==>" + temp_str.substr(i+2);
				temp_str = temp_str.substr(0,i);
				lh[n] = temp_str[0];
				temp_str = temp_str.substr(4);
				rh[n++] = temp_str;
				break;
			}
		}
		
		lh[n] = str[0];
		if(str.length()>4)
			str = str.substr(4);
		rh[n++] = str;
	}
	
	
	n_nt = 1;
	// Non-Terminal
	for (int i=0; i<n; i++) {
		bool flag = false;
		for(int j=0; j<n_nt; j++){
			if(lh[i] == nt[j]) flag = true;
		}
		if(flag) continue;
		nt[n_nt++] = lh[i];
	}
	nt[0] = 'Z';
	nt[n_nt] = '\0';
	
	// Terminal
	n_t = 0;
	for (int i=0; i<n; i++) {
		for (int j=0; j<rh[i].length(); j++) {
			bool flag = false;
			for (int k=0; k<=n_t; k++) {
				if(rh[i][j] == t[k]) flag = true;
				if('A' <= rh[i][j] && rh[i][j] <= 'Z') flag = true;
			}
			if(flag) continue;
			t[n_t++] = rh[i][j];
		}
	}
	t[n_t++] = EPSILON;
	t[n_t] = '\0';
	strcpy(tnt,t);
	strcat(tnt,nt);
	n_tnt = strlen(tnt);
	File.close();
	s = lh[1];
	
	lh[0] = 'Z';
	rh[0] = s;
	rh[0][1] = '\0';

	ofstream AG("Augmented-Grammar.txt",ios::ate);
	AG << "Augmented Grammar:\n";
	for(int i = 0; i < n; i++) {
		AG << i << '\t' << lh[i] << " ==> ";
		for (int j=0; j<rh[i].length(); j++) {
			if(rh[i][j] == 'i')
				AG << "id";
			else
				AG << rh[i][j];
		}
		AG << '\n'; 
	} 
		

	I[0].lh[0] = lh[0]; 
	I[0].rh[0] = '.';
	I[0].rh[0] += rh[0]; 
	I[0].n++; 

	// state I0
	l = 0;
	for(int i = 0; i < n; i++) {// n denotes number of production rules
		if(I[0].rh[l].length() > 1)
			temp = I[0].rh[l][1]; 
		else
			temp = '\0';
		l++;
		int j;
		for(j = 0; j < rr; j++) 
			if(temp == temp_read[j][0]) 
				break; 
		
		if(j == rr) { 
			temp_read[rr][0] = temp; 
			rr++; 
		} 
		else 
			continue;
		for(j = 0; j < n ; j++) { 
			if(lh[j] == temp) {
				I[0].rh[I[0].n] = "." + rh[j]; 
				I[0].lh[I[0].n] = lh[j]; 
				I[0].n++; 
			} 
		} 
	}
	 
	ns++; 
	// print state I0
	AG << "\nI" << ns-1 << ":\n"; 
	for(int i = 0; i < I[0].n; i++)  {
		AG << '\t' << I[0].lh[i] << " ==> ";
		for (int j=0; j<I[0].rh[i].length(); j++) {
			if(I[0].rh[i][j] == 'i')
					AG << "id";
				else
					AG << I[0].rh[i][j];
		}
		AG << '\n';
	}

	for(l = 0; l < ns; l++) //no of state(I[l])
		newstate(l, AG);
	AG.close();




	//first table
	int first[100][100];
	for (int i=0; i<n_nt; i++) {
		memset(first[i], 0, n_t*sizeof(int));
	}
	int flag, flag2, z, e_index = indexOf(EPSILON,t);

	//calculations for first table
	do {
		flag = 0;
		// For each non terminal
		for (int i = 0; i < n_nt; i++) {
			// For each production rule
			for (int j = 0; j < n; j++) {
				if(lh[j] == nt[i]) {
					z = 0;
					next_pos:
					int k = indexOf(rh[j][z], t);
					if (k >= 0) {
						// Terminal
						if (first[i][k] == 0) {
							first[i][k] = flag = 1;
						}
					} else {
						// Non terminal
						k = indexOf(rh[j][z], nt);
						for (l = 0, flag2 = 0; l < n; l++) {
							if (lh[l] == rh[j][z] && rh[l][0] == EPSILON) {
								flag2 = 1;
								break;
							}
						}
						if (flag2) {
							// Epsilon in 'k'
							for (l = 0; l < n_t; l++) {
								if ((first[i][l] == 0) && (first[k][l] == 1)) {
									first[i][l] = flag = 1;
								}
							}

							if (z < rh[j].length()) {
								z++;
								goto next_pos;
							}
						} else {
							// Epsilon is not in 'k'
							for (l = 0; l < n_t; l++) {
								if ((first[i][l] == 0) && (first[k][l] == 1)) {
									first[i][l] = flag = 1;
								}
							}
						}
					}
				}
			}
		}
	} while(flag);
	
	ofstream out("First.txt",ios::ate);
	out << "First Table :\n";
	
	out << '\n';
	for (int i = 0; i < n_nt; i++) {
		out << nt[i] << ": ";
		for (int j = 0; j < n_t; j++) {
			if(first[i][j]) {
				if(t[j] == 'i') out << "id ";
				else out << t[j] << ' ';
			}
		}
		out << '\n';
	}
	out.close();
	//follow table
	int follow[100][100];
	for (int i=0; i<n_t+1; i++) {
		memset(follow[i], 0, sizeof(follow[i]));
	}
	int bita, l1;
	follow[indexOf(lh[0], nt)][n_t] = 1;
	string sub1, sub2;

	//calculations for follow table
	do {
		flag = 0;
		for (int i = 0; i < n_nt; i++) {
			// struct p_rules rules = production_rules[i], temp_rules;
			// For each production rule
			for (int j = 0; j < n; j++) {
				if(lh[j] == nt[i]) {
					// For each character in the rule
					for (int k = 0; k < rh[j].length(); k++) {
						z = indexOf(rh[j][k], nt);
						if (z < 0) continue;

						// kth element is non terminal
						sub1 = rh[j].substr(k + 1);
						if (sub1.length() <= 0) {
							// follow[i] -> follow[z];
							for (l = 0; l < n_t + 1; l++) {
								if (follow[i][l] == 1 && follow[z][l] == 0) follow[z][l] = flag = 1;
							}
						}
						else{
							flag2 = 0;
							for (l = 0; l < sub1.length(); l++) {
								// first(sub1[l]) -> follow(z)
								//terminal
								if((bita = indexOf(sub1[l],t)) >= 0 && l != e_index && follow[z][bita] == 0){
									follow[z][bita] = flag = 1;
									break;
								}
								//non_terminal
								else if((bita = indexOf(sub1[l],nt)) >= 0){
									for (l1 = 0; l1 < n_t; l1++) {
										if(l1 == e_index){
											if( first[bita][l1] == 1) flag2 = 1; // epsilon exist
											continue;
										}
										if (first[bita][l1] == 1 && follow[z][l1] == 0) follow[z][l1] = flag = 1;
									}
									if(flag2 == 1){
										for (l1 = 0; l1 < n_t + 1; l1++)
											if (follow[i][l1] == 1 && follow[z][l1] == 0) follow[z][l1] = flag = 1;
									}
									else break;
								}
							}
						}
					}
				}
			}
		}
	} while (flag);
	ofstream out2("Follow.txt",ios::ate);
	
	out2 << "Follow Table:\n";
	out2 << "\n";
	for (int i = 0; i < n_nt; i++) {
		out2 << nt[i] << ":\t";
		for (int j = 0; j < n_t + 1; j++) {
			if(j == e_index) continue;
			if(j==0)
				out2 << "$" << "\t";
			if(follow[i][j]) {
				if(t[j] == 'i')
					out2 << "id" << "\t";
				else
					out2 << t[j] << "\t";
			}
		}
		out2 << '\n';
	}
	out2.close();
	//reduce
	e_index = indexOf(EPSILON,t);
	for(int i = 0; i < ns; i++){
		for(int j = 0; j < I[i].n; j++){
			size_t v = I[i].rh[j].find('.'); // no of chars before '.'
			if(v == string::npos) continue;
			if(v + 1 == I[i].rh[j].length()) { // A->aBb.
				for(int k = 0; k < n; k++){
					int a = 0, b = 0;
					for (int q=0; q<I[i].rh[j].length(); q++){
						a += I[i].rh[j][q];
					}
					for (int q=0; q<rh[k].length(); q++) {
						b += rh[k][q];
					}
					if((a-b) == 46){
						I[i].last = k;
						break;
					}
				}
			} 
		}
	}
	for(int i = 0; i < ns; i++){
		if(I[i].last > -1){
			int j = I[i].last;
			
			ch = lh[j];
			for(int k = 0; k <= n_t; k++){
				if(k == e_index) continue;
				if(follow[j][k] == 1){
					if (k == n_t) {
						k = indexOf('Z',tnt);
					}
					if(j == 0)
						strcpy(slr[i][k],"ACC");
					else{
						strcpy(slr[i][k],"R");
						sprintf(templ,"%d",j);
						strcat(slr[i][k],templ); 
					}
				}
			}
		}
	}
	
	ofstream out3("SLR-Table.txt",ios::ate);
	out3 << "SLR Table\n";
	for(int i = 0; i < n_tnt; i++) {
		if(tnt[i] == 'i')
			out3 << "\t" << "id";
		else
			out3 << "\t" << tnt[i];
	}
	out3 << '\n';
	for(int i = 0; i < ns; i++){
		out3 << i << "\t";
		for(int j = 0; j < n_tnt; j++){
			out3 << slr[i][j] << "\t";
		}
		out3 << '\n';
	}
	out3.close();
}
