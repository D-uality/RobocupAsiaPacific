#ifndef BaseFunctions
#define BaseFunctions

extern const int DMotors[4];
extern const int MID;
extern const int MIN;
extern const int MAX;
extern const int StraightWeight;
extern const int CSensors[6];
extern int CSensorV[6];
extern const int CLBound;
extern const int CUBound;
extern int InNum[6][2][2];
extern int CalibrationNum[6][2];

extern const int RunOut;
extern const int ReadCSensorOut;
extern const int PIDOut;

extern int mode;

void Mode();
void Run(int v1, int v2);
void ReadCSensorV();

#endif