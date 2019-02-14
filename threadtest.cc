// threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include <string.h>
#include <ctype.h>
#include <synch.h>


//Begin code changes by Chau Cao
//Global Variable Declarations for use with Task 2: Shouting threads
//shoutArray - Constant array of cstrings for shouting purposes
//inputType - Types of input for passage into prompts function to receive users inputType
//totalShouts - assigned when valid user input for # of shouts is received.
//  Used as loop control for shout threads.
const char * shoutArray[] = {
  "Non-stop!",
  "Why do you assume you're the smartest in the room?",
  "Soon that attitude may be your doom",
  "He will never be satisfied",
  "We were meant to be satisifed",
  "What would be enough?",
  "Look around, look around!",
  "History has its eyes on you",
  "Why do you write like you're running out of time?",
  "Write day and night like you're running out of time?",
  "Every day you fight like you're running out of time",
  "I am not throwin' away my shot!",
  "I am Alexander Hamilton!"
};
const char * inputType[] = {"threads", "shouts"};
int totalShouts;


//---------------------Begin changes by Chau Cao--------------------
//Global Variables to be used with Project 2 task 4: PO Sim w/ Semaphores
//Struct declaration to define mail and post office
//Semaphores declared to protect critical resources
struct mail {
  char * message;
  int sender;
};

struct postOffice {
  int totalPeople;
  int sizeOfMailbox;
  int totalMessages;
  int messagesSent;
  int messagesRead;
  bool * deadlockMonitor;
  int deadlockCount;
  Semaphore * deadlock;
  Semaphore * writer;
  int * mailCount;
  mail ** mailbox;
  const char * messageArray[13] = {
    "Letter 1",
    "Letter 2",
    "Letter 3",
    "Letter 4",
    "Letter 5",
    "Letter 6",
    "Letter 7",
    "Letter 8",
    "Letter 9",
    "Letter 10",
    "Letter 11",
    "Letter 12",
    "Letter 13"
  };
};
Semaphore ** mailboxSemaphore;
Semaphore ** freeSpaceSemaphore;
postOffice poss;
//------------------------End Changes by Chau Cao------------------------

//----------------------------------------------------------------------

//End code changes by Chau Cao

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------



void
SimpleThread(int which)
{
    int num;


    for (num = 0; num < 5; num++) {
	       printf("*** thread %d looped %d times\n", which, num);
         currentThread->Yield();
    }
}

//Begin Code Changes by Chau Cao
//inputEval
//function that receives a string pointer to determine the type of input received
//  from the user.
/*
return values
0 = number
1 = negative number
2 = decimal number
3 = negative decimal number
4 = character array
*/
int inputEval(char *inputArr){
  int length = strlen(inputArr);
  int decimalCount = 0;
  bool isNegative = false;
  int loopCtrl = 0;

  //Following code block is used to for fringe cases involving the first character
  //if the string is only 1 char long, run isDigit.
  if(length == 1)
  {
    if(isdigit(inputArr[loopCtrl]) > 0)
    {
      return 0;
    }
    else {
      return 4;
    }
  }
  //If the first char is a '-' character, set isNegative to true and advance loopCtrl
  if(inputArr[loopCtrl] == '-')
  {
    isNegative = true;
    loopCtrl = loopCtrl + 1;
  }
  //If the first item is a decimal point, return that the string is a char array
  if(inputArr[loopCtrl] == '.')
  {
    return 4;
  }

  //All other cases, loop through the character array. If a decimal point is found
  //  increment the decimal counter. If the counter is ever higher than 1, return 4
  //  If there is a non number character found, return 4.
  else {
    while(loopCtrl < length) {
      if(inputArr[loopCtrl] == '.')
      {
        decimalCount = decimalCount + 1;
        if (decimalCount > 1) {
          return 4;
        }
        loopCtrl = loopCtrl + 1;
      }
      else if(isdigit(inputArr[loopCtrl]) == 0) {
        return 4;
      }
      else {
        loopCtrl = loopCtrl + 1;
      }
    }
    //If the function gets this far, the input is some type of number.
    //conditionals determine the type of number and return the appropriate variables
    //see chart above for reference
    if ((isNegative == true) && (decimalCount == 1)) {
      return 3;
    }
    else if ((isNegative == true) && (decimalCount == 0)) {
      return 1;
    }
    else if (decimalCount == 1) {
      return 2;
    }
    else {
      return 0;
    }
  }
}

