import numpy
#config
distinct_urls_needed=10
#normal distribution param
mu=10
sigma=1
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
indexs=numpy.random.randint(0, len(url_vec),distinct_urls_needed)
nums = numpy.random.normal(mu, sigma,distinct_urls_needed)
class url_t:
    url=""
    num=0
    def __init__(self,u,n):
        self.url=u
        self.num=n
    def __lt__(self, other):
        return self.num>other.num
ans_vec=[]
for i in range(distinct_urls_needed):
    for j in range(int(nums[i])):
        f.write(url_vec[indexs[i]]+'\n')
    ans_vec.append(url_t(url_vec[indexs[i]],int(nums[i])))
ans_vec.sort()
for ele in ans_vec:
    fa.write(ele.url+" "+str(ele.num)+'\n')
f.close()
fa.close()
