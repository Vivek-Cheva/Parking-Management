#include <stdio.h>// standard header file
#include <stdlib.h>// for cls..etc
#include <conio.h>// for getch() function
#include <time.h>// for time related functions
#include <windows.h>// to get system time
#include "user.c"// it contains user ids and passwords
char *ch[10][10]; // layout blocks(10 x 10)
void printlogo()  // prints custom page
{
  printf(
      "\n\t\t\t\t\t\t          KARS PARKING    \n"
      "\t\t\t\t\t\t  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
      "\t\t\t\t\t\t  SRM | Vijayawada | 1001001001\n");
  printf("\t\t");
  for (int i = 0; i < 50; i++)
    printf("--");
  printf("\n");
}
int duration(int billno) // caluculates the time elapsed for a coustomer
{

  int t, diff, pt, b, k = 0;
  FILE *fp;
  fp = fopen("customer.txt", "r"); // to read bill no and time(sec) which were saved

  while (1)
  {
    fscanf(fp, "%d%ld", &b, &t);
    if (b == billno)
    {
      k = 1;
      break;
    }
    if (feof(fp))
      break;
  }
  if (k == 0)
  {
    printf("bill num not found");
    return 0;
  }
  pt = time(NULL); // present time (in sec);
  fclose(fp);
  diff = pt - t;
  return diff; // returns the time elapsed (in sec)
}
float amount(int num) // caluculates the amount to be paid
{
  time_t pt; // time.h has a structure time_t so pt is a variable
  pt = time(NULL);
  float final;
  int diff = duration(num); // returns 0 is bill num not found
  if (diff == 0)
    return 0.00;
  if (diff <= 3600)
    final = 25;
  else
  {
    diff -= 3600;
    final = diff * 0.00138888888888888888888888888889 + 25; // the big float is cost/sec
  }
  return final;
}
void apply_changes() // to update the booked slots...it contain only booked slots
{
  FILE *d;
  d = fopen("booked.txt", "r");
  if (d == NULL)
  { // it creates booked.txt if it does not exists
    FILE *x;
    x = fopen("booked.txt", "w");
    fclose(x);
  }
  else
  {
    int a, b;
    fscanf(d, "%d%d", &a, &b);
    if (feof(d))
    {

      fclose(d);

      FILE *D;
      D = fopen("booked.txt", "w");
      fprintf(D, "%d %d ", 100, 100); // to read the file a garbage lot is stored intially(if the file is empty)
      fclose(D);
    }

    else
    {
      fclose(d);
      FILE *fp;

      fp = fopen("booked.txt", "r"); // updates the lot to Booked
      int k, h;
      for (int i = 0; !feof(fp); i++)
      {

        fscanf(fp, "%d%d", &k, &h);
        ch[k][h] = "Booked";
      }
      fclose(fp);
    }
  }
}
void print_layout() // to print the entire layout
{
  apply_changes();
  system("cls");
  char blk = 'A';
  for (int h = 0; h <= 111; h++)
  { // intial outline
    printf("*");
  }
  printf("\n\n");
  for (int j = 0; j < 10; j++) // print Layout of whole stand
  {
    printf("|");

    for (int i = 0; i < 10; i++)
    {
      if (ch[j][i] == NULL)
      {
        printf("    %c%d    ", blk, i + 1);
      }
      else
      {
        printf("  %s  ", ch[j][i]);
      }
      printf("|");
    }
    blk++;
    printf("\n\n");
    for (int h = 0; h <= 111; h++)
    {
      printf("*");
    }
    printf("\n");
    for (int i = 0; i <= 111; i++) // printing lane1
    {
      printf("-");
    }
    printf("\n\n"); // lane space

    for (int k = 0; k <= 5; k++)
    {
      printf(" - - - lane %d- - -", j + 1);
    }
    printf(" -");
    printf("\n\n");
    for (int i = 0; i <= 111; i++) // printing land2
    {
      printf("-");
    }
    printf("\n");
    for (int h = 0; h <= 111; h++)
    {
      printf("*");
    }
    printf("\n");

    printf("\n");
  }
}
void out(int a, int b) // to free the slot... to remove exited slot from file
{
  int billno;
  billno = ((a + 1) * 100) + (a * 10 + b); // to convert lot code to bill number
  float cost = amount(billno);             // if bill num not found it returns 0.00
  if (cost == 0.00)
    return;
  printf("\tAmount to be paid : %.2f", cost);
  char c[10];
  printf("\n\tStatus (paid/due) : ");
  scanf("%s", c);
  char s = 'A' + a;
  if (!strcmp("paid", c)) // generates bill only the status is paid
  {
    system("cls");
    FILE *m;
    m = fopen("booked.txt", "r");
    FILE *t;
    t = fopen("yes.txt", "w");
    for (int i = 0;; i++)
    {
      int x, y;
      fscanf(m, "%d", &x);
      fscanf(m, "%d", &y);
      if (feof(m))
        break;
      if (x != a || y != b)
      {
        fprintf(t, "%d %d ", x, y);
      }
    }

    fclose(m);
    fclose(t);
    remove("booked.txt");
    rename("yes.txt", "booked.txt");
    ch[a][b] = NULL;

    FILE *h;
    h = fopen("history.txt", "a"); // to save in the history
    SYSTEMTIME stime;
    GetLocalTime(&stime); // to get the present time

    fprintf(h, "OUT>>>%d %d/%d/%d %02d:%02d:%02d \n", billno, stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute, stime.wSecond);
    fclose(h);
    int sec_elapsed = duration(billno); // calculation of time duration
    int hr = sec_elapsed / 3600;
    int mn = (sec_elapsed % 3600) / 60;
    printf("\t\t\t\t\t\t  --------------------------------------\n"
           "\t\t\t\t\t\t              KARS PARKING              \n"
           "\t\t\t\t\t\t  --------------------------------------\n"
           "\t\t\t\t\t\t                RECEIPT                 \n"
           "\t\t\t\t\t\t             --------------             \n"
           "\t\t\t\t\t\t                                        \n"
           "\t\t\t\t\t\t                           %d/%d/%d     \n"
           "\t\t\t\t\t\t                           %02d:%02d:%02d  \n"
           "\t\t\t\t\t\t                                        \n"
           "\t\t\t\t\t\t    Bill No           :  %d             \n"
           "\t\t\t\t\t\t    Amount to be paid :  %.2f           \n"
           "\t\t\t\t\t\t    Time elapsed      :  %dhr:%dmin     \n"
           "\t\t\t\t\t\t                                        \n"
           "\t\t\t\t\t\t             THANK YOU :)               \n"
           "\t\t\t\t\t\t  ......................................\n"
           "\t\t\t\t\t\t   First hour: 25 Rs/-                  \n"
           "\t\t\t\t\t\t   Every additional hour: 5 Rs/-        \n"
           "\t\t\t\t\t\t  ......................................\n",
           stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute, stime.wSecond, billno, cost, hr, mn);
    printf("\n\n");
    printf("\t\t\t\t\t\t  Your transcation is succefully completed\n\n");
    printf("\t\t\t\t\t\t             Thank you :)\n\n");
    printf("\t\t\t\t\t\t  Your car is on lane %d >>> slot %c %d\n", a + 1, s, b + 1);
    FILE *fp; // deletes the entry in the customer.txt
    FILE *cp;
    cp = fopen("temp.txt", "w");
    fp = fopen("customer.txt", "r");
    int b;
    long long int tm;
    for (int i;; i++)
    {
      fscanf(fp, "%d%lld", &b, &tm);
      if (feof(fp))
        break;
      if (b != billno)
      {
        fprintf(cp, "%d %lld\n", b, tm);
      }
    }
    fclose(fp);
    fclose(cp);
    remove("customer.txt");
    rename("temp.txt", "customer.txt");
  }
  else
  {
    return;
  }
}
int check_vacancy(int a, int b) // to check whether the slot is vacant or not
{
  int k = 1;
  FILE *bk;
  bk = fopen("booked.txt", "r");
  int x, y;
  for (int i = 0;; i++)
  {
    if (feof(bk))
      break;
    fscanf(bk, "%d%d", &x, &y);
    if (x == a && y == b)
    {
      k = 0;
      break;
    }
  }
  fclose(bk);
  return k;
}
void generatebill(int i, int j) // It generates the entry bill and stores the details of a coustomer
{
  char name[100];
  char city[100];
  long long phn;
  int hr, min, dd, mm, yyyy, billno;
  char num_plate[10];
  billno = ((i + 1) * 100) + ((i * 10) + j);
  printlogo();
  printf("\n\tENTRY DETAILS\n\n"); // Details of the coustomer for entry
  fflush(stdin);
  printf("\tName     : ");
  scanf("%[^\n]s", name);
  printf("\tPhone No : ");
  scanf("%lld", &phn);
  fflush(stdin);
  printf("\tREG No   : ");
  scanf("%[^\n]s", num_plate);
  system("cls");
  SYSTEMTIME stime;
  GetLocalTime(&stime);
  printf("\t\t\t\t\t\t----------------------------------\n"
         "\t\t\t\t\t\t           KARS PARKING          \n\n"
         "\t\t\t\t\t\t                      %d/%d/%d   \n"
         "\t\t\t\t\t\t                      %02d:%02d:%02d  \n"
         "\t\t\t\t\t\t  Bill no : %d                    \n"
         "\t\t\t\t\t\t  Name    : %s                       \n"
         "\t\t\t\t\t\t  Phn no  : %lld                   \n"
         "\t\t\t\t\t\t  REG NO  : %s                     \n"
         "\t\t\t\t\t\t                                 \n"
         "\t\t\t\t\t\t           THANK YOU :)          \n"
         "\t\t\t\t\t\t......................................\n"
         "\t\t\t\t\t\t  First hour: 25 Rs/-                  \n"
         "\t\t\t\t\t\t  Every additional hour: 5 Rs/-        \n"
         "\t\t\t\t\t\t  Bill number is mandatory for logout  \n"
         "\t\t\t\t\t\t......................................\n",
         stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute, stime.wSecond, billno, name, phn, num_plate);
  printf("\nTo confirm booking, please enter your bill no again (or) 0 to cancel: ");
  int b;
  scanf("%d", &b);
  while (b != billno)
  {
    if (b == 0)
    {
      printf("cancled sucessfully<'-'>....press any key");
      getch();
      return;
    }
    printf("Bill no dosen't match\n plese try again: ");
    scanf("%d", &b);
  }
  FILE *h;
  h = fopen("history.txt", "a");
  fprintf(h, "IN>>>%s %s %d  %lld %d/%d/%d %02d:%02d:%02d\n", name, num_plate, billno, phn, stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute, stime.wSecond);
  fclose(h);
  printf("\n\n");
  FILE *fp;
  fp = fopen("customer.txt", "a");
  fprintf(fp, "%d %lld\n", billno, time(NULL));
  fclose(fp);
  FILE *pp;
  pp = fopen("booked.txt", "a");
  fprintf(pp, " %d %d", i, j);
  fclose(pp);
  printf("The lot booking process is complete\n");
  printf("press any key to continue....");
  getch();
}
void booking() // It checks the vacent slots and allocate a slot to the coustomer
{
  system("cls");
  int k = 0;
  int i, j;
  for (i = 0; i < 10; i++)
  {
    for (j = 0; j < 10; j++)
    {
      if (check_vacancy(i, j))
      {
        generatebill(i, j);
        k = 1;
        break;
      }
    }
    if (k == 1)
      break;
  }
  if (k = 0)
    printf("Sorry no slots are avaliable");
}

