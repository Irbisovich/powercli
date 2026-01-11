String command = "";
String logins[][6] = {{"guest", "NULL", "0"}, {"admin", "1234", "1"}};
String info[][2] =  {{"name", "POWERcli"}, {"release date", "11-01-2026"}, {"version", "Alpha, v1.0.0"}, {"mode", "unknown"}};
String curr_user = "not logined";

void setup() {
  Serial.begin(9600);

  Serial.print("POWERcli v1.0 (11.01.2026)\n\rUnknown mode. Print help to list all commands\n\r");
  Serial.print("\n\r (" + curr_user + ") > ");
}

String input_string(String line) {
  Serial.print("\n\r" + line);
  String totalline = "";

  while (true) {
    if (Serial.available()) {
      char letter = Serial.read();

      if (int(letter) == 13) {
        break;
      } else if (int(letter) == 8) {
        totalline.remove(totalline.length() - 1);
        Serial.print("\b \b");
      } else {
        Serial.print(letter);
        totalline = totalline + letter;
      }
    }
  }

  return String(totalline);
}

void(* reset) (void) = 0;

void processcmd(String command) {
  Serial.print("\n\r");

  String action = command.substring(0, command.indexOf(" "));
  if (action == "help") {
    Serial.print("\n\r help / Returns list of all commands.\n\r login / Login into user account.\n\r reboot / Reboot device.\n\r userlist / Returns list of all users.\n\r shutdown / Shutdown device.\n\r info / Returns info about CLI.");
  } else if (action == "login") {
    String username = command.substring(action.length() + 1);
    String password = input_string(" (" + username + " / pass) > ");
    
    for (int i = 0; i < sizeof(logins) / sizeof(logins[0]); i++) {
      if (logins[i][0] == username and (logins[i][1] == password or logins[i][1] == "NULL")) {
        curr_user = username;
      }
    }
  } else if (action == "reboot") {
    reset();
  } else if (action == "shutdown") {
    Serial.print("\n\r\n\rYou can now shutdown your device safely.\n\r");

    while (true) {
      delay(1);
    }
  } else if (action == "userlist") {
    for (int i = 0; i < sizeof(logins) / sizeof(logins[0]); i++) {
      Serial.print("\n\r " + logins[i][0] + " / " + ((logins[i][1] == "NULL") ? "don\'t need PASS" : "need PASS") + " / " + logins[i][2]);
    }
  } else if (action == "info") {
    for (int i = 0; i < sizeof(info) / sizeof(info[0]); i++) {
      Serial.print("\n\r " + info[i][0] + " / " + info[i][1]);
    }
  } else {
    Serial.print("\n\rUnknown command \"" + action + "\".");
  }

  Serial.print("\n\r");
}

void loop() {
  if (Serial.available()) {
    char letter = Serial.read();

    if (int(letter) == 13) {
      processcmd(command);
      Serial.print("\n\r (" + curr_user + ") > ");
      command = "";
    } else if (int(letter) == 8) {
      command.remove(command.length() - 1);
      Serial.print("\b \b");
    } else {
      Serial.print(letter);
      command = command + letter;
    }
  }
}
