String RunningProgramGui()
{

  //PrintAndWebOut(WebArgumentsReceivedInput );

  //Serial.println("Program is running. CHecking for goto statemets");
  CheckFOrWebVarInput();
  if (CheckFOrWebGOTO() == 1)
  {
    WaitForTheInterpertersResponse = 0;
    RunningProgram == 1;
    RunBasicTillWait();
    WaitForTheInterpertersResponse = 1;
  }


  if (refreshBranch != "")
  {

    inData = String(" goto " + refreshBranch + " ");
    WaitForTheInterpertersResponse = 0;
    ExicuteTheCurrentLine();
    runTillWaitPart2();
    RunBasicTillWait();
  }


  String WebOut = String(MobileFreindlyWidth) + String(F("<script src='WebSockets.js'></script>"))  + HTMLout;


  if (BasicDebuggingOn == 1)
  {
    Serial.println(F("Web out first"));
    Serial.println( WebOut);
    Serial.println(F("HTML out"));
    Serial.println( HTMLout);
  }

  for (int i = TotalNumberOfVariables - 1; i >= 0; i--)
  {
    delay(0);
    WebOut.replace(String(F("VARS|")) + String(i), AllMyVariables[i].getVar());
  }
  
  
  WebOut.replace(F("**graphics**"), F("<iframe src='graphics.htm' style='border:none' name='gra' width='*gwid*' height='*ghei*' scrolling='no'></iframe>"));
  WebOut.replace(F("*gwid*"),  String(GraphicsEliments[0][1]));
  WebOut.replace(F("*ghei*"),  String(GraphicsEliments[0][2]));
  

  if (BasicDebuggingOn == 1)
  {
    Serial.println( WebOut);
  }
  return WebOut;
}



void PrintAndWebOut(String itemToBePrinted)
{
  Serial.println(itemToBePrinted);
  WebSocketSend( "print~^`" + itemToBePrinted);
  itemToBePrinted.replace(' ' , char(160));
  if (HTMLout.length() < 4000)
    HTMLout = String(HTMLout + "<hr>" + itemToBePrinted);
  else
  {
    HTMLout = String(HTMLout + String(F("<hr> BUFFER TOO BIG! PROGRAM STOPPED")));
    Serial.println(F("BUFFER TOO BIG! PROGRAM STOPPED"));
    RunningProgram = 0;
  }
  return;
}


void AddToWebOut(String itemToBePrinted)
{
  WebSocketSend( "wprint~^`" + itemToBePrinted);
  itemToBePrinted.replace(' ' , char(160));
  if (HTMLout.length() < 4000)
    HTMLout = String(HTMLout + itemToBePrinted);
  else
  {
    HTMLout = String(HTMLout + String(F("<hr> BUFFER TOO BIG! PROGRAM STOPPED")));
    RunningProgram = 0;
  }
  return;
}


void SendAllTheVars()
{
  for (int i = 0; i < TotalNumberOfVariables; i++)
  {
    if (AllMyVariables[i].getName() == "") break;
    WebSocketSend( "var~^`" + String(i) + "~^`" + String(AllMyVariables[i].getVar()));
    delay(0);
    Serial.println(i);
  }
  return;
}



