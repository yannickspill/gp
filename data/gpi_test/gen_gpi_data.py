#!/usr/bin/env python

import sys,os

from numpy import *

import IMP
import IMP.core
import IMP.isd

#data
npoints = 11 # q values
ndraws = 10 # times we evaluate gpir for each of tau lambda and sigma
logmin = -2
logmax = 2 # range on log scale for nuisance values
q = linspace(-10,10,num=npoints)
I = random.normal(q) + q**2
q = [[i] for i in q ]
err = ones(npoints)
N = 10

#functions
m = IMP.Model()
a = IMP.isd.Nuisance.setup_particle(IMP.Particle(m),0.0)
b = IMP.isd.Nuisance.setup_particle(IMP.Particle(m),0.0)
mean = IMP.isd.Linear1DFunction(a,b)
tausig = IMP.isd.Scale.setup_particle(IMP.Particle(m),1.0)
lam = IMP.isd.Scale.setup_particle(IMP.Particle(m),1.0)
sig2 = IMP.isd.Scale.setup_particle(IMP.Particle(m),1.0)
cov = IMP.isd.Covariance1DFunction(tausig, lam)

#gpi
gpi = IMP.isd.GaussianProcessInterpolation(q, I.tolist(),
                                err.tolist(), N, mean, cov, sig2)
gpir = IMP.isd.GaussianProcessInterpolationRestraint(m, gpi)
sf = IMP.core.RestraintsScoringFunction([gpir])

print "#a b sigma tau lambda score"
nuisances = [sig2,tausig,lam]
for nuis in nuisances:
    for val in logspace(logmin,logmax,num=ndraws):
        nuis.set_scale(val)
        ene = sf.evaluate(False)
        aval = a.get_nuisance()
        bval = b.get_nuisance()
        sigval = sig2.get_scale()**0.5
        tauval = tausig.get_scale()/sigval
        lamval = lam.get_scale()
        print aval,bval,sigval,tauval,lamval,ene
    nuis.set_scale(1.0)








