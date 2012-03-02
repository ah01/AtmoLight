/*
    AtmoLight with PWM output

    Receive Atmo-Classic protocol from Serial and set PWM output to given 
    AtmoLight channel
    
    "THE BEER-WARE LICENSE":
    <adam@horcica.cz> wrote this file. As long as you retain this notice you
    can do whatever you want with this stuff. If we meet some day, and you think
    this stuff is worth it, you can buy me a beer in return 
*/

// RGB LED output pins
#define LED_R 10
#define LED_G 11
#define LED_B 9

// AtmoLight RGB channels indexes 
#define CH_SUM      0
#define CH_LEFT     1
#define CH_RIGHT    2
#define CH_BOTTOM   3
#define CH_TOP      4

typedef struct {
    int R; 
    int G; 
    int B; 
} RGB;

// Received AtmoLight data
RGB data[5];


void setup()
{
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);

    Serial.begin(38400);

    set_ch(0);
}


void loop()
{
    if (readData())
    {
        set_ch(CH_SUM);
    }    
}

// wait for 1 byte from Serial
int read()
{
    while (Serial.available() == 0);
    return Serial.read();
}

// read AtmoLight packet from Serial
bool readData()
{
    // see: http://www.vdr-wiki.de/wiki/index.php/Atmo-plugin

    if (Serial.available() > 0)
    {
        if (read() == 0xFF && read() == 0x00 && read() == 0x00) // header
        {
            int count = read();
            
            if (count = 0x0F) // Atmo-Classic send 5 RGB channels (5 * 3 == 15)
            {
                for (int i = 0; i < 5; i++)
                {
                    data[i].R = read();
                    data[i].G = read();
                    data[i].B = read(); 
                }
                
                return true;
            }
        }
    }
    return false;
}

// set output color to given AtmoLight channel
void set_ch(int i)
{
    byte r = (byte)data[i].R;
    byte g = (byte)data[i].G;
    byte b = (byte)data[i].B;
    
    color(r, g, b);
} 

// set PWM output color
void color(byte r, byte g, byte b)
{
    analogWrite(LED_R, r);
    analogWrite(LED_G, g);
    analogWrite(LED_B, b);
}
