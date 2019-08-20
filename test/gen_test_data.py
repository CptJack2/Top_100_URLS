#import numpy
#config
urls_needed=1000
#normal distribution param
mu=500
sigma=100

global url_vec

class url_t:
    url=""
    num=0
    def __init__(self,u,n):
        self.url=u
        self.num=n
    def __lt__(self, other):
        return self.num>other.num

def gen_from_ans():
    try:
        f=open('../data/ans.txt','r')
    except Exception as e:
        print(e)
    try:
        fo=open('../data/Data.txt','w')
    except Exception as e:
        print(e)
    urls=[]
    nums=[]
    for line in f:
        t=line.strip().split(' ')
        urls.append(t[0])
        nums.append(int(t[1]))
    for i,url in enumerate(urls):
        for j in range(nums[i]):
            fo.write(url+'\n')

    f.close()
    fo.close()

def gen_normal_distributed(mu,sigma, urls_needed):
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

#main
total=0
for i in range(138):
    f=open("../data/res_0_"+str(i)+".txt")
    f.readline()
    t=int(f.readline().strip())
    total+=t
print(total)
#gen_from_ans()
#read in urls
# try:
#     f=open('../test/urls.txt','r')
# except Exception as e:
#     print(e)
#
# url_vec=[]
# for line in f:
#     line=line.strip()
#     url_vec.append(line)
# f.close()
#
# gen_normal_distributed(mu,sigma,urls_needed)

