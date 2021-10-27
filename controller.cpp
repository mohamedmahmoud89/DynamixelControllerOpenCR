#include "controller.hpp"

void Controller::Init(){
  const char *log = NULL;
  bool result = false;

  speed_factor=1;
  
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

void Controller::SendData(){
  const char* log=NULL;
  bool result=false;
  uint8_t ids[2]={1,2};
  int32_t data_read[2]={0,0};

  result = dxl.syncRead(0,ids,2);

  result=dxl.getSyncReadData(0,data_read,&log);

  Serial.println("l="+String(data_read[0]));
  Serial.println("r="+String(data_read[1]));
}

Controller::Command Controller::InterpretCmd(const String cmd){
  if(cmd=="fwd")return FWD; 
  if(cmd=="stop")return STOP; 
  if(cmd=="bwd")return BWD; 
  if(cmd=="left")return LEFT; 
  if(cmd=="right")return RIGHT;
  if(cmd=="left_ctrl")return LEFT_CTRL; 
  if(cmd=="right_ctrl")return RIGHT_CTRL;  
  if(cmd=="up")return UP; 
  if(cmd=="down")return DOWN; 
  if(cmd=="ping")return PING;
  if(cmd=="query")return QUERY;
  return NONE;
; 
}

void Controller::ExecCmd(const Command cmd){
  const char* log=NULL;
  bool result=false;
  uint8_t ids[2]={1,2};
  int32_t fwd_data[2]={speedIncrementStraight,speedIncrementStraight};
  int32_t rev_data[2]={-speedIncrementStraight,-speedIncrementStraight};
  int32_t turn_right_data[2]={speedIncrementTurn,-speedIncrementTurn};
  int32_t turn_left_data[2]={-speedIncrementTurn,speedIncrementTurn};
  int32_t turn_right_ctrl_data[2]={speedIncrementTurnActive,-speedIncrementTurnActive};
  int32_t turn_left_ctrl_data[2]={-speedIncrementTurnActive,speedIncrementTurnActive};
  int32_t org_speed[2]={current_speed[0]/speed_factor,current_speed[1]/speed_factor};
  
  switch(cmd){
      case FWD:
          current_speed[0]=speed_factor*fwd_data[0];
          current_speed[1]=speed_factor*fwd_data[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case STOP:
          speed_factor=1;
          current_speed[0]=0;
          current_speed[1]=0;
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case BWD:
          current_speed[0]=speed_factor*rev_data[0];
          current_speed[1]=speed_factor*rev_data[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case LEFT:
          current_speed[0]=speed_factor*turn_left_data[0];
          current_speed[1]=speed_factor*turn_left_data[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case RIGHT:
          current_speed[0]=speed_factor*turn_right_data[0];
          current_speed[1]=speed_factor*turn_right_data[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case LEFT_CTRL:
          current_speed[0]=speed_factor*turn_left_ctrl_data[0];
          current_speed[1]=speed_factor*turn_left_ctrl_data[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case RIGHT_CTRL:
          current_speed[0]=speed_factor*turn_right_ctrl_data[0];
          current_speed[1]=speed_factor*turn_right_ctrl_data[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      /*case UP:
          if((current_speed[0])&&(current_speed[0])&&((speed_factor+1)*speedIncrement<=maxSpeed)) speed_factor++;
          current_speed[0]=speed_factor*org_speed[0];
          current_speed[1]=speed_factor*org_speed[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;
      case DOWN:
          if((current_speed[0])&&(current_speed[0])&&(speed_factor>1)) speed_factor--;
          current_speed[0]=speed_factor*org_speed[0];
          current_speed[1]=speed_factor*org_speed[1];
          result=dxl.syncWrite(0, ids, 2, current_speed, 1, &log);
          break;*/
      case QUERY:
          SendData();
          break;
      default:
          break;  
  }
}

String Controller::RecieveData(){
  String cmd;
  while(Serial.available()>0){
    char data=char(Serial.read());
    if(data=='#')
      break;
    cmd+=data;
  }
  return cmd;
}

void Controller::Update(){
  if(Serial.available()>0){
    String data=RecieveData();
    Command cmd=InterpretCmd(data);
    
    if(cmd!=NONE){
      ExecCmd(cmd); 
      Serial.println("over");
    }
  }
}
