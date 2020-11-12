#include "controller.hpp"

void Controller::Init(){
  const char *log = NULL;
  bool result = false;

  result = dxl.init("", baudRate, &log);
  
  result=dxl.ping(1,&log);
  result=dxl.ping(2,&log);

  result=dxl.setVelocityControlMode(1,&log);
  result=dxl.setVelocityControlMode(2,&log);

  String write_str("Goal_Velocity");
  result = dxl.addSyncWriteHandler(1, write_str.c_str(), &log);

  String read_str="Present_Velocity";
  result = dxl.addSyncReadHandler(1, read_str.c_str(), &log);

  result = dxl.torqueOn(1, &log);
  result = dxl.torqueOn(2, &log);
}

void Controller::Update(){
  const char* log=NULL;
  bool result=false;
  uint8_t ids[2]={1,2};
  int32_t data_read[2]={0,0};

  result = dxl.syncRead(0,ids,2);

  result=dxl.getSyncReadData(0,data_read,&log);

  Serial.println("l="+String(data_read[0]));
  Serial.println("r="+String(data_read[1]));
}

void Controller::Update(const char cmd){
  const char* log=NULL;
  bool result=false;
  uint8_t ids[2]={1,2};
  int32_t fwd_data[2]={maxSpeed,maxSpeed};
  int32_t rev_data[2]={-maxSpeed,-maxSpeed};
  int32_t turn_right_data[2]={maxSpeed,0};
  int32_t turn_left_data[2]={0,maxSpeed};
  int32_t org_speed[2]={current_speed[0]/speed_factor,current_speed[1]/speed_factor};
  
  switch(cmd){
      case 'f':
          current_speed[0]=speed_factor*fwd_data[0];
          current_speed[1]=speed_factor*fwd_data[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case 's':
          speed_factor=0;
          current_speed[0]=0;
          current_speed[1]=0;
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case 'b':
          current_speed[0]=speed_factor*rev_data[0];
          current_speed[1]=speed_factor*rev_data[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case 'r':
          if((speed_factor+1)*speedIncrement<=maxSpeed) speed_factor++;
          current_speed[0]=speed_factor*turn_right_data[0];
          current_speed[1]=speed_factor*turn_right_data[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case 'l':
          if(speed_factor>1) speed_factor--;
          current_speed[0]=speed_factor*turn_left_data[0];
          current_speed[1]=speed_factor*turn_left_data[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      default:
          break;  
  }

  Update();
}
