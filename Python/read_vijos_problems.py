def read(url="https://vijos.org/p"):
    import urllib
    import urllib2
    page=urllib.unquote(urllib2.urlopen(urllib2.Request(url)).read()).split('\n')
    l=0
    while l<len(page) and page[l]!='    <tbody>':
        l+=1
    if l>=len(page): return []
    r=l+1
    while r<len(page) and page[r]!='            </tbody>':
        r+=1
    if r>=len(page): return []
    page='\n'.join(page[l:r+1])
    import xml.etree.ElementTree as et
    root = et.fromstring(page)
    table=[]
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
    for i in root:
        table.append([])
        for j in i:
            if trim(j.text):
                table[-1].append(trim(j.text))
            else:
                for k in j:
                    if trim(k.text):
                        table[-1].append(trim(k.text))
    return table
def my_len(s):
    l=0
    for i in s:
        if ord(i)>=128 and not i==u'\u2018' and not i==u'\u201c' and not i==u'\u2019' and not i==u'\u201d':
            l+=2
        else:
            l+=1
    return l
def print_table(table):
    import os
    slen=(45,8,7,4,3)
    def pr(s,l):
        le=my_len(s)
        if le>l:
            le=l
        os.sys.stdout.write(s+(l-le)*' ')
    for i in table:
        cnt=0
        if len(i)<len(slen): i.append('')
        for j in i:
            pr(j,slen[cnt])
            cnt+=1
        print('')
url='https://vijos.org/p'
table=[]
for i in range(1,11):
    table.extend(read(url+'?page='+str(i)))
table=sorted(table,key=lambda x:int(x[4]),reverse=True)
print_table(table)
