#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ================= WIFI ================= */

const char* ssid = "ANPR_ESP32";
const char* password = "12345678";

WebServer server(80);

/* ================= OLED ================= */

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

/* ================= HTML ================= */

String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ANPR Upload</title>
</head>
<body>
<h2>Upload Number Plate Image</h2>

<form method="POST" action="/upload" enctype="multipart/form-data">
<input type="file" name="image">
<input type="submit" value="Upload">
</form>

</body>
</html>
)rawliteral";

/* ================= OLED ================= */

void showText(String text)
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 10);
  display.println(text);

  display.display();
}

/* ================= ROOT ================= */

void handleRoot()
{
  server.send(200, "text/html", webpage);
}

/* ================= UPLOAD ================= */

void handleUpload()
{
  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START)
  {
    Serial.print("<IMAGE>");
  }

  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    Serial.write(upload.buf, upload.currentSize);
  }

  else if (upload.status == UPLOAD_FILE_END)
  {
    Serial.print("</IMAGE>");

    server.send(
      200,
      "text/plain",
      "Upload Complete"
    );

    String plate = "";

    unsigned long startTime = millis();

    while (millis() - startTime < 10000)
    {
      if (Serial.available())
      {
        String response =
          Serial.readStringUntil('\n');

        int start =
          response.indexOf("<PLATE>");

        int end =
          response.indexOf("</PLATE>");

        if (start != -1 && end != -1)
        {
          plate =
            response.substring(
              start + 7,
              end
            );

          break;
        }
      }
    }

    if (plate.length() > 0)
      showText(plate);
    else
      showText("No Plate");
  }
}

/* ================= SETUP ================= */

void setup()
{
  Serial.begin(115200);

  /* OLED INIT */

  Wire.begin(
    OLED_SDA,
    OLED_SCL
  );

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C))
  {
    while (true);
  }

  showText("Starting");

/* WIFI ACCESS POINT */

  WiFi.softAP(
    ssid,
    password
  );

  IPAddress IP =
    WiFi.softAPIP();

  showText(IP.toString());

/* SERVER ROUTES */

  server.on(
    "/",
    HTTP_GET,
    handleRoot
  );

  server.on(
    "/upload",
    HTTP_POST,
    [](){},
    handleUpload
  );

  server.begin();
}

/* ================= LOOP ================= */

void loop()
{
  server.handleClient();
}