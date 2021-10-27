#include <DynamixelWorkbench.h> 

class Controller{

  enum Command{
    NONE=0,
    FWD,
    BWD,
    STOP,
    LEFT,
    RIGHT,
    LEFT_CTRL,
    RIGHT_CTRL,
    UP,
    DOWN,
    PING,
    QUERY
  };
    
  DynamixelWorkbench dxl;
  static const int maxSpeed{500};
  static const int baudRate{1000000};
  static const int speedIncrementStraight{50};
  static const int speedIncrementTurn{30};
  static const int speedIncrementTurnActive{20};
  unsigned int speed_factor{1};
  int32_t current_speed[2]{0,0};
  
  void SendData();
  void ExecCmd(const Command rx_data);
  String RecieveData();
  Command InterpretCmd(const String cmd);
   
public:
  void Init();
  void Update();
};
