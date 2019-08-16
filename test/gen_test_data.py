import numpy
#config
urls_needed=1000
#normal distribution param
mu=500
sigma=100
#read in urls
try:
    f=open('../test/urls.txt','r')
except Exception as e:
    print(e)
url_vec=[]
for line in f:
    line=line.strip()
    url_vec.append(line)
f.close()
#gen urls subject to normal distribution
try:
    f=open('../data/Data.txt','w')
except Exception as e:
    print(e)
try:
    fa=open('../data/ans.txt','w')
except Exception as e:
    print(e)
indexs=numpy.random.randint(0, len(url_vec), urls_needed)
ind_set=set()
for i in indexs:
    ind_set.add(i)
indexs=list(ind_set)
nums = numpy.random.normal(mu, sigma,len(indexs))
class url_t:
    url=""
    num=0
    def __init__(self,u,n):
        self.url=u
        self.num=n
    def __lt__(self, other):
        return self.num>other.num
ans_vec=[]
for i in range(len(indexs)):
    num=int(nums[i])
    ans_vec.append(url_t(url_vec[indexs[i]],num))
    for j in range(num):
        f.write(url_vec[indexs[i]]+'\n')
ans_vec.sort()
for ele in ans_vec:
    fa.write(ele.url+" "+str(ele.num)+'\n')
f.close()
fa.close()