int main()
{
  int id;        // user ID (user 1,user 2,user 3)
  char pass[10]; // user passwords are in user.c
  system("cls");
user_login: // login page
  printlogo();
  printf("\n\n");

  printf("\t\t\t\t\t\t\t.----------.\n");
  printf("\t\t\t\t\t\t\t|User login|\n");
  printf("\t\t\t\t\t\t\t'----------'\n");
  printf("\n\n\t\t\t\t\tEnter ID       : "); // user 1, user 2, user 3;
  scanf("user%d", &id);

  fflush(stdin);
  printf("\t\t\t\t\tEnter Password : ");
  scanf("%s", pass);

  if (password(id, pass)) // check whether the  user id and password are correct
  {
    FILE *fp;
    fp = fopen("history.txt", "a"); // to note entry of user in the history
    fprintf(fp, "\n\nUser %d\n\n", id);
    fclose(fp);
    int select;
  service: // HOME page....
    system("cls");
    printlogo();
    printf("\n");
    printf("\t\t\t\t\t\t\t Choose a Service:\n");
    printf("\t\t\t\t\t\t\t-------------------\n\n");
    printf("\t\t\t\t\t 1. SHOW LAYOUT\n");
    printf("\t\t\t\t\t 2. BOOK LOT\n");
    printf("\t\t\t\t\t 3. CUSTOMER LOGOUT \n");
    printf("\t\t\t\t\t 4. USER LOGOUT \n");
    printf("Choose an option: ");
    scanf("%d", &select);
    apply_changes(); // to read data from a file and to update the booked slots
    if (select == 1) // Layout
    {
      printlogo();
      print_layout();
      printf("press any key to continue....");

      if (getch())
        goto service;
    }
    else if (select == 2) // lot booking
    {

      printlogo();
      booking();

      if (getch())
        goto service;
    }
    else if (select == 3) // coustomer logout
    {
      system("cls");
      printlogo();
      printf("\n\tEnter bill number : ");
      int x, i, j;
      scanf("%d", &x);
      j = x % 10; // to convert the bill no into the lot index(i,j)
      x /= 10;
      i = x % 10;
      out(i, j);
      apply_changes();
      printf("\npress any key to continue....");

      if (getch())
        ;
      fflush(stdin);
      goto service;
    }
    else if (select == 4) // user logout
    {
      system("cls");
      fflush(stdin);
      goto user_login;
    }
  }
  else
  {
    fflush(stdin);
    printf("\n\t\t\t\tIncorrect ID or Password\n\n");
    printf("press any key to retry: ");
    getch();
    system("cls");
    goto user_login;
  }
}