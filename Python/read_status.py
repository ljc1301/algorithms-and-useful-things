def my_len(s):
    l=0
    for i in s:
        if ord(i)>=128:
            l+=2
        else:
            l+=1
    return l
def trim(s):
    if s==None:
        return ''
    l=0
    while l<len(s) and (s[l]==' ' or s[l]=='\t' or s[l]=='\n' or s[l]=='\r'):
        l+=1
    if l>=len(s):
        return ''
    r=len(s)-1
    while l<=r and (s[r]==' ' or s[r]=='\t' or s[r]=='\n' or s[r]=='\r'):
        r-=1
    return s[l:r+1]
def print_title():
    slen=(7,30,30,10,10,8,10,7,9,8,20)
    ss=('编号','用户','题目','状态','分数','耗时','内存','语言','代码长度','测评机','提交时间')
    def pr(s,l):
        le=my_len(s)
        if le>l:
            le=l
        print(s,end=(l-le)*' ')
    for i in range(len(slen)):
        pr(ss[i],slen[i])
    print('')
def print_table(table):
    slen=(7,30,30,10,10,8,10,7,9,8,20)
    ss=('编号','用户','题目','状态','分数','耗时','内存','语言','代码长度','测评机','提交时间')
    def pr(s,l):
        le=my_len(s)
        if le>l:
            le=l
        print(s,end=(l-le)*' ')
    for i in table:
        cnt=0
        if len(i)<len(slen): i.insert(9,'')
        for j in i:
            pr(j,slen[cnt])
            cnt+=1
        print('')
def change(s):
    ret=''
    for i in s:
        if i=='&':
            ret+='＆'
        else:
            ret+=i
    return ret
def rechange(s):
    ret=''
    for i in s:
        if i=='＆':
            ret+='&'
        else:
            ret+=i
    return ret
def read(s='http://oi.wzms.com/solutions'):
    import urllib.request
    response = urllib.request.urlopen(s)
    html = response.read()
    s = change(html.decode('utf8'))
    li = s.split('\n')
    l = 0
    while l<len(li) and li[l] != "<tbody id='solutions-tbody'>":
        l += 1
    if l>=len(li):
        return []
    r = l + 1
    while li[r] != '</tbody>':
        r += 1
    r+=1
    li = li[l:r]
    s=''
    for i in li:
        s+=i+'\n'
    import xml.etree.ElementTree as et
    root = et.fromstring(s)
    table=[]
    for i in root:
        table.append([])
        for j in i:
            if trim(j.text):
                table[-1].append(rechange(trim(j.text)))
            else:
                for k in j:
                    if trim(k.text):
                        table[-1].append(rechange(trim(k.text)))
    return table
table=[]
print_title()
t=read()
while t:
    table.extend(t)
    print_table(t)
    t=read('http://oi.wzms.com/solutions?top='+str(int(table[-1][0])-1))
#table=sorted(table,key=lambda l:[100-int(l[3]) if l[3]!='编译错误' else 100,int(l[4][:-2]),float(l[5][:-2])],reverse=False)
#table=sorted(table,key=lambda l:int(l[0]),reverse=False)
#print_table(table)
