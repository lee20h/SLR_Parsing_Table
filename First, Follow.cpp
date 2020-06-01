// 201610674 컴퓨터공학부 이영훈

#include<iostream>
#include<ctype.h>  // isupper()
#include<string.h> 
#include<vector>
#include<fstream>
using namespace std;
  
// Functions to calculate Follow 
void followfirst(char c, int c1, int c2); 
void follow(char c); 
  
// Function to calculate First 
void findfirst(char c, int q1, int q2, int cycle); 
  
int cnt, n = 0; // input grammar
  
// Stores the final result  
// of the First Sets 
char calc_first[100][100]; 
  
// Stores the final result 
// of the Follow Sets 
char calc_follow[100][100]; 
int m = 0; 
  
// Stores the production rules 
string production[100];
char f[100], first[100]; 
char ck; 
  
int main() { 
    int jm = 0; 
    int km = 0; 
    char c; 
    ifstream file;
    file.open("input.txt");
    if(file.is_open()) {
    	while(!file.eof()) {
    		string str, temp;
    		getline(file,str);
			bool flag = false;
			for (int i=0; i<str.length(); i++) {
				if(str[i] == ' ' || str[i] == '\t' || str[i] == '\v') { // tab exception
					str = str.substr(0,i) + str.substr(i+1);
				}
			}
			for (int i=4; i<str.length(); i++) {
				if(str[i] == 'i' && str[i+1] == 'd') { // id -> i
					str = str.substr(0,i+1) + str.substr(i+2);
				}
				
				else if(str[i] == '|') { // || separator
					temp = str[0];
					temp += "==>" + str.substr(i+2);
					
					str = str.substr(0,i);
					production[cnt++] = str;
					production[cnt++] = temp;
					flag = true;
					break;
				}
			}
			if(!flag) {
				production[cnt++] = str;
			}
		}
	}
	file.close();
    // The Input grammar 
    char done[10]; 
    int ptr = -1; 
    // Initializing the calc_first array 
    for(int k = 0; k < cnt; k++) { 
        for(int kay = 0; kay < 100; kay++) { 
            calc_first[k][kay] = '!'; 
        } 
    } 
    int point1 = 0, point2, xxx; 
	
    for(int k = 0; k < cnt; k++) { 
        c = production[k][0]; 
        point2 = 0; 
        xxx = 0; 
          
        // Checking if First of c has 
        // already been calculated 
        for(int kay = 0; kay <= ptr; kay++) 
            if(c == done[kay]) 
                xxx = 1; 
                  
        if (xxx == 1) 
            continue; 
            
        // Function call     
        findfirst(c, 0, 0, 0); 
          
        // Adding c to the calculated list 
        done[++ptr] = c; 
        cout << "\n First(" << c << ") = { "; 
        calc_first[point1][point2++] = c; 
          
        // Printing the First Sets of the grammar 
        for(int i = 0 + jm; i < n; i++) { 
            int chk = 0; 
              
            for(int lark = 0; lark < point2; lark++) { 
                  
                if (first[i] == calc_first[point1][lark]) { 
                    chk = 1; 
                    break; 
                } 
            } 
            if(chk == 0) { 
            	if(first[i] == 'i') cout << "id" << ' '; // i -> id
                else cout << first[i] << ' ';
                calc_first[point1][point2++] = first[i]; 
            } 
        } 
        cout << "}\n"; 
        jm = n; 
        point1++; 
    } 
    cout << '\n';
    
    char donee[10]; 
    ptr = -1; 
    
    // Initializing the calc_follow array 
    for(int k = 0; k < cnt; k++) { 
        for(int kay = 0; kay < 100; kay++) { 
            calc_follow[k][kay] = '!'; 
        } 
    } 
    point1 = 0; 
    for(int e = 0; e < cnt; e++) { 
        ck = production[e][0];
        point2 = 0; 
        xxx = 0; 
          
        // Checking if Follow of ck 
        // has alredy been calculated 
        for(int kay = 0; kay <= ptr; kay++) 
            if(ck == donee[kay]) 
                xxx = 1; 
                  
        if (xxx == 1) 
            continue; 
          
        // Function call 
        follow(ck); 
          
        // Adding ck to the calculated list 
        donee[++ptr] = ck; 
        cout << " Follow(" << ck << ") = { ";
        calc_follow[point1][point2++] = ck; 
          
        // Printing the Follow Sets of the grammar 
        for(int i = 0 + km; i < m; i++) { 
            int chk = 0; 
            for(int lark = 0; lark < point2; lark++)  { 
                if (f[i] == calc_follow[point1][lark]) { 
                    chk = 1; 
                    break; 
                } 
            } 
            if(chk == 0) { 
            	if(f[i] == 'i') cout << "id" << ' '; // i -> id
                else cout << f[i] << ' ';
                calc_follow[point1][point2++] = f[i]; 
            } 
        } 
        cout << "}\n\n";
        km = m; 
        point1++;  
    } 
} 
  