// inputValidation
// Driver code block for task 1: Input inputValidation
// Call for the user to input some input between 1 and 1024 characters Following
// If input entered is outside the bounds of those constraints, throws an error
// Calls inputEval helper function to determine what type of input is entered
//  by the user.
void inputValidation(int qualifier) {
  char * inputArr = new char[1026]; //input buffer
  char * check;
  char typeArr[4][16] = {"number", "decimal", "negative","character array"}; //input types as cstrings
  int evaluate = 0;

  //prompt user for input and get input
  printf("Enter some input[between 1 and 1024 characters]: ");
  fgets(inputArr, 1026, stdin);

  //Following statement checks if the user provided input that exceeded the buffer,
  //or entered no inputType
  //if the input is valid, inputEval is called and the case statements
  //print out the type of input provided.
  if (((check = strchr(inputArr, '\n')) != NULL) && (inputArr[0] != '\n') && (inputArr[1025] != '\n')) {
    *check = '\0';
    evaluate = inputEval(inputArr);
    switch (evaluate) {
      case 0:
        printf("The input is a %s\n", typeArr[0]);
        break;
      case 1:
        printf("The input is a %s %s\n", typeArr[2], typeArr[0]);
        break;
      case 2:
        printf("The input is a %s %s\n", typeArr[1], typeArr[0]);
        break;
      case 3:
        printf("The input is a %s %s %s\n", typeArr[2], typeArr[1], typeArr[0]);
        break;
      case 4:
        printf("The input is a %s\n", typeArr[3]);
        break;
    }
  }
  // Exceptions for bad input.
  else {
    if(inputArr[0] == '\n')
    {
      printf("No input provided. Terminating...\n");
    }
    else {
      printf("Input provided exceeds declared maximum size. Terminating...\n");
    }
  }
  delete[] inputArr;
}

// shoutValidation
// function to determine if user input is valid for task 2
// If the input provided is not an integer between 1 and 10000 return bad input
bool shoutValidation(char *input) {
  int length = strlen(input);
  int check;
  int scope;
  if(length > 6) {
    return false;
  }
  else {
    for(int i = 0; i < length; i = i + 1) {
      if((check = isdigit(input[i])) == 0) {
        return false;
      }
    }
  }
  scope = atoi(input);
  if((scope == 0) || (scope > 10000)) {
    return false;
  }
  else {
    return true;
  }
}


// shoutPrompts
// function that prompts the user for inputType
// Passing the function a 0 asks the user for number of threads
// Passing the funcction a 1 asks the user for number of shouts.
// The user has 3 chances to provide valid input.
// The program terminates if invalid input is entered 3 times
int shoutPrompts (int promptType) {
  char * num = new char[7];
  char * check;
  bool validInput = false;
  int attempts = 0;
  int result;

  while((validInput == false) && attempts < 3) {
    printf("How many %s would you like(between 1 and 10000): ", inputType[promptType]);
    fgets(num, 7, stdin);
    if (((check = strchr(num, '\n')) != NULL) && (num[0] != '\n') && (num[7] != '\n')) {
      *check = '\0';
      validInput = shoutValidation(num);
    }
    if(validInput == false) {
      printf("The input is invalid\n");
      attempts = attempts + 1;
      if(check == NULL){
        scanf("%*[^\n]"); //2 lines of code to clear stdin input buffer. Stack overflow possibility?
        scanf("%*c");
      }
    } //clear fgets buffer
  }
  if(validInput == false)
  {
    printf("User has input invalid input three times. Terminating...");
    return 0;
  }
  else {
    result = atoi(num);
    delete num;
    return result;
  }
}

