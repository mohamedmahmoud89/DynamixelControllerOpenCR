#include <DynamixelWorkbench.h> 

class Controller{
  DynamixelWorkbench dxl;
  static const int maxSpeed{500};
  static const int baudRate{1000000};
  static const int speedIncrement{50};
  unsigned int speed_factor{1};
  int32_t current_speed[2]{0,0};
  
public:
  void Init();
  void Update();
  void Update(const char cmd);
};
