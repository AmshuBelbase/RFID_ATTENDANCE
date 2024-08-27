#include <esp_task_wdt.h>  // Include the necessary header for the watchdog functions

#define WDT_TIMEOUT 3  // Timeout in seconds for the watchdog

int i = 0;
int d = 1000;
int d2 = 1000;

void setup() {
  Serial.begin(115200);

  // Initialize the watchdog timer with a 10-second timeout
  esp_task_wdt_init(WDT_TIMEOUT, true);
  // esp_task_wdt_add(NULL);  // Add the main loop to the watchdog
}

void loop() {
  // esp_task_wdt_reset();
  Serial.println("Inside Loop - Reset Watchdog Timer");
  delay(d);
  d += 2000;

  // Only reset the watchdog timer inside myFunction
  if (!myFunction()) {
    // Handle the case where myFunction fails if needed
  }
}

bool myFunction() {
  Serial.println("Inside Function - Reset Watchdog Timer");
  // Add the watchdog timer for this function
  esp_task_wdt_add(NULL);

  // Reset the watchdog timer within myFunction
  esp_task_wdt_reset();

  
  delay(d2);
  d2 += 1000; 

  // Remove the watchdog timer after the function completes
  esp_task_wdt_delete(NULL);  
  // If myFunction takes more than WDT_TIMEOUT seconds, the ESP will reset
  return false;
}
