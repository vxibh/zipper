#include "classes.cpp"
#include "mapOfNumbers.cpp"
#include<iostream>

int binaryToDecimal(string str)
{
    int dec_num = 0;
    int power = 0 ;
    int n = str.length() ;
   
      for(int i = n-1 ; i>=0 ; i--){
      if(str[i] == '1'){
        dec_num += (1<<power) ;
      }
      power++ ;
    }
   
    return dec_num;
}

void findNytCode(AdaptiveHuffmanTreeNode* head, AdaptiveHuffmanTreeNode* nyt, string &nytCode, string temp){
    if(head == NULL){
        return;
    }
    if(head->left == NULL && head->right == NULL && head==nyt){
        nytCode = temp;
        return;
    }
    findNytCode(head->left,nyt, nytCode, temp+"0");
    findNytCode(head->right,nyt, nytCode, temp+"1");
}

void updateTree(string current_char, AdaptiveHuffmanTree* tree){
    AdaptiveHuffmanTreeNode* newNode = new AdaptiveHuffmanTreeNode(1);
    newNode->parent = tree->nyt->parent;
    newNode->left = tree->nyt;
    newNode->right = new AdaptiveHuffmanTreeNode(1, current_char);
    newNode->right->parent = newNode;
    tree->nyt->parent->left = newNode;
    tree->nyt->parent = newNode;

    newNode = newNode->parent;
    while(newNode!=NULL){
        newNode->weight = newNode->left->weight + newNode->right->weight;
        if(newNode->left->weight > newNode->right->weight){
            AdaptiveHuffmanTreeNode* temp = newNode->left;
            newNode->left = newNode->right;
            newNode->right = temp;
        }
        newNode=newNode->parent;
    }
    string nytCode = "";
    findNytCode(tree->head,tree->nyt, nytCode, "");
    tree->nytCode = nytCode;
}

string decode(string input){
    //2 parameters e and r
    // m = 94 = 2^e + r
    //e = 6, r = 30
    int e = 6;
    int r = 30;
    AdaptiveHuffmanTree* tree = new AdaptiveHuffmanTree();
    string output = "";

    map<int, string> mp = mapOfNumbers();

    int i;
    string temp = "";
    string current_char = "";

    for(i = 0; i < e; i++){
        temp += input[i];
    }
    //is bit value < r?
    if(binaryToDecimal(temp)<r){
        // i = i+1;
        temp += input[i];
        output += mp[binaryToDecimal(temp)+1]; 
        current_char = mp[binaryToDecimal(temp)+1];
        i = i+1;
    }
    else{
        output += mp[binaryToDecimal(temp) + r + 1];
        current_char = mp[binaryToDecimal(temp) + r + 1];
    }
    AdaptiveHuffmanTreeNode* head = new AdaptiveHuffmanTreeNode(1);
    tree->head = head;
    tree->head->left = tree->nyt;
    tree->nyt->parent = tree->head;
    tree->head->right = new AdaptiveHuffmanTreeNode(1, current_char);
    tree->head->right->parent = tree->head;
    tree->nytCode = "0";

    while(i<input.size()){
        temp = "";
        string tempNytCode = "";
        if(i + tree->nytCode.size() < input.size()){
            for(int j = i; j < i + tree->nytCode.size(); j++){
                tempNytCode += input[j];
            }
        }
        //if nyt code, i.e. element does not exist in tree
        if(tempNytCode == tree->nytCode){
            
            i = i+tree->nytCode.size();
            int j;

            for(j= i; j < i + e; j++){
                temp += input[j];
            }
            // cout << temp << " "<<j<< endl;
            // break;
            if(binaryToDecimal(temp) < r){
                temp += input[j];
                output += mp[binaryToDecimal(temp) + 1]; 
                current_char = mp[binaryToDecimal(temp) + 1];
                i = j+1;

                //add new node to tree + update the tree
                updateTree(current_char, tree);
            }
            
            else{
                output += mp[binaryToDecimal(temp) + r + 1];
                current_char = mp[binaryToDecimal(temp) + r + 1];
                i = j;

                //add new node to tree + update the tree
                updateTree(current_char, tree);
            }
            
        }
        //if not nyt code, i.e. element already exists in tree
        else{
            AdaptiveHuffmanTreeNode* curr = tree->head;
            while(curr->character==""){
                if(input[i] == '0'){
                    curr = curr->left;
                }
                else{
                    curr = curr->right;
                }
                temp += input[i];
                i++;
            }
            // cout << curr->character << endl;
            output += curr->character;
            curr->weight+=1;
            curr = curr->parent;
            
            while(curr!=NULL){
                curr->weight = curr->left->weight + curr->right->weight;
                if(curr->left->weight > curr->right->weight){
                    AdaptiveHuffmanTreeNode* temp = curr->left;
                    curr->left = curr->right;
                    curr->right = temp;
                }
                curr=curr->parent;
            }

            // cout << tree->head->weight << endl;
            // // cout << i << endl;
            // break;
        }
    }

    return output;
}

int main(){
    string s = "01001100001100000100000100000110111000000000111000001100111100000010011011111000001000111111000010010110111111100001101011111111000010001101111011111011110111011011111111100010000011111111110000101001111111111100000111111111111111100001001111110111111111111100100001110111111111111110001000101101111011111011011110110111111110111111111111110111111111111011111111111111100000001111111101110111111111111110110111101111111111111101101111111111111111000100011111111111111110101111111111111111011111111111111011000101101111111111111111101001011101111111111111111111001011001111111111111111111100101101111111111111111111111011111111111111111111111111111111111111111111100101111110111111111111111111111100010011111111111111111111111110000001110111111101111111111111111101111011111111111111111111110100100100";
    cout << decode(s) << endl;
    return 0;
}