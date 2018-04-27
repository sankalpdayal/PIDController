#include <iostream>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    ifstream fin;
    fin.open("Debug.txt");
	bool filefound = false;
    if(fin.is_open()) {
		filefound = true;
        std::string s;
		while (std::getline(fin, s))
		{
			std::istringstream iss(s);
			// process pair (a,b)
		}

        fin.close();
		cout<<s<<endl;
		size_t pos = 0;
		std::string token;
		double temp[8];
		int ind = 0;
		std::string delimiter = ",";
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			cout<<token<<endl;
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