#include <bits/stdc++.h>
//#include "helper.h"
#include <fstream>
#define MAX 10
#define PB push_back
#define rep(i,l,h) for(int i=(l); i<=(h);i++)
using namespace std;
int degree, M, B;

struct Block{

	bool isLeaf;
	int nKeys;
	int key[MAX];
	Block *childBlock[MAX];
	Block *parentBlock;
	Block *nextLeaf;   /* points to the next leaf in case of a leafblock */

	/* constructor */
	Block(){
		isLeaf = false;
		nextLeaf = NULL;
		parentBlock = NULL;
		nKeys = 0;
		rep(i, 0, MAX-1){
			key[i] = INT_MAX;
			childBlock[i] = NULL;
		}
	}
};

Block *rootBlock;

void splitLeafBlock(Block *lblock){

	int	i=0, lkeys = (degree)/2, x;
	
	/* splitting the leaf block */
	Block *rblock = new Block();

	while(lkeys<degree){
		rblock->key[i] = lblock->key[lkeys];
		rblock->nKeys++;
		lblock->key[lkeys]=INT_MAX;
		lblock->nKeys--;
		lkeys++; i++;
	}
	rblock->isLeaf = true;
	rblock->parentBlock = lblock->parentBlock;
	rblock->nextLeaf = lblock->nextLeaf;
	lblock->nextLeaf = rblock;

	/* adding a new child pointer in the parent, corresponding to the new block formed by splitting*/
	Block *pblock = lblock->parentBlock;

	/* if root is the only block in the tree */
	if(pblock==NULL){
		//cout<<"splitting root\n";
		Block *newblock = new Block();
		newblock->key[0] = lblock->key[lblock->nKeys-1];
		newblock->childBlock[0] = lblock;
		newblock->childBlock[1] = rblock;
		newblock->nKeys++;

		lblock->parentBlock = newblock;
		rblock->parentBlock = newblock;
		rootBlock = newblock;
		rootBlock->isLeaf = false;

	}else{

		/* key value to be added in parentblock */
		x = lblock->key[lblock->nKeys-1];
		int temp; Block *tmp;

		i=0;
		while((i<=pblock->nKeys) && (x>pblock->key[i])){
			i++;
		}

		/* insert the value and pointer corresponding to the rightblock in the parent */
		temp = pblock->key[i];
		tmp = pblock->childBlock[i+1];
		pblock->key[i] = x;
		pblock->childBlock[i+1] = rblock;
		x = temp;
		rblock = tmp;
		i++;

		while(i<=pblock->nKeys){

			temp = pblock->key[i];
			tmp = pblock->childBlock[i+1];
			pblock->key[i] = x;
			pblock->childBlock[i+1] = rblock;
			x = temp;
			rblock = tmp;
			i++;
		}

		i=0;
		while(pblock->childBlock[i]!=NULL){
			pblock->childBlock[i]->parentBlock = pblock;
			i++;
		}

		pblock->nKeys++;
	}
}

void splitInteriorBlock(Block *lblock){

	int	i=0, lkeys = (degree)/2, x;

	x = lblock->key[lkeys];
	lblock->key[lkeys]=INT_MAX;
	lblock->nKeys--;
	lkeys++;
	
	/* splitting the parent block */
	Block *rblock = new Block();

	while(lkeys<degree){
		rblock->key[i] = lblock->key[lkeys];
		rblock->childBlock[i] = lblock->childBlock[lkeys];
		rblock->nKeys++;
		lblock->key[lkeys]=INT_MAX;
		lblock->childBlock[lkeys]=NULL;
		lblock->nKeys--;
		lkeys++; i++;
	}
	rblock->childBlock[i] = lblock->childBlock[lkeys];

	rblock->isLeaf = false;
	rblock->parentBlock = lblock->parentBlock;

	i=rblock->nKeys;
	while(i>=0){
		rblock->childBlock[i]->parentBlock=rblock;
		i--;
	}

	Block *pblock = lblock->parentBlock;

	/* if the root gets split */
	if(pblock==NULL){
		Block *newblock = new Block();
		newblock->key[0] = x;
		newblock->childBlock[0] = lblock;
		newblock->childBlock[1] = rblock;
		newblock->nKeys++;
		lblock->parentBlock = newblock;
		rblock->parentBlock = newblock;

		rootBlock = newblock;
		rootBlock->isLeaf = false;

	}else{

		int temp; Block *tmp;

		i=0;
		while((i<=pblock->nKeys) && (x>pblock->key[i])){
			i++;
		}

		/* insert the value and pointer corresponding to the rightblock in the parent */
		temp = pblock->key[i];
		tmp = pblock->childBlock[i+1];
		pblock->key[i] = x;
		pblock->childBlock[i+1] = rblock;
		x = temp;
		rblock = tmp;
		i++;

		while(i<=pblock->nKeys){

			temp = pblock->key[i];
			tmp = pblock->childBlock[i+1];
			pblock->key[i] = x;
			pblock->childBlock[i+1] = rblock;
			x = temp;
			rblock = tmp;
			i++;
		}

		/*explicitly make parent point to children*/
		i=0;
		while(pblock->childBlock[i]!=NULL){
			pblock->childBlock[i]->parentBlock = pblock;
			i++;
		}

		pblock->nKeys++;
	}
}

