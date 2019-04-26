
#include "time.h"

    /* need define? */
// #define ___DEBUG_PRINT_          // 시스템 동작중 print Debug가 필요한 경우 사용
//#define CHECK_BIT                 // Health Status 확인모드 (FPS_SYSTEM 주석처리 필요)
#define FPS_SYSTEM                  // FPS System 동작모드   (CHECK_BIT 주석처리 필요)

 /*
    8,000,000 --> interlock before 200ms data
    8,000,000 --> interlock after 200ms data
 */
unsigned int data[2*8*1000*1000]= {0,};


/*
        Function Define
*/


int main(void)
{
    unsigned int page_size64=sysconf(_SC_PAGESIZE);
    int fd = 0;

    INIT_DEVICE64(page_size64, &fd, 1);

#ifdef CHECK_BIT
    int version = 0;
    int BoardID = 0;
    int LinkError = 0;
    int Temp = 0;
    int a1234 = 0;
    int abcde = 0;
    int b1234 = 0;

    int interlock_bram1 = 0;
    int interlock_bram2 = 0;
    int bram1_dry_data = 0, bram1_wet_data = 0;
    int bram2_dry_data = 0, bram2_wet_data = 0;

    version = CTR_CMD_MEM1[9];
    printf("Version   = %#x\n", version);

    BoardID = CTR_CMD_MEM1[10];
    printf("BoardID   = %#x\n", BoardID);

    LinkError = CTR_CMD_MEM1[11];
    printf("LinkError = %#x\n", LinkError);

    Temp = CTR_CMD_MEM1[12];
    printf("Temp      = %#x\n", Temp);

    a1234 = CTR_CMD_MEM1[13];
    printf("a1234     = %#x\n", a1234);

    abcde = CTR_CMD_MEM1[14];
    printf("abcde     = %#x\n", abcde);

    b1234 = CTR_CMD_MEM1[15];
    printf("b1234     = %#x\n", b1234);
#endif

#ifdef FPS_SYSTEM
   FPS_START();
#endif
    return 0;
}

