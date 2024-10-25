# ❝ DoorKeeper - กลอนประตูรักษาการอัจฉริยะ ❞

Semester 1, Academic Year 2024
As part of the project, subject 01236254 Circuit and Electronics and subject 01236255 Introduction to Internet of Things

IoT System and Information Engineering

King Mongkut's Institute of Technology Ladkrabang

![My Project Screenshot](image/kmitlfight.png)

## 📁 Table of Contents

- [DoOrNot](#doornot)
  - [Table of Contents](#table-of-contents)
  - [Links](#links)
  - [รายชื่อสมาชิก](#รายชื่อสมาชิก)
  - [แนวคิดและที่มา](#แนวคิดและที่มา)
  - [ภาพรวมโครงงาน](#ภาพรวมโครงงาน)
  - [Block Diagram](#block-diagram)
  - [ส่วนประกอบ](#ส่วนประกอบ)
  - [Final Product](#final-product)
  - [Tech Stack](#tech-stack)
  - [Languages](#languages)

## Links

- [Dashboard](https://doornot.fakepng.dev/ui)

## รายชื่อสมาชิก

- 66010399 นายนพรุจ จิตถวิล
- 66010756 นายวริศวุธ จันทร์เชื้อ
- 66011314 นายกฤษณ์ เกษมเทวินทร์

## แนวคิดและที่มา

คณะวิศวกรรมศาสตร์สาขาระบบไอโอทีและสารสนเทศ ได้มีพื้นที่ส่วนหนึ่งเฉพาะสำหรับนักศึกษาและอาจารย์ในสาขานี้ โดยรวมไปถึงพื้นที่ทำงานส่วนกลาง หรือ Co-working space เฉพาะในสาขาที่กำลังปรับปรุงอยู่ ณ ปัจจุบัน ทางเราจึงได้มีแนวคิดที่จะทำโครงงาน DoOrNot โดยมีแนวคิดในที่จะพัฒนาความปลอดภัยและความเป็นส่วนตัวในระดับหนึ่ง ของห้อง Co-working ในสาขาของเรา จึงได้นำความรู้ทางด้าน IoT และ Circuit มาออกแบบประตูที่เฉพาะบุคคลในสาขาสามารถเข้ามาใช้งานได้โดยใช้บัตรนักศึกษาในการแสกนเข้า ห้อง Co-working ได้

## ภาพรวมโครงงาน

![image](/assets/Design.png)

## Block Diagram

![block diagram](/assets/BlockDiagram.png)

## ส่วนประกอบ

|                     รูป                      |            ส่วนประกอบ             |         รายละเอียด         | ราคา (฿) |
| :------------------------------------------: | :-------------------------------: | :------------------------: | -------: |
| ![ESP8266DevKit](/assets/ESP8266DevKit.webp) |          ESP8266 DevKit           | บอร์ดควบคุมการทำงานของระบบ |   150.00 |
|     ![RFIDRC522](/assets/RFIDRC522.jpg)      |        RFID Module (rc522)        |     บอร์ดอ่านบัตร RFID     |    55.00 |
|   ![RELAYMODULE](/assets/RELAYMODULE.jpg)    |           Relay Module            |  บอร์ดรีเลย์ควบคุมกรประตู  |    58.00 |
| ![SOLENIOIDLOCK](/assets/SOLENOIDLOCK.webp)  |           Solenoid Lock           |          กรประตู           |   190.00 |
|        ![Buzzer](/assets/BUZZER.jpg)         |              Buzzer               |   บอร์ดส่งเสียงแจ้งเตือน   |     9.00 |
|    ![DCSTEPDOWN](/assets/DCSTEPDOWN.jpg)     | DC-DC Step Down (12VDC -> 3.2VDC) |    บอร์ดแปลงกระแสไฟฟ้า     |    30.00 |
|   ![POWERSUPPLY](/assets/POWERSUPPLY.jpg)    |  Power Supply (220VAC -> 12VDC)   |    บอร์ดแปลงกระแสไฟฟ้า     |   280.00 |
|           ![PCB](/assets/PCB.jpg)            |                PCB                |       Prototype PCB        |    45.00 |
|        ![BUTTON](/assets/BUTTON.webp)        |              Button               |           ปุ่มกด           |     9.00 |
|      ![TERMINAL](/assets/TERMINAL.jpg)       |          Screw terminal           |         Connector          |    15.00 |
|      ![ACSOCKET](/assets/ACSOCKET.jpg)       |          Socket AC Power          |      Socket AC Power       |    10.00 |
|      Total (ไม่รวมค่าขนส่งและค่าอื่นๆ)       |                                   |                            |   851.00 |

## Final Product

![FINAL](/assets/FINAL.jpg)

## Tech Stack

- [VS Code](https://code.visualstudio.com/): IDE
- [PlatformIO](https://platformio.org/): Arduino compiler
- [Node.js](https://nodejs.org/en/): Javascript runtime
- [Express.js](https://expressjs.com/): Web framework for Node.js
- [Typescript](https://www.typescriptlang.org/): Strong type Javascript
- [Prisma](https://www.prisma.io/): Typescript ORM
- [PostgreSQL](https://www.postgresql.org/): Database
- [Node-RED](https://nodered.org/): Flow-based programming for IoT
- [Mosquitto](https://mosquitto.org/): MQTT broker
- [Docker](https://www.docker.com/): Containerization
- [Portainer](https://www.portainer.io/): Docker management
- [Cloudflare](https://www.cloudflare.com/): DNS, CDN, SSL
- [Cloudflare Tunnel](https://www.cloudflare.com/products/tunnel/): Expose local server to the internet
- [Nginx Proxy Manager](https://nginxproxymanager.com/): Reverse proxy
- [Proxmox](https://www.proxmox.com/en/): Hypervisor
- [GitHub](https://github.com): Version control

## Languages

- [Javascript](https://developer.mozilla.org/en-US/docs/Web/JavaScript)
- [Typescript](https://www.typescriptlang.org/)
- [C++](https://en.wikipedia.org/wiki/C%2B%2B)
