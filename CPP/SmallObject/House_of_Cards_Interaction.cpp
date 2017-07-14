#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
const int INF=0x7FFFFFFF;
const int maxn=26;
const int UP=0;
const int DOWN=1;
const int FLOOR=2;
int cards[maxn]; 
struct State
{
	int n,score,player,pos;
	int card[8],type[8],hold[2];
	char ans[50];
	int point(int c1,int c2,int c3)
	{
		int cnt=0;
		if(c1>0) cnt++; 
		if(c2>0) cnt++; 
		if(c3>0) cnt++;
		if(cnt>=2) return abs(c1)+abs(c2)+abs(c3);
		return -abs(c1)-abs(c2)-abs(c3);
	}
	State copy()
	{
		State ret;
		ret.n=n;
		ret.pos=pos+1;
		ret.score=score;
		ret.player=player^1;
		memcpy(ret.card,card,sizeof(card)); 
		memcpy(ret.type,type,sizeof(type)); 
		memcpy(ret.hold,hold,sizeof(hold)); 
		memcpy(ret.ans,ans,sizeof(ans)); 
		return ret;
	}
	void expand(vector<State>& ret)
	{
		State t;
		int i;
		ret.clear();
		if(!hold[player])
		{
			t=copy();
			t.hold[player]=cards[pos];
			t.ans[0]='\0'; sprintf(t.ans,"hold");
			ret.push_back(t);
		}
		for(i=0;i<7;i++)
		{
			if(type[i]==DOWN && type[i+1]==UP)
			{
				t=copy();
				t.score+=point(card[i],card[i+1],cards[pos]);
				t.type[i]=t.type[i+1]=FLOOR;
				t.card[i]=t.card[i+1]=cards[pos];
				t.ans[0]='\0'; sprintf(t.ans,"use deck make floor on %d and %d",i+1,i+2);
				ret.push_back(t);
				if(hold[player])
				{
					t=copy();
					t.score+=point(card[i],card[i+1],hold[player]);
					t.type[i]=t.type[i+1]=FLOOR;
					t.card[i]=t.card[i+1]=hold[player];
					t.hold[player]=cards[pos];
					t.ans[0]='\0'; sprintf(t.ans,"use held make floor on %d and %d",i+1,i+2);
					ret.push_back(t);
				}
			}
			if(hold[player] && type[i]==FLOOR && type[i+1]==FLOOR && card[i]==card[i+1])
			{
				t=copy();
				t.score+=point(card[i],cards[pos],hold[player]);
				t.type[i]=UP; t.type[i+1]=DOWN;
				t.card[i]=cards[pos]; t.card[i+1]=hold[player];
				t.hold[player]=0;
				t.ans[0]='\0'; sprintf(t.ans,"use deck and held make up and down on %d and %d",i+1,i+2);
				ret.push_back(t);
				t.ans[0]='\0'; sprintf(t.ans,"use held and deck make up and down on %d and %d",i+1,i+2);
				swap(t.card[i],t.card[i+1]); 
				ret.push_back(t);
			}
		}
	}
	bool isFinal()
	{
		if(pos!=n) return false;
		//score+=hold[0]+hold[1];
		return true;
	}
}; 
int alphabeta(State& s,int alpha,int beta,State& ret)
{
	if(s.isFinal()) return s.score+s.hold[0]+s.hold[1];
	vector<State> children=vector<State>();
	s.expand(children);
	int n=children.size(),i,v,player=s.player,t=0,tv=(!player)?alpha:beta;
	for(i=0;i<n;i++)
	{
		v=alphabeta(children[i],alpha,beta,ret);
		if(!player) { if(alpha<v) { alpha=v; t=i; } }
		else { if(beta>v) { beta=v; t=i; } }
		if(beta<=alpha) break;
	}
	ret=children[t];
	return (!player) ? alpha : beta;
}
int main()
{
    State s,a;
	vector<State> children=vector<State>();
    string str;
	char t[100],ch;
    int caseno=0,ans,i,choice,n;
    while(1)
    {
    	scanf("%s",t); str=t;
    	if(str=="End") break;
    	scanf("%d",&s.n);
    	if(s.n==0) break;
    	s.n<<=1; s.pos=8; s.score=0;
    	for(i=0;i<8;i++)
    	{
    		scanf("%s",t);
    		sscanf(t,"%d%c",&s.card[i],&ch);
    		if(ch=='B') s.card[i]*=-1;
    		cards[i]=s.card[i];
    		s.type[i]=1&i;
    		if(!i) s.player=(ch!='R'); 
    	}
    	for(i=8;i<s.n;i++)
    	{
    		scanf("%s",t);
    		sscanf(t,"%d%c",&cards[i],&ch);
    		if(ch=='B') cards[i]*=-1;
    	}
    	s.hold[0]=s.hold[1]=0;
//    	printf("Case %d: ",++caseno);
		printf("\nCase #%d:\n",++caseno);
		//ans=alphabeta(s,-INF,+INF,a);
		do
		{
			if((s.player && str=="Birgit")||(!s.player && str=="Axel") || str=="people")
			{
		    	a=s.copy(); a.pos--; a.player^=1;
				alphabeta(s,-INF,+INF,a);
				s.expand(children);
				n=children.size();
				printf("%-6d: help\n",-1);
				for(i=0;i<n;i++)
					printf("%-6d: %s\n",i,children[i].ans);
				scanf("%d",&choice);
				if(choice==-1)
				{
					s=a.copy(); s.pos--; s.player^=1;
			    	printf("%s: %s\n",(s.player)?"Axel  ":"Birgit",s.ans);
				}
				else
				{
			    	printf("%s: %s\n",(!s.player)?"Axel  ":"Birgit",children[choice].ans);
			    	s=children[choice].copy(); s.pos--; s.player^=1;
			    	printf("best  : %s\n",a.ans);
				}
			}
			else
			{
		    	alphabeta(s,-INF,+INF,a);
		    	s=a.copy(); s.pos--; s.player^=1;
		    	printf("%s: %s\n",s.player?"Axel  ":"Birgit",s.ans);
	    	}
	    }while(!s.isFinal());
		ans=s.score+s.hold[0]+s.hold[1];
		if(str=="CPU" || str=="people")
	    	if(ans==0)
	    		printf("Axel and Birgit tie\n\n");
	    	else if(ans>0)
	    		printf("Axel wins %d\n\n",ans);
	    	else
	    		printf("Birgit wins %d\n\n",-ans);
	    else
	    {
	    	if(str=="Birgit") ans=-ans; 
	    	if(ans==0)
	    		printf("Axel and Birgit tie\n\n");
	    	else if(ans>0)
	    		printf("%s wins %d\n\n",str.c_str(),ans);
	    	else
	    		printf("%s loses %d\n\n",str.c_str(),-ans);
    	}
    }
    return 0;
}
