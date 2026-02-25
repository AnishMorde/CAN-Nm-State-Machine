# CAN-Nm-State-Machine

A C implementation of the CAN Network Management (CAN-Nm) state machine, as specified in the AUTOSAR standard. This project demonstrates a finite state machine that manages vehicle network communication states, handling transitions between sleep and active modes.

## Overview

The CAN-Nm state machine is a critical component in automotive systems that manages the power state of the CAN bus network. It ensures efficient power management while maintaining network availability when needed.

## State Machine Architecture

The state machine implements 5 distinct states:

1. **BUS_SLEEP_MODE** - The network is in sleep state, consuming minimal power
2. **REPEAT_MESSAGE_MODE** - Network is preparing for normal operation, sending repeat messages
3. **NORMAL_MODE** - Network is fully operational with active communication
4. **READY_SLEEP_MODE** - Network is transitioning to sleep state, preventing new requests
5. **PREPARE_BUS_SLEEP_MODE** - Final preparation before entering sleep mode

## State Transitions

```
BUS_SLEEP_MODE
    ↓ (NET_REQUEST or BUS_SLEEP_TIMEOUT)
REPEAT_MESSAGE_MODE
    ↓ (REPEAT_MESSAGE_TIMEOUT)
NORMAL_MODE
    ↓ (NORMAL_TIMEOUT) ↑ (NET_REQUEST from READY_SLEEP)
    ↓ (NET_RELEASE)
READY_SLEEP_MODE
    ↓ (READY_SLEEP_TIMEOUT)
PREPARE_BUS_SLEEP_MODE
    ↓ (PREPARE_BUS_SLEEP_TIMEOUT)
BUS_SLEEP_MODE
```

## Events

The state machine responds to the following events:

- `NM_EVENT_NET_REQUEST` - Network communication requested
- `NM_EVENT_NET_RELEASE` - Network communication no longer needed
- `NM_EVENT_BUS_SLEEP_TIMEOUT` - Bus sleep timeout expired
- `NM_EVENT_NORMAL_TIMEOUT` - Normal mode timeout expired
- `NM_EVENT_REPEAT_MESSAGE_TIMEOUT` - Repeat message timeout expired
- `NM_EVENT_READY_SLEEP_TIMEOUT` - Ready sleep timeout expired
- `NM_EVENT_PREPARE_BUS_SLEEP_TIMEOUT` - Prepare bus sleep timeout expired

## Timing Configuration

```c
NM_REPEAT_MESSAGE_TIME      = 500 ms   // Repeat message phase duration
NM_NM_TIMEOUT_TIME          = 1000 ms  // Normal mode timeout
NM_READY_SLEEP_TIME         = 500 ms   // Ready sleep phase duration
NM_WAIT_BUS_SLEEP_TIME      = 100 ms   // Bus sleep wait time
NM_PREPARE_BUS_SLEEP_TIME   = 500 ms   // Prepare sleep phase duration
NM_MAIN_PERIOD              = 10 ms    // Main scheduler period
```

## Files

- `Code/Main.c` - Main state machine implementation including:
  - State and event enumerations
  - Timer management functions
  - Event handler with state transition logic
  - Scheduler for timer management
  - Main program demonstrating the state machine

## Compilation and Execution

### Build
```bash
gcc -o can_nm Code/Main.c
```

### Run
```bash
./can_nm
```

## How It Works

1. The `main()` function initializes the state machine in BUS_SLEEP_MODE and runs a scheduler loop
2. The `scheduler()` function decrements active timers and triggers events when timeouts expire
3. The `EventHandler()` function processes events and performs state transitions
4. Timer functions initialize the timeout values for each state

## Example Flow

1. System starts in **BUS_SLEEP_MODE**
2. A network request triggers transition to **REPEAT_MESSAGE_MODE**
3. After repeat message timeout, system enters **NORMAL_MODE**
4. When network is released, system enters **READY_SLEEP_MODE**
5. After ready sleep timeout, system transitions to **PREPARE_BUS_SLEEP_MODE**
6. Finally returns to **BUS_SLEEP_MODE** after prepare timeout

## Standards Reference

This implementation follows the AUTOSAR specification for CAN Network Management (CAN-Nm), providing a standard approach to managing network communication states in automotive systems.

## License

This project is provided as-is for educational and reference purposes.