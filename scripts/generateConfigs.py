matrices = ["2cubes_sphere",
            "apache2",
            "bcsstk36",
            "bmw7st_1",
            "bmwcra_1",
            "bundle1",
            "cant",
            "cfd1",
            "cfd2",
            "consph",
            "crankseg_1",
            "ct20stif",
            "denormal",
            "Dubcova2",
            "Dubcova3",
            "gridgena",
            "gyro",
            "hood",
            "matlist",
            "msc10848",
            "msc23052",
            "m_t1",
            "nasasrb",
            "nd3k",
            "nd6k",
            "offshore",
            "oilpan",
            "olafu",
            "pdb1HYS",
            "Pres_Poisson",
            "qa8fm",
            "raefsky4",
            "s3dkq4m2",
            "s3dkt3m2",
            "ship_001",
            "ship_003",
            "shipsec1",
            "shipsec5",
            "shipsec8",
            "smt",
            "thermomech_dM",
            "thread",
            "vanbody",
            "x104",
            "pwtk",
            "msdoor",
            "parabolic_fem",
            "G3_circuit",
            "audikw_1",
            "tmt_sym",
            "thermal2",
            "StocF-1465",
            "LF10",
            "Hook_1498",
            "Flan_1565",
            "Emilia_923",
            "cbuckle",
            "bundle_p",
            "boneS10",
            "bone010_M",
            "bone010"]
matricesDir = "/home/gitlab-runner/sparse/Matrices/"
reorderingsDir = "/home/gitlab-runner/sparse/Reorderings/Metis5.1.0/"
configsDir = "/home/gitlab-runner/sparse/config/"
solutionDir = "/home/gitlab-runner/sparse/SolverOutput/"
for m in matrices:
    filename = configsDir+m+".cfg"
    f = open(filename, 'w')
    a = "a="+matricesDir+m+".bin\n"
    b = "b=qqq.bin\n"
    t = "t=slasbin\n"
    mm = "m=multifrontal\n"
    r = "r="+reorderingsDir+m+".iperm\n"
    p = "p=10\n"
    s = "s=40\n"
    nt = "nt=1\n"
    x = "x="+solutionDir+m+".out\n"
    f.write(a)
    f.write(b)
    f.write(t)
    f.write(mm)
    f.write(r)
    f.write(p)
    f.write(s)
    f.write(nt)
    f.write(x)
    f.close()