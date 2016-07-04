#ifndef __IMU_H__
#define __IMU_H__



#include <stdint.h>


#ifndef DEBUG_ON_PC
#include "contiki.h"
PROCESS_NAME(imu_process);
#endif


#define VELOCITY_BUF_SIZE 2048
#define Accelerometer_Scale 2048.0f
#define Gyroscope_Scale 16.4f
#define Pi 3.14159f
#define G 9.81f


#define PREPROCESS_BUF_SIZE 64

//����ֵ�������ٸ������㱣���ȶ�����Ϊ�����ȶ�̬���˳��ȶ�̬
#define PREPROCESS_STATE_CHANGE_CONDITION_LENGTH 16


typedef struct {
	float Vx;
	float Vy;
	float Vz;
} IMU_Velocity_Frame;

typedef struct {
	float Ax;
	float Ay;
	float Az;
	float Gx;
	float Gy;
	float Gz;
	float StationaryChannelCounter;
} IMU_Preprocess_Frame;

typedef struct {
	float Ax;
	float Ay;
	float Az;
	float Gx;
	float Gy;
	float Gz;
	uint32_t PointCount;
}InitialCalibrationAverageCalculator;




typedef struct
{

#define IMU_RAW_DATA_BUFFER_LENGTH 20

	float Raw_Buffer[IMU_RAW_DATA_BUFFER_LENGTH * 6];
	int8_t Raw_Buffer_Head;
	int8_t Raw_Buffer_Tail;






	//�����ж��Ƿ�Ϊ��ֹ״̬���˲���״̬����
	// y = filter( x )
	char LPF_X1;

	//�����жϵ�ǰ״̬�ĸ���ͨ����ֵ Xmin,Xmax,Ymin,Ymax,Zmin,Zmax
	float ChannelThresHold_G[6];
	float ChannelThresHold_A[6];


	uint8_t IMU_New_Data_To_Be_Process;
	uint8_t In_Static_State;


	//�������λ��
	float PosX, PosY, PosZ;


	IMU_Preprocess_Frame PreprocessBuf[PREPROCESS_BUF_SIZE];

	IMU_Velocity_Frame VelocityBuf[VELOCITY_BUF_SIZE];

	uint16_t PreprocessBuf_Pointer;
	uint16_t VelocityBuf_Pointer;



	float q[4];
	float Int_Error[3];
	float Kp, Ki;
	float SamplePeriod;

	InitialCalibrationAverageCalculator  InitCaliAverage;
	
	//���ڼ�СZ����Ư�ƣ�������β�̬֮��Z����仯С����ֵ������ΪZ����û���ƶ���ǿ��������Z��ͬ.�����п���Ӱ�쵽����¥�ݼ��
	float Z_threadhold;
	float Z_old_val;

}IMU_Handler;


void resetIMU(IMU_Handler* hIMU, float SamplePeriod);
void updateIMU(IMU_Handler* hIMU, float* Gyroscope, float* Accelerometer);
void IMU_new_data(IMU_Handler* hIMU, float* Gyroscope, float* Accelerometer);
#ifndef DEBUG_ON_PC
void IMU_Process(IMU_Handler* hIMU);
#endif

void ResetInitialCalibration(IMU_Handler* hIMU);
void DoInitialAverageValueCalculate(IMU_Handler* hIMU, float* Gyroscope, float* Accelerometer);
void SetThreshold(IMU_Handler* hIMU);
void RotateIMUCoordinateByAccAndGyr(IMU_Handler* hIMU, float* Gyroscope, float* Accelerometer);




#endif
