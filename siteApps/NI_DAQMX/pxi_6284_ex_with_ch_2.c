#include <stdio.h>
#include <NIDAQmx.h>

#define NUM_CHANNELS 32

int main()
{
    // DAQmx 초기화
    int32 status;
    DAQmxInitialize();

    // 태스크 생성
    TaskHandle taskHandle;
    status = DAQmxCreateTask("MyTask", &taskHandle);
    if (status != 0)
    {
        printf("DAQmxCreateTask() 오류: %d\n", status);
        return 1;
    }

    // 입력 채널 구성
	// AI 채널을 만듭니다.
    for (i = 0; i < NUM_CHANNELS; i++)
    {
        status = DAQmxCreateAIVoltageChan(taskHandle, "Dev1/ai0%d", DAQmx_Val_Diff, -10.0, 10.0, DAQmx_Val_Volts, NULL);
        if (status != 0)
        {
            printf("DAQmxCreateAIVoltageChan() 오류: %d\n", status);
            return -1;
        }
    }

    // 시작 트리거 구성
    int32 trigSource = "PFI0";
    int32 trigEdge = DAQmx_Val_Rising;
    status = DAQmxCfgDigEdgeStartTrig(taskHandle, trigSource, trigEdge);
    if (status != 0)
    {
        printf("DAQmxCfgDigEdgeStartTrig() 오류: %d\n", status);
        return 1;
    }

    // 샘플 읽기
    float64* samps = (float64*)malloc(sizeof(float64) * 32 * numSamps);
    status = DAQmxReadAnalogF64(taskHandle, numSamps, 10.0, DAQmx_Val_GroupByChannel, samps, 32 * numSamps, &numSamps, NULL);
    if (status != 0)
    {
        printf("DAQmxReadAnalogF64() 오류: %d\n", status);
        return 1;
    }

    // 샘플 평균값 계산
    float64 avg[32] = { 0.0 };
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < numSamps; j++)
        {
            avg[i] += samps[i * numSamps + j];
        }
        avg[i] /= numSamps;
    }

    // 결과 출력
    for (int i = 0; i < 32; i++)
    {
        printf("채널 %d 평균값: %f\n", i, avg[i]);
    }

    // 태스크 종료
    status = DAQmxStopTask(taskHandle);
    if (status != 0)
    {
        printf("DAQmxStopTask() 오류: %d\n", status);
        return 1;
    }
    status = DAQmxClearTask(taskHandle);
    if (status != 0)
    {
        printf("DAQmxClearTask() 오류: %d\n", status);
        return 1;
    }

    // DAQmx 종료
    DAQmxUninitialize();

    return 0;
}

#include <stdio.h>
#include <NIDAQmx.h>

int main()
{
    // DAQmx 초기화
    int32 status;
    DAQmxInitialize();

    // 태스크 생성
    TaskHandle taskHandle;
    status = DAQmxCreateTask("MyTask", &taskHandle);
    if (status != 0)
    {
        printf("DAQmxCreateTask() 오류: %d\n", status);
        return 1;
    }

    // 입력 채널 구성
    int32 channelNumStart = 0;
    int32 channelNumEnd = 31;
    float64 min = 0.0;
    float64 max = 10.0;
    int32 numSamps = 100;
    status = DAQmxCfgInputAnalogF64(taskHandle, channelNumStart, channelNumEnd, min, max, DAQmx_Val_Volts, numSamps);
    if (status != 0)
    {
        printf("DAQmxCfgInputAnalogF64() 오류: %d\n", status);
        return 1;
    }

    // 시작 트리거 구성
    int32 trigSource = "PFI0";
    int32 trigEdge = DAQmx_Val_Rising;
    status = DAQmxCfgDigEdgeStartTrig(taskHandle, trigSource, trigEdge);
    if (status != 0)
    {
        printf("DAQmxCfgDigEdgeStartTrig() 오류: %d\n", status);
        return 1;
    }

    // 샘플 읽기
    float64* samps = (float64*)malloc(sizeof(float64) * numSamps * 32);
    status = DAQmxReadAnalogF64(taskHandle, numSamps, 10.0, DAQmx_Val_GroupByChannel, samps, numSamps * 32, &numSamps, NULL);
    if (status != 0)
    {
        printf("DAQmxReadAnalogF64() 오류: %d\n", status);
        return 1;
    }

    // 샘플 평균값 계산
    float64 avg = 0.0;
    for (int i = 0; i < numSamps; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            avg += samps[i * 32 + j];
        }
    }
    avg /= numSamps * 32;

    // 결과 출력
    printf("샘플 평균값: %f\n", avg);

    // 태스크 종료
    status = DAQmxStopTask(taskHandle);
    if (status != 0)
    {
        printf("DAQmxStopTask() 오류: %d\n", status);
        return 1;
    }
    status = DAQmxClearTask(taskHandle);
    if (status != 0)
    {
        printf("DAQmxClearTask() 오류: %d\n", status);
        return 1;
    }

    // DAQmx 종료
    DAQmxUninitialize();

    return 0;
}
