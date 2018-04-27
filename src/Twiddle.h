#ifndef TWIDDLE_H
#define TWIDDLE_H

#include "PID.h"

#define MAX_CTE 4.0

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
  int duration_on_track;
  
  int best_duration;
  double error_sum;
  double best_avg_error;  
  
  int target_duration;
  
  ofstream debugfile;
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
  void UpdateRunError(double cte, double speed);
  
  /*
  * Reset error and duration
  */
  void ResetRunError();
  
  void UpdateP();
  
  bool CheckIfNewErrorIsLess();
  
  void WriteDebugOutput();
};
#endif /* TWIDDLE_H */