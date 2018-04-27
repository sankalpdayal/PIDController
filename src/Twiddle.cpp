#include <iostream>
#include <math.h>
#include "Twiddle.h"

using namespace std;

Twiddle::Twiddle() {}

Twiddle::~Twiddle() {}

void Twiddle::Init(){
	std::cout << "Twiddle init"<<std::endl;
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
	
	best_duration = 0;
	error_sum = 0.0;
	best_avg_error = 0.0;
	
	target_duration = 100;
	static_duration = 0;
	
	debugfile.open("../Debug.txt");
}

void Twiddle::UpdateRunError(double cte, double speed){
	duration++;
	if (speed>0.5 && fabs(cte)< MAX_CTE){
		error_sum += fabs(cte);	
		duration_on_track++;
	}
	if (speed<=0.5){
		static_duration++;
	} 
	else{
		static_duration = 0;
	}
	
	if (duration >= target_duration || static_duration == 100){
		run_reset = true;
	}
}

void Twiddle::ResetRunError(){
	duration = 0;
	error_sum = 0.0;
	duration_on_track = 0;
	static_duration = 0;
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
		if (best_avg_error > error_sum/duration_on_track){
			best_avg_error = error_sum/duration_on_track;
			new_error_is_less = true;
		}
		
	}
	if (duration_on_track >= target_duration-25){
		target_duration *=2;
	}
	if (new_error_is_less)
		this->WriteDebugOutput();
	
	return new_error_is_less;
}

void Twiddle::WriteDebugOutput()
{
	debugfile << p[0] << "," << p[1] << "," << p[2] << "," << iter << "," << p_ind << "," << cond_ind << "," << best_duration << "," << best_avg_error << std::endl;
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