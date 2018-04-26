#ifndef TWIDDLE_H
#define TWIDDLE_H

#define TARGET_DURATION 1000
#define MAX_CTE 5.0

class Twiddle {
public:
  int iter;
  
  double p[3];
  double dp[3];
  bool run_reset;
  
  int p_ind;
  int cond_ind;
  
  int duration;
  double error_sum;
  
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
};
#endif /* PID_H */