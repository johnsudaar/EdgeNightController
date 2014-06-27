#ifndef LUMAX_H
#define LUMAX_H

typedef struct
 {
  unsigned short Ch1, Ch2, Ch3, Ch4, Ch5, Ch6, Ch7, Ch8, TTL;
 } TLumax_Point;

int Lumax_DongleCom(int PhysicalDevice, int Command, unsigned long Vendor_ID, unsigned long *DataOut, unsigned long *DataIn);
int Lumax_SetTTL(int Handle, int TTL);
int Lumax_StopFrame(int Handle);
int Lumax_SetDmxMode(int Handle, int NumOfTxChannels, int NumOfRxChannels);
int Lumax_SendDmx(int Handle, unsigned char *DmxBuffer, int Length);
int Lumax_ReceiveDmx(int Handle, unsigned char *DmxBuffer, int Length);
int Lumax_CloseDevice(int Handle);
int Lumax_OpenDevice(int PhysicalDevice, int Channel);
int Lumax_GetPhysicalDevices(void);
int Lumax_WaitForBuffer(int Handle, int Timeout, int *TimeToWait, int *BufferChanged);
int Lumax_SendFrame(int Handle, TLumax_Point *Points, int NumOfPoints, int ScanSpeed, int UpdateMode, int *TimeToWait);

#endif // LUMAX_H