void printTree(vector < Block* > blocks){
    vector < Block* > newBlocks;
    for(int i=0; i<blocks.size(); i++){ //for every block
        Block *curBlock = blocks[i];

        cout <<"[|";
        int j;
        for(j=0; j<curBlock->nKeys; j++){  /*traverse the childBlocks, print values and save all the childBlocks */
            cout << curBlock->key[j] << "|";
            if(curBlock->childBlock[j]!=NULL)
            newBlocks.push_back(curBlock->childBlock[j]);
        }
        if(curBlock->key[j]==INT_MAX && curBlock->childBlock[j]!=NULL)
            newBlocks.push_back(curBlock->childBlock[j]);

        /*---------Printing right pointing leaves----------*/

        /*else if(curBlock->key[0]<INT_MAX && curBlock->isLeaf==true){
        	if(curBlock->nextLeaf!=NULL){
	        	cout<<" -nl- ";
	        	cout<<curBlock->nextLeaf->key[0];
	        	cout<<" :: ";
        	} 
        }
		*/
        cout << "]  ";
    }

    if(newBlocks.size()==0){ /*if there is no childBlock block left to send out then just the end of the recursion*/

        puts("");
        puts("");
        blocks.clear();
    }
    else {               /*else send the childBlocks to the recursion to continue to the more depth*/
        puts("");
        puts("");
        blocks.clear();
        printTree(newBlocks);
    }
}

void insertQuery(Block *block, int x){

	if(block!=NULL){
		rep(i, 0, block->nKeys){

			/* if Non-leaf node found, recursively insert x */
			if((x<block->key[i]) && (block->childBlock[i]!=NULL)){

				insertQuery(block->childBlock[i], x);
				block->isLeaf = false;
				/* recursive splitting of parent block */
				if(block->nKeys > degree-1){
					splitInteriorBlock(block);
				}
				return;

			} /* if it's a leaf block, then insert x and return back to the parent block */
			else if((x<block->key[i]) && (block->childBlock[i]==NULL)){

				rep(j, 0, block->nKeys-1){
					if(x < (block->key[j])){
						x += block->key[j];
						block->key[j] = x - block->key[j];
						x -= block->key[j];
					}
				}
				block->key[block->nKeys] = x;
				block->nKeys++;
				if((block->nKeys) > degree-1){
					/* split the leaf block and assign the new child to parent block */
					//cout<<"split\n";
					splitLeafBlock(block);
				}
				break;
			}
		}
	}else{ 
	/* initializing the root block */
		rootBlock = new Block();
		rootBlock->isLeaf=true;
		rootBlock->nKeys++;
		rootBlock->key[0]=x;
	}

/*	vector<Block*> tree;
	tree.clear();
	tree.push_back(rootBlock);
	printTree(tree);*/
}

pair<Block*, bool> findElement(Block *block, int x){

	bool status=false;
	Block *t=NULL;
	int i;
	pair<Block*, bool> p1, p2;
	if(block==NULL)
		return make_pair(block, false);
	else{
		/* leaf block */
		if(block->childBlock[0]==NULL){
			i=0;
			while(i<block->nKeys && block->key[i]<x){
				i++;
			}
			if(i>=block->nKeys){
				return make_pair(t, false);
			}else if(block->key[i]!=x){
				return make_pair(block, false);
			}else{
				return make_pair(block, true);
			}
		}else{

			i=0;
			while(i<block->nKeys && block->key[i]<x){
				i++;
			}
			if(i==block->nKeys){
				return make_pair(t, false);
			}else if(x==block->key[i]){
				return make_pair(block, true);
			}
			else{
				if(block->childBlock[i+1]->key[0]==x){
					p1 = findElement(block->childBlock[i],x);
					p2 = findElement(block->childBlock[i+1],x);
					if(p1.first!=NULL && p1.second==true)
						return p1;
					else if(p2.first!=NULL && p2.second==true)
						return p2;
					else
						return make_pair(block, false);
				}
				return findElement(block->childBlock[i], x);
			}
		}
	}
}

