#include <iostream>
#include "Twiddle.h"
#include <math.h>

using namespace std;

Twiddle::Twiddle() {}

Twiddle::~Twiddle() {}

void Twiddle::Init(){
	cout << "Twiddle init"<<endl;
	iter = 0;
	for (int i=0; i<3 ;i++){
		p[i] = 0;
		dp[i] = 0;
	}
	run_reset = false;
	p_ind = 0;
	cond_ind = 0;
	duration = 0;
	error_sum = 0.0;
}

void Twiddle::UpdateRunError(double cte){
	duration++;
	error_sum += cte;
	if (fabs(cte) > MAX_CTE || duration == TARGET_DURATION)
		run_reset = true;
}

void Twiddle::ResetRunError(){
	duration = 0;
	error_sum = 0.0;
}