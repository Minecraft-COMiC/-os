#include <COMiC/types.h>


#ifndef COMiC_OS_H
#define COMiC_OS_H

#if COMiC_LIMITED
# pragma pack(push, 1)
#endif
typedef struct COMiC_Error
{
    const char *message;

    void (*deallocator)(char *);
} COMiC_Error;


#include <COMiC/_os.h>

/*
 * types 'COMiC_OS', 'COMiC_OS_Thread' & 'COMiC_OS_Lock' is os dependent and must be defined in '_os.h'
 */


const char *COMiC_OS_GetPlatformName(void);

enum _COMiC_OS_Init_ReturnCode
{
    _COMiC_OS_Init_ReturnCode_SUCCESSFUL = 0,
    _COMiC_OS_Init_ReturnCode_REINIT = -1,

    _COMiC_OS_Init_ReturnCode_UNEXPECTED_ERROR = -2,
    _COMiC_OS_Init_ReturnCode_FAILED_CREATING_GLOBALLOCK = 1,
};

enum _COMiC_OS_Init_ReturnCode _COMiC_OS_Init(void);

COMiC_bool _COMiC_OS_IsInited(void);

void _COMiC_OS_Finalize(void);

void _COMiC_OS_GlobalLock_Acquire(void);

void _COMiC_OS_GlobalLock_Release(void);

COMiC_success _COMiC_OS_Instance_Init(_COMiC_OS_Instance *storage, COMiC_usize heap_max_size);

void _COMiC_OS_Instance_Finalize(_COMiC_OS_Instance *storage);

_COMiC_OS_Instance *_COMiC_OS_Instance_GetCurrent(void);

typedef struct COMiC_OS_Time
{
    COMiC_int16 year;
    COMiC_uint8 month;
    COMiC_uint8 day;
    COMiC_uint8 hour;
    COMiC_uint8 minute;
    COMiC_uint8 second;
    COMiC_uint16 milliseconds;
} COMiC_OS_Time;

COMiC_OS_Time COMiC_OS_GetTime(void);

COMiC_success COMiC_OS_Thread_Create(COMiC_OS_Thread *storage, void (*func)(void *data), void *data);

COMiC_success COMiC_OS_Thread_Pause(COMiC_OS_Thread *storage);

COMiC_success COMiC_OS_Thread_Resume(COMiC_OS_Thread *storage);

COMiC_success COMiC_OS_Thread_Wait(COMiC_OS_Thread *storage, COMiC_uint32 milliseconds);

COMiC_success COMiC_OS_Thread_Kill(COMiC_OS_Thread *storage);

void COMiC_OS_Thread_Destroy(COMiC_OS_Thread *storage);

COMiC_success COMiC_OS_Thread_RegisterCurrent(_COMiC_OS_Instance *os_instance, COMiC_OS_Thread *storage);

void COMiC_OS_Thread_Unregister(COMiC_OS_Thread *storage);

COMiC_OS_Thread *COMiC_OS_Thread_GetCurrent(void);

#define COMiC_Error_Set(MSG) (COMiC_Error_SetHeap(NULL, (MSG)))
#define _COMiC_Error_Set(STORAGE, MSG) (_COMiC_Error_SetHeap((STORAGE), NULL, (MSG)))

static inline void _COMiC_Error_Clear(COMiC_Error *storage);

static inline void _COMiC_Error_SetHeap(COMiC_Error *storage, void (*deallocator)(char *), const char *msg)
{
    _COMiC_Error_Clear(storage);
    storage->message = msg;
    storage->deallocator = deallocator;
}

void COMiC_Error_SetHeap(void (*deallocator)(char *), const char *msg);

static inline const char *_COMiC_Error_Get(COMiC_Error *storage)
{
    return storage->message;
}

const char *COMiC_Error_Get(void);

static inline void _COMiC_Error_Clear(COMiC_Error *storage)
{
    if (storage->message != NULL)
    {
        if (storage->deallocator != NULL)
        {
            storage->deallocator((char *) storage->message);
            storage->deallocator = NULL;
        }
        storage->message = NULL;
    }
}

void COMiC_Error_Clear(void);

const char *COMiC_OS_Error_Get(void);

void COMiC_OS_Error_Clear(void);

void COMiC_Error_Fatal(const char *msg);

void *COMiC_Alloc(COMiC_usize size);

#define COMiC_Calloc(N, S) (COMiC_Alloc((N) * (S))

void *COMiC_ReAlloc(void *p, COMiC_usize size);

#define COMiC_ReCalloc(P, N, S) (COMiC_ReAlloc((P), (N) * (S))

void COMiC_Free(void *p);

COMiC_success COMiC_OS_Lock_Create(COMiC_Lock *storage);

COMiC_success COMiC_OS_Lock_Lock(COMiC_Lock *storage/*, COMiC_uint32 milliseconds*/);

void COMiC_OS_Lock_Unlock(COMiC_Lock *storage);

/* COMiC_bool COMiC_OS_Lock_IsLocked(COMiC_Lock *storage); */

void COMiC_OS_Lock_Destroy(COMiC_Lock *storage);

COMiC_success COMiC_OS_CondVar_Create(COMiC_CondVar *storage, COMiC_Lock *lock);

COMiC_success COMiC_OS_CondVar_Wait(COMiC_CondVar *storage/*, COMiC_uint32 milliseconds*/);

void COMiC_OS_CondVar_Notify(COMiC_CondVar *storage);

void COMiC_OS_CondVar_NotifyAll(COMiC_CondVar *storage);

void COMiC_OS_CondVar_Destroy(COMiC_CondVar *storage);

#if COMiC_LIMITED
# pragma pop()
#endif

#endif /* COMiC_OS_H */
