#include "TFile.h"
#include "TNtuple.h"
#include "TH1F.h"
#include "TF1.h"
#include <chrono>

#include "helperfunctions.cpp"

/*
Main function for running the trackfinding algorithm. A menu based user interface is provided in order to allow the user to differentiate between
the one track and many track data files. 

Other includes are part of the other header files apart from chrono which is used for timing

The ROOT header files for TFile, TH1F, TF1 and TNtuple are also included for data storage and fitting
*/

class range_error{};
class value_error{};

//Error classes used to prevent erroneous user inpt

int main(){
    fitting_parameters fp;
    //fitting parameters defined in helperfunctions.cpp
    char menu_option = ' ';
    std::string filename = "";
    int event_start = 0;
    int total_event_no = 1;
    int buffer_size = 1;
    //Declared variables, these are changed in the menu options
    std::cout << "Enter 'o' for one track analysis and printing \n" << "Enter 'm' for many track analysis, histogram fitting and writing to an NTuple \n" <<
                      "Enter 's' for single track analysis and printing \n" <<  "Enter 'q' to quit: ";

    while(std::cin >> menu_option)  //While function is used to allow continual user input until asked to quit
    {
        if (menu_option == 'q')
            break;
        // Quit option, breaks loop
        if (menu_option == 'o')
        {
            filename = "onetrack.raw"; 
            std::vector<event> event_buffer = reading(filename,0,1);
            //Reading function defined in helperfunctions.cpp

            event E;
            E = track_finder(event_buffer[0],fp);
            //Declare the single event and populate it with the 1 event buffer from the reading

            if (E.check_fit_status())
            {
                E.print();
                E.quick_print();
                //If a good fit is found print results, defined in event.cpp
            }
        std::cout << "===========================" << std::endl;
        }

        if (menu_option == 'm')
        {
            filename = "manytracks.raw";
            event_start = 0;
            total_event_no = 1000000;
            buffer_size = 100000;
            double mean_velocity = 0;
            double velocity_weight = 0;
            double mean_angle = 0;
            double angle_weight = 0;
            double std_velocity = 0;
            double std_angle = 0;

            //Full dataset used with 1E6 events, a buffer is used to decrease memory usage, this buffer_size can be adapted for specific cases, 1E5 was found to give
            //the best performance
            
            auto start = std::chrono::high_resolution_clock::now();
            //timing function

            TFile *f = new TFile("tracks.root","recreate","Drift Velocity and Track Angle Histograms");
            TNtuple *ntuple = new TNtuple("ntuple","Drift Velocity and Track Angle Data","velocity:v_e:fit:f_e");
            TH1F *velocityh = new TH1F("vh","Drift Velocity Histogram",100,26.1,26.5);
            TH1F *angleh = new TH1F("ah","Track Angle Histogram",100,0,20);

            //ROOT file and Ntuple declared, Ntuple used to store data for further analysis
            
            for (int k=event_start; k<event_start+total_event_no/buffer_size; ++k)
            {
            //Buffering loop, iterates through buffer_size sized chunks of the total file size 
                std::vector<event> event_buffer = reading(filename,k,buffer_size); //fill vector of events from buffer

                for (event E:event_buffer) //loop through events in the vector of events in the buffer
                {     
                    E = track_finder(E,fp);
                    if (E.check_fit_status())
                        {
                            double atan = std::atan(E.get_fit_gradient());
                            double m = square(1/(1+square(E.get_velocity()))) * square(E.get_fit_error()) * square(atan);
                            // calculates the error in the fit gradient as an error on the track angle
                            ntuple->Fill(E.get_velocity(),E.get_velocity_error()/E.get_velocity(),atan,std::sqrt(m));
                            velocityh->Fill(E.get_velocity()*1e4,(E.get_velocity_error()/E.get_velocity()));
                            angleh->Fill(atan*(180/M_PI),atan/(std::sqrt(m)));
                            // fill the datafile with velocity, velocity error, track angle and track angle error
                            // fill histograms
                        }
                    else
                        continue;
                }    
            }
            
            velocityh->Fit("gaus","Quiet");
            angleh->Fit("gaus","Quiet");
            TF1 *vfit = velocityh->GetFunction("gaus");
            TF1 *afit = angleh->GetFunction("gaus");
            std::cout << "============ Fit Parameters ===============" << std::endl;
            std::cout << "Mean Velocity: " << vfit->GetParameter(1) << " +/- " << vfit->GetParError(1) << " microns/ns " << std::endl;
            std::cout << "Velocity Sigma: " << vfit->GetParameter(2) << " +/- " << vfit->GetParError(2) << " microns/ns " << std::endl;
            std::cout << "Mean Angle: " << afit->GetParameter(1) << " +/- " << afit->GetParError(1) << " degrees " << std::endl;
            std::cout << "Angle Sigma: " << afit->GetParameter(2) << " +/- " << afit->GetParError(2) << " degrees " << std::endl;
            // Fitting histograms to a gaussian and extracting parameters of the fit. These histograms are also printed to the ROOT file

            f->Write();
            f->Close();

            std::cout << "\nData written to tracks.root" << std::endl;

            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;
            std::cout << "===========================" << std::endl;
        }


        if (menu_option == 's')
        {
            int event_number = 1;
            std::cout << "Enter which event number, int between 0 and 1000000: ";

            //Menu option for checking single event in the 1E6 total events
            
            try
            {
                std::cin >> event_number;
                // Read in a single event number
                if (std::cin.fail()) throw value_error{}; 
                // If user input is not an int throw an error
                if (event_number < 0 || event_number > 1000000) throw range_error{};
                // If user input is out of range, throw an error


                filename = "manytracks.raw";
                event_start = event_number;
                total_event_no = 1;
                buffer_size = 1;
            
                for (int k=event_start; k<event_start+total_event_no/buffer_size; ++k){
                    std::vector<event> event_buffer = reading(filename,k,buffer_size);
                    for (event E:event_buffer)
                    {     
                        E = track_finder(E,fp);
                        
                        if (E.check_fit_status())
                            E.print();
                            E.quick_print();
                    }    
                }

                std::cout << "===========================" << std::endl;

            }
            catch (range_error)
            {
                std::cout << "Out of Range " << std::endl;
                break;
            }
            catch (value_error)
            {
                std::cout << "Not an Integer " << std::endl;
                break;
            }
            // Errors are caught if thrown and the program ends without user errors causing unknown problems 
        }

    std::cout << "Enter 'o' for one track \n" << "Enter 'm' for many tracks \n" <<
                      "Enter 's' for single track \n" <<  "Enter 'q' to quit: ";

    }
    return 0;
}



