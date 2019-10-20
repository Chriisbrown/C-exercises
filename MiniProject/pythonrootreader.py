import re, array, os
import ROOT
from ROOT import TFile, TTree, gROOT, AddressOf
from ROOT import TCanvas, TProfile, TH1F


gROOT.ProcessLine(
    " struct FitParameters_t{\
        Float_t   Drift_Velocity;\
        Float_t   Fit_Gradient;\
};");

def staff():
    
    FitParam = ROOT.FitParameters_t()
    f = TFile("FitParam.root", "RECREATE")
    tree = TTree('T', "Fitparameters from main program")
    tree.Branch("FitParam", FitParam,
                'Drift_Velocity:Fit_Gradient')

    fname = "test.txt"

    hdv = TH1F("hdv", "Drift Velocity Distribution", 100, 0.002615, 0.002645)
    hfg = TH1F("hfg", "Track Angle Distribution", 100, 0, 0.3)
    hprof = TProfile("hprof", "Profile of Drift Velocity vs Track Angle", 100, 0.002615,0.002645,0,0.3)

    histos = [ 'hdv', 'hfg', 'hprof']
    for name in histos:
        exec('%sFill = %s.Fill' % (name,name))

    for line in open(fname).readlines():
        t = list(filter( lambda x: x, re.split( ',', line )))
        FitParam.Drift_Velocity = float(t[0])
        FitParam.Fit_Gradient = float(t[1])
        hdv.Fill(float(t[0]))
        hfg.Fill(float(t[1]))
        hprof.Fill(float(t[0]),float(t[1]))

        tree.Fill()


    c1 = TCanvas("c1", "Drift Velocity Histogram", 200, 10, 700, 500)
    c1.SetGridx()
    c1.SetGridy()
    hdv.Print()
    hdv.Draw()

    c1.Modified()
    c1.Update()


    c2 = TCanvas("c2", "Angular Distribution Histogram", 200, 10, 700, 500)
    c2.SetGrid()
    hfg.Print()
    hfg.Draw()
    c2.Modified()
    c2.Update()

    c3 = TCanvas("c3", "Profile", 200, 10, 700, 500)
    hprof.Print()
    hprof.Draw()
    c3.Modified()
    c3.Update()

    tree.Print()
    tree.Write()

    for name in histos:
        exec('del %sFill' % name)
    del histos

    

    x = input("Enter any key to continue")





staff()








