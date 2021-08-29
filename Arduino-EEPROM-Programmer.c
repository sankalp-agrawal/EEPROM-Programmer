int aData = 2;
int aClock = 3;
int aLatch = 4;
int A8 = 5;
int A9 = 6;
int A10 = 7;
int iData = 8;
int iClock = 9;
int iLatch = 10;
int writeEnable = 11;
int outputEnable = 12;

//Functions:
long D_FunctionSet = 56;
long D_Control = 14;
long D_EntryMode = 6;
long D_Letters[52] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
long D_Clear = 1;
long D_Zeros = 0;
long D_punctuation[] = {33,34,40,41,44,46,63,32};
long D_Exclamation = D_punctuation[0];
long D_Quotation = D_punctuation[1];
long D_L_Parenthesis = D_punctuation[2];
long D_R_Parenthesis = D_punctuation[3];
long D_Comma = D_punctuation[4];
long D_Period = D_punctuation[5];
long D_Question = D_punctuation[6];
long D_Space = D_punctuation[7];
long D_Nextline = 192;
long Halt = 255;


void setup() {
  pinMode(aData, OUTPUT);
  pinMode(aClock, OUTPUT);
  pinMode(aLatch, OUTPUT);
  pinMode(A8,OUTPUT);
  pinMode(A9,OUTPUT);
  pinMode(A10,OUTPUT);
  pinMode(iData,OUTPUT);
  pinMode(iClock,OUTPUT);
  pinMode(iLatch,OUTPUT);
  pinMode(writeEnable,OUTPUT);
  pinMode(outputEnable,OUTPUT);
  digitalWrite(outputEnable, HIGH);
  digitalWrite(outputEnable, HIGH);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(outputEnable, HIGH);
  WriteROM();
  //WriteEEPROM2();
  digitalWrite(outputEnable, HIGH);
  Serial.println("Finished");
  while (1) {
  }
}

void WriteROM () {
  writeLoop(0,2047,0); //Sets all addresses to resting
  //dataWrite(1, D_Clear);
  dataWrite(2, D_FunctionSet);
  dataWrite(3, D_Control);
  dataWrite(4, D_EntryMode);
  char string[] = "Hello World!";
  int i;
  int f;
  if (strlen(string)<16) {
    for (i=0;i<strlen(string);i++) {
      int x = string[i]-'0';
      x = x+48;
      dataWrite(i+5,x);
    }
    dataWrite(i+5, D_Zeros);
    dataWrite(i+6, Halt);
  } else {
    for (i=0;i<16;i++) {
      int x = string[i]-'0';
      x = x+48;
      dataWrite(i+5,x);
    }
    Serial.println("First line done");
    dataWrite(i+5, D_Nextline);
    for (i=16;i<strlen(string);i++) {
      int x = string[i]-'0';
      x = x+48;
      dataWrite(i+6,x);
    }
    dataWrite(i+6, D_Zeros);
    dataWrite(i+7, Halt);
  }
}

void WriteEEPROM1 () {
  writeLoop(0,2047,219); //Sets all addresses to resting
  writeLoop(0,255,75); //First Step (000). Sets CO and RI.
  writeLoop(256,511,211); //Second Step (001). Sets RO
  writeLoop(1792,2047,251);
  dataWrite(512+D_FunctionSet,211); // Display Function Set (D_FunctionSet). Sets RO
  dataWrite(512+D_Control,211); // Display on/off control (D_Control). Sets RO
  dataWrite(512+D_EntryMode,211); // Display Entry Mode Set (D_EntryMode). Sets RO
  dataWrite(512+D_Clear,211); // Display Clear (D_Clear). Sets RO
  dataWrite(512+D_Zeros,211); // Display Zeros (D_Zeros). Sets RO
  dataWrite(512+D_Nextline,211); // Display Next Line (D_Nextline). Sets RO
  
  writeLoop(D_Letters[0]+512,D_Letters[25]+512,211); // Display Letters Uppercase. Sets RO
  writeLoop(D_Letters[26]+512,D_Letters[51]+512,211); // Display Letters Lowercase. Sets RO
  for (long a = 0; a < 8; a++) { 
    dataWrite(512+D_punctuation[a],211); //Display punctuation. Sets RO
  }
}

