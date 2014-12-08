#include <Adafruit_NeoPixel.h>

#define BUTTON_PAINT 0
#define BUTTON_PREVIEW 4
#define BUTTON_WHEEL 2
#define NEOPIXEL_OUTPUT 1
#define DIAL_INPUT 3
#define DIAL_INPUT_ANALOG 3

Adafruit_NeoPixel rgbLight = Adafruit_NeoPixel(1, NEOPIXEL_OUTPUT, NEO_RGB + NEO_KHZ800);

void setup() {
  pinMode(BUTTON_PAINT, INPUT);
  pinMode(BUTTON_PREVIEW, INPUT);
  pinMode(BUTTON_WHEEL, INPUT);
  pinMode(DIAL_INPUT, INPUT);
  pinMode(NEOPIXEL_OUTPUT, OUTPUT);
  
  rgbLight.begin();
  rgbLight.show();
}

void loop() {
  int analogReading = 0;
  int hue = 0;
  
  if(!digitalRead(BUTTON_PAINT)) {
    // regular paint mode
    analogReading = analogRead(DIAL_INPUT_ANALOG);
    hue = map(analogReading, 0, 1023, 0, 360);
    solidColour(hue, 255);
  } else if(!digitalRead(BUTTON_PREVIEW)) {
    // preview mode, less bright
    analogReading = analogRead(DIAL_INPUT_ANALOG);
    hue = map(analogReading, 0, 1023, 0, 360);
    solidColour(hue, 160);
  } else if(!digitalRead(BUTTON_WHEEL)) {
    // wheel mode, dial controls speed of colour wheel
    analogReading = analogRead(DIAL_INPUT_ANALOG);
    analogReading = map(analogReading, 0, 1023, 5, 100);
    colourWheel(analogReading);
  } else {
    // light off
    rgbLight.setPixelColor(0, rgbLight.Color(0, 0, 0));
  }
  
  rgbLight.show();
}

void solidColour(int hue, unsigned char value) {
  int rgb[3];

  getRGB(hue, 255, value, rgb);
  rgbLight.setPixelColor(0, rgbLight.Color(rgb[0], rgb[1], rgb[2]));
}

void colourWheel(int delayTime) {
  static unsigned long previousMillis = 0;
  static int hue = 0;
  unsigned long currentMillis = millis();
  int rgb[3];
    
  if(currentMillis - previousMillis > delayTime) {
    previousMillis = currentMillis;

    hue += 1;
    hue %= 360;

    getRGB(hue, 255, 255, rgb);
    rgbLight.setPixelColor(0, rgbLight.Color(rgb[0], rgb[1], rgb[2]));
  }
}

const byte dim_curve[] = {
    0,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,
    6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,
    8,   8,   9,   9,   9,   9,   9,   9,   10,  10,  10,  10,  10,  11,  11,  11,
    11,  11,  12,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,
    15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,
    20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  25,  26,  26,
    27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  32,  33,  33,  34,  35,  35,
    36,  36,  37,  38,  38,  39,  40,  40,  41,  42,  43,  43,  44,  45,  46,  47,
    48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
    63,  64,  65,  66,  68,  69,  70,  71,  73,  74,  75,  76,  78,  79,  81,  82,
    83,  85,  86,  88,  90,  91,  93,  94,  96,  98,  99,  101, 103, 105, 107, 109,
    110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
    146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
    193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255,
};

void getRGB(int hue, int sat, int val, int colors[3]) { 
  /* convert hue, saturation and brightness ( HSB/HSV ) to RGB
     The dim_curve is used only on brightness/value and on saturation (inverted).
     This looks the most natural.      
  */
 
  val = dim_curve[val];
  sat = 255-dim_curve[255-sat];
 
  int r;
  int g;
  int b;
  int base;
 
  if (sat == 0) { // Acromatic color (gray). Hue doesn't mind.
    colors[0]=val;
    colors[1]=val;
    colors[2]=val;  
  } else  { 
 
    base = ((255 - sat) * val)>>8;
 
    switch(hue/60) {
    case 0:
        r = val;
        g = (((val-base)*hue)/60)+base;
        b = base;
    break;
 
    case 1:
        r = (((val-base)*(60-(hue%60)))/60)+base;
        g = val;
        b = base;
    break;
 
    case 2:
        r = base;
        g = val;
        b = (((val-base)*(hue%60))/60)+base;
    break;
 
    case 3:
        r = base;
        g = (((val-base)*(60-(hue%60)))/60)+base;
        b = val;
    break;
 
    case 4:
        r = (((val-base)*(hue%60))/60)+base;
        g = base;
        b = val;
    break;
 
    case 5:
        r = val;
        g = base;
        b = (((val-base)*(60-(hue%60)))/60)+base;
    break;
    }
 
    colors[0]=r;
    colors[1]=g;
    colors[2]=b; 
  }   
}
