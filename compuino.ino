#include "util.h"
#include "engine.h"

Engine* engine;

/*
 LOG_LEVEL_SILENT  
 LOG_LEVEL_FATAL   
 LOG_LEVEL_ERROR   
 LOG_LEVEL_WARNING 
 LOG_LEVEL_NOTICE  
 LOG_LEVEL_TRACE   
 LOG_LEVEL_VERBOSE 
*/

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  Log.begin(LOG_LEVEL_TRACE, &Serial);
  Log.trace(F("Starting..." CR));
  engine = new Engine();
  #ifdef MEMORY_FREE_H
    Log.notice(F("Free memory in Arduino : %u" CR), freeMemory());
  #endif

};

void loop() {
  engine->loop();
};