int FPS_START()
{
    /* loop variable */
    int index = 0;
    int current = 0;

    int i = 0; //i = bram index
    int interrupt_status= 0;     // 인터럽트 상태값
    int interlock_index = 0;     // 인터락이 걸린 위치값

    unsigned int bram_index = 0; // 인터락 후 남은 Bram의 index
    unsigned int temp_index = 0; // 인터락 후 남은 Bram의 데이터를 쓴 후의 DDR index

#ifdef ___DEBUG_PRINT_
    unsigned int msec_100 = 0;
    unsigned int msec_10 = 0;
    unsigned int msec_1 = 0;
    unsigned int count = 0;
#endif




    while(1)
    {

        if(current == MAX_200MS)            // 200ms의 데이터가 꽉차면 위치값을 초기화( 인터락 발생 x )
            current = 0;

        switch(INTERRUPT)   // 0 = no operation , 1 = bram1 data read / 2 = bram2 data read
        {
            case ODD :
                {

                    CLEAR_INTERRUPT // interrupt값 0 셋팅

                    for(index=current; index < current + MAX_10MS; index+=4) // #define MAX_10MS = 4 * 1us * 1000 * 10 = 40,000
                    {

                        if(((CTR_CMD_MEM[i] & 0x80000000) >> 31) == 1)  // 인터락 발생여부 확인
                        {
                            interrupt_status = 1;
                            interlock_index = index;
                            bram_index = i;
                            break;
                        }

                        i = i+1;

                        data[index] = CTR_CMD_MEM[i + 0];
                        data[index+1] = CTR_CMD_MEM[i + 20000];
                        data[index+2] = CTR_CMD_MEM[i + 40000];
                        data[index+3] = CTR_CMD_MEM[i + 60000];
                    }

                    break;
                }

            case EVEN :
                {
                    CLEAR_INTERRUPT


                    for(index=current; index < current + MAX_10MS; index+=4)
                    {
                        
                        if(((CTR_CMD_MEM[i+10000] & 0x80000000) >> 31) == 1)
                        {
                            interrupt_status = 2;
                            interlock_index = index;            //인터락이 걸린 시점을 확인함.
                            bram_index = i;
                            break;
                        }

                        i = i+1;


                        data[index] = CTR_CMD_MEM[i + 10000];
                        data[index+1] = CTR_CMD_MEM[i + 30000];
                        data[index+2] = CTR_CMD_MEM[i + 50000];
                        data[index+3] = CTR_CMD_MEM[i + 70000];

                    }

                    break;
                }

            default :
                continue;
        }

        if(interrupt_status)        //인터락이 발생하면 바로 루프를 빠져나옴
            break;

        current = index;            //data의 인덱스 위치값을 저장
        i = 0;                      //bram 위치값을 초기화

    }


    // 800000부터 시작함.
    /* interlock 이후 남은 Bram의 Data를 write 해야함. */

#ifdef ___DEBUG_PRINT_
    printf("bram_index = %d\n", bram_index);
#endif

    if(interrupt_status == 2)                       //만약 인터락이 걸린 위치가 bram2번이면?
    {
        for(index=800000; index < 840000; index+=4)         //해당 루프는 40000번을 반복하지않음. 아래에서 루프를 빠져나감
        {
            if(bram_index == 10000)                         //루프 시작시 bram_index의 값은 0~10000 사이, 
            {
                temp_index = index;                         //data의 위치값을 기억
                i = 0;
                break;
            }

            bram_index = bram_index+1;

            data[index] = CTR_CMD_MEM[bram_index + 10000];
            data[index+1] = CTR_CMD_MEM[bram_index + 30000];
            data[index+2] = CTR_CMD_MEM[bram_index + 50000];
            data[index+3] = CTR_CMD_MEM[bram_index + 70000];

        }
    }
    else if(interrupt_status == 1)                  //만약 인터락이 걸린 위치가 bram1번이면?
    {
        for(index=800000; index < 840000; index+=4)
        {
            if(bram_index == 10000)
            {
                temp_index = index;
                i = 0;
                break;
            }

            bram_index = bram_index+1;
            
            data[index] = CTR_CMD_MEM[bram_index ];
            data[index+1] = CTR_CMD_MEM[bram_index + 20000];
            data[index+2] = CTR_CMD_MEM[bram_index + 40000];
            data[index+3] = CTR_CMD_MEM[bram_index + 60000];

        }
    }

#ifdef ___DEBUG_PRINT_
    printf("temp_index? = %d\n", temp_index);
#endif

#if 1
int Current_Offset = 0;

    Current_Offset = temp_index;            //나머지 bram데이터를 쓰고난 data의 인덱스 위치를 옵셋으로 지정

    while(Current_Offset < MAX_400MS)       //400ms까지 데이터를 쓰고나면 루프를 빠져나감.
    {
        switch(INTERRUPT)
        {
            case ODD :
                {
                    CLEAR_INTERRUPT;

                    for(index = Current_Offset; index < Current_Offset + MAX_10MS; index+=4)              // #define MAX_10MS = 4 * 1us * 1000 * 10 = 40,000
                    {
                        i = i+1;

                        data[index] = CTR_CMD_MEM[i + 0];
                        data[index+1] = CTR_CMD_MEM[i + 20000];
                        data[index+2] = CTR_CMD_MEM[i + 40000];
                        data[index+3] = CTR_CMD_MEM[i + 60000];
                    }

                    break;
                }

            case EVEN :
                {
                    CLEAR_INTERRUPT;

                    for(index=Current_Offset; index < Current_Offset + MAX_10MS; index+=4)
                    {
                        i = i+1;

                        data[index] = CTR_CMD_MEM[i + 10000];
                        data[index+1] = CTR_CMD_MEM[i + 30000];
                        data[index+2] = CTR_CMD_MEM[i + 50000];
                        data[index+3] = CTR_CMD_MEM[i + 70000];
                    }

                    break;
                }

            default :
                continue;
        }

        Current_Offset = index;
        i = 0;
    }
#endif

    Create_Log(interlock_index, data);      //인터락이 걸린 위치값부터 파일을 생성

    return 0;
}

