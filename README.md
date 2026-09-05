# JB_Balancing_Car

> A two-wheeled self-balancing robot built around the self-designed JB_board.

## 📖 Project Overview

**JB_Balancing_Car** is a two-wheeled self-balancing robot developed from scratch.

The project combines:

- IMU
- DC geared motors
- Quadrature encoders
- Motor driver
- Real-time control
- PD control
- Velocity-loop architecture
- Timer-based task scheduling
- A self-designed control board — **JB_board**

The project was developed as a practical study of the complete robotics control chain:

```text
Sensor
  ↓
State Estimation
  ↓
Control
  ↓
Motor Output
  ↓
Mechanical Motion
  ↓
Sensor Feedback
```

---

## 🎯 Project Goals

The main objectives were:

- Build a two-wheeled self-balancing robot.
- Design and use a custom control board.
- Integrate an MPU6050 IMU.
- Control two DC motors.
- Acquire encoder feedback.
- Implement a real-time upright controller.
- Explore a cascaded velocity-control architecture.
- Practice hardware/software co-design.
- Develop systematic hardware debugging skills.

---

## 🧩 System Architecture

```text
                 ┌──────────────┐
                 │   MPU6050    │
                 │     IMU      │
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │ State        │
                 │ Estimation   │
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │ Upright PD   │
                 │ Controller   │
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │ Motor Driver │
                 └──────┬───────┘
                        │
                 ┌──────┴──────┐
                 ▼             ▼
              Motor L       Motor R
                 │             │
                 ▼             ▼
             Encoder L     Encoder R
                 │             │
                 └──────┬──────┘
                        ▼
                    Feedback
```

---

## 🔧 Hardware

### Main Components

- **JB_board** — custom control board
- MPU6050 — 6-axis IMU
- Two DC geared motors
- Quadrature encoders — AB-phase feedback
- Motor driver
- 2S LiPo battery
- Two-wheel mechanical chassis

---

## 💻 Software Architecture

The control system uses periodic execution based on hardware timers.

The basic structure is:

```text
Timer
  ↓
Interrupt
  ↓
Set Control Flag
  ↓
Main Loop
  ↓
Read Sensors
  ↓
Calculate Control
  ↓
Output Motor PWM
```

The timer interrupt is kept lightweight, while the main loop performs sensor processing and control calculations.

---

## ⚙️ Upright Control

The primary controller is a PD controller.

```text
Angle Error
     ↓
     P
     +
Angular Velocity
     ↓
     D
     ↓
 Motor PWM
```

The proportional term provides the main restoring action.

The derivative term provides damping and helps reduce oscillation.

---

## 🔄 Velocity Loop

The project was extended toward a cascaded control architecture:

```text
Target Speed
     ↓
Velocity PI Controller
     ↓
Target Angle
     ↓
Upright PD Controller
     ↓
Motor PWM
```

This separates two control objectives:

- **Upright loop** — keeps the robot balanced.
- **Velocity loop** — controls the robot's movement speed.

---

## ⏱️ Real-Time Control

Timer-based scheduling is used to create periodic control tasks.

The basic concept is:

```text
Hardware Timer
      ↓
ISR
      ↓
Flag
      ↓
Main Loop
      ↓
Control Task
```

ISR (中断服务程序) is used to respond to timer events and set the corresponding flag.

The main loop then performs the actual control work.

This approach avoids placing large amounts of computation inside the interrupt routine.

---

## 🔄 Encoder Development

The encoder subsystem became an important part of the project.

The project experimented with:

- External interrupt-based detection
- Timer-based polling
- A/B phase direction detection
- Encoder count accumulation
- Speed feedback

The basic quadrature encoder concept is:

```text
A Phase
   +
B Phase
   ↓
Pulse + Direction
```

The system detects an A-phase transition and checks the B-phase state to determine the direction of rotation.

---

## ⚠️ Hardware Debugging

One of the most important problems encountered during development was encoder instability.

The encoder behaved normally when the motors were not driven.

