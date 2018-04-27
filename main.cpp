#include <iostream>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

int main(){
{
    string filename = "Debug.txt";
    ifstream fin;
    fin.open(filename);
	bool filefound = false;
    if(fin.is_open()) {
		filefound = true;
        fin.seekg(-1,ios_base::end);                // go to one spot before the EOF

        bool keepLooping = true;
        while(keepLooping) {
            char ch;
            fin.get(ch);                            // Get current byte's data

            if((int)fin.tellg() <= 1) {             // If the data was at or before the 0th byte
                fin.seekg(0);                       // The first line is the last line
                keepLooping = false;                // So stop there
            }
            else if(ch == '\n') {                   // If the data was a newline
                keepLooping = false;                // Stop at the current position.
            }
            else {                                  // If the data was neither a newline nor at the 0 byte
                fin.seekg(-2,ios_base::cur);        // Move to the front of that data, then to the front of the data before it
            }
        }

        std::string s;            
        getline(fin,s);                      // Read the current line

        fin.close();
		
		size_t pos = 0;
		std::string token;
		double temp[8];
		int ind = 0;
		std::string delimiter = ",";
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			temp[ind] = atof(token.c_str());
			ind++;
			s.erase(0, pos + delimiter.length());
		}
		double p[3];
		p[0] = temp[0];
		p[1] = temp[1];
		p[2] = temp[2];
		int iter = int(temp[3]);
		int p_ind = int(temp[4]);
		int cond_ind = int(temp[5]);
		int best_duration = int(temp[6]);
		double best_avg_error = temp[7];
		cout << p[0] << "," << p[1] << "," << p[2] << "," << iter << "," << p_ind << "," << cond_ind << "," << best_duration << "," << best_avg_error << std::endl;

    }

    return 0;
}