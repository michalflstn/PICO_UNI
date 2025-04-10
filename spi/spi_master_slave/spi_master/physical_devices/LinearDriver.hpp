#ifndef PICO_EXAMPLES_LINEARDRIVER_HPP
#define PICO_EXAMPLES_LINEARDRIVER_HPP
#include <cstring>
#include "../utilities/base_types/Decoder.hpp"
#include "../loop/common_data/device_variables.hpp"  
#include "../loop/common_data/common_variables.hpp"
// const //move from common_var 250409
#define AxisX                    90 //коды осей
#define AxisY                    95
#define AxisZ                    99


class LinearDriverBase
{
 protected:
  OutputPort *z_a;
  OutputPort *z_b;
 public:
   LinearDriverBase();
   virtual ~LinearDriverBase();
   virtual void move(int command, int freq, int duty, int nsteps, bool dir) const;
};

class LinearDriverBB: public LinearDriverBase
{protected: 
  bool    _flgOnlyZ; 
  ConfigLinearDriveBB _configlineardrive;
  OutputPort *y_a;
  OutputPort *y_b;
  OutputPort *x_a;
  OutputPort *x_b;
 public:
   LinearDriverBB(bool flgOnlyZ,ConfigLinearDriveBB configlineardrive);  
  ~LinearDriverBB();
   void move(int command, int freq, int duty, int nsteps, bool dir) const override;
};

class LinearDriverWB: public LinearDriverBase
{
 private:
    OutputPort *turnon_x;
    OutputPort *turnon_y;
    OutputPort *turnon_z ;
 protected:
  ConfigLinearDriveWB _configlineardrive;
 public:
   LinearDriverWB(ConfigLinearDriveWB configlineardrive);
   ~LinearDriverWB();
   void move(int command, int freq, int duty, int nsteps, bool dir) const override;
};
#endif
