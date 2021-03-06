#ifndef TWIDDLE_H
#define TWIDDLE_H

#include "PID.h"

#define MAX_CTE 4.0
#include <fstream>
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
  int target_duration;
  int duration_on_track;
  int static_duration;
  int best_duration;
  
  double error_sum;
  double best_avg_error;  
  
  double speed_sum;
  double best_avg_speed;
  
  
  std::ofstream debugfile;
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
  
  bool ReadParameters();
};
#endif /* TWIDDLE_H */