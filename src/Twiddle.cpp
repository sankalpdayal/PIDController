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
	
	duration_with_speed = 0;
	
	best_duration = 0;
	error_sum = 0.0;
	best_avg_error = 0.0;
	
	target_duration = 100;
}

void Twiddle::UpdateRunError(double cte, double speed){
	duration++;
	if (speed>0.5){
		error_sum += cte;	
		duration_with_speed++;
	}
	if (duration >= target_duration){
		run_reset = true;
	}
}

void Twiddle::ResetRunError(){
	duration = 0;
	duration_with_speed = 0;
	error_sum = 0.0;
}

bool Twiddle::CheckIfNewErrorIsLess(){
	bool new_error_is_less = false;
	if (error_sum/duration_with_speed < best_avg_error){
		best_avg_error = error_sum/duration_with_speed;
		new_error_is_less = true;
		if (duration_with_speed >= target_duration){
			target_duration *=2;
		}
	}
	return new_error_is_less;
}
void Twiddle::UpdateP(){
	if (!twiddle_init){
		twiddle_init = true;
		best_duration = duration_with_speed;
		best_avg_error = error_sum/duration_with_speed;
		p_ind = 0;
		p[p_ind] += dp[p_ind];
		cond_ind = 0;
	}
	else{
		std::cout << "P vals:" << p[0] <<"," << p[1] <<"," << p[2] <<std::endl;
		if (cond_ind == 0){
			if (this->CheckIfNewErrorIsLess()){
				dp[p_ind] *= 1.1;
				p_ind +=1;
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
			p_ind +=1;
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