#ifndef CONST_H
#define CONST_H

// ************************
// * HARDWARE LIMITATIONS *
// ************************

#define MAX_X 65535
#define MAX_Y 65535
#define MIN_FPS 3

// ************************
// *     NETWORKING       *
// ************************

#define LISTEN_PORT 4242
#define INSTR_IN_HELLO 0x48
#define INSTR_IN_STOP 0x53

#define INSTR_IN_REFRESH 0x52
#define INSTR_IN_LINE 0x4C
#define INSTR_IN_RECT 0x44
#define INSTR_IN_CIRCLE 0x43


#define INSTR_OUT_ACK 0x41
#define INSTR_OUT_GO 0x47
#define INSTR_OUT_STOP 0x53

// ************************
// *   DRAWING UTILITIES  *
// ************************

#define CIRCLE_RES 12

// ************************
// *        MATH          *
// ************************

#define PI 3.14159265

#endif // CONST_H
