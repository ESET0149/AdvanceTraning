#include <Preferences.h>
Preferences prefs;

void setup(){
Serial.begin(115200);
prefs.begin("my-app");
int counter =prefs.getInt("counter" , 1);
Serial.print ("Reboot count:");
Serial.println(counter);
counter++;
prefs.putInt("counter", counter);


}
void loop (){

}
