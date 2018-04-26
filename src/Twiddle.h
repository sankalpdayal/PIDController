#ifndef TWIDDLE_H
#define TWIDDLE_H

#include "PID.h"

#define TARGET_DURATION 10000
#define MAX_CTE 5.0

class Twiddle {
public:
  
  PID pid;
  int iter;
  
  double p[3];
  double dp[3];
  bool run_reset;
  bool twiddle_init;
  
  int p_ind;
  int cond_ind;
  
  int duration;
  int best_duration;
  double error_sum;
  double best_avg_error;  
  /*
  * Constructor
  */
  Twiddle();

  /*
  * Destructor.
  */
  virtual ~Twiddle();
  
  /*
  * Initialize Twiddle.
  */
  void Init();
  
   /*
  * Update error and duration
  */
  void UpdateRunError(double cte);
  
  /*
  * Reset error and duration
  */
  void ResetRunError();
  
  void UpdateP();
};
#endif /* TWIDDLE_H */