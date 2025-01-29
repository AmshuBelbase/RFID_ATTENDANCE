# RFID Attendance System

## Overview
The RFID Attendance System is designed for the SRM Team Robocon to efficiently manage and track attendance using RFID technology. This system automates the process of logging attendance, reducing manual effort and improving accuracy.

## Features
- RFID-based attendance logging
- Real-time data storage and retrieval
- Secure authentication
- User-friendly interface
- Portable and efficient system

## Components Used
- **RFID Reader**: Reads RFID tags for user authentication
- **RFID Tags**: Unique identifiers assigned to team members
- **Microcontroller (ESP8266)**: Processes RFID data and communicates with the database
- **Database (Firebase)**: Stores attendance records
- **LCD Display**: Displays real-time attendance status
- **Buzzer/LED**: Provides feedback on successful or failed scans

## Installation and Setup
### 1. Hardware Setup
1. Connect the RFID reader to the microcontroller as per the datasheet.
2. Interface the LCD display and buzzer for status indication.
3. Power up the system using a suitable power source.

### 2. Software Setup
1. Install necessary libraries for RFID and microcontroller communication.
2. Upload the firmware to the microcontroller using Arduino IDE or PlatformIO.
3. Configure the database and update the firmware with the database credentials.
4. Deploy a web or mobile interface for monitoring attendance records (if applicable).

## Usage
1. Team members scan their RFID tags at the reader.
2. The system validates the tag and logs attendance in the database.
3. Feedback is provided via LCD and buzzer.
4. Attendance records can be viewed on the connected interface.

## Future Improvements
- Integration with facial recognition for multi-layer authentication.
- Cloud-based analytics for attendance trends.
- Mobile app integration for real-time notifications.

## Contributors
Developed by the SRM Team Robocon.

## License
This project is open-source and available under the MIT License.

