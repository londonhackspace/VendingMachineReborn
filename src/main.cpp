/*
 _     _   _ ____   __     _______ _   _ ____ ___ _   _  ____   __  __    _    ____ _   _ ___ _   _ _____
| |   | | | / ___|  \ \   / / ____| \ | |  _ \_ _| \ | |/ ___| |  \/  |  / \  / ___| | | |_ _| \ | | ____|
| |   | |_| \___ \   \ \ / /|  _| |  \| | | | | ||  \| | |  _  | |\/| | / _ \| |   | |_| || ||  \| |  _|
| |___|  _  |___) |   \ V / | |___| |\  | |_| | || |\  | |_| | | |  | |/ ___ \ |___|  _  || || |\  | |___
|_____|_| |_|____/     \_/  |_____|_| \_|____/___|_| \_|\____| |_|  |_/_/   \_\____|_| |_|___|_| \_|_____|
Written to stop tgreer from throwing the vending machine away
I'm bad at programming
*/

#include <EthernetUdp.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <Ethernet.h>
#include <MySQL_Cursor.h>
#include <MySQL_Connection.h>
#include <MySQL_Packet.h>
#include <MySQL_Encrypt_Sha1.h>
#include <Energia.h>
#include <Keypad.h>
#include <PN532_HSU.h>
#include <PN532.h>
#include <SPI.h>
#include <Energia.h>
#include <Adafruit_CharacterOLED.h>
#include <HttpClient.h>
#include <Keypad.h>
#include <ArduinoJson.h>
Adafruit_CharacterOLED lcd(OLED_V2, PF_1, PF_2, PF_3, PL_0, PM_3, PL_4, PL_5);
PN532_HSU pnhsu(Serial6);
PN532 nfc(pnhsu);
volatile int cashinserted; //Used in ISR, has to be volatile
volatile boolean changed = true;
int inhibitpin = PP_3;
int Coin10Pin = PG_0; //The coin mech pulls these pins down (*AND APPARENTLY UP TO 12V SOMETIMES) if an appropriate coin is accepted
int Coin20Pin = PB_4;//Interfacing with the coin mech directly is difficult and can only be done by the PL2303 for some reason
int Coin50Pin = PB_5;//COIN MECH (Weird serial-like interface(CCTALK)) --> PL2303 --> Raspberry Pi --> Launchpad
int Coin100Pin = PK_0;
int Coin200Pin = PK_1;
int selection[] = {-1, -1}; //Sets default unselected selection
int itemtovend;
int stock[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int cost[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int restock;
int motors[] = {PH_3, PH_2, PL_3, PL_2, PL_1, PN_2, PN_3, PP_2, PH_0, PP_4, PD_4, PA_7, PP_5, PK_7, PK_6, PH_1};//Pins for all 16 Motors - easier to declare
boolean undispensed = true;
int buttonState[10];
int lastButtonState[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
unsigned long debounceDelay = 50;
int comparator1pin = PN_3;
int comparator2pin = PP_2;
int motorvalue;
long lastcardscan = 0;
byte mac_addr[] = {0x00, 0x1A, 0xB6, 0x02, 0xA2, 0x00};
char newbalance[] = "0.0";
unsigned long lastdeclare;
String currentuser;
IPAddress server_addr(123,123,123,123); // SQL SERVER
EthernetClient client;
String hostname = "abc.xyz";
IPAddress acserver(123,123,123,123);
String nodeid = "xx";
String secret = "xxxxxxxxxxxxx";
MySQL_Connection conn((Client *)&client);
MySQL_Cursor cur = MySQL_Cursor(&conn);
char user[] = "vend";              // MySQL user login username
char password[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";        // MySQL user login password
char UPDATE_BALANCE[100];
char AUTH_QUERY[100];
char BALANCE_QUERY[100];
char STOCK_QUERY[100];
char vendarr[2];
bool authstat;
long requestedbalance;
String uidstring = "0";
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {PE_4, PE_0, PE_1, PE_2};
byte colPins[COLS] = {PE_3, PD_7, PA_6};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );




String getUID(){
  String uidstring;
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if(success){
  //lcd.clear();
  //lcd.setCursor(0, 0);
  Serial.println("Found a card!");
  lastcardscan = millis();
  for (uint8_t i=0; i < uidLength; i++)
  {
    if(uid[i] < 10){
      uidstring = uidstring + "0";
    }
    uidstring = uidstring + String(uid[i], HEX);
  }
  uidstring.toUpperCase();
  return uidstring;
  }else{
  return "0";
  }
}




void updateuserbalance(String id, int money){
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(10);
  }
  else{
    Serial.println("Connection failed. (Someone might have just lost or gained money?)");
  }
  char UPDATE_BALANCE[100];
  String cashstring = String(money);
  Serial.print("CASHSTRING IS EQUAL TO");
  Serial.print(cashstring);
  Serial.println("");
  String updaterequest = "UPDATE vend.users SET BALANCE = '" + cashstring + "' WHERE ID = '" + id + "'";
  updaterequest.toCharArray(UPDATE_BALANCE, 100);
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
// Execute the query
  cur_mem->execute(UPDATE_BALANCE);
// Note: since there are no results, we do not need to read any data
// Deleting the cursor also frees up memory used
  delete cur_mem;
  conn.close();
}




int getuserbalance(String id){
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(10);
  }
  else{
    Serial.println("Connection failed.");
    return 0;
  }
  row_values *row = NULL;
  char BALANCE_QUERY[100];
  String authquery = "SELECT BALANCE FROM vend.users WHERE ID = '" + id + "'";
  authquery.toCharArray(BALANCE_QUERY, 100);
  cur.execute(BALANCE_QUERY);
// Fetch the columns (required) but we don't use them.
  if(cur.get_columns() == NULL){
    Serial.print("That Card looks INVALID");
    return 0;
  }
// Read the row (we are only expecting the one)
  do {
    row = cur.get_next_row();
    if (row != NULL) {
      requestedbalance = atol(row->values[0]);
    }
  } while (row != NULL);
// Now we close the cursor to free any memory
  cur.close();
  conn.close();
  Serial.print("you asked for a balance?");
  Serial.print(requestedbalance);
  Serial.println("");
  return int(requestedbalance);
}

void reducestock(int x, int y){
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(10);
  }
  else{
    Serial.println("Connection failed. Stock on database now out of sync. Local stock counters still ok.");
  }
  char UPDATE_STOCK[100];
  int NEWQUANTITY = y - 1;
  sprintf(UPDATE_STOCK, "UPDATE vend.stock SET quantity = '%i' WHERE place = '%i'", NEWQUANTITY, y);
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
// Execute the query
  cur_mem->execute(UPDATE_STOCK);
// Note: since there are no results, we do not need to read any data
// Deleting the cursor also frees up memory used
  delete cur_mem;
  conn.close();
}



