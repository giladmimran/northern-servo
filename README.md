# Northern Star Servo Controller

Dual-MCU embedded control system for high-payload servos, featuring push-button inputs and RS-485 communication over 15m Cat6 with PoE.

## 🏗 System Architecture

The system uses a Master-Slave dual-MCU topology to ensure reliable long-distance communication in a physical environment.
* **Communication:** RS-485 protocol over a 15-meter distance.
* **Wiring:** Cat6 cable utilizing Power over Ethernet (PoE) for streamlined power and data delivery.
* **Feedback:** I2C closed-loop feedback for precise servo positioning and error correction.

## 🔌 Hardware Setup

* **Controllers:** 2x Arduino modules (Master for input, Slave for actuation).
* **Actuators:** High-payload servos.
* **Input:** Manual push-button interface for direct user control.
* **Networking:** RS-485 transceiver modules.

## 🚀 How It Works

1. The user inputs a physical command via the push buttons on the Master MCU.
2. The Master encodes the command and transmits it via the RS-485 protocol over the Cat6 cable.
3. The Slave MCU receives the signal, decodes the instruction, and drives the heavy-duty servos.
4. The I2C feedback loop verifies that the servo has reached the target position accurately.

## 📸 Visuals

![Wiring Diagram](replace-with-your-image-link.png)
