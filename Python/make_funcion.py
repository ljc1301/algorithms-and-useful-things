__metaclass__=type

def gcd(a,b):
    while b:
        a,b=b,a%b
    return a

class frac:
    def __init__(self,a=0,b=1):
        g=gcd(a,b)
        self.a=a//g
        self.b=b//g
        if self.b<0:
            self.a=-self.a
            self.b=-self.b

    def __str__(self):
        if self.b==1:
            return str(self.a)
        return '%d/%d'%(self.a,self.b)

    def __add__(self,x):
        return frac(self.a*x.b+x.a*self.b,self.b*x.b)

    def __sub__(self,x):
        return frac(self.a*x.b-x.a*self.b,self.b*x.b)

    def __mul__(self,x):
        return frac(self.a*x.a,self.b*x.b)

    def __div__(self,x):
        return frac(self.a*x.b,self.b*x.a)

    def __len__(self):
        return bool(self.a)

    def __neg__(self):
        return frac(-self.a,self.b)

def gauss_jordan(n,mat):
    for i in range(n):
        k=i;
        while (k<n) and not mat[k][i]:
            k+=1
        if k>=n:
            return False
        if k!=i:
            mat[i],mat[k]=mat[k],mat[i]
        for j in range(i+1,n+1):
            mat[i][j]/=mat[i][i]
        mat[i][i]=frac(1);
        for j in range(n):
            if i!=j:
                for k in range(i+1,n+1):
                    mat[j][k]-=mat[j][i]*mat[i][k]
                mat[j][i]=frac()
    return True

mat=[]
n=int(raw_input())
for i in range(n):
    x,y=[int(i) for i in raw_input().split()]
    mat.append([])
    for j in range(n):
        mat[-1].append(frac(x**(n-j-1)))
    mat[-1].append(frac(y))
gauss_jordan(n,mat)
flag=False
for i in range(n):
    if mat[i][n]:
        if mat[i][n].a>0:
            if flag:
                if i==n-1:
                    print '+',mat[i][n],
                elif i==n-2:
                    if mat[i][n].a==1 and mat[i][n].b==1:
                        print '+ x',
                    else:
                        print '+ '+str(mat[i][n])+'x',
                else:
                    if mat[i][n].a==1 and mat[i][n].b==1:
                        print '+ x^'+str(n-i-1),
                    else:
                        print '+ '+str(mat[i][n])+'x^'+str(n-i-1),
            else:
                flag=True
                if i==n-1:
                    print mat[i][n],
                elif i==n-2:
                    if mat[i][n].a==1 and mat[i][n].b==1:
                        print 'x',
                    else:
                        print str(mat[i][n])+'x',
                else:
                    if mat[i][n].a==1 and mat[i][n].b==1:
                        print 'x^'+str(n-i-1),
                    else:
                        print str(mat[i][n])+'x^'+str(n-i-1),
        else:
            flag=True
            if i==n-1:
                print '-',-mat[i][n],
            elif i==n-2:
                if mat[i][n].a==-1 and mat[i][n].b==1:
                    print '- x',
                else:
                    print '- '+str(-mat[i][n])+'x',
            else:
                if mat[i][n].a==-1 and mat[i][n].b==1:
                    print '- x^'+str(n-i-1),
                else:
                    print '- '+str(-mat[i][n])+'x^'+str(n-i-1),
if not flag:
    print '0',
print ''