pair<Block*, bool> findQuery(Block *block, int x){

	bool status=false;
	Block *t=NULL;
	int i;
	if(block==NULL)
		return make_pair(block, false);
	else{
		/* leaf block */
		if(block->childBlock[0]==NULL){
			i=0;
			while(i<block->nKeys && block->key[i]<x){
				i++;
			}
			if(i>=block->nKeys){
				return make_pair(t, false);
			}else if(block->key[i]!=x){
				return make_pair(block, false);
			}else{
				return make_pair(block, true);
			}
		}else{

			i=0;
			while(i<block->nKeys && block->key[i]<x){
				i++;
			}
			if(i==block->nKeys){
				return make_pair(t, false);
			}/*else if(x==block->key[i]){
				return findQuery(block->childBlock[i+1], x);
			}*/
			else{
				return findQuery(block->childBlock[i], x);
			}
		}
	}
}

int rangeQuery(int l, int r){

	Block *block;
	int cnt=0;
	pair<Block*, bool> rquery;
	rquery = findQuery(rootBlock, l);
	if(rquery.first ==NULL){
		return cnt;
	}else{
		block = rquery.first;
		int i=0;
		while(i<block->nKeys && block->key[i]<l){
			i++;
		}
		if(i==block->nKeys){
			return cnt;
		}else{
			while(block!=NULL && block->isLeaf==true && block->key[i]<=r && block->key[i]>=l){

				while(i<block->nKeys && block->key[i]>=l && block->key[i]<=r){
					cnt++;
					i++;
				}
				i=0;
				block = block->nextLeaf;
			}
			return cnt;
		}
	}
}

int countQuery(int x){

	pair<Block*, bool> rquery;
	rquery = findQuery(rootBlock, x);
	if(rquery.second==false)
		return 0;
	else{
		int i=0, cnt=0;
		Block* block; block = rquery.first;
		while(i<block->nKeys && block->key[i]<x){
			i++;
		}
		if(i==block->nKeys){
			return cnt;
		}else{
			while(block!=NULL && block->isLeaf==true && block->key[i]==x){

				while(i<block->nKeys && block->key[i]==x){
					cnt++;
					i++;
				}
				i=0;
				block = block->nextLeaf;
			}
			return cnt;
		}
	}
}

/*Helper Functions*/

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

int getDegree(int blocksize){
	return (blocksize-8)/12 + 1;
}

int getQueryType(string s){
	if(s=="INSERT"){
		return 1;
	}else if(s=="FIND"){
		return 2;
	}else if(s=="COUNT"){
		return 3;
	}else if(s=="RANGE"){
		return 4;
	}
}

void printValues(vector<int> v){

	int i, n=v.size();
	for(i=0; i<n; i++){
		cout<<v[i]<<" ";
	}
	cout<<endl;
}

void printInputBuffer(vector<vector<string> > input_buffer){
	cout<<"Input Buffer contents\n";
	if(M-2>=0){
		rep(i, 0, M-2){
			if(input_buffer[i].size()>0){
				rep(j,0, input_buffer[i].size()-1){
					cout<<input_buffer[i][j]<<endl;
				}
			}
		}
	}
	cout<<endl;
}