After the motors were driven, the encoder signals became unreliable.

The debugging process involved:

- Pull-up resistors
- Filter capacitors
- Signal wiring
- Encoder input levels
- Timer polling
- Interrupt detection
- PCB traces
- Motor current
- Electromagnetic interference

The final investigation showed that the physical PCB layout was an important part of the problem.

Motor-related current switching introduced interference into nearby encoder signal traces.

This led to an important engineering lesson:

> **A digital signal is still a physical electrical signal.**

The MCU does not see an abstract `HIGH` or `LOW`.

It sees a real voltage waveform affected by:

- Current switching
- Ground noise
- Electromagnetic coupling
- PCB routing
- Signal integrity
- Wiring

---

## 🧠 Major Engineering Lessons

### 1. Software cannot always fix a hardware problem

If the physical signal is already corrupted, changing the decoding algorithm may not solve the root cause.

```text
Bad Physical Signal
        ↓
Better Algorithm
        ↓
Still Unreliable
```

The debugging process should therefore move from:

```text
Physical Layer
      ↓
Electrical Signal
      ↓
MCU Input
      ↓
Software
```

---

### 2. High-current and low-level signals require different treatment

The motor power path and encoder/IMU signal path have very different electrical characteristics.

```text
Motor
 ↓
Large Current
 ↓
Fast Switching
```

while:

```text
Encoder / IMU
 ↓
Low-level Signal
 ↓
Sensitive to Noise
```

PCB routing, grounding, filtering, and signal separation therefore become critical.

---

### 3. Change one variable at a time

The project also reinforced a fundamental debugging rule:

> **Change one variable at a time.**

Examples include:

- Removing motor drive
- Changing encoder connections
- Swapping left/right channels
- Testing different pull-up resistors
- Adding filtering
- Cutting PCB traces
- Using external wires
- Comparing different decoding methods

This makes it possible to distinguish correlation from causation.

---

## 🏁 Project Result

The V1 system achieved the following milestones:

- Custom control board designed and used
- MPU6050 integration
- Motor control
- Upright PD control
- Timer-based control architecture
- Encoder signal acquisition experiments
- Initial velocity-loop architecture
- Real-world hardware debugging

The V1 PCB design also exposed hardware limitations that prevented the encoder subsystem from reaching the reliability required for further closed-loop development.

Instead of endlessly modifying V1, the hardware was therefore frozen and the lessons were carried forward into the next board revision.

---

## 🛠️ JB_board V2 Direction

The next hardware revision will focus on electrical reliability.

Planned improvements include:

### Power Layout

Separate:

```text
High-current motor power
```

from:

```text
Low-level sensor signals
```

### Signal Routing

Improve:

- Encoder routing
- Grounding
- Signal separation
- Filtering
- Pull-up configuration

### Hardware Reliability

The next revision will prioritize:

```text
Reliable Electrical Signal
        ↓
Reliable Sensor Data
        ↓
Reliable Control
```

before further control optimization.

---

## 📚 What This Project Represents

JB_Balancing_Car represents my first serious attempt to understand a robot as a complete system.

```text
Mechanical System
       +
Electrical System
       +
Embedded Firmware
       +
Control Theory
       +
Real-Time System
       +
Debugging
```

The most valuable result of V1 was not simply whether the robot could stand.

It was learning how failures propagate through a real robotic system and how to trace those failures back to their physical root cause.

---

## 🚀 Future Development

Future development will continue through a new hardware revision:

```text
JB_board V2
      ↓
Reliable Encoder
      ↓
Velocity Loop
      ↓
Cascaded Control
      ↓
Parameter Tuning
      ↓
More Advanced Robot Control
```

The V1 repository will remain as a record of the first hardware and control iteration.

---

## 📌 Project Status

**V1.0 — Frozen**

The V1 hardware is considered a completed experimental iteration.

Future development will be transferred to the next hardware revision.

---

## 👤 Author

**JB**

Robotics / Embedded Systems / Control

GitHub: `baijiazheng`
