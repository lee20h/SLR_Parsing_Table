// 201610674 컴퓨터공하부 이영훈 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EPSILON '_'

#define MAX 30

char lh[MAX], rh[MAX][10], s, t[10], nt[10], tnt[20], read[MAX][10], temp, templ[MAX], tempr[MAX][10], *ptr, temp2[5], slr[MAX][MAX][6];
int i,j,k,p,n = 1, ns = 0, rr = 0,n_tnt, tn = 0, n_t, n_nt;

struct state{
	char lh[MAX], rh[MAX][10];
	int n, last;
} I[MAX];


char *substr(char *s, int l, int r) {
	int len = (r - l), i;
	char *sub = new char(len + 1);
	for (i = 0; i < len; i++) sub[i] = s[i + l];
	sub[len] = '\0';
	return sub;
}

int fine_old(struct state s1,struct state s2) 
{ 
	int t; 
	if(s1.n != s2.n) 
		return 0; 
	if(strcmp(s1.lh,s2.lh) != 0) 
		return 0; 
	for(t = 0; t < s1.n; t++) 
		if(strcmp(s1.rh[t],s2.rh[t]) != 0) 
			return 0; 
	return 1; 
} 

int indexOf(char ip, char symbol[]) {
	if (ip == '$') return strlen(symbol);
	int i;
	for(i = 0; i < strlen(symbol); i++)
		if(symbol[i] == ip)
			return i;
	return -1;
}

void closure() 
{ 
	int r,s,t,l1=0,rr1=0; 
	char *ptr1,read1[MAX][10]; 
	
	for(r = 0; r < I[ns].n; r++) 	{ 
		ptr1 = strchr(I[ns].rh[l1],'.'); 
		t = ptr1 - I[ns].rh[l1]; 
		if(t + 1 == strlen(I[ns].rh[l1])) { // A->aBb.
			l1++; 
			continue; 
		} 
		// temp = I[ns].rh[l1][t+1]; 
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
				I[ns].rh[I[ns].n][0] = '.'; 
				I[ns].rh[I[ns].n][1] = '\0'; 
				strcat(I[ns].rh[I[ns].n],rh[s]); 
				I[ns].lh[I[ns].n] = lh[s]; 
				I[ns].lh[I[ns].n + 1] = '\0'; 
				I[ns].n++; 
			} 
		} 
	} 
} 

void newstate(int l) {
	int t1; 
	char read1[MAX][10],rr1 = 0,*ptr1; 
	for(i = 0; i < I[l].n; i++) 
	{ // ith rule of state Il
		temp2[0] = '.'; // Here temp2 is "."
		ptr1 = strchr(I[l].rh[i],'.'); 
		t1 = ptr1 - I[l].rh[i]; // no of chars before '.'
		if( t1 + 1 == strlen(I[l].rh[i])) // A->aBb.
			continue;
		
		temp2[1] = I[l].rh[i][t1+1]; // Here temp2 is ".B"
		temp2[2] = '\0'; // Here temp2 is ".B\0"
		
		for(j = 0; j < rr1; j++) 
			if( strcmp(temp2,read1[j]) == 0 ) 
				break; 
		if(j == rr1) {
			strcpy(read1[rr1],temp2); 
			read1[rr1][2] = '\0'; 
			rr1++; 
		} 
		else 
			continue; 
		
		for(j = 0; j < I[0].n; j++) {
			ptr = strstr(I[l].rh[j],temp2); 
			if(ptr) { 
				templ[tn] = I[l].lh[j]; 
				templ[tn + 1] = '\0'; 
				strcpy(tempr[tn],I[l].rh[j]); 
				tn++; 
			} 
		}

	
		for(j = 0; j < tn; j++) {//I1
			ptr = strchr(tempr[j],'.'); 
			p = ptr - tempr[j]; 
			tempr[j][p] = tempr[j][p+1]; // Shift dot one position
			tempr[j][p+1] = '.'; 
			I[ns].lh[I[ns].n] = templ[j]; 
			I[ns].lh[I[ns].n + 1] = '\0'; 
			strcpy(I[ns].rh[I[ns].n],tempr[j]); 
			I[ns].n++; 
		} 
		
		closure(); // got closure of I1
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
				//slr[l][j] = temp2[1]; // INSERT INTO slr_table;
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



		printf("\n\nI%d:",ns); 
		for(j = 0; j < I[ns].n; j++) 
			printf("\n\t%c ==> %s",I[ns].lh[j],I[ns].rh[j]);
		ns++; 
		tn = 0; 
		for(j = 0; j < 15; j++) { 
			templ[j] = '\0'; 
			tempr[j][0] = '\0'; 
		} 
	} 



}





