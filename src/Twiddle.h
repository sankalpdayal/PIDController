#ifndef TWIDDLE_H
#define TWIDDLE_H
class Twiddle {
public:
  int iter;
  /*
  * Constructor
  */
  Twiddle();

  /*
  * Destructor.
  */
  virtual ~Twiddle();
  
  /*
  * Initialize PID.
  */
  void Init();
};
#endif /* PID_H */