void cash10() {
  cashinserted = cashinserted + 10;
  changed = true;
  while(digitalRead(Coin10Pin) == LOW){
  }
}
void cash20() {
  cashinserted = cashinserted + 20;
  changed = true;
  while(digitalRead(Coin20Pin) == LOW){
  }
}
void cash50() {
  cashinserted = cashinserted + 50;
  changed = true;
  while(digitalRead(Coin50Pin) == LOW){
  }
}
void cash100() {
  cashinserted = cashinserted + 100;
  changed = true;
  while(digitalRead(Coin100Pin) == LOW){
  }
}
void cash200() {
  cashinserted = cashinserted + 200;
  changed = true;
  while(digitalRead(Coin200Pin) == LOW){
  }
}
void cutmotor(){
  for(int i=0; i < 15; i++){
    digitalWrite(motors[i], LOW);
  }
}





//SELECT
void selectiondeclare(int key){//checks which
  if((millis() - lastdeclare) > 500){
    lastdeclare = millis();
    if ((selection[0] != -1) && (selection[1] != -1)){//Checks if a selection has already been made. If so, it resets the selection to "Unselected" (-1, -1), so a new selection can be made
      selection[0] = -1;
      selection[1] = -1;
    }
    if (selection[0] == -1){//If the first digit is "unselected" it assigns the appropriate key number
      selection[0] = key;
    }else if (selection[1] == -1){
      selection[1] = key;
      Serial.print("YOU HAVE POSSIBLY SUCESSFULLY SELECTED");
      itemtovend = (selection[0] * 10) + selection[1]; //Turns the list into an XX value
      Serial.print(itemtovend, DEC);
      Serial.println("");
    }
  }

}





