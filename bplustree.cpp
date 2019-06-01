#include <bits/stdc++.h>
#include<stdio.h>

#define MIN_ORDER 4

using namespace std;


typedef struct Node {
	bool is_leaf;
	int nkeys;
	int* keys;
	struct Node** pointers;
	struct Node* parent;
	struct Node* next;
} Node;


class B_Plus_Tree{

	bool debug_flag = false;
	int tree_order = MIN_ORDER;

	public:
	
	Node* root = NULL;

	B_Plus_Tree(int order){
		tree_order = order;
	}

	void print_tree() {

		queue<Node*> q;

		Node* n = NULL;
		int i = 0;

		if (root == NULL) {
			printf("Empty tree.\n");
			return;
		}
		
		q.push(root);
		
		while(!q.empty()) {
			
			n = q.front();
			q.pop();

			for (i = 0; i < n->nkeys; i++) {

				printf("%d ", n->keys[i]);
			}
			if (!n->is_leaf)
				for (i = 0; i <= n->nkeys; i++)
					q.push(n->pointers[i]);
					
			
			printf("    ");
		}
		printf("\n");
	}

	int count_occurences(int key) {

		int count = 0,i=0;
		Node* node = root;

		if (!root) {
			if(debug_flag){
				cout<<"Empty tree\n";
			}
			return 0;
		}

		while(node->is_leaf==false)
			node = node->pointers[0];
		
		while(true) {
			for (i = 0; i < node->nkeys; i++) {

				if(node->keys[i]==key)
					count++;
				if(debug_flag){
					printf("%d ", node->keys[i]);
				}
			}

			if (node->pointers[tree_order - 1] != NULL) {
				node = node->pointers[tree_order - 1];
			}
			else
				break;
		}
		return count;	
	}
	
	bool find_key(int key) {

		if (root == NULL)
			return false;
	    
	    int i = 0;
	    Node* leaf = root;

	    if(root){

	    	while (!leaf->is_leaf) {

				i = 0;
				while (i < leaf->nkeys) {
					if (key >= leaf->keys[i]) 
						i++;
					else 
						break;
				}

				leaf = (Node*)leaf->pointers[i];

    		}
	    }

		for(i = 0; i < leaf->nkeys; i++)
			if(leaf->keys[i] == key){
				break;
			}

		if (i == leaf->nkeys){
			return false;
		}
		else{
			return true;
		}
	}

	int range_query(int start, int end) {

		if(!root)
			return 0;

		int i, num_found;
		num_found = 0;

		Node* n = root;

	    if(root){

	    	while (!n->is_leaf) {

				i = 0;
				while (i < n->nkeys) {
					if (start >= n->keys[i]) 
						i++;
					else 
						break;
				}

				n = (Node*)n->pointers[i];

    		}
	    }

		
		if (!n){
			return num_found;
		}

		int *temp = n->keys;
		int size = n->nkeys;

		for(i = 0; i < size && temp[i] < start; i++);
		
		if (i == n->nkeys){
			return num_found;
		} 

		temp = n->keys;
		size = n->nkeys;			
		
		while(n){

			for (; i < n->nkeys && n->keys[i] <= end; i++) {
				num_found++;
			}

			n = n->pointers[tree_order - 1];
			size = 0;
			i = 0;
		}

		return num_found;
	}

	Node* create_node(){

		Node* new_Node = NULL;
		new_Node = (Node*)malloc(sizeof(Node));
		
		if (new_Node == NULL) {
			if(debug_flag){
				cout<<"Unable to allocate space for new node by malloc\n";
			}
			exit(0);
		}
		
		new_Node->keys = (int*)malloc((tree_order - 1) * sizeof(int));
		
		if (new_Node->keys == NULL) {
			if(debug_flag){
				cout<<"Unable to allocate space for new node key pointer\n";
			}
			exit(0);
		}
		
		new_Node->pointers = (Node**)malloc(tree_order * sizeof(Node*));
		
		if (new_Node->pointers == NULL) {
			if(debug_flag){
				cout<<"Unable to allocate space for new node record pointer\n";
			}
			exit(0);
		}
		
		new_Node->is_leaf = false;
		new_Node->nkeys = 0;
		new_Node->parent = NULL;
		new_Node->next = NULL;
		
		return new_Node;
	}

