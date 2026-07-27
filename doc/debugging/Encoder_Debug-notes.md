# Encoder Feedback Corruption Caused by Motor PWM EMI

**Project:** JB_Balancing_Car_V1  
**Module:** Motor Control / Encoder Feedback System  
**Type:** Hardware Debugging Record  
**Date:** 2026-07

---

## 1. Problem Description（问题描述）

During the development of the balancing car velocity control system, abnormal encoder feedback signals were observed when driving the motors.

在加入电机驱动代码后，编码器反馈出现异常：

- Encoder position increased without physical movement.
- Encoder speed showed large values during motor stall.
- Encoder direction became unstable.
- Left and right wheel feedback occasionally showed opposite or random directions.

**Typical abnormal data:**

| Parameter | Value |
|-----------|-------|
| Speed Left | 0 |
| Speed Right | -980 |
| Tick Left | 0 |
| Tick Right | -8607 |

**However:**

- Manual rotation of the wheel produced correct encoder feedback.
- Forward and reverse rotation direction were correctly detected.
- Encoder ISR logic was verified.

> **Conclusion:** The problem was unlikely to originate from encoder decoding software.

---

## 2. Initial Hypothesis（初始猜测）

The possible causes were divided into four categories:

---

### 2.1 Software Issue

**Possible causes:**
- Incorrect interrupt configuration
- Wrong A/B phase decoding
- Counter overflow
- Incorrect direction judgment

**Verification:**
- Manual wheel rotation produced correct direction.
- Encoder ISR counter matched physical movement.

**Conclusion:** ❌ Software logic was not the root cause.

---

### 2.2 Encoder Hardware Failure

**Possible causes:**
- Magnetic encoder damage
- Signal output abnormal
- B phase missing

**Verification:**
- Without motor operation:
  - Encoder A/B signals were stable.
  - Position counting worked normally.

**Conclusion:** ❌ Encoder hardware was functional.

---

### 2.3 Power Supply Problem

Because abnormal behavior appeared during motor operation, battery voltage was checked.

A low battery condition was discovered:

| Item | Value |
|------|-------|
| 2S Li-ion battery nominal voltage | 7.4V |
| Actual voltage during debugging | ~4.6V |

**However, after charging:** The problem still existed.

**Conclusion:** ⚠️ Power instability existed, but was not the primary cause.

---

## 3. Key Experimental Observation（关键实验）

A critical relationship was discovered:

| Condition | Result |
|-----------|--------|
| Motor OFF, PWM = 0 | Encoder: Normal |
| Motor ON, PWM output active | Encoder: Incorrect |

> The encoder failure only appeared when the motor driver generated PWM switching.

**This indicated:**
- The encoder signal itself was not wrong.
- The surrounding electromagnetic environment was affecting the signal.

---

## 4. Root Cause Analysis（根因分析）

### Motor PWM Electromagnetic Interference (EMI)

The motor driver generates high-frequency switching signals:

```
TB6612 PWM:
HIGH
 |
 |
LOW
 |
 |
HIGH
```

The rapid current change produces:
- Magnetic field variation
- Electric field coupling
- Ground noise

---

### PCB Layout Problem

The PCB layout had:

```
Motor PWM trace
======================

Encoder signal trace
======================
```

> The high-current switching trace was placed close and parallel to the low-level encoder signal line.

This caused **electromagnetic coupling**.

---

### Signal Corruption Mechanism

**Real signal:**
```
0 -------- 1 -------- 0
```

**Corrupted signal:**
```
0 --1--0--1--0--1
```

The MCU interpreted these noise edges as encoder transitions.

**Therefore:**
- False interrupts occurred.
- Position accumulated incorrectly.
- Speed calculation became meaningless.
- Direction judgment became random.

---

## 5. Verification Experiments（验证实验）

---

### Experiment 1: Motor disconnected

**Result:** Encoder normal

> Confirmed that encoder hardware and software were functional.

---

### Experiment 2: Different PWM levels

**Observation:**

| Motor Power | PWM | Encoder Error |
|-------------|-----|---------------|
| Low | Small | Reduced |
| High | Large | Increased |

> This confirmed the relationship between motor switching energy and encoder disturbance.

---

### Experiment 3: Pull-up resistor improvement

Additional pull-up resistors were added:

| Resistor | Result |
|----------|--------|
| 10kΩ | Signal stability improved |
| 4.7kΩ | Further improvement |

**However:** The issue was not completely solved.

> **Conclusion:** The signal needed stronger noise immunity, but the PCB layout remained the fundamental problem.

---

## 6. Final Solution（最终解决方案）

---

### Immediate Solution

For current prototype:
1. Separate encoder signal routing from motor PWM traces.
2. Improve signal pull-up strength.
3. Add filtering capacitors where necessary.
4. Reduce unnecessary wire length.

---

### PCB V2.0 Design Improvements

#### 1. Separate Power and Signal Areas

**High current section:**
```
Battery
   |
Motor Driver
   |
Motor Output
```

**Low signal section:**
```
Encoder
IMU
Communication
```

---

#### 2. Avoid Parallel Routing

**Bad:**
```
PWM =================
ENC =================
```

**Good:**
```
PWM -------->


    (signal separation)


ENC -------->
```

> If crossing is necessary: **Cross at 90 degrees.**

---

#### 3. Add Motor Noise Suppression

Add capacitor across motor terminals:

```
Motor +
    ||
   100nF
    ||
Motor -
```

**Purpose:** Reduce brush noise and switching disturbance.

---

#### 4. Improve Encoder Signal Design

**Recommended:**
- Stronger pull-up resistor.
- Shorter signal path.
- Ground reference near signal.
- Optional RC filtering.

---

## 7. Engineering Lessons Learned（工程总结）

This debugging process changed the understanding of embedded robotics systems.

**Initially:**
> Incorrect data means incorrect code.

**After debugging:**
> Sensor data is the result of the entire physical system.

---

### The Signal Chain

```
Motor Current
     ↓
Electromagnetic Field
     ↓
PCB Layout
     ↓
Sensor Signal Quality
     ↓
MCU Input
     ↓
Control Algorithm
```

> A software error may actually originate from hardware interaction.

---

## 8. Future Design Principle（未来设计原则）

For next generation robot platforms:

---

### Hardware
- Consider EMC during PCB design.
- Separate high-current and sensitive signal paths.
- Design for signal integrity.

---

### Firmware
- Add sensor diagnostic tools.
- Monitor ISR frequency.
- Detect impossible encoder states.

---

### System Engineering

> A robot is not only code.

**It is an integrated system:**
```
Mechanical
    ↓
Electrical
    ↓
Sensors
    ↓
Firmware
    ↓
Control Algorithm
```

---

## Debug Summary

| Item | Result |
|------|--------|
| Encoder software | Normal |
| Encoder hardware | Normal |
| Battery | Contributed but not root cause |
| Motor PWM | Trigger condition |
| Root cause | PWM EMI coupling |
| Final fix | PCB layout improvement |