String BasicGraphics()
{
  String BasicGraphicsOut;
  BasicGraphicsOut = GraphicsStartCode;
  BasicGraphicsOut.replace(F("*wid*"),  String(GraphicsEliments[0][1]));
  BasicGraphicsOut.replace(F("*hei*"),  String(GraphicsEliments[0][2]));

  String GraphicsEliment;
  for (int i = 1; i <= GraphicsEliments[0][0]; i++)
  {
    if (GraphicsEliments[i][0] == 1) //For Line
    {
      GraphicsEliment = GraphicsLineCode;
    }

    if (GraphicsEliments[i][0] == 2) //For Circle
    {
      GraphicsEliment = GraphicsCircleCode;
    }


    if (GraphicsEliments[i][0] == 3) //For ELLIPSE
    {
      GraphicsEliment = GraphicsEllipseCode;
    }

    if (GraphicsEliments[i][0] == 4) //For rectangle
    {
      GraphicsEliment = GraphicsRectangleCode;
    }


    GraphicsEliment.replace(F("*x1*"),  String(GraphicsEliments[i][1]));
    GraphicsEliment.replace(F("*y1*"),  String(GraphicsEliments[i][2]));
    GraphicsEliment.replace(F("*x2*"),  String(GraphicsEliments[i][3]));
    GraphicsEliment.replace(F("*y2*"),  String(GraphicsEliments[i][4]));

    if (GraphicsEliments[i][5] == 0 ) GraphicsEliment.replace("*collor*", F("black"));
    if (GraphicsEliments[i][5] == 1 ) GraphicsEliment.replace("*collor*", F("Navy"));
    if (GraphicsEliments[i][5] == 2 ) GraphicsEliment.replace("*collor*", F("Green"));
    if (GraphicsEliments[i][5] == 3 ) GraphicsEliment.replace("*collor*", F("Teal"));
    if (GraphicsEliments[i][5] == 4 ) GraphicsEliment.replace("*collor*", F("Maroon"));
    if (GraphicsEliments[i][5] == 5 ) GraphicsEliment.replace("*collor*", F("Purple"));
    if (GraphicsEliments[i][5] == 6 ) GraphicsEliment.replace("*collor*", F("Olive"));
    if (GraphicsEliments[i][5] == 7 ) GraphicsEliment.replace("*collor*", F("Silver"));
    if (GraphicsEliments[i][5] == 8 ) GraphicsEliment.replace("*collor*", F("Gray"));
    if (GraphicsEliments[i][5] == 9 ) GraphicsEliment.replace("*collor*", F("Blue"));
    if (GraphicsEliments[i][5] == 10 ) GraphicsEliment.replace("*collor*", F("Lime"));
    if (GraphicsEliments[i][5] == 11 ) GraphicsEliment.replace("*collor*", F("Aqua"));
    if (GraphicsEliments[i][5] == 12 ) GraphicsEliment.replace("*collor*", F("Red"));
    if (GraphicsEliments[i][5] == 13 ) GraphicsEliment.replace("*collor*", F("Fuchsia"));
    if (GraphicsEliments[i][5] == 14 ) GraphicsEliment.replace("*collor*", F("Yellow"));
    if (GraphicsEliments[i][5] == 15 ) GraphicsEliment.replace("*collor*", F("White"));
    BasicGraphicsOut += GraphicsEliment;
  }
  BasicGraphicsOut += "</svg>";
  return BasicGraphicsOut;
}








byte CheckFOrWebGOTO()
{
  String bla;
  byte x = 0;

  for (int i = 0; i <= TotalNumberOfLines - 1; i++)
  {
    int str_len = String(i).length() + 1 + 4;
    char ArgumentToTest[str_len];
    String(String("goto" + String(i))).toCharArray(ArgumentToTest, str_len);
    delay(0);
    bla = server->arg(ArgumentToTest);
    if (bla.length() > 0)
    {
      x = i;
    }
  }

  //Serial.println(x);
  if (x != 0)
  {
    for (int i = 0; i <= TotalNumberOfLines - 1; i++) {
      delay(0);

      String gotoTest = BasicProgram(i);
      gotoTest.trim();

      if (gotoTest == ButtonsInUse[x] | String(gotoTest + ":") == ButtonsInUse[x])
      {
        //Serial.println("This is the line I am going to");
        //Serial.println(BasicProgram(i));
        RunningProgramCurrentLine = i - 1;
        return 1;
      }
    }

  }

  WaitForTheInterpertersResponse = 1;
}







void CheckFOrWebVarInput()
{
  String bla;


  for (int i = 0; i < TotalNumberOfVariables; i++)
  {
    int str_len = String(i).length() + 1;
    char ArgumentToTest[str_len];
    String(i).toCharArray(ArgumentToTest, str_len);
    delay(0);
    bla = server->arg(ArgumentToTest);
    if (bla.length() > 0)
    {
      bla.replace('+', ' ');
      AllMyVariables[i].setVar(GetRidOfurlCharacters(bla));
    }
  }
  return;
}




String GenerateIDtag(String TempateString)
{
  LastElimentIdTag = String(millis());
  TempateString.replace(F("myid"), LastElimentIdTag );
  return TempateString;
}




String RequestWebSocket(String Request)
{
  WebSocketSend(  Request);
  WebSockMessage = "";
  for (int i = 0; ((i < 5) && (WebSockMessage == "")); i++) // wait for the answer
  {
    webSocket.loop();
    delay(0);
  }
  return WebSockMessage;
}


