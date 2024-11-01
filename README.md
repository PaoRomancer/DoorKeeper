

# ✨  ❝ DoorKeeper - กลอนประตูรักษาการอัจฉริยะ ❞ ✨

As part of the project, subject 01236254 Circuit and Electronics and subject 01236255 Introduction to Internet of Things
Semester 1, Academic Year 2024

IoT System and Information Engineering

King Mongkut's Institute of Technology Ladkrabang



## 📁 Table of Contents

  🔸 [Links](#-links) <br> 
  🔸 [Credits](#-Credits) <br> 
  🔸 [Concept and rigin](#-concept-and-origin) <br>
  🔸 [Project Overview](#-Project-Overview) <br>
  🔸 [Block Diagram](#-block-diagram) <br>
  🔸 [Component](#-Component) <br>
  🔸 [Final Product](#-final-product) <br>
  🔸 [Tech Stack](#-Future-Goals) <br>
  🔸 [Languages](#-Languages) <br>

## 🌐 Links
- Presentation Video 👉 [Click Here](https://water-quality-dashboard.vercel.app/login.html)
- My Website 👉 [Click Here](https://water-quality-dashboard.vercel.app/login.html)
- Dashboard  👉 [Click Here](https://water-quality-dashboard.vercel.app/login.html)

## 👥 Credits

### Member
- NATTHAWIT NOWANGHAN : Hardware (Device) & Cloud & User Interface
- TECHAWICH  AJJAGLAB : Create a model that can be used to simulate the project
- THANATHON  PINYOMETAKUN : Database & Cloud & User Interface  

### Thank you to the advisors (ขอขอบคุณผู้ให้คำปรึกษา)
- ดร.นัชนัยน์ รุ่งเหมือนฟ้า
- ผศ.สรพงษ์ วชิรรัตนพรกุล
- ธิติ ศรีประสงค์
- กฤษณ์ เกษมเทวินทร์
- อังศุชวาล	สมิตชาติ
- อรรถกร นาราช
- ธีรภัทร	ราชปัก
- ธนาตย์ จอมใจเอกขน
- เสฎฐวุฒิ สวัสดี
  

## ❓ Concept and origin

The DoorKeeper project was initiated out of the necessity to enhance the security of co-working spaces, particularly in branches where external individuals could freely access the area. Additionally, controlling the number of users in the room was a key factor to consider, in order to prevent overcrowding and promote a suitable working environment. With the idea of applying IoT technology, the DoorKeeper project focuses on developing an access control system using student ID cards, which helps restrict access to the area exclusively to staff members of the branch. Furthermore, the system includes a real-time room occupancy tracking feature, enabling administrators to plan space utilization more efficiently.

This project addresses the need for creating a secure and private working environment for students and faculty members, reducing the risk of theft or loss of important documents and assets. It also enhances the user experience by ensuring that there is adequate and comfortable workspace, fostering a positive work atmosphere and ultimately improving overall productivity.

![My Project Screenshot](image/image3.png) 

## 🌈 Project Overview

![My Project Screenshot](image/image2.png) 

## ✨ Block Diagram

![My Project Screenshot](image/image4.png) 

## 📍 Component

|                     image                    |            Component             |         details         | value (฿) |
| :------------------------------------------: | :-------------------------------: | :------------------------: | -------: |
| ![My Project Screenshot](image/ARD-UNO-R4W.png)|          Arduino UNO R4 WiFi      | System operation control board |   1099.00 |
|     ![RFIDRC522](image/rfidpic.png)      |        RFID Card Reader/Detector Module Kit (RC522) พร้อม Tag Card และ Tag พวงกุญแจ        |     RFID card reader board     |    50.00 |
|   ![RELAYMODULE](image/relaypic.png)    |           Relay 5V 1 Channel Isolation High And Low Trigger relay            |  Door control relay board  |    30.00 |
| ![SOLENIOIDLOCK](image/jfpic.png)  |           JF-0826B 12V กลอนไฟฟ้า กลอนประตูไฟฟ้า JF-0826B แม่เหล็กไฟฟ้าโซลินอยด์แม่เหล็กไฟฟ้า          |          Electric door lock          |   120.00 |
|        ![Buzzer](image/buzzerpic.png)         |              Buzzer               |  The board sends a notification sound.    |     5.00 |
|   ![POWERSUPPLY](image/dcpic.png)    |  Power Supply (220VAC -> 12VDC)   |    Power converter board     |   280.00 |
|           ![PCB](image/Breadboardpic.png)            |                Breadboard                |       บอร์ดทดลอง Breadboard 400 holes       |    24.00 |
|        ![BUTTON](image/buttonpic.png)        |              Button               |           reset round button switch           |     7.00 |
|      ![TERMINAL](image/irpic.png)       |          E18-D80NK Photoelectric Sensor Infrared Reflectance Proximity Sensor for Arduino           |         Count the number of people          |    85.00 |
|      Total (excluding shipping and other costs)       |                                   |                            |   1700.00 (Baht) |

## 🤩 Final Product

![My Project Screenshot](image/finalpj.png) 

## 🚀 Future Goals

#### Face Recognition Integration:
Integrate a camera module with face recognition software (such as OpenCV) to add an additional layer of security. This could help confirm identity beyond just the RFID scan, improving security for sensitive areas.

#### Hall magnetic Sensor Module:
Currently we are using delay to command the electric lock to lock, in the future we will develop using magnetic sensor.

## 💻 Languages
- [C++](https://en.wikipedia.org/wiki/C%2B%2B)
