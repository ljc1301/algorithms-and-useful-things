#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
using namespace std;
struct node
{
	node *l,*r;
	char ch;
	node():l(NULL),r(NULL),ch(' ') { }
}*list;
struct transfer
{
	vector<string> operations;
	string final;
};
const int maxn=100;
int n,len,m_config;
map<char,transfer> turing[maxn];
map<string,int> names;
void init()
{
	int i;
	system("cls");
	list=new node;
	for(i=0;i<maxn;i++) turing[i].clear();
}
void print()
{
	node* i;
	system("cls");
	for(i=list;i->l!=NULL;i=i->l);
	for(;i!=NULL;i=i->r) printf("%c",i->ch);
}
void del()
{
	node* i;
	for(i=list;i->r!=NULL;i++);
	for(i=i->l;i!=NULL;i=i->l) delete i->r;
	delete list;
}
void split(string& s,vector<string>& ret)  
{  
    int i;
	string t="";
	ret.clear();
	for(i=0;i<s.length();i++)
		if(s[i]==',')
		{
			ret.push_back(t);
			t="";
		}
		else
			t=t+s[i];
	ret.push_back(t);
}
bool read()
{
	int i,j,t[maxn],k;
	char str[maxn],str2[maxn],str3[maxn];
	string temp;
	vector<string> ret,ret2;
	printf("Please input the number of m-config., run time and frequency(0 0 0 to end):");
	if(!(scanf("%d %d %d",&n,&len,&frequency)==3 && n && len)) return false;
	for(i=0;i<n;i++)
	{
		printf("Please input the name and the number of transfer #%d:",i);
		scanf("%s %d",str,&t[i]);
		names[str]=i;
		for(j=0;j<t[i];j++)
		{
			printf("Please input the symbol, operations and final m-config.:");
			scanf("%s %s %s",str,str2,str3);
			temp=str; split(temp,ret);
			temp=str2;
			if(temp[0]=='N' || temp[0]=='n') ret2.clear();
			else split(temp,ret2);
			for(k=0;k<ret.size();k++)
			{
				if(ret[k].length()>1)
					if(ret[k]=="else")
						ret[k]="?";
					else
						ret[k]=" ";
				turing[i][ret[k][0]].operations=ret2;
				turing[i][ret[k][0]].final=str3;
			}
		}
	}
	m_config=0;
	return true;
}
bool change()
{
	transfer t;
	int final,i;
	if(!turing[m_config].count(list->ch))
		if(turing[m_config].count('?'))
			t=turing[m_config]['?'];
		else
			return false;
	else
		t=turing[m_config][list->ch];
	if(!names.count(t.final)) return false;
	final=names[t.final];
	for(i=0;i<t.operations.size();i++)
	{
		switch(t.operations[i][0])
		{
			case 'L': case 'l':
				if(list->l==NULL) { list->l=new node; list->l->r=list; }
				list=list->l;
				break;
			case 'R': case 'r':
				if(list->r==NULL) { list->r=new node; list->r->l=list; }
				list=list->r;
				break;
			case 'E': case 'e':
				list->ch=' ';
				break;
			case 'P': case 'p':
				list->ch=t.operations[i][1];
				break;
			default:
				return false;
		}
	}
	m_config=final;
	return true;
}
int main()
{
	system("mode con cols=120 lines=40");
	init();
	while(read())
	{
		system("cls");
		while(len-- && change()) if(!(len%frequency)) print();
		del();
		printf("\n");
		system("pause");
		init();
	}
	return 0;
}
