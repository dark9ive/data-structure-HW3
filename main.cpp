#include<iostream>
#include<fstream>
#include<set>
#include<unordered_set>
#include<algorithm>
#define MAX_TIME 360

using namespace std;

class tree_node{
	private:
		int value[3] = {0};
		int top = 0;
		tree_node* pointer[4] = {NULL};
		tree_node* parent = NULL;
	public:
		tree_node();
		tree_node(tree_node&);
		int insert(int val);
		void grow();
		friend void b_tree_insert(int value, tree_node* root);
		void print();
		void clear();
		bool search(int target);
};

tree_node* ROOT = NULL;

tree_node::tree_node(){
	return;
}

tree_node::tree_node(tree_node & source_node){
	for(int a = 0; a < 3; a++){
		this->value[a] = source_node.value[a];
		this->pointer[a] = source_node.pointer[a];
	}
	this->top = source_node.top;
	this->parent = source_node.parent;
}

int tree_node::insert(int val){
	for(int a = 0; a < top; a++){
		if(this->value[a] > val){
			for(int b = 2; b > a; b--){
				value[b] = value[b-1];
				pointer[b+1] = pointer[b];
			}
			value[a] = val;
			top++;
			return a;
		}
	}
	value[top] = val;
	top++;
	return top-1;
}

void tree_node::grow(){
	if(top == 3){
		if(parent == NULL){
			parent = new tree_node;
			parent->pointer[0] = this;
			ROOT = this->parent;
		}
		int posi = parent->insert(value[1]);
		parent->pointer[posi+1] = new tree_node;
		parent->pointer[posi+1]->value[0] = this->value[2];
		parent->pointer[posi+1]->pointer[0] = this->pointer[2];
		parent->pointer[posi+1]->pointer[1] = this->pointer[3];
		if(parent->pointer[posi+1]->pointer[0] != NULL){
			parent->pointer[posi+1]->pointer[0]->parent = parent->pointer[posi+1];
		}
		if(parent->pointer[posi+1]->pointer[1] != NULL){
			parent->pointer[posi+1]->pointer[1]->parent = parent->pointer[posi+1];
		}
		parent->pointer[posi+1]->top = 1;
		parent->pointer[posi+1]->parent = this->parent;

		this->pointer[2] = NULL;
		this->pointer[3] = NULL;
		this->value[1] = 0;
		this->value[2] = 0;
		this->top = 1;
		parent->grow();
	}
}

void b_tree_insert(int value, tree_node* root){
	if(value < root->value[0]){
		if(root->pointer[0] == NULL){
			root->insert(value);
			root->grow();
		}
		else{
			b_tree_insert(value, root->pointer[0]);
		}
	}
	else if((value > root->value[0] && value < root->value[1]) || root->top == 1){
		if(root->pointer[1] == NULL){
			root->insert(value);
			root->grow();
		}
		else{
			b_tree_insert(value, root->pointer[1]);
		}
	}
	else if((value > root->value[1] && value < root->value[2]) || root->top == 2){
		if(root->pointer[2] == NULL){
			root->insert(value);
			root->grow();
		}
		else{
			b_tree_insert(value, root->pointer[2]);
		}
	}
	else{
		if(root->pointer[3] == NULL){
			root->insert(value);
			root->grow();
		}
		else{
			b_tree_insert(value, root->pointer[3]);
		}
	}
	return;
}

void tree_node::print(){
	for(int a = 0; a < top; a++){
		if(pointer[a] != NULL){
			//cout << "Go down " << a << endl;
			this->pointer[a]->print();
		}
		cout << this->value[a] << " ";
	}
	if(pointer[top] != NULL){
		//cout << "Go down " << top << endl;
		this->pointer[top]->print();
	}
	//cout << "Go up" << endl;
	return;
}

void tree_node::clear(){
	for(int a = 0; a < top; a++){
		if(pointer[a] != NULL){
			this->pointer[a]->clear();
		}
	}
	if(pointer[top] != NULL){
		this->pointer[top]->clear();
	}
	delete this;
	return;
}