void follow(char c) { 
    // Adding "$" to the follow 
    // set of the start symbol 
    if(production[0][0] == c) {
        f[m++] = '$'; 
    } 
    for(int i = 0; i < cnt; i++) { 
        for(int j = 4; j < production[i].length(); j++) { 
            if(production[i][j] == c) {
                if(production[i][j+1] != '\0') { 
                    // Calculate the first of the next 
                    // Non-Terminal in the production 
                    followfirst(production[i][j+1], i, j+2); 
                } 
                  
                if(production[i][j+1]=='\0' && c!=production[i][0]) { 
                    // Calculate the follow of the Non-Terminal 
                    // in the L.H.S. of the production 
                    follow(production[i][0]); 
                } 
            }  
        } 
    } 
} 

void followfirst(char c, int c1, int c2) {       
    // The case where we encounter 
    // a Terminal 
    if(!(isupper(c))) 
        f[m++] = c;
    else{ 
        int i = 0, j = 1; 
        for(i = 0; i < cnt; i++) { 
            if(calc_first[i][0] == c) 
                break; 
        }
        
        //Including the First set of the 
        // Non-Terminal in the Follow of 
        // the original query 
        while(calc_first[i][j] != '!') {
            if(calc_first[i][j] != '_')  { 
                f[m++] = calc_first[i][j]; 
            } 
            else { 
                if(production[c1][c2] == '\0') 
                { 
                    // Case where we reach the 
                    // end of a production 
                    follow(production[c1][0]);
                } 
                else
                {
                    // Recursion to the next symbol
                    // in case we encounter a "_"
                    followfirst(production[c1][c2], c1, c2+1);
                } 
            } 
            j++; 
        } 
    } 
}

void findfirst(char c, int q1, int q2, int cycle) 
{ 
    // The case where we  
    // encounter a Terminal 
    if(!(isupper(c))) { 
        first[n++] = c; 
    }
    for(int j = cycle; j < cnt; j++) { 
        if(production[j][0] == c) { 
            if(production[j][4] == '_') { // 1~3 index '==>'
                if(production[q1][q2] == '\0') 
                    first[n++] = '_'; 
                else if(production[q1][q2] != '\0' && (q1 != 0 || q2 != 0)) { 
                    // Recursion to calculate First of New 
                    // Non-Terminal we encounter after epsilon 
                    findfirst(production[q1][q2], q1, (q2+1), j); 
                } 
                else
                    first[n++] = '_'; 
            } 
            else if(!isupper(production[j][4])) { 
                first[n++] = production[j][4]; 
            } 
            else { 
                // Recursion to calculate First of 
                // New Non-Terminal we encounter  
                // at the beginning
                
                findfirst(production[j][4], j, 5, cycle+1);
            } 
        } 
    }  
} 
  
