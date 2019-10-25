#include "Tfile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TNtuple.h"

void historeader(){
    TCanvas *c1 = new TCanvas("c1","Canvas Example",200,10,600,480);
    TCanvas *c2 = new TCanvas("c2","Canvas Example",200,10,600,480);

    TFile *MyFile = new TFile("histograms.root","NEW");

    TNtuple *nt = (TNtuple*)MyFile->Get("velocity");

    
    TH1F *h1 = new TH1F("h1","Drift Velocity",100, 0.002610, 0.002640);
    TH1F *h2 = new TH1F("h2","Fit Gradient",100,0,0.3); 

    nt->Project

    c1->cd();
    h1->Draw();

    c2->cd();
    h2->Draw();

    MyFile->Close();
}