// shout
// Function that is passed to threads that prints a random shout to the console
// threads yield 2-5 times before shouting again
void shout(int identifier) {
  int randShoutIndex; //random shout index
  int randomYieldNumber; //randomly assigned number of yields
  int yieldCount; //control variable to determine how many yield executions have occured
  bool yieldController; // loop control. Thread conties to yield while false
  for(int i = 0; i < totalShouts; i++) {
    yieldCount = 0; //initialize controls to default values
    yieldController = false;
    randShoutIndex = (Random() % (12 - 0 + 1)) + 0;
    printf("Thread %d --- %s --- has shouted %d time(s) \n", identifier, shoutArray[randShoutIndex], i + 1);
    randomYieldNumber =(Random() % (5 - 2 + 1)) + 2;
    //yields until random yield number is met then breaks out of loop
    while(yieldController == false) {
      currentThread->Yield();
      yieldCount++;
      if(yieldCount == randomYieldNumber)
      {
        yieldController = true;
      }
    }
  }
}

// shoutingThreads
// Control function for task 2 that is forked to by root threads
// If valid input is provided, forks the desired amount of threads to shouts
//  the desired amount of times.
void shoutingThreads(int identifier) {
  int numThreads = 0;
  int numShouts = 0;
  printf("For the following prompts, the user has 3 attempts to input valid integers \n"); //Limiting input attempts
  numThreads = shoutPrompts(0);
  if(numThreads == 0)
  {
  }
  else {
    numShouts = shoutPrompts(1);
  }
  if(numShouts == 0) {
  }
 else {
    totalShouts = numShouts;
    for (int i = 0; i < numThreads; i = i + 1) {
      Thread *t = new Thread("Shouting thread");
      t->Fork(shout, i);
    }
    currentThread->Finish();
  }
}
//----------------------Code by Chau Cao---------------------------
//possValidation
//helper function to evaluate user inputType
//returns true if valid false otherwise
//Valid input: Integers in the range of 0 and 10000
bool possValidation(char *input) {
  int length = strlen(input);
  int check;
  int scope;
  if(length > 6) {
    return false;
  }
  else {
    for(int i = 0; i < length; i = i + 1) {
      if((check = isdigit(input[i])) == 0) {
        return false;
      }
    }
  }
  scope = atoi(input);
  if((scope == 0) || (scope > 10000)) {
    return false;
  }
  else {
    return true;
  }
}

//possInputEval
//driver function for input evaluation
//calls possValidation if the newline character is found in the buffer
//if input exceeds buffer or is invalid, the 0 is returned.
//valid input is returned in integer format
int possInputEval(char *input) {
  char * check;
  bool validInput = false;
  int result;
  if (((check = strchr(input, '\n')) != NULL) && (input[0] != '\n') && (input[10] != '\n')) {
    *check = '\0';
    validInput = possValidation(input);
  }
  if(check == NULL){
    scanf("%*[^\n]"); //2 lines of code to clear stdin input buffer. Stack overflow possibility?
    scanf("%*c");
  }
  if(validInput == false) {
    printf("The input is invalid\n");
    return 0;
  }
  else {
    result = atoi(input);
    return result;
  }
}

//possRead
//reads next message
//increments total messages read for debugging purposes
//decrements current mail counter
void possRead(int identifier) {
  mailboxSemaphore[identifier]->P();
  printf("--Person %d claims mailboxSemaphore[%d]\n", identifier, identifier);
  poss.messagesRead++;
  printf("---Person %d reads the %dth message: %s sent by Person %d --- %d messages have been read\n", identifier, poss.mailCount[identifier]- 1, poss.mailbox[poss.mailCount[identifier]-1][identifier].message, poss.mailbox[poss.mailCount[identifier]-1][identifier].sender, poss.messagesRead);
  poss.mailCount[identifier]--;
  freeSpaceSemaphore[identifier]->V();
  printf("----Person %d releases and increases freeSpaceSemaphore[%d] by 1\n", identifier, identifier);
  mailboxSemaphore[identifier]->V();
  printf("-----Person %d releases mailboxSemaphore[%d]\n", identifier, identifier);
}