	Node* insert_node(Node* root, Node* n, int left_index, int key, Node* right) {

		if(debug_flag&&!root){
			cout<<"Insert Node :root is NULL \n";
		}
		
		int num = n->nkeys;
		int k = left_index;
		Node* num_pointers;

		for (int i = num; i > left_index; i--) {
			n->pointers[i + 1] = n->pointers[i];
			// k += n->keys[i];
			if(debug_flag){
				cout<<n->keys[i]<<" "<<n->keys[i - 1]<<"\n";
			}
			n->keys[i] = n->keys[i - 1];
			// k += n->keys[i];
		}
		

		n->pointers[left_index + 1] = right;
		// k += left_index;
		if(debug_flag){
			cout<<n->nkeys<<"\n";
		}
		num_pointers = n->pointers[left_index + 1];
		n->keys[left_index] = key;
		n->nkeys++;

		if(debug_flag){
			cout<<"n->nkeys: "<<n->nkeys<<"\n";
		}

		return root;
	}

	Node* split_node(Node* root, Node* p_node, int left_index, int key, Node* right) {

		if(debug_flag){
			if(!root){
				cout<<"root is null in split node\n";
			}
		}

		int count = 0;
		int q = 0;
		int i = 0, j = 0, split = 0, k_prime = 0;
		Node* new_Node = NULL, * child = NULL;
		int * temp_keys = NULL;
		Node * first_ptr = NULL;
		Node* second_ptr = NULL;
		Node** temp_ptr = NULL;

		temp_ptr = (Node**)malloc((tree_order + 1) * sizeof(Node*));
		if (temp_ptr == NULL) {
			exit(0);
		}
		temp_keys = (int*)malloc(tree_order * sizeof(int));
		if (temp_keys == NULL) {
			exit(0);
		}

		Node* temp = p_node;
		int count_ptr = temp->nkeys;

		for (i = 0, j = 0; i < count_ptr + 1; i++, j++) {
			if (j == left_index + 1){
				count += left_index;
				j++;
			}
			second_ptr = temp->pointers[i];
			temp_ptr[j] = temp->pointers[i];
		}

		for (i = 0, j = 0; i < count_ptr; i++, j++) {
			if (j == left_index){
				count_ptr = temp->nkeys + 1;
				j++;
			}
			if (q <count_ptr) 
				q = count_ptr;
			temp_keys[j] = temp->keys[i];
		}

		temp_ptr[left_index + 1] = right;
		temp_keys[left_index] = key;


		if(tree_order % 2 == 0)
			split = tree_order/2;
		else
			split = tree_order/2 + 1;


		new_Node = create_node();
		p_node->nkeys = 0;
		
		Node* broot = NULL;
		broot = p_node;


		int sum = 0;
		if(debug_flag&&split==0){
			cout<<"Split value error\n";
		}

		for(i = 0; i < split - 1; i++) {
			p_node->pointers[i] = temp_ptr[i];
			sum++;
			p_node->keys[i] = temp_keys[i];
			p_node->nkeys++;
			if(sum>p_node->nkeys){
				sum = p_node->nkeys;
				cout<<"sum"<<sum<<"\n";
			}
		}
		
		p_node->pointers[i] = temp_ptr[i];

		if(sum>0&&debug_flag)
		{
			cout<<sum<<"\n";
		}

		k_prime = temp_keys[split - 1];
		broot = p_node;
		for (++i, j = 0; i < tree_order; i++, j++) {
			new_Node->pointers[j] = temp_ptr[i];
			if(debug_flag){
				cout<<temp_keys[i]<<"\n";
			}
			new_Node->keys[j] = temp_keys[i];
			new_Node->nkeys++;
			if(debug_flag){
				if(new_Node->nkeys==0)
					cout<<"new node is empty:";
			}
		}
		
		new_Node->pointers[j] = temp_ptr[i];
		
		free(temp_ptr);
		free(temp_keys);

		new_Node->parent = p_node->parent;
		
		if(debug_flag){
			if(!p_node->parent)
				cout<<"p_node->parent is NULL\n";
		}

		for (i = 0; i <= new_Node->nkeys; i++) {
			child = new_Node->pointers[i];
			if(debug_flag){
				cout<<"nkeys: "<<new_Node->nkeys<<"\n";
			}
			child->parent = new_Node;
		}

		if(debug_flag){
			if(!p_node)
				cout<<"p_node is NULL\n";
		}

		return move_to_parent(root, p_node, k_prime, new_Node);
	}