void WriteEEPROM2 () {
  writeLoop(0,2047,194); //Sets all addresses to resting
  //writeLoop(0,255,194);
  writeLoop(256,511,66); //Second Step (001). Sets II
  
  dataWrite(512+D_FunctionSet,130); // Display Function Set (D_FunctionSet). Sets DI
  dataWrite(1024+D_FunctionSet,198); // Display Enable Pulse. Pulses E
  
  dataWrite(512+D_Control,130); // Display on/off control (D_Control). Sets DI
  dataWrite(1024+D_Control,198); // Display Enable Pulse. Pulses E
  
  dataWrite(512+D_EntryMode,130); // Display Entry Mode Set (D_EntryMode). Sets DI
  dataWrite(1024+D_EntryMode,198); // Display Enable Pulse. Pulses E
  
  dataWrite(512+D_Clear,130); // Display Clear (D_Clear). Sets DI
  dataWrite(1024+D_Clear,198); // Display Enable Pulse. Pulses E
  
  dataWrite(512+D_Nextline,130); // Display Next Line (D_Nextline). Sets DI
  dataWrite(1024+D_Nextline,198); // Display Enable Pulse. Pulses E
  
  writeLoop(D_Letters[0]+512,D_Letters[25]+512,146); // Display Letters Uppercase. Sets DI, RW=0, RS=1
  writeLoop(D_Letters[26]+512,D_Letters[51]+512,146); // Display Letters Lowercase. Sets DI, RW=0, RS=1
  writeLoop(D_Letters[0]+1024,D_Letters[25]+1024,214); // Pulses E on 100.
  writeLoop(D_Letters[26]+1024,D_Letters[51]+1024,214); // Pulses E on 100.

  for (long a = 0; a < 8; a++) { 
    dataWrite(512+D_punctuation[a],146); //Display punctuation. Sets DI, RW=0, RS=1
    dataWrite(1024+D_punctuation[a],214); //Display punctuation. Pulses E on 100.
  }
  
  dataWrite(512+Halt, 192); // Halts on step 010
}


void writeLoop(long start, long finish, long data) {
  long i;
  for (i=start;i<finish+1;i=i+1) {
    dataWrite(i,data);
    String slash = " / ";
    String arrow = " --> ";
    String output = i + slash + finish + arrow + data;
    Serial.println(output);
  }
  
}

void dataWrite(long address, long data) {
  digitalWrite(outputEnable, HIGH);
  digitalWrite(writeEnable, HIGH);
  outputAddress(address);
  dataOutput(data);
  digitalWrite(writeEnable, LOW);
  delay(10);
  digitalWrite(writeEnable, HIGH);
  delay(10);
}

void outputAddress(long address) {
  long eight = address%256;
  int first = address/256;
  aShift(eight);
  if (first >= 4) {
    digitalWrite(A10, HIGH);
  } else {
    digitalWrite(A10, LOW);
  }
  if ((first == 2)||(first == 3)||(first == 6)||(first == 7)) {
    digitalWrite(A9, HIGH);
  } else {
    digitalWrite(A9, LOW);
  }
  if ((first == 1)||(first == 3)||(first == 5)||(first == 7)) {
    digitalWrite(A8, HIGH);
  } else {
    digitalWrite(A8, LOW);
  }
}

void aShift(long eight) {
  digitalWrite(aLatch, LOW);
  shiftOut(aData, aClock, MSBFIRST, eight);
  digitalWrite(aLatch, HIGH);
}

void dataOutput(long data) {
  digitalWrite(iLatch, LOW);
  shiftOut(iData, iClock, MSBFIRST, data);
  digitalWrite(iLatch, HIGH);
}
