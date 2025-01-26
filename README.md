#Smart Classroom Power Automation ⚡

An innovative IoT-based solution to automate classroom power management, ensuring energy efficiency and real-time monitoring. 🚀
🌟 Features

    Automated Power Control: Lights and fans are activated based on student presence using RFID technology.
    Real-Time Monitoring: Live status updates for classroom strength, power usage, and device states via Grafana dashboards.
    Energy Efficiency: Minimizes unnecessary power consumption by dynamically controlling devices.
    Data Analytics: Tracks student attendance and analyzes power consumption trends stored in InfluxDB.
    Cost Estimation: Provides both estimated and actual electricity bills based on power usage.

🛠️ Technology Stack

    Hardware: ESP32, RFID scanner, and tags.
    Cloud Database: InfluxDB for storing and managing data.
    Visualization: Grafana for interactive and responsive dashboards.
    Programming: C++ for ESP32 and scripting for dashboard logic.

🎯 Key Functionalities

    Live Class Strength: Displays the current number of students in the classroom.
    Power Usage Tracking: Monitors lights and fans to calculate real-time power consumption.
    Billing Insights:
        Estimated Bill: Estimated power usage × ₹5 per unit.
        Actual Bill: Actual power consumed × ₹5 per unit.
    Attendance Tracking: Student entry data logged for analysis and reporting.

🔧 How to Set Up

    Clone the repository:

    git clone https://github.com/YourUsername/SmartClassroomPowerAutomation.git

    Flash the ESP32 with the provided code and connect the hardware components.
    Set up InfluxDB and Grafana for data storage and visualization.
    Link the Grafana dashboard to display real-time metrics.

🚀 Future Enhancements

    Integrate a camera module for enhanced student detection.
    Add predictive analytics for power usage trends.
    Implement mobile app support for remote monitoring and control.
    Expand to multi-classroom setups for institutional use.

🖼️ Screenshots

Include visuals of the Grafana dashboard and hardware setup.
