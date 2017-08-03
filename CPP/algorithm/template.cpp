#include <cstdio>
#include <cstdlib>
template<typename T,T(*func)(const T&,const T&)> T reduce(T *begin,T *end)
{
	T ans=*begin;
	for(begin++;begin!=end;begin++)
		ans=func(ans,*begin);
	return ans;
}
template<typename T> T add(const T &a,const T &b) { return a+b; }
template<typename T> T times(const T &a,const T &b) { return a*b; }
template<typename T> T min(const T &a,const T &b) { return a<b?a:b; }
template<typename T> T max(const T &a,const T &b) { return a>b?a:b; }
int main()
{
	int n,i;
	double *a;
	scanf("%d",&n);
	a=(double *)malloc(sizeof(double)*n);
	for(i=0;i<n;i++) scanf("%lf",a+i);
	printf("%lf\n",reduce<double,add>(a,a+n));
	printf("%lf\n",reduce<double,times>(a,a+n));
	printf("%lf\n",reduce<double,min>(a,a+n));
	printf("%lf\n",reduce<double,max>(a,a+n));
	free(a);
	return 0;
}