int main(){
	FILE *f;
	int l,t1;
	f = fopen("slr_input.txt","r");
	char ch;
	for(i = 0; i < MAX; i++) { 
		I[i].n = 0; 
		I[i].last = -1;
		I[i].lh[0] = '\0'; 
		I[i].rh[0][0] = '\0'; 
	}

	for(i = 0; i < MAX; i++)
		for(j = 0; j < MAX; j++)
			slr[i][j][0] = '\0'; 

	// Terminal
	ch = fgetc(f);
	i = 0;
	while(ch != '\n'){
		t[i] = ch;
		ch = fgetc(f);
		i++;
	}
	t[i++] = EPSILON;
	t[i] = '\0';
	n_t = i/*sizeof(t)*/;

	// Non Terminal
	i = 1;
	ch = fgetc(f);
	while(ch != '\n'){
		nt[i] = ch;
		ch = fgetc(f);
		i++;
	}
	nt[0] = 'S';
	nt[i] = '\0';
	n_nt = i /*sizeof(nt)*/;

	strcpy(tnt,t);
	strcat(tnt,nt);
	n_tnt = strlen(tnt);

	//Start Symbol
	s = fgetc(f);

	// Productions
	ch = fgetc(f);

	while(!feof(f)) { 
		fscanf(f,"%c",&lh[n]); 
		fscanf(f,"%s\n",rh[n]); 
		n++; 
	}
	fclose(f);

	lh[0] = 'S';
	rh[0][0] = s;
	rh[0][1] = '\0';

	printf("Augmented Grammar:\n"); 
	for(i = 0; i < n; i++) 
		printf("%d\t%c ==> %s\n",i,lh[i],rh[i]); 

	I[0].lh[0] = lh[0]; 
	strcpy(I[0].rh[0], ".");
	strcat(I[0].rh[0], rh[0]); 
	I[0].n++; 


	// state I0
	l = 0;
	for(i = 0; i < n; i++) {// n denotes number of production rules
		temp = I[0].rh[l][1]; 
		l++; 
		for(j = 0; j < rr; j++) 
			if(temp == read[j][0]) 
				break; 
		if(j == rr) { 
			read[rr][0] = temp; 
			rr++; 
		} 
		else 
			continue; 
		for(j = 0; j < n ; j++) { 
			if(lh[j] == temp) { 
				I[0].rh[I[0].n][0] = '.'; 
				strcat(I[0].rh[I[0].n], rh[j]); 
				I[0].lh[I[0].n] = lh[j]; 
				I[0].n++; 
			} 
		} 
	} 
	ns++; 

	// print state I0
	printf("\nI%d:\n",ns-1); 
	for(i = 0; i < I[0].n; i++) 
		printf("\t%c ==> %s\n",I[0].lh[i],I[0].rh[i]);


	for(l = 0; l < ns; l++) //no of state(I[l])
		newstate(l);

	




	//first table
		int **first = new int*[n_nt+1];
	for (int i=0; i<n_nt+1; i++) {
		first[i] = new int[n_t+1];
		memset(first[i], 0, n_t*sizeof(int));
	}
	//int first[n_nt][n_t];
	//memset(first, 0, n_nt*n_t*sizeof(int));
	int flag, flag2, z, e_index = indexOf(EPSILON,t);

	//calculations for first table
	do {
		flag = 0;
		// For each non terminal
		for (i = 0; i < n_nt; i++) {
			// struct p_rules rules = production_rules[i], temp_rules;

			// For each production rule
			for (j = 0; j < n; j++) {
				if(lh[j] == nt[i]) {
					z = 0;
					next_pos : 
					k = indexOf(rh[j][z], t);
					if (k >= 0) {
						// Terminal
						if (first[i][k] == 0) {
							first[i][k] = flag = 1;
						}
					}
					else {
						// Non terminal
						k = indexOf(rh[j][z], nt);
						// temp_rules = production_rules[k];
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

							if (z < strlen(rh[j])) {
								z++;
								goto next_pos;
							}
						}
						else {
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

	printf("\nFirst Table:\n");

	printf("\n");
	for (i = 0; i < n_nt; i++) {
		printf("%c: ", nt[i]);
		for (j = 0; j < n_t; j++) {
			if(first[i][j])
				printf("%c ", t[j]);
		}
		printf("\n");
	}

	//follow table
	int follow[n_nt][n_t + 1];
	int bita, l1;
	memset(follow, 0, n_nt * (n_t + 1) * sizeof(int));
	follow[indexOf(lh[0], nt)][n_t] = 1;
	char *sub1, *sub2;

	//calculations for follow table
	do {
		flag = 0;
		for (i = 0; i < n_nt; i++) {
			// struct p_rules rules = production_rules[i], temp_rules;
			// For each production rule
			for (j = 0; j < n; j++) {
				if(lh[j] == nt[i]) {
					// For each character in the rule
					for (k = 0; k < strlen(rh[j]); k++) {
						z = indexOf(rh[j][k], nt);
						if (z < 0) continue;

						// kth element is non terminal
						sub1 = substr(rh[j], k + 1, strlen(rh[j]));
						if (strlen(sub1) <= 0) {
							// follow[i] -> follow[z];
							for (l = 0; l < n_t + 1; l++) {
								if (follow[i][l] == 1 && follow[z][l] == 0) follow[z][l] = flag = 1;
							}
						}
						else{
							flag2 = 0;
							for (l = 0; l < strlen(sub1); l++) {
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


	printf("\nFollow Table:\n");
	char follow_char[100];
	int follow_limit = 0;
	int follow_print = 0;
	for (i = 0; i < n_t; i++) {
		if(i == e_index) continue;
		printf("\t%c", t[i]);
		follow_char[follow_limit++] = t[i];
	}
	printf("\t$\n");
	for (i = 0; i < n_nt; i++) {
		printf("%c:\t", nt[i]);
		for (j = 0; j < n_t + 1; j++) {
			
			if(j == e_index) continue;
			printf("%d\t", follow[i][j]);
		}
		printf("\n");
	}
	
	//reduce
	e_index = indexOf(EPSILON,t);
	for(i = 0; i < ns; i++){
		for(j = 0; j < I[i].n; j++){
			ptr = strchr(I[i].rh[j],'.'); 
			t1 = ptr - I[i].rh[j]; // no of chars before '.'
			if( t1 + 1 == strlen(I[i].rh[j])) {// A->aBb.
				for(k = 0; k < n; k++){
					int a = 0, b = 0, q;
					for (q=0; q<10; q++) 
						a += I[i].rh[j][q];
					for (q=0; q<10; q++) 
						b += rh[k][q];
					
					if((a-b) == 46) {
						I[i].last = k;
						break;
					}
				}
				continue;
			} 
		}
	}

	for(i = 0; i < ns; i++){
		if(I[i].last > -1){
			j = I[i].last;
			ch = lh[j];
			for(k = 0; k < n_t + 1; k++){
				if(k == e_index) continue;
				if(follow[j][k] == 1){
					if (k == n_t)
						k = indexOf('S',tnt);
						
					if(j == 0)
						strcpy(slr[i][k], "Acc");
					else{
						strcpy(slr[i][k],"R");
						sprintf(templ,"%d",j);
						strcat(slr[i][k],templ); 
					}
				}
			}
		}
	}
	


	printf("\nSLR Table\n");
	for(i = 0; i <= n_tnt; i++)
		printf("\t%c", tnt[i]);
	printf("\n");
	for(i = 0; i < ns; i++){
		printf("%d\t", i);
		for(j = 0; j < n_tnt; j++){
			printf("%s\t", slr[i][j]);
		}
		printf("\n");
	}
}