	Node* insert_into_leaf(Node* leaf, int key) {
		
		int i, insertion_point;

		insertion_point = 0;
		int num_keys = leaf->nkeys;
		int k =0;
		int * temp = leaf->keys;

		while (insertion_point < num_keys && temp[insertion_point] < key){
			k = temp[insertion_point];
			insertion_point++;
		}

		k = 0;
		for(int i = 0;i<insertion_point;i++)
			k += insertion_point;

		for (i = leaf->nkeys; i > insertion_point; i--) {
			k = leaf->keys[i];
			leaf->keys[i] = leaf->keys[i - 1];
			
			k = k + leaf->keys[i];
			
			leaf->pointers[i] = leaf->pointers[i - 1];
		}

		leaf->keys[insertion_point] = key;
		
		for(int i = 0;i<insertion_point;i++)
			k += insertion_point;

		leaf->nkeys++;
		
		if(debug_flag){
			if(leaf->nkeys==1){
				cout<<"Error in number of keys\n";
			}
		}

		return leaf;
	}

	Node* split_leaf(Node* root, Node* leaf, int key) {

		Node* new_leaf;
		int * temp_keys;
		int insertion_index, split, new_key, i, j;

		new_leaf = create_node();
		new_leaf->is_leaf = true;

		temp_keys = (int*)malloc(tree_order * sizeof(int));
		if (temp_keys == NULL) {
			exit(0);
		}

		int first = 0;
		insertion_index = 0;
		while (insertion_index < tree_order - 1 && leaf->keys[insertion_index] < key){
			insertion_index++;
			first += leaf->keys[insertion_index];
		}

		for(i = 0, j = 0; i < leaf->nkeys; i++, j++) {
			if (j == insertion_index){
				first = insertion_index; 
				j++;
			}
			first += insertion_index;
			temp_keys[j] = leaf->keys[i];
		}



		temp_keys[insertion_index] = key;

		leaf->nkeys = 0;

		if((tree_order - 1) % 2 == 0)
			split = (tree_order - 1)/2;
		else
			split = (tree_order - 1)/2 + 1;


		Node* ptr = NULL;

		for(i = 0; i < split; i++) {
			leaf->keys[i] = temp_keys[i];
			ptr = leaf;
			leaf->nkeys++;
		}

		for (i = split, j = 0; i < tree_order; i++, j++) {
			new_leaf->keys[j] = temp_keys[i];
			ptr = leaf;
			first += new_leaf->keys[j];
			new_leaf->nkeys++;
		}

		free(temp_keys);

		new_leaf->pointers[tree_order - 1] = leaf->pointers[tree_order - 1];
		
		Node* previous = new_leaf;

		leaf->pointers[tree_order - 1] = new_leaf;

		previous = leaf->pointers[tree_order - 1];

		new_leaf->parent = leaf->parent;
		ptr = new_leaf;
		first += 1;
		new_key = new_leaf->keys[0];

		if(debug_flag){
			cout<<new_key<<"\n";
			if(new_key==0){
				cout<<"new_key val inconsistent\n";
			}
		}

		return move_to_parent(root, leaf, new_key, new_leaf);
	}

	Node* move_to_parent(Node* root, Node* left, int key, Node* right) {


		if (left->parent == NULL){

			if(debug_flag&&root){

				cout<<root->nkeys<<"\n";
			}

			Node* root = create_node();
			
			root->keys[0] = key;
			root->nkeys++;
			
			root->parent = NULL;
			left->parent = root;
			
			if(debug_flag&&!root){
				cout<<"root is also null\n";
			}

			root->pointers[1] = right;
			root->pointers[0] = left;
			
			right->parent = root;
			
			return root;

		}

		Node* parent;
		parent = left->parent;
		int left_index = 0;

		while (left_index <= parent->nkeys && parent->pointers[left_index] != left)
			left_index++;



		if (parent->nkeys < tree_order - 1)
			return insert_node(root, parent, left_index, key, right);

		if(debug_flag){
			cout<<"left_index "<<left_index<<"\n";
		}

		return split_node(root, parent, left_index, key, right);
	}

	Node* insert(Node* root, int key) {

		
		Node* leaf = NULL;

		if (root == NULL){

			Node* root = create_node();
			root->is_leaf = true;
			root->keys[0] = key;
			root->parent = NULL;
			root->nkeys++;

			return root;
		}

		int i = 0;
		leaf = root;

	    if(root){

	    	while (!leaf->is_leaf) {

				i = 0;
				while (i < leaf->nkeys) {
					if (key >= leaf->keys[i]) 
						i++;
					else 
						break;
				}

				leaf = (Node*)leaf->pointers[i];

    		}
	    }



		if (leaf->nkeys < tree_order - 1) {
			leaf = insert_into_leaf(leaf, key);
			return root;
		}

		return split_leaf(root, leaf, key);
	}
};

