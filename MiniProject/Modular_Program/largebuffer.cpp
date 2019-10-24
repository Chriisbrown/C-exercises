#include "TFile.h"
#include "TH1F.h"
#include "helperfunctions.cpp"

int main(){

    std::string filename = "manytracks.raw";
    int event_start = 0;
    int total_event_no = 1000000;
    int buffer_size = 10000;
    
    auto start = std::chrono::high_resolution_clock::now();

    TFile *f = new TFile("miniproject.root","recreate","Mini Project File");
    
    TH1F *h1 = new TH1F("h1","Drift Velocity",100, 0.002615, 0.002645);
    TH1F *h2 = new TH1F("h2","Fit Gradient",100,0,0.3); 

    double fit_error = 0.0;

    for (int k=event_start; k<event_start+total_event_no/buffer_size; ++k){
        std::vector<event> event_buffer = reading(filename,k,buffer_size);
        for (event E:event_buffer)
        {     
            E = calculation(E);
            if (E.good_fit)
                {
                    h1->Fill(E.drift_velocity,100*E.drift_error/E.drift_velocity);
                    h2->Fill(std::atan(E.fit_gradient),100*(E.fit_error / (1+E.fit_gradient*E.fit_gradient))/( std::atan(E.fit_gradient)));
                }
        }    
    }

    f->Write();
    f->Close();

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    return 0;
}