//possCheckMail
//checks mailbox for mail
//if mail present, attempt to access critical resource, call read function and free resources
//yield after reading
//if maile is still present in the mailbox, continue reading until otherwise
void possCheckMail(int identifier) {
  printf("-Person %d checks - has %d letters in mailbox\n", identifier, poss.mailCount[identifier]);
  while(poss.mailCount[identifier] > 0) {
    possRead(identifier);
    printf("------Person %d yields after a read\n", identifier);
    currentThread->Yield();
  }
  return;
}

//possSendMail
//send mail to random person
void possSendMail(int identifier) {

  //initialize variables to randomly choose recipient and message
  int randomRecipient = 0;
  int randMessageIndex = 0;
  bool validRandom = false;
  mail * randomMessage = new mail;
  randomMessage->message = new char[13];
  //loop until valid random input acquired (aka not self)
  while(validRandom == false) {
    randomRecipient = (Random() % ((poss.totalPeople - 1) + 1));
    if(randomRecipient != identifier) {
      validRandom = true;
    }
  }
  randMessageIndex = (Random() % (12 - 0 + 1)) + 0;
  //insert values into message and attempt to mail
  strcpy(randomMessage->message, poss.messageArray[randMessageIndex]);
  randomMessage->sender = identifier;

  printf("--------Person %d is trying to send: %s : to Person %d \n", identifier, randomMessage->message, randomRecipient);
  printf("---------Person %d is trying to claim freeSpaceSemaphore[%d]\n", identifier, randomRecipient);
  freeSpaceSemaphore[randomRecipient]->P();
  printf("----------Person %d gets and decreases freeSpaceSemaphore[%d] by 1\n", identifier, randomRecipient);
  mailboxSemaphore[randomRecipient]->P();
  printf("-----------Person %d claims mailboxSemaphore[%d]\n", identifier, randomRecipient);

  //only one person is allowed to write at a time to not exceed the total messages alloted for simulation
  poss.writer->P();
  if(poss.messagesSent == poss.totalMessages){
    freeSpaceSemaphore[randomRecipient]->V(); //undo action of taking resource
    mailboxSemaphore[randomRecipient]->V();
  }
  else {
    poss.mailbox[poss.mailCount[randomRecipient]][randomRecipient] = *randomMessage;
    poss.mailCount[randomRecipient]++;
    printf("------------Person %d successfully sends: %s : to Person %d\n", identifier, randomMessage->message, randomRecipient);
    mailboxSemaphore[randomRecipient]->V();
    poss.messagesSent++;
    printf("-------------Person %d successfully updates the overall message count to %d\n", identifier, poss.messagesSent);
  }
  poss.writer->V();
  return;

  delete randomMessage;
}

void possPerson(int identifier) {
  int randomYieldNumber; //randomly assigned number of yields
  int yieldCount; //control variable to determine how many yield executions have occured
  bool yieldController; // loop control. Thread conties to yield while false

  //loop while messages can still be sent or there is mail to be read
  while(poss.messagesSent < poss.totalMessages || poss.mailCount[identifier] > 0){
    printf("Person %d enters the post office\n", identifier);
    possCheckMail(identifier);
    printf("-------Person %d's mailbox is empty\n", identifier);
    //will not attempt to send mail if total has been met
    if(poss.messagesSent < poss.totalMessages) {
      possSendMail(identifier);
    }
    printf("--------------Person %d leaves the post office\n", identifier);

    //if nothing to do, will not return
    if((poss.messagesSent == poss.totalMessages) && (poss.mailCount[identifier] == 0)) {
      break;
    }

    //else busy waiting 2-5 cycles to return to post office
    else {
      yieldCount = 0; //initialize controls to default values
      yieldController = false;
      randomYieldNumber =(Random() % (5 - 2 + 1)) + 2;
      //yields until random yield number is met then breaks out of loop
      while(yieldController == false) {
        printf("---------------Person %d yields the %dth cycle\n", identifier, yieldCount + 1);
        currentThread->Yield();
        yieldCount++;
        if(yieldCount == randomYieldNumber) {
          yieldController = true;
        }
      }
    }
  }
}

