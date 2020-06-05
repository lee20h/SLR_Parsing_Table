#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>

using namespace std;

char terminals[100]={};
int no_t;
char non_terminals[100]={};
int no_nt;
char goto_table[100][100];
char reduce[20][20];
char follow[20][20];char fo_co[20][20];
char first[20][20];

struct state{
    int prod_count;
    char prod[100][100]={{}};
};

void add_dots(struct state *I){
    for(int i=0;i<I->prod_count;i++){
        for (int j=99;j>3;j--)
            I->prod[i][j] = I->prod[i][j-1];
        I->prod[i][3]='.';
    }
}

void augument(struct state *S,struct state *I){
    if(I->prod[0][0]=='S')
        strcpy(S->prod[0],"Z->.S");
    else{
        strcpy(S->prod[0],"S->.");
        S->prod[0][4]=I->prod[0][0];}
    S->prod_count++;
}

void get_prods(struct state *I){
    cout<<"Enter the number of productions:\n";
    cin>>I->prod_count;
    cout<<"Enter the number of non terminals:"<<endl;
    cin>>no_nt;
    cout<<"Enter the non terminals one by one:"<<endl;
    for(int i=0;i<no_nt;i++)
        cin>>non_terminals[i];
    cout<<"Enter the number of terminals:"<<endl;
    cin>>no_t;
    cout<<"Enter the terminals (single lettered) one by one:"<<endl;
    for(int i=0;i<no_t;i++)
        cin>>terminals[i];
    cout<<"Enter the productions one by one in form (S->ABc):\n";
    for(int i=0;i<I->prod_count;i++){
        cin>>I->prod[i];
    }
}

bool in_state(struct state *I,char *a){
    for(int i=0;i<I->prod_count;i++){
        if(!strcmp(I->prod[i],a))
            return true;
    }
    return false;
}

char mark_symbol(char a[100]){
    for(int i=0;i<strlen(a);i++)
        if(a[i]=='.'){
            return a[i+1];
		}

}

void move_dot(char *b,int len){
    for(int i=0;i<len;i++){
        if(b[i]=='.'){
            swap(b[i],b[i+1]);
            break;
        }
    }
}

bool same_state(struct state *I0,struct state *I){
    if (I0->prod_count != I->prod_count)
        return false;
    for (int i=0; i<I0->prod_count; i++) {
        if (strcmp(I0->prod[i], I->prod[i]) != 0)
            return false;
    }
    return true;
}

void closure(struct state *I,struct state *I0){
    char a={};
    for(int i=0;i<I0->prod_count;i++){
        a=mark_symbol(I0->prod[i]);
        if(isupper(a)){
            for(int j=0;j<I->prod_count;j++){
                if(I->prod[j][0]==a){
                    if(!in_state(I0,I->prod[j])){
                        strcpy(I0->prod[I0->prod_count],I->prod[j]);
                        I0->prod_count++;
                    }
                }
            }
        }
    }
}

void goto_state(struct state *I,struct state *S,char a){
    int time=1;
    for(int i=0;i<I->prod_count;i++){
        if(mark_symbol(I->prod[i])==a){
                if(time==1){
                    time++;
                }
                move_dot(I->prod[i], strlen(I->prod[i]));
                strcpy(S->prod[S->prod_count],I->prod[i]);
                S->prod_count++;
        }
    }
}

void print_prods(struct state *I){
    for(int i=0;i<I->prod_count;i++)
        printf("%s\n",I->prod[i]);
    cout<<endl;
}

bool in_array(char a[20],char b){
    for(int i=0;i<strlen(a);i++)
        if(a[i]==b)
            return true;
    return false;
}

/*char* chars_after_dots(struct state *I){
    char a[20]={};
    for(int i=0;i<I->prod_count;i++){
        if(!in_array(a,mark_symbol(I->prod[i]))){
                a[strlen(a)]=mark_symbol(I->prod[i]);
            }
    }
    return &a[0];
}*/

void cleanup_prods(struct state * I){
    char a[100]={};
    for(int i=0;i<I->prod_count;i++)
        strcpy(I->prod[i],a);
    I->prod_count=0;
}

int return_index(char a){
	if(!isupper(a)) {
		for(int i=0;i<no_t;i++)
        	if(terminals[i]==a)
            	return i;
	}
	else {
		for(int i=0;i<no_nt;i++)
	        if(non_terminals[i]==a)
	            return no_t+i;
	}
}

