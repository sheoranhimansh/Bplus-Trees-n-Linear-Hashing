#include <bits/stdc++.h>
#define PB push_back
#define SZ(a) (int)(a.size())
#define rep(i,l,h) for(int i=(l); i<=(h);i++)
using namespace std;

vector<vector<int> > hash_table;
vector<int> counter; // counter[i] points to the index in ith bucket to place a new number
int N, splitPointer, B, M;

char *stringToChar(string line){
	
	int n=line.length();
	char *y = new char[n + 1];
	for(int i=0; i<=n; i++)
		y[i]='\0';

	while(n>0){	
		y[n-1]=line[n-1];
		n--;
	}
	return y;
}

void initHashTable(){
	vector<int> v;
	v.clear();
	N = 2;
	//B = 2;
	splitPointer=0;
	rep(i, 0, N-1){
		hash_table.PB(v);
		counter.PB(0);
	}
}


void printHashTable(){
	
	rep(i, 0, SZ(hash_table)-1){
		cout<<i<<" --> ";
		rep(j, 0, counter[i]-1){
			cout<<hash_table[i][j]<<" ";
		}
		cout<<endl;
	}
}

void insertRecord(int a){

	vector<int> v;
	v.clear();

	/* calculate hash value*/
	int cnt, b = a%N;
	if(b<0){
		b+=N;
	}
	
	if(b<splitPointer){
		b = a%(2*N);
		if(b<0){
			b+=2*N;
		}
	}
	
	/*insert hash value in hash table*/
	if(counter[b]>=SZ(hash_table[b]))
		hash_table[b].PB(a);
	else
		hash_table[b][counter[b]]=a;
	counter[b]++;

	/*Overflow*/
	if(counter[b]>B){

		/* split the bucket pointed to by split pointer */
		cnt = counter[splitPointer]-1;
		counter[splitPointer]=0;

		rep(i, 0, cnt){
			a = hash_table[splitPointer][i];
			b = a%(2*N);
			if(b<0){
				b+=2*N;
			}

			if(b!=splitPointer){
				/*make space for b buckets*/
				rep(j, SZ(hash_table), b){
					counter.PB(0);
					hash_table.PB(v);
				}
			}

			/*insert hash value in hash table*/
			if(counter[b]>=SZ(hash_table[b]))	
				hash_table[b].PB(a);
			else
				hash_table[b][counter[b]]=a;
			counter[b]++;
		
		}
		splitPointer++;
		if(splitPointer==N){
			splitPointer=0;
			N*=2;
		}	
	}
}

bool searchRecord(int a){
	int b = a%N;	
	if(b<0){
		b+=N;
	}
	
	if(b<splitPointer){
		b = a%(2*N);
		if(b<0){
			b+=2*N;
		}
	}

	rep(i, 0, counter[b]-1){
		if(hash_table[b][i]==a)
			return true;
	}
	return false;
}

void printOutputBuffer(vector<int> output_buffer, int curr_id){
	rep(i, 0, curr_id-1){
		cout<<output_buffer[i]<<" ";
	}
	cout<<endl;
}

int main(int argc, char const *argv[])
{
	if(argc != 4)
		cout<<"Insufficient input arguments\n";
	else{
		int a, val, fl=0;
		string line;
		M = atoi(argv[2]);
		B = atoi(argv[3])/4;  // Bucket capacity
		if(B>=1){
			vector<vector<int> > input_buffer(M-1);
			vector<int> output_buffer;
			int curr_inp_buff=0, curr_id=0, curr_out_id=0;

			/* Initializing the input and output buffers*/
			rep(i, 0, M-2){
				rep(j, 0, B-1){
					input_buffer[i].PB(-1);
				}
			}

			rep(i, 0, B-1){
				output_buffer.PB(-1);
			}

			initHashTable();
			ifstream infile(argv[1]);

			while(getline(infile, line)){
				val = atoi(stringToChar(line));
				if(curr_id<B){
					input_buffer[curr_inp_buff][curr_id]=val;
					curr_id++;
				}else if(curr_inp_buff<(M-2)){
					curr_inp_buff++;
					input_buffer[curr_inp_buff][0]=val;
					curr_id++;
				}else{  

					/*process the entire input*/
					rep(i, 0, M-2){
						rep(j,0, input_buffer[i].size()-1){
							a = input_buffer[i][j];
							if(!searchRecord(a)){
								insertRecord(a);
								if(curr_out_id<B){

									output_buffer[curr_out_id]=a;
									curr_out_id++;
								}else{  

									/*contents of output buffer*/
									rep(i, 0, output_buffer.size()-1){
										cout<<output_buffer[i]<<endl;
									}
									output_buffer[0]=a;
									curr_out_id=1;
								}
							}
						}
					}
					input_buffer[0][0]=val;
					curr_inp_buff=0;
					curr_id=1;
				}
			}
			if(curr_inp_buff>=0 &&  curr_id>0){
				rep(i, 0, curr_inp_buff-1){
					rep(j,0, input_buffer[i].size()-1){
						a = input_buffer[i][j];
						if(!searchRecord(a)){
							insertRecord(a);
							if(curr_out_id<B){

								output_buffer[curr_out_id]=a;
								curr_out_id++;
							}else{
								/*contents of output buffer*/
								rep(i, 0, output_buffer.size()-1){
									cout<<output_buffer[i]<<endl;
								}
								output_buffer[0]=a;
								curr_out_id=1;
							}
						}
					}
				}
				/*last buffer processing*/
				rep(j, 0, curr_id-1){
					a = input_buffer[curr_inp_buff][j];
					if(!searchRecord(a)){
						insertRecord(a);
						if(curr_out_id<B){

							output_buffer[curr_out_id]=a;
							curr_out_id++;
						}else{
							/*contents of output buffer*/
							rep(i, 0, output_buffer.size()-1){
								cout<<output_buffer[i]<<endl;
							}
							output_buffer[0]=a;
							curr_out_id=1;
						}
					}
				}

				if(curr_out_id>0){
					rep(i, 0, curr_out_id-1){
						cout<<output_buffer[i]<<endl;
					}
				}
			}
		}else
			cout<<"Bucket size too small\n";
	}
	return 0;
}
