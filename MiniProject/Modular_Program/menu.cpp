#include "TFile.h"
#include "TNtuple.h"
#include <chrono>
#include "helperfunctions.cpp"

class range_error{};
class value_error{};

int main(){

    fitting_parameters fp;
    
    char g = ' ';
    std::string filename = "";
    int event_start = 0;
    int total_event_no = 1;
    int buffer_size = 1;

    std::cout << "Enter 'o' for one track analysis and printing \n" << "Enter 'm' for many track analysis and printing to a root NTuple \n" <<
                      "Enter 's' for single track analysis and printing \n" <<  "Enter 'q' to quit: ";

    while(std::cin >> g)
    {
        if (g == 'q')
            break;

        if (g == 'o')
        {
            filename = "onetrack.raw"; 
            std::vector<event> event_buffer = reading(filename,0,1);
            event E;
            E = calculation(event_buffer[0],fp);
            if (E.check_fit_status())
            {
                E.print();
                E.quick_print();
            }
        std::cout << "===========================\n";
        }

        if (g == 'm')
        {
            filename = "manytracks.raw";
            event_start = 0;
            total_event_no = 1000000;
            buffer_size = 100000;
            
            auto start = std::chrono::high_resolution_clock::now();

            TFile *f = new TFile("tracks.root","recreate","Drift Velocity and Track Angle Histograms");
            TNtuple *ntuple = new TNtuple("ntuple","Drift Velocity and Track Angle Histograms","velocity:v_e:fit:f_e");
            
            

            for (int k=event_start; k<event_start+total_event_no/buffer_size; ++k){
                std::vector<event> event_buffer = reading(filename,k,buffer_size);
                for (event E:event_buffer)
                {     
                    E = calculation(E,fp);
                    if (E.check_fit_status())
                        {
                            double m = square(1/(1+square(E.get_velocity()))) * square(E.get_fit_error()) * square(std::atan(E.get_fit_gradient()));
                            ntuple->Fill(E.get_velocity(),E.get_velocity_error()/E.get_velocity(),std::atan(E.get_fit_gradient()),std::sqrt(m));
                        }
                    else
                        continue;
                }    
            }

            f->Write();
            f->Close();

            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            std::cout << "Elapsed time: " << elapsed.count() << " s\n";
            std::cout << "===========================\n";
        }


        if (g == 's')
        {
            int I = 1;
            std::cout << "Enter which event number, int between 0 and 1000000: ";
            
            try{
                std::cin >> I;
                if (std::cin.fail()) throw value_error{}; 
                if (I < 0 || I > 1000000) throw range_error{};


                filename = "manytracks.raw";
                event_start = I;
                total_event_no = 1;
                buffer_size = 1;
            
                for (int k=event_start; k<event_start+total_event_no/buffer_size; ++k){
                    std::vector<event> event_buffer = reading(filename,k,buffer_size);
                    for (event E:event_buffer)
                    {     
                        E = calculation(E,fp);
                        
                        if (E.check_fit_status())
                            E.print();
                            E.quick_print();
                    }    
                }

                std::cout << "===========================\n";

            }
            catch (range_error){
                std::cout << "Out of Range \n";
            }
            catch (value_error){
                std::cout << "Not an Integer \n";
            }

            

            
        }

    std::cout << "Enter 'o' for one track analysis and printing '\n'" << "Enter 'm' for many track analysis and printing to root histograms '\n'" <<
                 "Enter 's' for single track analysis and printing '\n'" <<  "Enter 'q' to quit: ";



    }
    return 0;
}



