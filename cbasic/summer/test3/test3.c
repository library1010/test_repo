#include <stdio.h>

void readNumber(int number);
void readDigit(int digit, int flag, int flag1);
void readThousand(int thousand);
int intPow(int a, int b);

int main() {
  int input;
  int temp;
  int i, thousand;

  do {
    printf("Enter a number (0 <= input <= 999 999 999): ");
    while(scanf("%d", &input) != 1) {
      printf("Wrong input!");
      while(getchar() != '\n');
    }
    while(getchar() != '\n');
  } while(input<0 || input > 999999999);

  printf("Cach doc cua so nay: ");
  if(input == 0) {
    printf("khong\n");
    return 0;
  }
  readThousand(input);
  printf("\n");

  return 0;
}

int intPow(int a, int b) {
  int i;
  int output = 1;
  for(i=0; i<b; i++) {
    output *= a;
  }
  return output;
}

void readThousand(int thousand) {
  int i;
  int k = intPow(10, 9);
  int temp;

  for(i=2; i>=0; i--) {
    k /= 1000;
    temp = thousand/k;
    thousand = thousand %k;
    if(k == 1 && temp == 0)  break;
    else if(temp == 0) continue;
    readNumber(temp);
    switch(i) {
    case 2:
      printf("trieu ");
      break;
    case 1:
      printf("nghin ");
      break;
    case 0:
      break;
    }
  }
}

void readNumber(int number) {
  int j;
  int k=1000;
  int temp;
  int flag = 0;
  int flag1 = 0;

  if(number <= 9) flag1 = 1;
  if(number%10 == 0) flag1 = 1;

  for(j=2; j>=0; j--) {
    k /= 10;
    temp = number / k;
    number = number % k;
    if(k == 1 && temp == 0)
      break;
    else if(temp == 0 && j == 1)
      flag = 1;
    else if(temp == 1 && j == 1) {
      printf("muoi ");
      continue;
    }
    else if(temp == 0) continue;
    readDigit(temp, flag, flag1);
    switch(j) {
    case 2:
      printf("tram ");
      break;
    case 1:
      if(flag == 0)
	printf("muoi ");
      else
	flag = 0;
      break;
    case 0:
      break;
    }
  }
}


void readDigit(int digit, int flag, int flag1) {
  switch(digit) {
  case 1:
    printf("mot ");
    break;
  case 2:
    printf("hai ");
    break;
  case 3:
    printf("ba ");
    break;
  case 4:
    printf("bon ");
    break;
  case 5:
    printf("nam ");
    break;
  case 6:
    printf("sau ");
    break;
  case 7:
    printf("bay ");
    break;
  case 8:
    printf("tam ");
    break;
  case 9:
    printf("chin ");
    break;
  case 0:
    if(flag == 0)
      printf("khong ");
    else if(flag1 == 0)
      printf("linh ");
    else if(flag1 == 1)
      break;
  }
}