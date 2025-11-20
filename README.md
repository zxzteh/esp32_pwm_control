# ESP32 PWM Button Controller

Framework: **PlatformIO + Arduino**  
Language: **C++**

This project implements PWM LED control on ESP32 with an interrupt-driven button handler,  
a UART CLI interface, and FreeRTOS task separation.
Supported input actions: single click, double click, and long press.  
Long press enables a smooth breathing mode (auto-cycling brightness).
Little bloated code, but it's necessary for modules and hw separation.

---

## 🚀 Features

### ✔ PWM Control (0–100%, 10% steps)
- **Single click** → increase brightness by 10%  
- **Double click** → turn off (0%)  
- **Long press** → enable breathing mode  
  (automatic fade in/out between 0% and 100%, step 1%)

### ✔ Button Handling via Interrupt (ISR)
- ISR only reads the GPIO state into a `volatile` variable  
- Full logic (debounce, single/double/long press) implemented in a dedicated controller  
- Internal FSM guarantees accurate detection

### ✔ UART CLI (115200 8N1)
Supported commands:
``` Cli
SET PWM <X> // X = 0..100
GET PWM
```

### FreeRTOS Tasks (3 tasks as required)
- **UART Task**  
- **PWM/LED Task**  
- **Main/Button Task**

### Demo
<img src="img/demo.gif" alt="demo" width="400"/>
