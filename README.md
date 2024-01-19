# ESP32-MCP2515-OBD2-CANBUS
This repository contains code and documentation for a vehicle diagnostics tool that interfaces with the On-Board Diagnostics II (OBD-II) system using an MCP2515 CAN Controller and an ESP32 microcontroller. The project aims to read real-time vehicle data such as speed, engine RPM, and coolant temperature directly from the vehicle's OBD-II port.

Key Features:

    Data Acquisition: Reads real-time data from a vehicle’s OBD-II system.
    Vehicle Parameters: Extracts vehicle speed, engine RPM, and coolant temperature.
    MCP2515 CAN Controller: Utilizes the MCP2515 for efficient CAN bus communication.
    Arduino Integration: Leverages an Arduino board for processing and control.
    SPI Communication: Employs SPI protocol for microcontroller communication.

Hardware Components:

    MCP2515 CAN Controller Module
    ESP32 Board
    OBD-II Connector
    Necessary wiring and power supply
**Description:**

The Controller Area Network (CAN) bus is an integral communication system in automotive and industrial applications, operating as a multi-master, message-oriented protocol that enables various microcontrollers and devices to communicate without a host computer. Designed to operate at speeds up to 1 Mbps, the CAN bus utilizes differential signaling across two wires, enhancing signal integrity and reducing susceptibility to noise. Communication on the CAN bus is message-oriented, with each message having a unique identifier determining its priority, ensuring that higher priority messages get bus access first, which is crucial in time-sensitive applications. Devices on the network monitor the bus for messages, sending when the bus is idle and receiving messages relevant to them, thanks to the unique identifier system. The protocol's built-in error checking mechanisms ensure data integrity, even in harsh electrical environments, and its capacity for real-time communication is a significant advantage in automotive systems where rapid data transmission is essential. Furthermore, the CAN bus system significantly reduces the complexity and weight of wiring harnesses in vehicles, as it negates the need for direct connections between every device. This not only simplifies the vehicle’s electrical system but also enhances reliability and efficiency. Understanding the CAN bus system is crucial for engineers, developers, and hobbyists working in automotive and industrial environments, as it underpins the reliable and efficient communication between various electronic control units (ECUs) and devices, playing a pivotal role in the modern era of interconnected and intelligent machines.

**About communicating with car using microcontroler:**

The code communicates with a vehicle's OBD-II system via the CAN bus protocol to retrieve specific diagnostic data. In OBD-II communication, specific data points are requested using Parameter IDs (PIDs). Each PID corresponds to a particular piece of data, such as engine RPM or vehicle speed. In the code, PIDs 0x0C (for engine RPM) and 0x0D (for vehicle speed) are used.

**Sending Data over CAN Bus:**

To request data, the code constructs and sends a message over the CAN bus. This is done using the CAN.beginPacket method to start the message, followed by a series of CAN.write calls to add the necessary bytes to the message, and finally CAN.endPacket to send the message. The message structure for OBD-II queries is as follows:

    Number of additional bytes being sent.
    The mode of operation, which is 0x01 for "Show Current Data."
    The PID of the data being requested.

For example, to request engine RPM:

CAN.beginPacket(0x7df, 8);

CAN.write(0x02); // number of additional bytes

CAN.write(0x01); // mode "Show Current Data"

CAN.write(0x0C); // PID for engine RPM

CAN.endPacket();

Receiving Data over CAN Bus:

After sending a request, the code waits for a response using the CAN.parsePacket function, which reads incoming messages. The response message will contain the requested data, but it must first be validated and parsed. The validation checks include:

    Ensuring the message length is correct.
    Checking that the mode in the response is 0x41, which indicates a response to a "Show Current Data" request.
    Verifying the PID in the response matches the requested PID.

Once the message is validated, the data bytes can be extracted, and any necessary calculations are performed to convert the raw data into a human-readable format. For engine RPM, the formula ((A×256)+B)/4((A×256)+B)/4 is used, where A and B are the two data bytes in the response.
