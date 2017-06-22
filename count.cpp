#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <stdlib.h>
#include <sstream>
#include "time.h"
#include "math.h"
#include "stdio.h"
using namespace std;

int toInt(string str){
    int l = str.length();
    int ret=0;
    for (int i=0; i<l; i++){
        if (str[i]>=0)
            ret = ret*10 + (str[i]-'0');
        else return ret;
    }
    return ret;
}

string getTime(){
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    return tmp;
}

string print_vec(const vector<string> &v)
{
    if (v.size()==0) return "";
    string out;
    for (int i = 0; (size_t)(i)<v.size()-1; i++){
        out.append(v[i]);
        out.append(",");
    }
    out.append(v[v.size()-1]);
    return out;
}
string print_vec(const vector<string> &v, string c)
{
    if (v.size()==0) return "";
    string out;
    for (int i = 0; (size_t)(i)<v.size()-1; i++){
        out.append(v[i]);
        out.append(c);
    }
    out.append(v[v.size()-1]);
    return out;
}


vector<string> split(string line, char c){
    int l=line.length();
    vector<string> ret;
    string s="";
    int a=0;
    for(int i=0; i<l; i++){
        if(line[i]=='$'){
            ret.insert(ret.end(),s);
            return ret;
        }
        if(line[i]==c){
            ret.insert(ret.end(),s);
            a=i+1;
            s="";
        }else
            s=s+line[i];
    }
    if(s.length()!=0) ret.push_back(s);
    //cout<<print_vec(ret,";")<<endl;
    return ret;
}


struct Name{
    string name;
    vector<string> catagory1;
    vector<string> catagory2;
    Name(){
    }
    
    
    void setName(string s){
        name = s;
    }
    string getname(){
        return name;
    }
    void add_c1(string c){
        catagory1.insert(catagory1.end(),c);
    }
    void add_c2(string c){
        catagory2.insert(catagory2.end(),c);
    }
    void changeName(string s){
        name = s;
    }
    void setCat1(vector<string> v){
        catagory1=v;
    }
    void setCat2(vector<string> v){
        catagory2=v;
    }
    
};

struct Item {
    Name name;
    int count;
    int count_a;
    int count_b;
    int count_c;
    string time;
    
    Item(){
        count=0;
        count_a=0;
        count_b=0;
        count_c=0;
    }
    
    Item(string s,int i, int a, int b, int c) {
        name.setName(s);
        count =i;
        count_a=a;
        count_b=b;
        count_c=c;
    }
    
    Item(string line){
        vector<string> tempvec1=split(line,'|');
        name.setName(tempvec1[0]);
        name.setCat1(split(tempvec1[1],','));
        name.setCat2(split(tempvec1[2],','));
        loadCount(tempvec1[3]);
        loadTime(tempvec1[4]);
    }
    
    string getname(){
        return name.name;
    }
    
    void add_mom(){
        count_a++;
    }
    void add_aunt(){
        count_b++;
    }
    void add_son(){
        count_c++;
    }
    void add_count(){
        count++;
    }
    
    void add(int i){
        switch(i){
            case 0:{
                count_a++;
                break;
            }
            case 1:{
                count_b++;
                break;
            }
            case 2:{
                count_c++;
                break;
            }
            default:{
                cout<<"case error(mem)";
            }
        }
    }
    
    void print(int i){
        ofstream fout;
        if (i==0)
            fout.open("test.txt", ios::app);
        if (i==1)
            fout.open("test.txt", ios::out);
        if (i==2){
            cout<<name.getname()<<"|"<<print_vec(name.catagory1)<<"|"<<print_vec(name.catagory2)<<"|"
            <<count<<","<<count_a<<","<<count_b<<","<<count_c<<"|"<<endl;
            return;
        }
        
        cout<<name.getname()<<"|"<<print_vec(name.catagory1)<<"|"<<print_vec(name.catagory2)<<"|"
        <<count<<","<<count_a<<","<<count_b<<","<<count_c<<"|"<<time<<"$"<<endl;
        fout<<name.getname()<<"|"<<print_vec(name.catagory1)<<"|"<<print_vec(name.catagory2)<<"|"
        <<count<<","<<count_a<<","<<count_b<<","<<count_c<<"|"<<time<<"$"<<endl;
        fout.close();
        //cout<<"test written"<<endl;
    }
    
