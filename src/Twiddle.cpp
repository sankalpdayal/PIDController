#include <iostream>
#include "Twiddle.h"

using namespace std;

Twiddle::Twiddle() {}

Twiddle::~Twiddle() {}

void Twiddle::Init(){
	cout << "Twiddle init"<<endl;
	iter = 0;
}