void WebSocketSend(String MessageToSend)
{
  for (byte i = 0; i <= 5; i++)
  {
    if (WebSocketTimeOut[i] + 60000 >=  millis()) webSocket.sendTXT(i, MessageToSend);
    delay(0);
  }
}




void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.print(num);
      Serial.println(" winsock Disconnected!");
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.print(num);
        Serial.print(" winsock connected ");
        Serial.println(ip.toString());
        // send message to client
        WebSocketSend( "Connected");
      }
      break;
    case WStype_TEXT:
      if (lenght == 0)
      {
        WebSockMessage == "";
        break;
      }
      WebSockMessage = String((char*)payload);
      if (WebSockMessage == "OK") {
        WebSocketTimeOut[num] = millis();
        break;
      }

      // Serial.print(num);
      // Serial.print(" get text ");

      Serial.println(WebSockMessage);

      if (WebSockMessage == F("cmd:stop"))
      {
        HaltBasic("");
        break;
      }
      if (WebSockMessage == F("cmd:run"))
      {
        String WebOut;
        RunningProgram = 1;
        RunningProgramCurrentLine = 0;
        WaitForTheInterpertersResponse = 0 ;
        numberButtonInUse = 0;
        HTMLout = "";
        TimerWaitTime = 0;
        TimerCBtime = 0;
        GraphicsEliments[0][0] = 0;
        WebOut = F(R"=====(  <meta http-equiv="refresh" content="0; url=./input?" />)=====");

        clear_stacks();
        server->send(200, "text/html", WebOut);
        break;
      }
      if (WebSockMessage == F("cmd:pause"))
      {
        RunningProgram = 0;
        //WaitForTheInterpertersResponse = 1;
        break;
      }
      if (WebSockMessage == F("cmd:continue"))
      {
        RunningProgram = 1;
        //WaitForTheInterpertersResponse = 0;
        break;
      }


      if (WebSockMessage.startsWith(F("guievent:")))
      {
        Serial.println(WebSockMessage.substring(9));
        break;
      }
      if (WebSockMessage == F("vars"))
      {
        SendAllTheVars();
        break;
      }
      if (WebSockMessage.startsWith(F("guicmd:")))
      {
        Serial.println(WebSockMessage.substring(7));
        RunningProgram = 1;
        WaitForTheInterpertersResponse = 0;
        RunningProgramCurrentLine = WebSockMessage.substring(7).toInt() - 1;
        Serial.println("Current line = " + String(RunningProgramCurrentLine));
        runTillWaitPart2();
        break;
      }
      if (WebSockMessage.startsWith(F("guichange:")))
      {
        Serial.println(getValue(WebSockMessage, ':', 1).toInt());
        Serial.println(getValue(String(WebSockMessage + ":"), ':', 2));
        AllMyVariables[getValue(WebSockMessage, ':', 1).toInt()].setVar(getValue(WebSockMessage, ':', 2));
        WebSocketSend( "var~^`" + String(getValue(WebSockMessage, ':', 1).toInt()) + "~^`" + String(AllMyVariables[getValue(WebSockMessage, ':', 1).toInt()].getVar()));
        break;
      }



      if (WebSockMessage.startsWith("event:"))
      {
        if (WebSockEventBranchLine > 0)
        {
          WebSockEventName = WebSockMessage.substring(6);
          // if the program is in wait, it returns to the previous line to wait again
          return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
          WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
          RunningProgramCurrentLine = WebSockEventBranchLine + 1; // gosub after the WebSockEventBranch label
          WebSockEventBranchLine = - WebSockEventBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
        }
        WebSocketSend(  "_");
        break;
      }

      if (WebSockMessage.startsWith("change:"))
      {
        if (WebSockChangeBranchLine > 0)
        {
          WebSockChangeName = WebSockMessage.substring(7);
          // if the program is in wait, it returns to the previous line to wait again
          return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
          WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
          RunningProgramCurrentLine = WebSockChangeBranchLine + 1; // gosub after the WebSockChangeBranch label
          WebSockChangeBranchLine = - WebSockChangeBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
        }
        WebSocketSend( "_");
        break;
      }
      break;
  }
}

