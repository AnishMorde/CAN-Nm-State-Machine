#include <stdio.h> 

#define NM_REPEAT_MESSAGE_TIME 500
#define NM_NM_TIMEOUT_TIME 1000
#define NM_READY_SLEEP_TIME 500
#define NM_WAIT_BUS_SLEEP_TIME 100
#define NM_PREPARE_BUS_SLEEP_TIME 500
#define NM_MAIN_PERIOD 10

unsigned int RepeatTimer = 0;
unsigned int NmTimer = 0;
unsigned int ReadySleepTimer = 0;
unsigned int BusSleepTimer = 0 ;
unsigned int PrepareBusSleepTimer = 0;

typedef enum
{
    NM_STATE_BUS_SLEEP_MODE,
    NM_STATE_REPEAT_MESSAGE_MODE,
    NM_STATE_NORMAL_MODE,
    NM_STATE_READY_SLEEP_MODE ,
    NM_STATE_PREPARE_BUS_SLEEP_MODE
} Nm_StateType;

typedef enum
{
    NM_EVENT_NET_REQUEST,
    NM_EVENT_NET_RELEASE,
    NM_EVENT_BUS_SLEEP_TIMEOUT,
    NM_EVENT_NORMAL_TIMEOUT,
    NM_EVENT_REPEAT_MESSAGE_TIMEOUT,
    NM_EVENT_READY_SLEEP_TIMEOUT,
    NM_EVENT_PREPARE_BUS_SLEEP_TIMEOUT

} Nm_EventType;

Nm_StateType Nm_Curr_state = NM_STATE_BUS_SLEEP_MODE;


void RepeatMessageModeTimer()
{
    RepeatTimer = NM_REPEAT_MESSAGE_TIME;
}

void NormalModeTimer()
{
    NmTimer = NM_NM_TIMEOUT_TIME;
}

void ReadySleepModeTimer()
{
    ReadySleepTimer = NM_READY_SLEEP_TIME;
}

void PrepareBusSleepModeTimer()
{
    PrepareBusSleepTimer = NM_PREPARE_BUS_SLEEP_TIME;
}

void BusSleepModeTimer()
{
    BusSleepTimer = NM_WAIT_BUS_SLEEP_TIME;
}

void EventHandler(Nm_EventType event)
{

    switch (Nm_Curr_state)
    {
    case NM_STATE_BUS_SLEEP_MODE:

        if (event == NM_EVENT_BUS_SLEEP_TIMEOUT || event == NM_EVENT_NET_REQUEST)
        {
            Nm_Curr_state = NM_STATE_REPEAT_MESSAGE_MODE;
            printf("The state Transition BUS_SLEEP_STATE -> REPEAT_MESSAGE \n ");
            RepeatMessageModeTimer();
        }
        break;

    case NM_STATE_REPEAT_MESSAGE_MODE:
        if (event == NM_EVENT_REPEAT_MESSAGE_TIMEOUT)
        {
            Nm_Curr_state = NM_STATE_NORMAL_MODE;
            printf("The state Transition REPEAT_MESSAGE -> NORMAL_STATE \n ");
            NormalModeTimer();
        }
        break;

    case NM_STATE_NORMAL_MODE:
        if (event == NM_EVENT_NORMAL_TIMEOUT)
        {
            Nm_Curr_state = NM_STATE_REPEAT_MESSAGE_MODE;
            printf("The state Transition  NORMAL_STATE -> REPEAT_MESSAGE \n ");
            RepeatMessageModeTimer();
        }
        else if (event == NM_EVENT_NET_RELEASE)
        {
            Nm_Curr_state = NM_STATE_READY_SLEEP_MODE;
            printf("The state Transition  NORMAL_STATE -> READY_SLEEP_STATE \n ");
            ReadySleepModeTimer();
        }
        break;

    case NM_STATE_READY_SLEEP_MODE:
        if (event == NM_EVENT_NET_REQUEST)
        {
            Nm_Curr_state = NM_STATE_NORMAL_MODE;
            printf("The state Transition  READY_SLEEP_STATE -> NORMAL_STATE \n ");
            NormalModeTimer();
        }
        else if (event == NM_EVENT_READY_SLEEP_TIMEOUT)
        {
            Nm_Curr_state = NM_STATE_PREPARE_BUS_SLEEP_MODE;
            printf("The state Transition  READY_SLEEP_STATE -> PREPARE_BUS_SLEEP_STATE \n ");
            PrepareBusSleepModeTimer();
        }
        break;

    case NM_STATE_PREPARE_BUS_SLEEP_MODE:
        if (event == NM_EVENT_PREPARE_BUS_SLEEP_TIMEOUT)
        {
            Nm_Curr_state = NM_STATE_BUS_SLEEP_MODE;
            printf("The state Transition  PREPARE_BUS_SLEEP_STATE -> BUS_SLEEP \n ");
            BusSleepModeTimer();
        }
        break;

    default:
        break;
    }
}

void scheduler(void)
{

    if (RepeatTimer > 0)
    {
        RepeatTimer -= NM_MAIN_PERIOD;
        if (RepeatTimer == 0)
        {
            EventHandler(NM_EVENT_REPEAT_MESSAGE_TIMEOUT);
        }
    }

    if (NmTimer > 0)
    {
        NmTimer -= NM_MAIN_PERIOD;
        if (NmTimer <= 0)
        {
            NmTimer = 0;
            EventHandler(NM_EVENT_NORMAL_TIMEOUT);
        }
    }

    if (ReadySleepTimer > 0)
    {
        ReadySleepTimer -= NM_MAIN_PERIOD;
        if (ReadySleepTimer <= 0)
        {
            ReadySleepTimer = 0;
            EventHandler(NM_EVENT_READY_SLEEP_TIMEOUT);
        }
    }

    if (BusSleepTimer > 0)
    {
        BusSleepTimer -= NM_MAIN_PERIOD;
        if (BusSleepTimer <= 0)
        {
            BusSleepTimer = 0;
            EventHandler(NM_EVENT_BUS_SLEEP_TIMEOUT);
        }
    }

    if (PrepareBusSleepTimer > 0)
    {
        PrepareBusSleepTimer -= NM_MAIN_PERIOD;
        if (PrepareBusSleepTimer <= 0)
        {
            PrepareBusSleepTimer = 0;
            EventHandler(NM_EVENT_PREPARE_BUS_SLEEP_TIMEOUT);
        }
    }
}


int main() {
    BusSleepModeTimer();
    for (int i = 0; i < 500; ++i) {
        scheduler();
        if(i==400){
            EventHandler(NM_EVENT_NET_RELEASE);
        }

        if(Nm_Curr_state == NM_STATE_READY_SLEEP_MODE ){
            EventHandler( NM_EVENT_NET_REQUEST);
        }
     
    }
    return 0;
}
