#define Buzzer_PORT  PORT_B 
#define Buzzer_PIN   PIN_0 


// inverse kinamatics section

#define A_1      80
#define A_2      137
#define A_3      152 

#define X_offset 0 
#define range_offset (-80) 
#define range_Hight_offset 0
//this offset can be sensed in range lengh when the robot moves down to pick

#define Z_offset 75

#define divisor_degree 50

#define Base_Gear_No_Teeth    108
#define Main_Gear_No_Teeth    47
#define Arm_Gear_No_Teeth     47
#define Stepper_Gear_No_Teeth 19

#define Base_initial_angle  0 
#define Main_initial_angle   (12136 )// 64 *100
#define Arm_initial_angle     (1525 )// 59 *100


// the first move angles ( i'm ready angles)
#define Base_Default_angle   (0) 
#define Main_Default_angle   (128 * 100)// (128 * 100)// 
#define Arm_Default_angle    (40 * 100)//(35  * 100)

#define Garbage_Pos  0xA3
 
#define B_error_factor  1
#define M_error_factor  1
#define A_error_factor  1
// input output section 

#define Base_M_Port_Steps         PORT_A
#define Main_M_Port_Steps         PORT_A
#define Arm_M_Port_Steps          PORT_A

#define Base_M_Pin_Steps          PIN_6
#define Main_M_Pin_Steps          PIN_5
#define Arm_M_Pin_Steps           PIN_7

#define Base_M_Port_Dir           PORT_E
#define Main_M_Port_Dir           PORT_B
#define Arm_M_Port_Dir            PORT_E


#define Base_M_Pin_Dir            PIN_5
#define Main_M_Pin_Dir            PIN_4
#define Arm_M_Pin_Dir             PIN_4

#define Gripper_PORT              PORT_B
#define Gripper_PIN               PIN_1