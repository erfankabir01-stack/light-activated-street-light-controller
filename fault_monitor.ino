/*
  EEE234 Project 5 - Street Light Fault Monitor
  ================================================
  Stage 1: Lamp Monitoring
  Stage 2: LED (status indicator) Monitoring
  Stage 3: Sensor (LDR) Fault Detection

  Stage 1 detects a burnt-out / open-circuit lamp: supply voltage
  is present at L1, but no current is actually flowing.

  Stage 2 applies the identical idea to D2 (red, night indicator)
  and D3 (green, day indicator): each LED should only be dark when
  it is NOT commanded on. If it is commanded on but no current
  flows through it, that LED itself has failed.

  Stage 3 is different: the LDR has no independent second signal to
  compare against, so instead we check whether node A sits at a
  voltage a genuine LDR could ever physically produce. A real LDR's
  resistance is always finite, so node A can approach the rails but
  never quite touch them. A reading pinned right at an extreme means
  the sensor itself has failed open or shorted, not that it is truly
  reporting an extreme light level.

  Hardware:
    A0 (ADC0) = voltage across Rsense (10 ohm), lamp current sense.
    A1 (ADC1) = L1 supply voltage, via 10k/4.7k divider.
    A2 (ADC2) = voltage across R9 (100 ohm), D2 current sense.
    A3 (ADC3) = voltage across R10 (100 ohm), D3 current sense.
    A4 (ADC4) = op-amp pin 1 voltage, via 10k/4.7k divider.
                (D2's commanded state - HIGH means D2 should be lit)
    A5 (ADC5) = Q1 collector voltage, via 10k/4.7k divider.
                (D3's commanded state - HIGH means D3 should be lit)
    A6 (ADC6) = node A voltage (R1/LDR1 junction), via 10k/4.7k divider.
    Pin 8     = FAULT test output (HIGH = any fault). Probe this in
                Proteus for now; later stages will split this into
                separate per-fault outputs and drive the buzzer.
    Pin 1(TX) = diagnostic text, view on the Virtual Terminal at 9600 baud.

  Fault logic:
    LAMP_FAULT   : supply present (>6V actual) AND lamp current NOT
                   flowing (<0.30V at Rsense).
    D2_FAULT     : pin1 commanded HIGH (>6V actual) AND D2 current NOT
                   flowing (<0.30V at R9).
    D3_FAULT     : Q1 collector commanded HIGH (>6V actual) AND D3
                   current NOT flowing (<0.30V at R10).
    SENSOR_FAULT : node A below 0.5V (shorted LDR) OR above 11.5V
                   (disconnected LDR) - both outside what a real LDR
                   divider can ever produce.
*/

const int PIN_SENSE      = A0;   // lamp current sense
const int PIN_SUPPLY     = A1;   // lamp supply (divided)
const int PIN_D2_SENSE   = A2;   // D2 current sense
const int PIN_D3_SENSE   = A3;   // D3 current sense
const int PIN_D2_CMD     = A4;   // pin 1 / D2 commanded state (divided)
const int PIN_D3_CMD     = A5;   // Q1 collector / D3 commanded state (divided)
const int PIN_NODE_A     = A6;   // node A / LDR sensing voltage (divided)
const int PIN_FAULT      = 8;    // any-fault test output

const float VREF         = 5.0;
const float ADC_MAX      = 1023.0;
const float DIVIDER_GAIN = (10.0 + 4.7) / 4.7;   // undoes the 10k/4.7k divider

const float PRESENT_V      = 6.0;    // actual volts, above = "should be on"
const float CURRENT_FLOW_V = 0.30;   // sense-resistor volts, above = current flowing
const float SENSOR_LOW_V   = 0.5;    // node A below this => LDR likely shorted
const float SENSOR_HIGH_V  = 11.5;   // node A above this => LDR likely disconnected

void setup() {
  pinMode(PIN_FAULT, OUTPUT);
  Serial.begin(9600);
  Serial.println(F("Fault monitor online: Lamp + LED + Sensor stages."));
}

void loop() {
  float vSense   = analogRead(PIN_SENSE)  * (VREF / ADC_MAX);
  float vSupply  = analogRead(PIN_SUPPLY) * (VREF / ADC_MAX) * DIVIDER_GAIN;
  float vD2Sense = analogRead(PIN_D2_SENSE) * (VREF / ADC_MAX);
  float vD3Sense = analogRead(PIN_D3_SENSE) * (VREF / ADC_MAX);
  float vD2Cmd   = analogRead(PIN_D2_CMD)   * (VREF / ADC_MAX) * DIVIDER_GAIN;
  float vD3Cmd   = analogRead(PIN_D3_CMD)   * (VREF / ADC_MAX) * DIVIDER_GAIN;
  float vNodeA   = analogRead(PIN_NODE_A)   * (VREF / ADC_MAX) * DIVIDER_GAIN;

  bool lampFault   = (vSupply > PRESENT_V) && (vSense   <= CURRENT_FLOW_V);
  bool d2Fault     = (vD2Cmd  > PRESENT_V) && (vD2Sense <= CURRENT_FLOW_V);
  bool d3Fault     = (vD3Cmd  > PRESENT_V) && (vD3Sense <= CURRENT_FLOW_V);
  bool sensorFault = (vNodeA < SENSOR_LOW_V) || (vNodeA > SENSOR_HIGH_V);

  bool anyFault = lampFault || d2Fault || d3Fault || sensorFault;
  digitalWrite(PIN_FAULT, anyFault ? HIGH : LOW);

  Serial.print(F("Vsense="));   Serial.print(vSense, 2);
  Serial.print(F("V Vsupply=")); Serial.print(vSupply, 2);
  Serial.print(F("V LampFault=")); Serial.print(lampFault ? F("YES") : F("no"));

  Serial.print(F("  | D2sense=")); Serial.print(vD2Sense, 2);
  Serial.print(F("V D2cmd="));     Serial.print(vD2Cmd, 2);
  Serial.print(F("V D2Fault="));   Serial.print(d2Fault ? F("YES") : F("no"));

  Serial.print(F("  | D3sense=")); Serial.print(vD3Sense, 2);
  Serial.print(F("V D3cmd="));     Serial.print(vD3Cmd, 2);
  Serial.print(F("V D3Fault="));   Serial.print(d3Fault ? F("YES") : F("no"));

  Serial.print(F("  | NodeA="));   Serial.print(vNodeA, 2);
  Serial.print(F("V SensorFault=")); Serial.println(sensorFault ? F("YES") : F("no"));

  delay(200);
}
