/*
 _     _   _ ____   __     _______ _   _ ____ ___ _   _  ____   __  __    _    ____ _   _ ___ _   _ _____
| |   | | | / ___|  \ \   / / ____| \ | |  _ \_ _| \ | |/ ___| |  \/  |  / \  / ___| | | |_ _| \ | | ____|
| |   | |_| \___ \   \ \ / /|  _| |  \| | | | | ||  \| | |  _  | |\/| | / _ \| |   | |_| || ||  \| |  _|
| |___|  _  |___) |   \ V / | |___| |\  | |_| | || |\  | |_| | | |  | |/ ___ \ |___|  _  || || |\  | |___
|_____|_| |_|____/     \_/  |_____|_| \_|____/___|_| \_|\____| |_|  |_/_/   \_\____|_| |_|___|_| \_|_____|
Written to stop tgreer from throwing the vending machine away
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
long lastvend = 0;
int inhibitpin = PP_3;
int Coin10Pin = PG_0; //The coin mech pulls these pins down if an appropriate coin is accepted
int Coin20Pin = PB_4;
int Coin50Pin = PB_5;
int Coin100Pin = PK_0;
int Coin200Pin = PK_1;
int selection[] = {-1, -1}; //Sets default unselected selection
int itemtovend;
int stock[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
String cost[] = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"};
int restock;
int motors[] = {PN_3, PK_6, PH_1, PK_6, PH_1};
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
String currentuser = "-1";
String uidstring = "-1";
IPAddress server_addr(206,189,115,163);
EthernetClient client;
String hostname = "acserver.lan.london.hackspace.org.uk";

MySQL_Connection conn((Client *)&client);
MySQL_Cursor cur = MySQL_Cursor(&conn);
char user[] = "vend";              // MySQL user login username
char password[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";        // MySQL user login password

char acserver[] = "acserver.lan.london.hackspace.org.uk";
String nodeid = "xxxxxxx";
String secret = "xxxxxxx";

char UPDATE_BALANCE[100];
char AUTH_QUERY[100];
char BALANCE_QUERY[100];
char STOCK_QUERY[100];
char vendarr[2];
bool authstat;
long requestedbalance;
bool timedout = true;
//const byte ROWS = 4;
//const byte COLS = 3;
//char keys[ROWS][COLS] = {
//  {'1','2','3'},
//  {'4','5','6'},
//  {'7','8','9'},
//  {'*','0','#'}
//};
//byte rowPins[ROWS] = {PE_4, PE_0, PE_1, PE_2};
//byte colPins[COLS] = {PE_3, PD_7, PA_6};
//Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int item1select = PE_4;
int item2select = PE_0;
int item3select = PE_1;



String getUID(){
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if(success){
    String functionuidstring;
  //lcd.clear();
  //lcd.setCursor(0, 0);
    Serial.println("Found a card!");
    lastcardscan = millis();
    for (uint8_t i=0; i < uidLength; i++)
    {
      if(uid[i] < 10){
        functionuidstring = functionuidstring + "0";
      }
      functionuidstring = functionuidstring + String(uid[i], HEX);
    }
    functionuidstring.toUpperCase();
    return functionuidstring;
  }
  return "-1";
}




void updateuserbalance(String id, int money){
  Serial.println("THE UPDATER ID = ");
  Serial.print(id);
  Serial.println("");
  if(id == "-1"){
    return;
  }
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(100);
  }
  else{
    Serial.println("Connection failed. Someone may have lost or gained money");
  }
  char UPDATE_BALANCE[100];
  String cashstring = String(money);
  Serial.print("CASHSTRING IS EQUAL TO");
  Serial.print(cashstring);
  Serial.println("");
  String updaterequest = "UPDATE vend.users SET BALANCE = '" + cashstring + "' WHERE ID = '" + id + "'";
  updaterequest.toCharArray(UPDATE_BALANCE, 100);
  Serial.print(UPDATE_BALANCE);
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
// Execute the query
  cur_mem->execute(UPDATE_BALANCE);
// Note: since there are no results, we do not need to read any data
// Deleting the cursor also frees up memory used
  delete cur_mem;
  conn.close();
}




int getuserbalance(String requestid){
  Serial.println("Balance is being requested. Connecting...");
  String requestident = requestid;
  if((requestident.toInt()) < 0){
    return 0;
  }
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(100);
  }
  else{
    Serial.println("Connection failed. (user)");
    return 0;
  }
  row_values *row = NULL;
  char BALANCE_QUERY[100];
  String authquery = "SELECT BALANCE FROM vend.users WHERE ID = '" + requestid + "'";
  authquery.toCharArray(BALANCE_QUERY, 100);
  cur.execute(BALANCE_QUERY);
// Fetch the columns (required) but we don't use them.
  if(cur.get_columns() == NULL){
    Serial.print("That Card looks invalid");
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

void reducestock(String x, String y){
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(10);
  }
  else{
    Serial.println("Connection failed. STOCK");
  }
  char UPDATE_STOCK[100];
  String stockupdatestring = "UPDATE vend.stock SET quantity = '" + x + "' WHERE place = '" + y + "'";
  stockupdatestring.toCharArray(UPDATE_STOCK, 100);
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
// Execute the query
  cur_mem->execute(UPDATE_STOCK);
// Note: since there are no results, we do not need to read any data
// Deleting the cursor also frees up memory used
  delete cur_mem;
  conn.close();
}



void cash10() {
  //cashinserted = cashinserted + 10;
  //changed = true;
  //while(digitalRead(Coin10Pin) == LOW){
  //}
}
void cash20() {
  //cashinserted = cashinserted + 20;
  //changed = true;
  //while(digitalRead(Coin20Pin) == LOW){
  //}
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





void dispense(int sel){
  motorvalue = sel - 1;
  digitalWrite(motors[motorvalue], HIGH);
  delay(1000);
  digitalWrite(motors[motorvalue], LOW);
}






//VEND
void vend(int x, boolean y) {
  if((millis() - lastvend) > 5000){
    lastvend = millis();
    motorvalue = x - 1;
    if(motorvalue < 15){
      if(stock[motorvalue] <= 0){
        if(cashinserted >= cost[motorvalue].toInt()){
          Serial.println("VENDING...");
          Serial.println("Removing");
          Serial.println(cost[motorvalue].toInt());
          cashinserted = (cashinserted - cost[motorvalue].toInt());//Takes away money from balance
          changed = true;
          stock[motorvalue] = stock[motorvalue] - 1;
          reducestock(String(stock[motorvalue]), String(motorvalue + 1));
          dispense(x);
        }else{
          Serial.println("Insufficient funds");
        }
    }else{
      Serial.println("No Stock");
      lcd.clear();
      lcd.println("Out Of Stock");
      delay(3000);
      changed = 1;
    }
  }
}
}



String userinfo(String card){
  String getrequest = "GET /" + nodeid + "/card/" + card + " HTTP/1.1";
  String hostheader = "Host: " + hostname;
  String keyheader = "X-AC-Key: " + secret;
  Serial.println("Connecting");
  Serial.println(card);
  Serial.println("is the UID");
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
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
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
  delay(5000);
  lastvend = millis();
  Serial.println("I'm ALIVE!");
  //lcd.begin(16, 2);
  pinMode(Coin10Pin, INPUT_PULLUP);
  pinMode(Coin20Pin, INPUT_PULLUP);
  pinMode(Coin50Pin, INPUT_PULLUP);
  pinMode(Coin100Pin, INPUT_PULLUP);
  pinMode(Coin200Pin, INPUT_PULLUP);
  pinMode(item1select, INPUT_PULLUP);
  pinMode(item2select, INPUT_PULLUP);
  pinMode(item3select, INPUT_PULLUP);
  pinMode(inhibitpin, OUTPUT);
  attachInterrupt(Coin10Pin, cash10, LOW); //Interrupts for cash (Money NEEDS to be counted, therefore interrupts are used)
  attachInterrupt(Coin20Pin, cash20, LOW);//Without interrupts, money wouldn't be counted while the motor spins.
  attachInterrupt(Coin50Pin, cash50, LOW);
  attachInterrupt(Coin100Pin, cash100, LOW);
  attachInterrupt(Coin200Pin, cash200, LOW);
  Ethernet.begin(mac_addr);
  Serial.println("Connecting...");
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
  }
  else
    Serial.println("Connection failed.");
  for(int i=0; i < 5; i++){
    pinMode(motors[i], OUTPUT);
  }
  //pinMode(comparator1pin, INPUT_PULLUP);
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
  for (int i = 0; i < 16; i++) {
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
        cost[i] = String(row->values[0]);
      }
    } while (row != NULL);
// Now we close the cursor to free any memory
    cur.close();
    Serial.println("PRICE OF ");
    Serial.print(j, DEC);
    Serial.println(" = ");
    Serial.print(cost[i]);
  }
  conn.close();
  nfc.begin();
  //nfc.setPassiveActivationRetries(0xFF);
  nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();
  digitalWrite(inhibitpin, HIGH);
}











//LOOP
void loop() {                   // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  /*
  char keypadkey = keypad.getKey();
  if(keypadkey){
    if((keypadkey == '#') && (selection[0] != -1) && (selection[1] != -1)){
      vend(itemtovend, false);
      selection[0] = -1;
      selection[1] = -1;
    }
  }
  */
  //Serial.println("LOOP IS LOOPING");
  if(digitalRead(item1select) == LOW){
    vend(1, false);
  }
  if(digitalRead(item2select) == LOW){
    vend(2, false);
  }
  if(digitalRead(item2select) == LOW){
    vend(3, false);
  }
  if(changed == true){
    lastcardscan = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WELCOME TO LHS");
    lcd.setCursor(0, 1);
    lcd.print("Bal:");
    float poundbalance = cashinserted;
    poundbalance = poundbalance / 100;
    lcd.print(String(poundbalance));
    lcd.print(" GBP");
    Serial.println(String(cashinserted));
    changed = false;
  }

  //success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  //if(success){
  if((millis() - lastcardscan) > 6000 && timedout == false){
    Serial.print("TIMEOUT TRIGGERED!");
    digitalWrite(inhibitpin, HIGH);
    updateuserbalance(currentuser, cashinserted);
    currentuser = "-1";
    cashinserted = 0;
    changed = true;
    timedout = true;

  }
  if(currentuser == "-1"){
      //Serial.println("Getting UID...");
      uidstring = getUID();
      //Serial.println("Read UID as =");
      //Serial.println(uidstring);
      if(uidstring != "-1"){
        currentuser = userinfo(uidstring);
        for(int i = 0; i > 4; i++){
          currentuser = userinfo(uidstring);
          if(currentuser != "ERROR"){
            break;
          }
        }
          if(currentuser == "ERROR"){
            Serial.println("ACSERVER QUERY ERROR (MOST LIKELY INVALID)");
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.println("ACServer Error");
            lcd.setCursor(0,1);
            lcd.println("Try again.");
            delay(2000);
            changed = true;
            cashinserted = 0;
            currentuser = "-1";
          }else{
            lastcardscan = millis();
            Serial.print("CURRENT USER ID IS:");
            Serial.println(currentuser);
            cashinserted = getuserbalance(currentuser);
            Serial.println("Got the balance");
            changed = true;
            timedout = false;
            digitalWrite(inhibitpin, LOW);
      }
    }
  }
}