void dispense(int sel){       //THIS PROBABLY DOESN'T WORK
  motorvalue = sel - 1;
  digitalWrite(motors[motorvalue], HIGH);
  while(digitalRead(comparator1pin) == LOW){
    digitalWrite(motors[motorvalue], HIGH);
  }
  digitalWrite(motors[motorvalue], LOW);
}






//VEND
void vend(int x, boolean y) {
  motorvalue = x - 1;
  if(motorvalue < 15){
    if(y == false){
      if(cashinserted >= cost[motorvalue]){
        Serial.println("VENDING...");
        cashinserted = cashinserted - cost[motorvalue];//Takes away money from balance
        changed = true;
        stock[motorvalue] = stock[motorvalue] - 1;
      }else if(stock[motorvalue] <= 0){
        Serial.println("OUT OF STOCK");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("OUT OF STOCK");
        changed = 1;
      }
      else{
        Serial.println("Insufficient funds");
      }
    }
}else{
  Serial.println("IDIOT");
}
}



String userinfo(String card){
  String getrequest = "GET /" + nodeid + "/card/" + card + " HTTP/1.1";
  String hostheader = "Host: " + hostname;
  String keyheader = "X-AC-Key: " + secret;
  Serial.println("Connecting");
  client.setTimeout(1000);
  if (!client.connect(acserver, 1234)) {
    Serial.println(F("Connection failed"));
    return "ERROR";
  }
  Serial.println("CONNECTED!");
  client.println(getrequest);
  client.println(hostheader);
  client.println(keyheader);
  client.println("X-AC-JSON: 1");
  client.println("Connection: close");
  client.println();
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return "ERROR";
  }
  char status[64] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.0 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return "ERROR";
  }
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return "ERROR";
  }
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonBuffer jsonBuffer(capacity);
  JsonObject& root = jsonBuffer.parseObject(client);
  if (!root.success()) {
    Serial.println(F("Parsing failed!"));
    return "ERROR";
  }
  // Extract values
  if((String(root["numeric_status"].as<char*>())) == "-1"){
    return "ERROR";
  }else{
    return String(root["user_id"].as<char*>());
  }
}




