const int microphonePin = A0;

float realVal[50];
int realValIndex = 0;



void setup() {
  Serial.begin(9600);
}

void loop() {

  record_audio();

}


void record_audio(){

  int mn = 1024;
  int mx = 0;

  for (int i = 0; i < 100; ++i) {

    int val = analogRead(microphonePin);
    
    mn = min(mn, val);
    mx = max(mx, val);
  }

  int delta = mx - mn;

  realVal[realValIndex] = delta;
  realValIndex++;
  if(realValIndex > 49){
    realValIndex = 0;
  }

  Serial.print("Delta:");
  Serial.println(delta);

}