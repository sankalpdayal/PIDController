#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double _Kp, double _Ki, double _Kd) {
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;
	p_error = 0;
	i_error = 0;
	d_error = 0;
	last_cte = 0;
	if_init = false;
	cout<<"Coefficients: " << Kp << ', ' << Ki << ', ' << Kd << endl;
}

void PID::UpdateError(double cte) {
	p_error = cte;
	i_error += cte;
	if (!if_init){
		d_error = 0;
		if_init = true;
	}
	else{
		d_error = cte - last_cte;
	}

	last_cte = cte;
}

double PID::TotalError() {
	return (Kp*p_error + Ki*i_error + Kd*d_error);
}

