const byte LED_PIN = 13;
const byte METER_PIN = A4;

void startTimer(double timerFrequency);

double currentFrequency = 0.5; // Initial frequency of 0.5 Hz

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(METER_PIN, INPUT);
    Serial.begin(9600);

    startTimer(currentFrequency); // Start timer with initial frequency
}

void loop() {
    int potValue = analogRead(METER_PIN); // Reads the potentiometer value

    // Maps the potentiometer value to a frequency range of 0.1 Hz to 2 Hz
    double newFrequency = 0.1 + (potValue / 1023.0) * (2.0 - 0.1);

    // Updates the timer if there's a frequency change
    if (abs(newFrequency - currentFrequency) > 0.05) {
        currentFrequency = newFrequency;
        startTimer(currentFrequency);
        Serial.print("Updated Frequency: ");
        Serial.println(currentFrequency);
    }

    delay(500); // Delay
}

void startTimer(double timerFrequency) {
    noInterrupts(); // It disables the interrupts while setting the timer

    TCCR1A = 0; // Clear Timer and the Counter Control Registers
    TCCR1B = 0;

    int prescaler = 1024;
    int OCR1A_value = (16000000 / (prescaler * 2 * timerFrequency)) - 1;

    // Ensures that OCR1A does not exceed the maximum 16-bit limit
    if (OCR1A_value > 65535) {
        OCR1A_value = 65535;
    } else if (OCR1A_value < 1) {
        OCR1A_value = 1;
    }

    OCR1A = OCR1A_value;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A); // Enables Timer Compare Interrupt

    interrupts(); // Enable interrupts
}

ISR(TIMER1_COMPA_vect) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Toggle LED
}