void print_shift_table(int state_count){
    cout<<endl<<"********Shift Actions*********"<<endl<<endl;
    cout<<"\t";
    for(int i=0;i<no_t;i++)
        cout<<terminals[i]<<"\t";
    for(int i=0;i<no_nt;i++)
        cout<<non_terminals[i]<<"\t";
    cout<<endl;
    for(int i=0;i<state_count;i++){
        int arr[no_nt+no_t]={-1};
        for(int j=0;j<state_count;j++){
            if(goto_table[i][j]!='~'){
                    arr[return_index(goto_table[i][j])]= j;
            }
        }
        cout<<"I"<<i<<"\t";
        for(int j=0;j<no_nt+no_t;j++){
            if(i==1&&j==no_t-1)
                cout<<"ACC"<<"\t";
            if(arr[j]==-1||arr[j]==0)
                cout<<"\t";
            else{
            if(j<no_t)
                cout<<"S"<<arr[j]<<"\t";
            else
                cout<<arr[j]<<"\t";

            }
        }
        cout<<"\n";
    }
}

int get_index(char c,char *a){
for(int i=0;i<strlen(a);i++)
    if(a[i]==c)
    return i;
}

void add_dot_at_end(struct state* I){
    for(int i=0;i<I->prod_count;i++){
        strcat(I->prod[i],".");
    }
}

void add_to_first(int n,char b){
   for(int i=0;i<strlen(first[n]);i++)
        if(first[n][i]==b)
            return;
    first[n][strlen(first[n])]=b;
}

void add_to_first(int m,int n){
    for(int i=0;i<strlen(first[n]);i++){
            int flag=0;
        for(int j=0;j<strlen(first[m]);j++){
            if(first[n][i]==first[m][j])
                flag=1;
        }
        if(flag==0)
            add_to_first(m,first[n][i]);
    }
}

void add_to_follow(int n,char b){
   for(int i=0;i<strlen(follow[n]);i++)
        if(follow[n][i]==b)
            return;
    follow[n][strlen(follow[n])]=b;
}

void add_to_follow(int m,int n){
    for(int i=0;i<strlen(follow[n]);i++){
            int flag=0;
        for(int j=0;j<strlen(follow[m]);j++){
            if(follow[n][i]==follow[m][j])
                flag=1;
        }
        if(flag==0)
            add_to_follow(m,follow[n][i]);
    }
}

void add_to_follow_first(int m,int n){
    for(int i=0;i<strlen(first[n]);i++){
            int flag=0;
        for(int j=0;j<strlen(follow[m]);j++){
            if(first[n][i]==follow[m][j])
                flag=1;
        }
        if(flag==0)
            add_to_follow(m,first[n][i]);
    }
}

void find_first(struct state *I){
    for(int i=0;i<no_nt;i++){
        for(int j=0;j<I->prod_count;j++){
            if(I->prod[j][0]==non_terminals[i]){
                if(!isupper(I->prod[j][3])){
                    add_to_first(i,I->prod[j][3]);
                    }

            }
        }
    }
}

void find_follow(struct state *I){
    for(int i=0;i<no_nt;i++){
        for(int j=0;j<I->prod_count;j++){
            for(int k=3;k<strlen(I->prod[j]);k++){
                if(I->prod[j][k]==non_terminals[i]){
                    if(I->prod[j][k+1]!='\0'){
                        if(!isupper(I->prod[j][k+1])){
                            add_to_follow(i,I->prod[j][k+1]);
                        }
                    }
                }
            }
        }
    }
}

int get_index(int *arr,int n){
    for(int i=0;i<no_t;i++){
        if(arr[i]==n)
            return i;
    }
    return -1;
}

void print_reduce_table(int state_count,int *no_re,struct state *temp1){
    cout<<"**********Reduce actions**********"<<endl<<endl;
    cout<<"\t";
    int **arr;
    arr = new int*[temp1->prod_count+1];
    for (int i=0; i<temp1->prod_count+1; i++){
    	arr[i] = new int[no_t+1];
	}
    arr[temp1->prod_count][no_t] = -1;
    for(int i=0;i<no_t;i++){
        cout<<terminals[i]<<"\t";
    }
    cout<<endl;
    for(int i=0;i<temp1->prod_count;i++){
	    int n=no_re[i];
	    for(int j=0;j<strlen(follow[return_index(temp1->prod[i][0])-no_t]);j++){
	        for(int k=0;k<no_t;k++){
	            if(follow[return_index(temp1->prod[i][0])-no_t][j]==terminals[k])
	                arr[i][k]=i+1;
	        }
	    }
	    cout<<"I"<<n<<"\t";
	    for(int j=0;j<no_t;j++){
	        if(arr[i][j]!=-1&&arr[i][j]!=0&&arr[i][j]<state_count)
	            cout<<"R"<<arr[i][j]<<"\t";
	        else
	            cout<<"\t";
	    }
	    cout<<endl;
	}
	for (int i=0; i<temp1->prod_count+1; i++) {
		delete [] arr[i];
	}
	delete [] arr;
}

