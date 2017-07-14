int alphabeta(State& s,int alpha,int beta)
{
	if(s.isFinal()) return s.score;
	vector<State> children=vector<State>();
	s.expand(children);
	int n=children.size(),i,v,player=s.player;
	for(i=0;i<n;i++)
	{
		v=alphabeta(children[i],alpha,beta);
		if(!player) alpha=max(alpha,v);
		else beta=min(beta,v);
		if(beta<=alpha) break;
	}
	return (!player) ? alpha : beta;
}