int Create_Log(int Position, unsigned int *data)
{
    int fd = 0;
    int i = 0;
    char buf[1024] = { 0, };

    ACQ acq_node = {0,};

    fd = open("FPS.csv", O_RDWR| O_CREAT, FILEMODE);

    if(fd < 0)
        perror("Error : ");


    printf("(1) __Start File Write..... Write Position == %d\n", Position);

    for(i = Position; i<MAX_200MS; i+=4)
    {
        acq_node.interlock = (unsigned short)((data[i] & 0x80000000) >> 31); //1
        acq_node.day = (unsigned short)((data[i] & 0x03fe0000) >> 17); //9
        acq_node.hour = (unsigned short)((data[i] & 0x001f000) >> 12); //5
        acq_node.min = (unsigned short)((data[i] & 0x00000fc0) >> 6); //6
        acq_node.sec = (unsigned short)((data[i] & 0x0000003f) >> 0); //6
        acq_node.msec_100 = (unsigned short)((data[i+1] & 0xf0000000) >> 28); //4
        acq_node.msec_10 = (unsigned short)((data[i+1] & 0x03c00000) >> 22);
        acq_node.msec_1 = (unsigned short)((data[i+1] & 0x003c0000) >> 18);
        acq_node.count = (unsigned int)((data[i+1] & 0x0003ffff) >> 0);
        acq_node.Node_Status = ((((unsigned long long)data[i+2]) << 32) | (data[i+3]));

        sprintf(buf, "index = %d | interlock = %d | TimeStamp = %d - %d - %d - %d - %d - %d - %d | 1us Count = %d | Node Status = %#llx\n", i, acq_node.interlock, acq_node.day, acq_node.hour, acq_node.min, acq_node.sec, acq_node.msec_100, acq_node.msec_10, acq_node.msec_1, acq_node.count, acq_node.Node_Status);

        // sprintf(buf, "index = %d | interlock = %d | TimeStamp = %d - %d - %d - %d - %d - %d - %d | 1us Count %d Node Status = %#llx\n", i, acq_node.interlock, acq_node.day, acq_node.hour, acq_node.min, acq_node.sec, acq_node.msec_100, acq_node.msec_10, acq_node.msec_1, acq_node.count, acq_node.Node_Status);
        // sprintf(buf, "int = %d interlock = %d \t TimeStamp %d - %d - %d - %d - %d - %d - %d - %d - %#llx\n", i, acq_node.interlock, acq_node.day, acq_node.hour, acq_node.min, acq_node.sec, acq_node.msec_100, acq_node.msec_10, acq_node.msec_1, acq_node.count, acq_node.Node_Status);

        lseek(fd, 0, SEEK_CUR);
        // write(fd, buf, strlen(buf));
        lseek(fd, 0, SEEK_END);
        write(fd, buf, strlen(buf));
    }
    printf("I= %d\n", i);
    printf("(1) END!!!!\n");
    printf("(2) __Start File Write.....\n");

    for(i = 0; i<Position; i+=4)
    {
        acq_node.interlock = (unsigned short)((data[i] & 0x80000000) >> 31); //1
        acq_node.day = (unsigned short)((data[i] & 0x03fe0000) >> 17); //9
        acq_node.hour = (unsigned short)((data[i] & 0x001f000) >> 12); //5
        acq_node.min = (unsigned short)((data[i] & 0x00000fc0) >> 6); //6
        acq_node.sec = (unsigned short)((data[i] & 0x0000003f) >> 0); //6
        acq_node.msec_100 = (unsigned short)((data[i+1] & 0xf0000000) >> 28); //4
        acq_node.msec_10 = (unsigned short)((data[i+1] & 0x03c00000) >> 22);
        acq_node.msec_1 = (unsigned short)((data[i+1] & 0x003c0000) >> 18);
        acq_node.count = (unsigned int)((data[i+1] & 0x0003ffff) >> 0);
        acq_node.Node_Status = ((((unsigned long long)data[i+2]) << 32) | (data[i+3]));

        sprintf(buf, "index = %d | interlock = %d | TimeStamp = %d - %d - %d - %d - %d - %d - %d | 1us Count = %d | Node Status = %#llx\n", i, acq_node.interlock, acq_node.day, acq_node.hour, acq_node.min, acq_node.sec, acq_node.msec_100, acq_node.msec_10, acq_node.msec_1, acq_node.count, acq_node.Node_Status);

        // sprintf(buf, "index = %d | interlock = %d | TimeStamp = %d - %d - %d - %d - %d - %d - %d | 1us Count %d Node Status = %#llx\n", i, acq_node.interlock, acq_node.day, acq_node.hour, acq_node.min, acq_node.sec, acq_node.msec_100, acq_node.msec_10, acq_node.msec_1, acq_node.count, acq_node.Node_Status);
        // sprintf(buf, "int = %d | interlock = %d | TimeStamp = %d - %d - %d - %d - %d - %d - %d - %d - %#llx\n", i, acq_node.interlock, acq_node.day, acq_node.hour, acq_node.min, acq_node.sec, acq_node.msec_100, acq_node.msec_10, acq_node.msec_1, acq_node.count, acq_node.Node_Status);
        // sprintf(buf, "index = %d interlock = %d \t, %d, %d, %d, %d, %d, %d, %d, %d, %#llx\n", i, acq_node.interlock, acq_node.day, acq_node.hour, acq_node.min, acq_node.sec, acq_node.msec_100, acq_node.msec_10, acq_node.msec_1, acq_node.count, acq_node.Node_Status);

        lseek(fd, 0, SEEK_CUR);
        // write(fd, buf, strlen(buf));
        lseek(fd, 0, SEEK_END);
        write(fd, buf, strlen(buf));
    }

    printf("I= %d\n", i);
    printf("(2) END!!!!\n");
    printf("(3) __Start File Write.....\n");

    for(i=OFFSET_200MS; i<MAX_400MS; i+=4)
    {
        acq_node.interlock = (unsigned short)((data[i] & 0x80000000) >> 31); //1
        acq_node.day = (unsigned short)((data[i] & 0x03fe0000) >> 17); //9
        acq_node.hour = (unsigned short)((data[i] & 0x001f000) >> 12); //5
        acq_node.min = (unsigned short)((data[i] & 0x00000fc0) >> 6); //6
        acq_node.sec = (unsigned short)((data[i] & 0x0000003f) >> 0); //6
        acq_node.msec_100 = (unsigned short)((data[i+1] & 0xf0000000) >> 28); //4
        acq_node.msec_10 = (unsigned short)((data[i+1] & 0x03c00000) >> 22);
        acq_node.msec_1 = (unsigned short)((data[i+1] & 0x003c0000) >> 18);
        acq_node.count = (unsigned int)((data[i+1] & 0x0003ffff) >> 0);
        acq_node.Node_Status = ((((unsigned long long)data[i+2]) << 32) | (data[i+3]));

        sprintf(buf, "index = %d | interlock = %d | TimeStamp = %d - %d - %d - %d - %d - %d - %d | 1us Count = %d | Node Status = %#llx\n", i, acq_node.interlock, acq_node.day, acq_node.hour, acq_node.min, acq_node.sec, acq_node.msec_100, acq_node.msec_10, acq_node.msec_1, acq_node.count, acq_node.Node_Status);
        // sprintf(buf, "index = %d interlock = %d \t, %d, %d, %d, %d, %d, %d, %d, %d, %#llx\n", i, acq_node.interlock, acq_node.day, acq_node.hour, acq_node.min, acq_node.sec, acq_node.msec_100, acq_node.msec_10, acq_node.msec_1, acq_node.count, acq_node.Node_Status);

        lseek(fd, 0, SEEK_CUR);
        // write(fd, buf, strlen(buf));
        lseek(fd, 0, SEEK_END);
        write(fd, buf, strlen(buf));
    }
    printf("I= %d\n", i);   
    printf("(3) END!!!!\n");

    return 0;
}