//SETUP
void setup() {
  Serial.begin(115200);
  Serial.println("I'm ALIVE!");
  //lcd.begin(16, 2);
  pinMode(Coin10Pin, INPUT_PULLDOWN);
  pinMode(Coin20Pin, INPUT_PULLDOWN);
  pinMode(Coin50Pin, INPUT_PULLDOWN);
  pinMode(Coin100Pin, INPUT_PULLDOWN);
  pinMode(Coin200Pin, INPUT_PULLDOWN);
  pinMode(inhibitpin, OUTPUT);
  attachInterrupt(Coin10Pin, cash10, RISING); //Interrupts for cash (Money NEEDS to be counted, therefore interrupts are used)
  attachInterrupt(Coin20Pin, cash20, RISING);//Without interrupts, money wouldn't be counted while the motor spins.
  attachInterrupt(Coin50Pin, cash50, RISING);
  attachInterrupt(Coin100Pin, cash100, RISING);
  attachInterrupt(Coin200Pin, cash200, RISING);
  Ethernet.begin(mac_addr);
  Serial.println("Connecting...");
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
  }
  else
    Serial.println("Connection failed.");
  for(int i=0; i < 15; i++){
    pinMode(motors[i], OUTPUT);
  }
  pinMode(comparator1pin, INPUT_PULLUP);
  pinMode(comparator2pin, INPUT_PULLUP);
  row_values *row = NULL;
  for(int i=0; i < 16; i++){
    char STOCK_QUERY[100];
    int j = i + 1;
    sprintf(STOCK_QUERY, "SELECT quantity FROM vend.stock WHERE place = '%i'", j);
    cur.execute(STOCK_QUERY);
// Fetch the columns (required) but we don't use them.
    cur.get_columns();
// Read the row (we are only expecting the one)
    do {
      row = cur.get_next_row();
      if (row != NULL) {
        stock[i] = atol(row->values[0]);
      }
    } while (row != NULL);
// Now we close the cursor to free any memory
    cur.close();
    Serial.println("STOCK OF ");
    Serial.print(j, DEC);
    Serial.println(" = ");
    Serial.print(stock[i], DEC);
  }
  for (int i = 0; i < 17; i++) {
    char PRICE_QUERY[100];
    int j = i + 1;
    sprintf(PRICE_QUERY, "SELECT PRICE FROM vend.stock WHERE place ='%i'", j);
    cur.execute(PRICE_QUERY);
// Fetch the columns (required) but we don't use them.
    cur.get_columns();
// Read the row (we are only expecting the one)
    do {
      row = cur.get_next_row();
      if (row != NULL) {
        cost[i] = atol(row->values[0]);
      }
    } while (row != NULL);
// Now we close the cursor to free any memory
    cur.close();
    conn.close();
    Serial.println("PRICE OF ");
    Serial.print(j, DEC);
    Serial.println(" = ");
    Serial.print(cost[i], DEC);
  }
  nfc.begin();
  //nfc.setPassiveActivationRetries(0xFF);
  nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();
  digitalWrite(inhibitpin, HIGH);
}











//LOOP
void loop() {                   // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  char keypadkey = keypad.getKey();
  if(keypadkey){
    if((keypadkey == '#') && (selection[0] != -1) && (selection[1] != -1)){
      vend(itemtovend, false);
      selection[0] = -1;
      selection[1] = -1;
    }
  }

  if(changed == true){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current Balance");
    lcd.setCursor(0, 1);
    lcd.print("Bal:");
    float poundbalance = cashinserted;
    poundbalance = poundbalance / 100;
    lcd.print(String(poundbalance));
    lcd.print(" GBP"); // LCD DOES NOT SUPPORT £ SYMBOL
    Serial.println(String(cashinserted));
    changed = false;
  }
  if((digitalRead(comparator1pin) == LOW) && (digitalRead(comparator2pin) == LOW)){  //Motor is stuck?
    cutmotor();
  }

  if(((millis() - lastcardscan) > 45000) && currentuser != "-1"){//timeout of card
    digitalWrite(inhibitpin, HIGH);
    updateuserbalance(currentuser, cashinserted);
    currentuser = "-1";
    cashinserted = 0;
    changed = 1;

  }
  uidstring = getUID();
  if(uidstring != "0"){
    String currentuser = userinfo(uidstring);
    if(currentuser == "ERROR"){
      Serial.println("ACSERVER QUERY ERROR (MOST LIKELY INVALID)");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.println("ACSERVER ERROR");
      lcd.setCursor(0,1);
      lcd.println("Invalid card?");
      delay(2000);
      changed = 1;
      cashinserted = 0; //avoids bug where unautharised user gets previous user's balance
    }else{
      Serial.print("CURRENT USER ID IS:");
      Serial.println(currentuser);
      cashinserted = getuserbalance(currentuser);
      changed = 1;
      digitalWrite(inhibitpin, LOW);
    }
  }
}
