#include <bits/stdc++.h>
using namespace std;
string encodedText="";
string decodedText="";
class node{
public:
	int freq;
	string str;
	node *left;
	node *right;

	node(int num,string s){
		freq=num;
		str=s;
		left=NULL;
		right=NULL;
	}
};
node* finalnode;

class node2{
public:
	char charct;
	node2* left;
	node2* right;

	node2(char d){
		charct=d;
		left=NULL;
		right=NULL;
	}
};
int indx=0;
node2* destringify(string stg){
	node2* temp=new node2('\0');
	if(stg[indx]=='/'){
		indx++;
		temp->charct=stg[indx];
		indx++;
		return temp;
	}
	indx++;
	node2* lft=destringify(stg);
	temp->left=lft;
	indx++;
	node2* rgt=destringify(stg);
	temp->right=rgt;

	return temp;
}
node2 *fnlnode;

map<char,string> mp1;
void getpath(node* root,string path){
	string h=root->str;
	if(h.length()==1){
		char ch=h[0];
		mp1[ch]=path;
		return;
	}
	getpath(root->left,path+"0");
	getpath(root->right,path+"1");
}

string stringify(node* root){
	string r=root->str;
	if(r.length()==1){
		string cur="/";
		cur=cur+r;
		return cur;
	}
	return "0"+stringify(root->left)+"1"+stringify(root->right);
}

void encode(string data){
	priority_queue<pair<int,string>,vector<pair<int,string>>,greater<pair<int,string>>> pq;
	map<char,int> mp;
	for(int i=0;i<data.length();i++){
		mp[data[i]]++;
	}
	for(auto it:mp){
		string cur(1,it.first);
		pq.push(make_pair(it.second,cur));
	}
	map<string,node*> mapp;
	while(pq.size()>1){
		pair<int,string> p1,p2,p3;
		p1=pq.top();
		pq.pop();
		p2=pq.top();
		pq.pop();
		p3=make_pair(p1.first+p2.first,p1.second+p2.second);
		pq.push(p3);
		if(mapp.find(p1.second)==mapp.end()){
			node* n=new node(p1.first,p1.second);
			mapp[p1.second]=n;
		}
		if(mapp.find(p2.second)==mapp.end()){
			node* n1=new node(p2.first,p2.second);
			mapp[p2.second]=n1;
		}
		node *n2=new node(p1.first+p2.first,p1.second+p2.second);
		n2->left=mapp[p1.second];
		n2->right=mapp[p2.second];
		mapp[p1.second+p2.second]=n2;
		finalnode=n2;
	}

	string path="";
	getpath(finalnode,path);

	string binary_string="";
	for(int i=0;i<data.length();i++){
		binary_string=binary_string+mp1[data[i]];
	}

	int remainder=(8-(binary_string.length()%8))%8;
	string padding="";
	for(int i=0;i<remainder;i++){
		padding=padding+"0";
	}
	binary_string=binary_string+padding;

	string result="";
    for(int i=0;i<binary_string.length();i+=8){
        int number = 0;
        for(int j=0;j<8;j++){
        	int w=binary_string[i+j]-48;
            number = number*2 + (w-0);
        }
        char chr=char(number);
        result =result+chr;
    }
    string fromTree=stringify(finalnode);
    encodedText=fromTree+"$#"+padding+"$#"+result;
    cout<<"The encoded text of the given input: "<<endl<<endl<<encodedText<<endl<<endl;
}

void tokenize(string s, string key,string &newFromTree,string &newPadding,string &newResult)
{
	int k=0;
    int start = 0;
    int end = s.find(key);
    while (end != -1) {
    	if(k==0){
    		newFromTree=s.substr(start, end - start);
    	}
    	if(k==1){
    		newPadding=s.substr(start, end - start);
    	}
        start = end + key.size();
        end = s.find(key, start);
        k++;
    }
    newResult=s.substr(start, end - start);
}

void decode(){
	string newFromTree="";
	string newPadding="";
	string newResult="";
	string key="$#";
	tokenize(encodedText,key,newFromTree,newPadding,newResult);
	fnlnode=destringify(newFromTree);
	string binary_stg="";
	for(int i=0;i<newResult.length();i++){
		unsigned char charac=newResult[i];
		int no=int(charac);
		string bin="";
		for(int j=0;j<8;j++){
			int y=no%2;
			string z=to_string(y);
			bin=z+bin;
			no=no/2;
		}
		binary_stg=binary_stg+bin;
	}
	int len=newPadding.length();
	int len1=binary_stg.length();
	binary_stg=binary_stg.substr(0,len1-len);
	node2* temp=fnlnode;
	decodedText="";
	for(int i=0;i<binary_stg.length();i++){
		if(binary_stg[i]=='0'){
			temp=temp->left;
		}
		if(binary_stg[i]=='1'){
			temp=temp->right;
		}
		if(temp->charct!='\0'){
			decodedText=decodedText+temp->charct;
			temp=fnlnode;
		}
	}
	cout<<endl<<"The decoded text of the given input: "<<endl<<endl<<decodedText<<endl;
}

int main(){
	string inputString;          // inputString should contains atleast two type of characters
	getline(cin,inputString);
	encode(inputString);
	decode();
	float compressionRatio=float(decodedText.length())/encodedText.length();
	cout<<endl<<endl<<"The compression ratio is: "<<compressionRatio<<endl;
}
