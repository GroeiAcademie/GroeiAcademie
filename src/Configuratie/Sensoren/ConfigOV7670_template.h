#ifndef CONFIG_OV7670_H
#define CONFIG_OV7670_H

// Alleen ESP32-S3 ARDI32 en DEV; configureer de echte camerapinnen, geen standaardpinnen.
// #define SENSOR_OV7670_AANTAL 1
// OV7670 zonder FIFO: bestaande constructor met HardwareResourcePin pinnen[16].
// OV7670 + AL422B: dezelfde OV7670-constructor met HardwareResourcePin pinnen[20].
//   pinnen[0..15]: bestaande XCLK, SCCB, D0..D7, VSYNC, HREF, PCLK, PWDN, RESET.
//   pinnen[16] = WRST, [17] = RRST, [18] = WEN, [19] = RCLK.
//   cameraConfig.frame_size = FRAMESIZE_QVGA;
//   cameraConfig.pixel_format = PIXFORMAT_RGB565;
//   Alle GPIO's komen uit de feitelijke board-/modulebedrading, geen vaste pinnen.
//   AL422B-WCK fysiek aan OV7670-PCLK; AL422B-/OE actief laag; WEN actief hoog
//   volgens ArduCAM OV7670FIFO-voorbeeld. Aparte hardwareverificatie vereist.
// De bestaande OV7670::BewaarBeeldOpMicroSD("beeld.bmp") werkt voor beide routes.

#endif
