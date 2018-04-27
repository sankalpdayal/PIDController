#include <iostream>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include "Twiddle.h"

using namespace std;

Twiddle::Twiddle() {}

Twiddle::~Twiddle() {}

void Twiddle::Init(){
	std::cout << "Twiddle init"<<std::endl;
	if (this->ReadParameters())
		return;
	iter = 0;
	for (int i=0; i<3 ;i++){
		p[i] = 0.0;
		dp[i] = 1.0;
	}
	run_reset = false;
	twiddle_init = false;
	p_ind = 0;
	cond_ind = 0;
	
	duration = 0;
	duration_on_track = 0;
	target_duration = 100;
	static_duration = 0;
	best_duration = 0;
	
	error_sum = 0.0;
	best_avg_error = 0.0;
	
	speed_sum = 0.0;
	best_avg_speed = 0.0;
	
	
	debugfile.open("../Debug.txt");
}

void Twiddle::UpdateRunError(double cte, double speed){
	duration++;
	if (speed>0.5 && fabs(cte)< MAX_CTE){
		error_sum += fabs(cte);
		speed_sum += fabs(speed);
		duration_on_track++;
	}
	if (speed<=0.5){
		static_duration++;
	} 
	else{
		static_duration = 0;
	}
	
	if (duration >= target_duration || (duration > 1000 && static_duration >= 100)){
		run_reset = true;
	}
}

void Twiddle::ResetRunError(){
	duration = 0;
	duration_on_track = 0;
	static_duration = 0;
	
	error_sum = 0.0;
	
	speed_sum = 0.0;
}

bool Twiddle::CheckIfNewErrorIsLess(){
	bool new_error_is_less = false;
	if (duration_on_track > best_duration)
	{
		best_duration = duration_on_track;
		best_avg_error = error_sum/duration_on_track;
		new_error_is_less = true;
	}
	if (duration_on_track == best_duration)
	{
		double avg_error_current_run = error_sum/duration_on_track;
		if (avg_error_current_run < 0.15)
		{
			double avg_speed_current_run = speed_sum/duration_on_track;
			if (avg_speed_current_run > best_avg_speed){
				best_avg_speed = avg_speed_current_run;
				new_error_is_less = true;
			}
		}
		else{
			if (best_avg_error > avg_error_current_run){
				best_avg_error = avg_error_current_run;
				new_error_is_less = true;
			}
		}
		
	}
	if (duration_on_track >= target_duration-25){
		target_duration *=2;
		if (target_duration > 6400)
		{
			target_duration = 6400;
		}
	}
	if (new_error_is_less)
		this->WriteDebugOutput();
	
	return new_error_is_less;
}

void Twiddle::WriteDebugOutput()
{
	debugfile << p[0] << "," << p[1] << "," << p[2] << "," << dp[0] << "," << dp[1] << "," << dp[2] << "," << iter << "," << p_ind << "," << cond_ind << "," << best_duration << "," << best_avg_error << "," << best_avg_speed << "," << target_duration << std::endl;
}

void Twiddle::UpdateP(){
	if (!twiddle_init){
		twiddle_init = true;
		best_duration = duration_on_track;
		best_avg_error = error_sum/duration_on_track;
		this->WriteDebugOutput();
		p_ind = 0;
		p[p_ind] += dp[p_ind];
		cond_ind = 0;
	}
	else{
		std::cout << "P vals:" << p[0] <<"," << p[1] <<"," << p[2] <<std::endl;
		if (cond_ind == 0){
			if (this->CheckIfNewErrorIsLess()){
				dp[p_ind] *= 1.1;
				p_ind++;
				if (p_ind==3){
					p_ind = 0;
					iter++;
				}
				p[p_ind] += dp[p_ind];
				cond_ind = 0;
			}
			else{
				p[p_ind] -= 2*dp[p_ind];
				cond_ind = 1;
			}
		}
		if (cond_ind == 1){
			if (this->CheckIfNewErrorIsLess()){
				dp[p_ind] *= 1.1;
			}
			else{
				p[p_ind] += dp[p_ind];
				dp[p_ind] *= 0.9;
			}
			p_ind++;
			if (p_ind==3){
				p_ind = 0;
				iter++;
			}
			p[p_ind] += dp[p_ind];
			cond_ind = 0;
		}
	}
	this->ResetRunError();
}

bool Twiddle::ReadParameters()
{
    ifstream fin;
    fin.open("../Debug.txt");
	bool filefound = false;
    if(fin.is_open()) {
		filefound = true;
        std::string s, lastline;
		while (std::getline(fin, s))
		{
			if (s.length() > 5)
			{
				lastline = s;
			}
			// process pair (a,b)
		}

        fin.close();
		size_t pos = 0;
		std::string token;
		double temp[13];
		int ind = 0;
		std::string delimiter = ",";
		lastline += ",";
		while ((pos = lastline.find(delimiter)) != std::string::npos) {
			token = lastline.substr(0, pos);
			temp[ind] = atof(token.c_str());
			ind++;
			lastline.erase(0, pos + delimiter.length());
		}
		p[0] = temp[0];
		p[1] = temp[1];
		p[2] = temp[2];
		dp[0] = temp[3];
		dp[1] = temp[4];
		dp[2] = temp[5];
		iter = int(temp[6]);
		p_ind = int(temp[7]);
		cond_ind = int(temp[8]);
		best_duration = int(temp[9]);
		best_avg_error = temp[10];
		best_avg_speed = temp[11];
		target_duration = int(temp[12]);
		
		run_reset = false;
		twiddle_init = true;
		
		duration = 0;
		duration_on_track = 0;
		error_sum = 0.0;
		static_duration = 0;

		
		debugfile.open("../Debug.txt",std::fstream::app);
    }

    return filefound;
}