int main(int argc, char const *argv[])
{
	if(argc != 4)
		cout<<"Insufficient input arguments\n";
	else{

		bool status;
		string outp;
		M=4; B=100;
		int cnt, l, r, qt, x, gl, fl=0;
		int curr_inp_buff, curr_inp_sz, curr_out_sz;
		string line, fline;
		vector<int> res;
		vector<Block*> tree;
		char *tokens, *y;
		rootBlock=NULL;
		degree=4;
		pair<Block*, bool> fquery;

		M = atoi(argv[2]);
		B = atoi(argv[3]);

		vector<vector<string> > input_buffer(M-1);
		vector<string> output_buffer; // assuming output will have only strings

		degree = getDegree(B);
		if(degree<=0){
			cout<<"Given block size is too small\n";
			return 0;
		}

		ifstream infile(argv[1]);
		curr_inp_buff=0;
		curr_inp_sz=0;
		curr_out_sz=0;
		while(getline(infile, fline)){
			//cout<<"query: "<<fline<<endl;
			/*input buffer has space*/
			if(curr_inp_buff<(M-1) && (curr_inp_sz+fline.length())<=B){

				input_buffer[curr_inp_buff].PB(fline);
				curr_inp_sz += fline.length();
			}
			else if(curr_inp_buff<(M-2) && (fline.length()<=B)){
				input_buffer[curr_inp_buff+1].PB(fline);
				curr_inp_buff++;
				curr_inp_sz = fline.length();
			}
			else{
				/*process all instructions from input buffer*/
				if(input_buffer[0].size()>0){
					rep(i, 0, M-2){
						rep(j,0, input_buffer[i].size()-1){
							outp="";
							line = input_buffer[i][j];
							y = stringToChar(line); 
							tokens = strtok (y," ");
							qt = getQueryType(tokens);
							
							tokens = strtok(NULL, " ");
							x = atoi(tokens);
							
							if(qt==1){
								insertQuery(rootBlock, x);
								/*tree.clear();
								tree.push_back(rootBlock);
								printTree(tree); */
								
							}
							else if(qt==2){
								fquery = findElement(rootBlock, x);
								if(fquery.first ==NULL || fquery.second==false){
									outp += "NO";
								}else{
									outp += "YES";
								}
							}
							else if(qt==3){
								cnt = countQuery(x);
								outp = to_string(cnt);
							}
							else if(qt==4){

								l = atoi(tokens);
								tokens = strtok(NULL, " ");
								r = atoi(tokens);
								if(l>r){
									cnt=0;
								}
								else
									cnt = rangeQuery(l, r);
								outp = to_string(cnt);
							}

							/*if there is some output*/
							if(outp.length()>0){

								/*output buffer has space*/
								if(curr_out_sz + outp.length()<=B){
									curr_out_sz += outp.length();
									output_buffer.PB(outp);
								}else{
									rep(i, 0, output_buffer.size()-1){
										cout<<output_buffer[i]<<endl;
									}
									output_buffer.clear();

									output_buffer.PB(outp);
									curr_out_sz = outp.length();
								}
							}
						}
					}
				}else{
					/*if the entire input is processed*/
					rep(i, 0, output_buffer.size()-1){
						cout<<output_buffer[i]<<endl;
					}
					output_buffer.clear();

					output_buffer.PB(outp);
					curr_out_sz = outp.length();
				}

				curr_inp_buff=0;
				rep(i, 0, M-2)
					input_buffer[i].clear();

				input_buffer[0].PB(fline);
				curr_inp_sz = fline.length();
			}
		}
		/*process the remaining input buffers after reading the entire file*/
		if(input_buffer[0].size()>0 || output_buffer.size()>0){
			if(input_buffer[0].size()>0){
					rep(i, 0, M-2){
						rep(j,0, input_buffer[i].size()-1){
							outp="";
							line = input_buffer[i][j];
							y = stringToChar(line); 
							tokens = strtok (y," ");
							qt = getQueryType(tokens);
							
							tokens = strtok(NULL, " ");
							x = atoi(tokens);
							
							if(qt==1){
								insertQuery(rootBlock, x);
							}
							else if(qt==2){
								fquery = findElement(rootBlock, x);
								if(fquery.first ==NULL || fquery.second==false){
									outp += "NO";
								}else{
									outp += "YES";
								}
							}
							else if(qt==3){
								cnt = countQuery(x);
								outp = to_string(cnt);
							}
							else if(qt==4){

								l = atoi(tokens);
								tokens = strtok(NULL, " ");
								r = atoi(tokens);
								if(l>r)
									cnt=0;
								else
									cnt = rangeQuery(l, r);
								outp = to_string(cnt);
							}

							/*if there is some output*/
							if(outp.length()>0){

								/*output buffer has space*/
								if(curr_out_sz + outp.length()<=B){
									curr_out_sz += outp.length();
									output_buffer.PB(outp);
								}else{
									rep(i, 0, output_buffer.size()-1){
										cout<<output_buffer[i]<<endl;
									}
									output_buffer.clear();

									output_buffer.PB(outp);
									curr_out_sz = outp.length();
								}
							}
						}
					}
				}
				if(output_buffer.size()>0){
					/*if the entire input is processed*/
					rep(i, 0, output_buffer.size()-1){
						cout<<output_buffer[i]<<endl;
					}
					output_buffer.clear();

					output_buffer.PB(outp);
					curr_out_sz = outp.length();
				}
		}
	}
	return 0;
}