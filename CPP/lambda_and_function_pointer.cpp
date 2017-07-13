#include <cstdio>
#include <cstdlib>
double reduce(double(*func)(double,double),double *begin,double *end)
{
	double ans=*begin;
	for(begin++;begin!=end;begin++)
		ans=func(ans,*begin);
	return ans;
}
int main()
{
	int i,n;
	double *a;
	scanf("%d",&n);
	a=(double *)malloc(sizeof(double)*n);
	for(i=0;i<n;i++) scanf("%lf",a+i);
	printf("%lf\n",reduce([](double a,double b){ return a*b; },a,a+n));
	return 0;
}
