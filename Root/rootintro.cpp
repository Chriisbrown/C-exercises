#include "TFile.h"
#include "TH1F.h"
#include <cmath>
int main(int argc, char *argv[])
{
TFile *f = new TFile("batch.root", "recreate", "Batch example");
TH1F *h1 = new TH1F("h1", "Steps", 10, 0., 10.);
TH1F *h2 = new TH1F("h2", "Pyramid", 100, -50., 50.);
for (int i=0; i<10; i++)
{
h1->Fill(i+0.5, 2*(i+1));
}
for (int i=-50; i<50; i++)
{
h2->Fill(i+0.5, 50-std::fabs(i));
}
f->Write();
f->Close();
return 0;
}