int main(){
    struct state init; // 생성규칙 갯수와 생성규칙 가진 구조체 
    struct state temp;
	struct state temp1;
    int state_count=1;
    get_prods(&init); // 생성규칙 추가 
    temp=init;
    temp1=temp;
    add_dots(&init); // 생성 규칙 뒤로 한칸 씩 밀고 닷 추가 

    for(int i=0;i<100;i++)
        for(int j=0;j<100;j++)
            goto_table[i][j]='~'; // goto테이블 ~ 

    struct state I[50];
    augument(&I[0],&init); // 확장 규칙 Z -> .S , S -> . I[0] 에 적용 
    closure(&init,&I[0]); // I[0]의 mark symbol이 논터미널이면서 init 규칙에 없다면 추가 
    cout<<"\nI0:\n";
    print_prods(&I[0]); // I[0]의 생성규칙들 출력 

    char characters[20]={};
    bool isend = false;
    
    for(int i=0;i<state_count;i++) {
        char characters[20]={};
        for(int z=0;z<I[i].prod_count;z++)
            if(!in_array(characters,mark_symbol(I[i].prod[z]))) {// mark symbol이 없으면 넣어주기 
            	characters[strlen(characters)] = mark_symbol(I[i].prod[z]);
            }
        for(int j=0;j<strlen(characters);j++){
            goto_state(&I[i],&I[state_count],characters[j]); // characters가 I[i]의 mark symbol이라면 I[i]의 dot을 한칸 옮기고 I[state_count]에 저장 
            closure(&init,&I[state_count]);	//closure 계산 
            bool flag = false;
            for(int k=0;k<state_count-1;k++){
                if(same_state(&I[k],&I[state_count])){ // 같은 state를 갖나 
                    cleanup_prods(&I[state_count]); // I[state_count] 초기화 
					flag = true;
                    cout<<"I"<<i<<" on reading the symbol "<<characters[j]<<" goes to I"<<k<<".\n";
                    goto_table[i][k]=characters[j]; // goto_table에 등록 
                    break;
                }
            }
            
            if(!flag){ // 같은 state을 갖지 않은 경우
                cout<<"I"<<i<<" on reading the symbol "<<characters[j]<<" goes to I"<<state_count<<":\n";
                goto_table[i][state_count]=characters[j]; // goto 테이블 마지막에 추가 
                print_prods(&I[state_count]); // print 
                
                state_count++;
                
            }
        }
        if(isend) break;
    }

    int no_re[temp.prod_count]={-1}; // temp 즉, 본래의 생성규칙의 크기 
    terminals[no_t]='$'; // $로 시작 
	no_t++;

    add_dot_at_end(&temp1); // temp1 끝에 . 찍기 
    for(int i=0;i<state_count;i++){
        for(int j=0;j<I[i].prod_count;j++)
            for(int k=0;k<temp1.prod_count;k++)
                 if(in_state(&I[i],temp1.prod[k]))
                        no_re[k]=i;
    }

    find_first(&temp);
    for(int l=0;l<no_nt;l++){
    for(int i=0;i<temp.prod_count;i++){
        if(isupper(temp.prod[i][3])){
            add_to_first(return_index(temp.prod[i][0])-no_t,return_index(temp.prod[i][3])-no_t);
        }
    }}

    find_follow(&temp);
    add_to_follow(0,'$');
    for(int l=0;l<no_nt;l++){
        for(int i=0;i<temp.prod_count;i++){
            for(int k=3;k<strlen(temp.prod[i]);k++){
                if(temp.prod[i][k]==non_terminals[l]){
                        if(isupper(temp.prod[i][k+1])){
                            add_to_follow_first(l,return_index(temp.prod[i][k+1])-no_t);}
                        if(temp.prod[i][k+1]=='\0')
                            add_to_follow(l,return_index(temp.prod[i][0])-no_t);
                            }
                    }
            }
    }
    print_shift_table(state_count);
    cout<<endl<<endl;
    print_reduce_table(state_count,&no_re[0],&temp1);
}