//postOfficeSimSem
//driver function of task 6
void postOfficeSimSem(int identifier) {

  //define buffers and controller to receive user input
  char * P = new char[7];
  char * S = new char[7];
  char * M = new char[7];
  int inputControl = 0;

  printf("-----Starting Post Office Simulation w/ Semaphores-----\n");

  //loop each input until valid input is received
  //once validity determined assign values to global
  while(inputControl == 0){
    printf("Enter the number of people: ");
    fgets(P, 7, stdin);
    inputControl = possInputEval(P);
  }
  poss.totalPeople = inputControl;

  inputControl = 0;
  while(inputControl == 0) {
    printf("Enter the capacity of the mailbox: ");
    fgets(S, 7, stdin);
    inputControl = possInputEval(S);
  }
  poss.sizeOfMailbox = inputControl;

  inputControl = 0;
  while(inputControl == 0) {
    printf("Enter total number of messages: ");
    fgets(M, 7, stdin);
    inputControl = possInputEval(M);
  }
  poss.totalMessages = inputControl;

  //exception handler for condition of only 1 person
  if(poss.totalPeople == 1)
  {
    printf("Person 0 enters the post office\n");
    printf("Person 0 attempts to send mail to themselves \n");
    printf("The staff politely ask him to leave and not come back \n");
  }
  //dynamically allocate and initialize relevant globals
  else {
    //mailbox[][]
    poss.mailbox = new mail*[poss.sizeOfMailbox];
    for(int y = 0; y < poss.sizeOfMailbox; y++) {
      poss.mailbox[y] = new mail[poss.totalPeople];
    }
    poss.messagesSent = 0;
    poss.messagesRead = 0;
    poss.writer = new Semaphore("writerSemaphore", 1);
    poss.deadlockMonitor = new bool[poss.totalPeople];
    poss.mailCount = new int[poss.totalPeople];
    freeSpaceSemaphore = new Semaphore *[poss.totalPeople];
    mailboxSemaphore = new Semaphore *[poss.totalPeople];
    for (int x = 0; x < poss.totalPeople; x++) {
      poss.deadlockMonitor[x] = false;
      poss.mailCount[x] = 0;
      freeSpaceSemaphore[x] = new Semaphore("freeSpaceSemaphore", poss.sizeOfMailbox);
      mailboxSemaphore[x] = new Semaphore("mailboxSemaphore", 1);
    }

    for(int i = 0; i < poss.totalPeople; i++) {
      Thread * t = new Thread("POSS Thread");
      t->Fork(possPerson, i);
    }
  }
  delete P;
  delete S;
  delete M;
}
//---------------------- End Code by Chau Cao---------------------------
//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
// If 0, -A is not used, or invalid usage of -A, run default ThreadTest
// If 1. run Task 1
// If 2. run Task 2
//----------------------------------------------------------------------

void
ThreadTest()
{
    DEBUG('t', "Entering ThreadTest");
    Thread *t = new Thread("forked thread");

    switch(taskChoice) {
      case 0:
        printf("-A not used, 0, or invalid input provided to -A. Performing Default Thread Test\n");
        t->Fork(SimpleThread, 1);
        SimpleThread(0);
        currentThread->Finish();
        break;
      case 1:
        t->Fork(inputValidation, 0);
        currentThread->Finish();
        break;
      case 2:
        t->Fork(shoutingThreads, 0);
        currentThread->Finish();
        break;
      case 6:
        t->Fork(postOfficeSimSem, 0);
        DebugInit("main");
        currentThread->Finish();
        break;
      }

}
//end code changes by Chau Cao