bool tree_node::search(int target){
	for(int a = 0; a < top; a++){
		if(target == value[a]){
			return true;
		}
	}
	if(target < value[0]){
		if(pointer[0] == NULL){
			return false;
		}
		else{
			return pointer[0]->search(target);
		}
	}
	else if((target > value[0] && target < value[1]) || top == 1){
		if(pointer[1] == NULL){
			return false;
		}
		else{
			return pointer[1]->search(target);
		}
	}
	else if((target > value[1] && target < value[2]) || top == 2){
		if(pointer[2] == NULL){
			return false;
		}
		else{
			return pointer[2]->search(target);
		}
	}
	else{
		if(pointer[3] == NULL){
			return false;
		}
		else{
			return pointer[3]->search(target);
		}
	}
}

bool find(int target, int* array, int N){
	for(int a = 0; a < (1<<N); a++){
		if(array[a] == target){
			return true;
		}
		else if(array[a] > target){
			return false;
		}
	}
	return false;
}

int main(){
	srand(clock());
	ofstream csv_file;
	for(int N = 10; N <= 28; N++){

		clock_t timer = clock();
		ROOT = new tree_node;
		int NUM = rand()%(1<<N);
		int num = NUM;
		for(int a = 0; a < (1 << N); a++){
			//cout << num << endl;
			b_tree_insert(num, ROOT);
			num = (num+1234567)%(1<<30);
		}
		timer = clock() - timer;
		csv_file.open("insert.csv", ofstream::app);
		csv_file << (double)timer/CLOCKS_PER_SEC << ", ";
		csv_file.close();
		timer = clock();
		for(int a = 0; a < 100000; a++){
			num = rand()%(1<<30);
			ROOT->search(num);
		}
		timer = clock() - timer;
		csv_file.open("search.csv", ofstream::app);
		csv_file << (double)timer/CLOCKS_PER_SEC << ", ";
		csv_file.close();
		//ROOT->print();
		//cout << endl;
		ROOT->clear();
		
		timer = clock();
		set<int> myset;
		num = NUM;
		for(int a = 0; a < (1 << N); a++){
			myset.insert(num);
			num = (num+1234567)%(1<<30);
		}
		timer = clock() - timer;
		csv_file.open("insert.csv", ofstream::app);
		csv_file << (double)timer/CLOCKS_PER_SEC << ", ";
		csv_file.close();
		timer = clock();
		for(int a = 0; a < 100000; a++){
			num = rand()%(1<<30);
			myset.find(num);
		}
		timer = clock() - timer;
		csv_file.open("search.csv", ofstream::app);
		csv_file << (double)timer/CLOCKS_PER_SEC << ", ";
		csv_file.close();
		myset.clear();

		timer = clock();
		int* array = new int[1<<N];
		num = NUM;
		for(int a = 0; a < (1 << N); a++){
			array[a] = num;
			num = (num+1234567)%(1<<30);
		}
		sort(array, array+(1<<N));
		timer = clock() - timer;
		csv_file.open("insert.csv", ofstream::app);
		csv_file << (double)timer/CLOCKS_PER_SEC << ", ";
		csv_file.close();
		timer = clock();
		for(int a = 0; a < 100000; a++){
			num = rand()%(1<<30);
			find(num, array, N);
			if((double)(clock()-timer)/CLOCKS_PER_SEC >= MAX_TIME){
				break;
			}
		}
		timer = clock() - timer;
		csv_file.open("search.csv", ofstream::app);
		csv_file << (double)timer/CLOCKS_PER_SEC << ", ";
		csv_file.close();
		delete [] array;

		timer = clock();
		unordered_set<int> hashtable;
		num = NUM;
		for(int a = 0; a < (1 << N); a++){
			hashtable.insert(num);
			num = (num+1234567)%(1<<30);
		}
		timer = clock() - timer;
		csv_file.open("insert.csv", ofstream::app);
		csv_file << (double)timer/CLOCKS_PER_SEC << endl;
		csv_file.close();
		timer = clock();
		for(int a = 0; a < 100000; a++){
			num = rand()%(1<<30);
			hashtable.find(num);
		}
		timer = clock() - timer;
		csv_file.open("search.csv", ofstream::app);
		csv_file << (double)timer/CLOCKS_PER_SEC << endl;
		csv_file.close();
		hashtable.clear();

		cout << N << " completed." << endl;
	}
	return 0;
}
