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
for i in range(distinct_urls_needed):
    for j in range(int(nums[i])):
        f.write(url_vec[indexs[i]]+'\n')
    fa.write(url_vec[indexs[i]]+" "+str(int(nums[i]))+'\n')
f.close()
fa.close()
