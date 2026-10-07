#include "cy_device_headers.h"
#include "cy_syslib.h"
#include "cy_ipc_drv.h"
#include "cy_ipc_sema.h"

/* 定义共享内存区域 */
#define SHARED_MEMORY_BASE    0x28000000  // SRAM0起始地址
#define SHARED_MEMORY_SIZE    1024        // 1KB共享内存

/* 定义IPC通道 */
#define IPC_CHANNEL_0         0           // 用于核间通信的IPC通道
#define IPC_CHANNEL_1         1           // 用于信号量的IPC通道

/* 定义信号量 */
#define SEMAPHORE_0           0           // 用于同步的信号量

/* 共享内存结构体 */
typedef struct {
    uint32_t data[256];       // 数据缓冲区
    uint32_t writeIndex;      // 写入索引
    uint32_t readIndex;       // 读取索引
    uint32_t isInitialized;   // 初始化标志
} shared_memory_t;

/* 共享内存指针 */
static shared_memory_t* sharedMem = (shared_memory_t*)SHARED_MEMORY_BASE;

/* 初始化共享内存 */
void init_shared_memory(void) {
    if(sharedMem->isInitialized != 0x12345678) {
        sharedMem->writeIndex = 0;
        sharedMem->readIndex = 0;
        sharedMem->isInitialized = 0x12345678;
    }
}

/* 初始化IPC和信号量 */
void init_ipc_semaphore(void) {
    /* 初始化IPC驱动 */
    Cy_IPC_Drv_Init();
    
    /* 初始化信号量系统 */
    Cy_IPC_Sema_Init(IPC_CHANNEL_1, 32, NULL);
}

/* CM7_0核的示例代码 */
void cm7_0_example(void) {
    /* 初始化系统 */
    init_shared_memory();
    init_ipc_semaphore();
    
    /* 写入数据到共享内存 */
    while(1) {
        /* 等待信号量 */
        while(Cy_IPC_Sema_Status(SEMAPHORE_0) != 0);
        
        /* 写入数据 */
        sharedMem->data[sharedMem->writeIndex] = 0x12345678;
        sharedMem->writeIndex = (sharedMem->writeIndex + 1) % 256;
        
        /* 释放信号量通知CM7_1 */
        Cy_IPC_Sema_Set(SEMAPHORE_0);
        
        /* 延时 */
        Cy_SysLib_Delay(100);
    }
}

/* CM7_1核的示例代码 */
void cm7_1_example(void) {
    /* 初始化系统 */
    init_shared_memory();
    init_ipc_semaphore();
    
    /* 从共享内存读取数据 */
    while(1) {
        /* 等待信号量 */
        while(Cy_IPC_Sema_Status(SEMAPHORE_0) == 0);
        
        /* 读取数据 */
        uint32_t data = sharedMem->data[sharedMem->readIndex];
        sharedMem->readIndex = (sharedMem->readIndex + 1) % 256;
        
        /* 释放信号量通知CM7_0 */
        Cy_IPC_Sema_Clear(SEMAPHORE_0);
        
        /* 处理数据 */
        // TODO: 处理接收到的数据
    }
} 