vector<string> tokenize(string s){
    
    vector<string> tokens;
    
    string temp = "";
    
    for(int i=0;i<s.length();i++){
        
        if(s[i]!=' ')
            temp += s[i];
        else if(s[i]==' '){
            
            tokens.push_back(temp);
            temp = "";
        }
    }
    
    tokens.push_back(temp);
    
    return tokens;
}

void output_to_file(vector<string>& buffer){

	for(int i = 0;i<buffer.size();i++){
		cout<<buffer[i]<<endl;
	}
	buffer.clear();
}

void execute_operation(B_Plus_Tree& bt,vector<string>& input,vector<string>& output,int output_size){

	for(int i = 0;i<input.size();i++){
			
		if(input[i][0]=='F'){
            
            vector<string> input_vec;
            input_vec = tokenize(input[i]);

            bool retval = bt.find_key(stoi(input_vec[1]));
			
            if(output.size()>=output_size){

            	output_to_file(output);
            }

        	if(retval)            
            	output.push_back("YES");
            else
            	output.push_back("NO");	
			
        }
        else if(input[i][0]=='I'){

        	vector<string> input_vec;
            input_vec = tokenize(input[i]);

            // bool retval = bt.find_key(bt.root, stoi(input_vec[1]));

            // if(!retval)
            bt.root =  bt.insert(bt.root,stoi(input_vec[1]));

            	
   //          cout<<"Inside insert #################\n";
			// bt.print_tree(bt.root);
		 //    cout<<"#################\n";
        }
        else if(input[i][0]=='C'){
			
			vector<string> input_vec;
            input_vec = tokenize(input[i]);
            
            int count = bt.count_occurences(stoi(input_vec[1]));

            if(output.size()>=output_size){

            	output_to_file(output);
            }
            output.push_back(to_string(count));
            
        }
        else if(input[i][0]=='R'){

            vector<string> input_vec;
            input_vec = tokenize(input[i]);
            
            int input_key_1, input_key_2;

            input_key_1 = stoi(input_vec[1]);
            input_key_2 = stoi(input_vec[2]);

            if(input_key_2<input_key_1){

            	int temp = input_key_2;
            	input_key_2 = input_key_1;
            	input_key_1 = temp;
            }

            int count = bt.range_query(input_key_1, input_key_2);

            if(output.size()>=output_size){

            	output_to_file(output);
            }

            output.push_back(to_string(count));
        }
    }
}

int main(int argc, char ** argv){

    
    if(argc!=5){
        cout<<"Invalid input format\nExpected format\n";
        cout<<"[executable] [input filename] [M (nunber of buffer)] [B( buffer size eg. 4 KB)]\n";
        return 0;
    }

    string filename = argv[1];
    freopen(filename.c_str(),"r",stdin);
    freopen("output_tree.txt","w",stdout);


    string s(argv[4]);

    int num_of_buf = stoi(argv[2]);

    if(num_of_buf<=2){
        cout<<"Number of buffer must be atleast greater than 2\n";
        return 0;
    }

    int buf_size = stoi(argv[3]);

    string unit = argv[4];
    if(unit=="B"){
        buf_size = buf_size;
    }else if(unit == "KB")
        buf_size = buf_size * 1000;
    else if(unit == "MB")
        buf_size = buf_size * 1000000;
    else{
        cout<<"Invalid buffer size provided\n Expected B, KB, MB only\n";
        return 0;
    }

    vector<string> input_buf;
    vector<string> output_buf;

    int input_size = (num_of_buf - 1) * (buf_size/4);
    int output_size = buf_size/4;


    string input;
	int key;

	int order = 4;
	order = (buf_size - 8)/12;

	if(order < MIN_ORDER){

		cout<<"Order of B+ tree must be greater than "<<MIN_ORDER<<"\n"; 
		return 0;
	}




    B_Plus_Tree bt = B_Plus_Tree(order);


    while(getline(cin,input)){
    	
        if(input_buf.size() < input_size)
        
            input_buf.push_back(input);
        
        else{
        
        	execute_operation(bt,input_buf,output_buf,output_size);

		    input_buf.clear();

            input_buf.push_back(input);
        }
        
    }
    
    execute_operation(bt,input_buf,output_buf,output_size);

	input_buf.clear();

    output_to_file(output_buf);

    output_buf.clear();
    
    // bt.print_tree();

	return 0;
}

