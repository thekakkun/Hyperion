int led = D7;
int rate = 4800;

// Define GPRMC sentence values, excluding time, date, and checksum
String sentence = "$GPRMC,";
String validity = "A";
String latitude = "0000.00";
String hemisphere_NS = "N";
String longitude = "0000.00";
String hemisphere_EW = "E";
String speed = "0.0";
String course = "0.0";
String variation = "0.0";

String data_middle = "," + validity
    + "," + latitude
    + "," + hemisphere_NS
    + "," + longitude
    + "," + hemisphere_EW
    + "," + speed
    + "," + course
    + ",";
    
String data_end = + "," + variation
    + "," + hemisphere_EW + "*";
    


void setup()
{
    // Enable serial output
    Serial.begin(rate);
    // while(!Serial.isConnected()) Particle.process();
    Serial1.begin(rate, SERIAL_8N1);
    
    pinMode(led, OUTPUT);
}
    
void loop() 
{
    // Send time every minute at 00 seconds
    if ((Time.format(Time.now(), "%S")) == "00") {
        // Create GPRMC sentence without checksum
        String data = sentence 
            + Time.format(Time.now(), "%H%M%S")
            + data_middle
            + Time.format(Time.now(), "%d%m%y")
            + data_end;
            
        // Calculate checksum
        int checksum = 0;
        for (unsigned int i = 1; i < data.length() - 1; i++) {
            checksum ^= (byte) data[i];
        }
        
        // Join checksum to data
        data += (String(checksum, HEX)).toUpperCase();

        // Send data to serial ports (light LED to indicate sending)
        digitalWrite(led, HIGH);
        Serial1.println(data);
        Serial.println(data);
    }
    
    // Sync time every day at midnight
    if ((Time.format(Time.now(), "%H%M%S")) == "000000") {
        Particle.syncTime();
    }
    
    delay(1000);
    digitalWrite(led, LOW);
}