    void loadCount(string s){
        vector<string> v = split(s,',');
        count = toInt(v[0]);
        count_a = toInt(v[1]);
        count_b = toInt(v[2]);
        count_c = toInt(v[3]);
    }
    
    void loadTime(string t){
        time = t;
    }
    void refreshTime(){
        time = getTime();
    }
    
};



double percent(double r)
{
    r = r*10000;
    r = (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
    return r/100.00;
}
string percentstr(double r)
{
    r = r*10000;
    r = (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
    int i=(int)(r);
    string ret;
    ret=ret+to_string(i%10000/1000);
    ret=ret+to_string(i%1000/100);
    ret=ret+'.';
    ret=ret+to_string(i%100/10);
    ret=ret+to_string(i%10);
    
    return ret;
}


string getmem(int i){
    switch (i) {
        case 0:
            return "妈妈";
            break;
        case 1:
            return "二姨娘";
            break;
        case 2:
            return "儿子";
            break;
        default:
            return "其他成员";
            break;
    }
};

Item findItem(queue<Item> &menu, string name){
    Item ret = menu.front();
    queue<Item> temp;
    while(menu.size()!=0){
        if (name.compare(menu.front().getname())==0){
            ret = menu.front();
        }else
            temp.push(menu.front());
        menu.pop();
    }
    while(temp.size()!=0){
        menu.push(temp.front());
//      cout<<"aa";
        temp.pop();
    }
    return ret;
}

void cleartext(string filename){
    ofstream fout;
    fout.open(filename,ios::out);
    fout.close();
}

void printall(queue<Item> menu){
    cleartext("test.txt");
    while(menu.size()!=0){
        menu.front().print(0);
        menu.pop();
    }
}

Item locateItem(queue<Item> &menu){
    Item currentItem;
    ifstream infile;
    ofstream outfile;
    string namebuffer;
    //cout<<print_vec(menu.front().name.catagory1,"¥");
    cout<<"请输入菜名"<<endl;
    getline(cin, namebuffer);
    if(menu.size()!=0) currentItem =findItem(menu,namebuffer);
    //printall(menu);
    if (currentItem.getname().compare(namebuffer)){
        Item itembuffer(namebuffer,1,0,0,0);
        while(true){//catagory1 input
            string cat1buffer;
            cout<<"请逐个输入配料，输入‘继续’或回车结束"<<endl;
            getline(cin, cat1buffer);
            if (cat1buffer.compare("继续")==0||cat1buffer.length()==0) break;
            itembuffer.name.add_c1(cat1buffer);
        }
        while(true){//catagory2 input
            string cat2buffer;
            cout<<"请逐个输入使用的制作方法，输入‘继续’或回车结束"<<endl;
            getline(cin, cat2buffer);
            if (cat2buffer.compare("继续")==0||cat2buffer.length()==0) break;
            itembuffer.name.add_c2(cat2buffer);
            
        }
        //menu.push(itembuffer);
        currentItem= itembuffer;
    }
    else{
        currentItem.count++;
    }
    return currentItem;
}

int write(queue<Item> menu){
    Item currentItem = locateItem(menu);
    for(int i = 0; i<3; i++){
        cout<<"这次"<<getmem(i)<<"喜欢这个菜吗？"<<endl<<"喜欢请输入”y（es）“或者“是”或者“喜欢”，其他输入则为不喜欢：";
        string a;
        getline(cin, a);
        if (a.compare("y")==0||a.compare("yes")==0||a.compare("是")==0||!a.compare("喜欢"))
            currentItem.add(i);
    }
    //printall(menu);
    currentItem.refreshTime();
    menu.push(currentItem);
    
    printall(menu);
    
    return 1;
    
}



int main(){
    
    //cout<<"split test"<<endl;
    while(true){
        cout<<"输入“录入”来记录菜肴，输入“查询”来查询菜肴,“q”来退出"<<endl;
        string mode;
        getline(cin, mode);
        if (!mode.compare("q")) break;
        queue<Item> menu;
        ifstream in("test.txt");
        string line;
        cout<<"file opened"<<endl;
        int lineNum=0;
        if (in){
            while(getline(in, line)){
                menu.push(Item(line));
                lineNum++;
                cout<<"line "<<lineNum<<" loaded :";
                menu.back().print(2);
            }
        }else cout<<"nothing loaded"<<endl;
        if (!mode.compare("录入")) write(menu);
        
    }
    
    return 0;
    
}


