matrices = ["LF10"]
matricesDir = "/common/SLA/Matrices/"
reorderingsDir = "/common/SLA/Reorderings/metis5.1.0/"
configsDir = "/home/lebedev_s/configs/"
solutionDir = "/home/lebedev_s/solutions/"
for m in matrices:
    filename = configsDir+m+".cfg"
    f = open(filename, 'w')
    a = "a="+matricesDir+m+".bin\n"
    b = "b=qqq.bin\n"
    t = "t=slasbin\n"
    m = "m=multifrontal\n"
    r = "r="+reorderingsDir+m+".iperm\n"
    p = "p=10\n"
    s = "s=40\n"
    nt = "nt=1\n"
    x = "x="+solutionDir+m+".out"
    f.write(a)
    f.write(b)
    f.write(t)
    f.write(m)
    f.write(r)
    f.write(p)
    f.write(s)
    f.write(nt)
    f.write(x)
    f.close()


