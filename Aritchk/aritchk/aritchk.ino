#include <stdio.h>
#include <math.h>
#include <errno.h>

typedef struct Akind {
  char *name;
  int   kind;
  } Akind;

static int dalign;
static double t_nan; 
static Akind IEEE_8087  = { "IEEE_8087", 1 }, IEEE_MC68k  = { "IEEE_MC68k", 2 }, IBM   = { "IBM", 3 }, VAX   = { "VAX", 4 }, CRAY    = { "CRAY", 5};
char *emptyfmt = "";  /* avoid possible warning message with printf("") */

static Akind *
Lcheck(void)
{
  union {
    double d;
    long L[2];
    } u;
  struct {
    double d;
    long L;
    } x[2];

  if (sizeof(x) > 2*(sizeof(double) + sizeof(long)))
    dalign = 1;
  u.L[0] = u.L[1] = 0;
  u.d = 1e13;
  if (u.L[0] == 1117925532 && u.L[1] == -448790528)
    return &IEEE_MC68k;
  if (u.L[1] == 1117925532 && u.L[0] == -448790528)
    return &IEEE_8087;
  if (u.L[0] == -2065213935 && u.L[1] == 10752)
    return &VAX;
  if (u.L[0] == 1267827943 && u.L[1] == 704643072)
    return &IBM;
  return 0;
  }

static Akind *
icheck(void)
{
  union {
    double d;
    int L[2];
    } u;
  struct {
    double d;
    int L;
    } x[2];

  if (sizeof(x) > 2*(sizeof(double) + sizeof(int)))
    dalign = 1;
  u.L[0] = u.L[1] = 0;
  u.d = 1e13;
  if (u.L[0] == 1117925532 && u.L[1] == -448790528)
    return &IEEE_MC68k;
  if (u.L[1] == 1117925532 && u.L[0] == -448790528)
    return &IEEE_8087;
  if (u.L[0] == -2065213935 && u.L[1] == 10752)
    return &VAX;
  if (u.L[0] == 1267827943 && u.L[1] == 704643072)
    return &IBM;
  return 0;
  }

static Akind *
ccheck(void)
{
  union {
    double d;
    long L;
    } u;
  long Cray1;

  /* Cray1 = 4617762693716115456 -- without overflow on non-Crays */
  Cray1 = printf(emptyfmt) < 0 ? 0 : 4617762;
  if (printf(emptyfmt, Cray1) >= 0)
    Cray1 = 1000000*Cray1 + 693716;
  if (printf(emptyfmt, Cray1) >= 0)
    Cray1 = 1000000*Cray1 + 115456;
  u.d = 1e13;
  if (u.L == Cray1)
    return &CRAY;
  return 0;
  }

static int fzcheck(void)
{
  double a, b;
  int i;

  a = 1.;
  b = .1;
  for(i = 155;; b *= b, i >>= 1) {
    if (i & 1) {
      a *= b;
      if (i == 1)
        break;
      }
    }
  b = a * a;
  return b == 0.;
  }

static int need_nancheck(void)
{
  double t;

  errno = 0;
  t = log(t_nan);
  if (errno == 0)
    return 1;
  errno = 0;
  t = sqrt(t_nan);
  return errno == 0;
  }

 void get_nanbits(unsigned int *b, int k)
{
  union { double d; unsigned int z[2]; } u, u1, u2;

  k = 2 - k;
  u1.z[k] = u2.z[k] = 0x7ff00000;
  u1.z[1-k] = u2.z[1-k] = 0;
  u.d = u1.d - u2.d;  /* Infinity - Infinity */
  b[0] = u.z[0];
  b[1] = u.z[1];
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
}

void loop() {

  Akind *a = 0;
  int Ldef = 0;
  unsigned int nanbits[2];

  if (sizeof(double) == 2*sizeof(long)) a = Lcheck();
  else if (sizeof(double) == 2*sizeof(int)) 
  {
    Ldef = 1;
    a = icheck();
   }
  else if (sizeof(double) == sizeof(long)) a = ccheck();

  Serial.print("#define ");
  Serial.println(a->name);
 
  if (a) 
  {
    Serial.print("#define ");
    Serial.print("Arith_Kind_ASL ");
    Serial.println(a->kind);

    if (Ldef)
    {
      Serial.print("#define ");
      Serial.println("Long int");
      Serial.print("#define ");
      Serial.println("Intcast (int)(long)");
    }

    if (dalign)
    {
      Serial.print("#define ");
      Serial.println("Double_Align");
    }

    if (sizeof(char*) == 8)
    {
      Serial.print("#define ");
      Serial.println("X64_bit_pointers");
    }

#ifndef NO_LONG_LONG
    if (sizeof(long long) < 8)
#endif
      
     Serial.print("#define NO_LONG_LONG");
          
     if (a->kind <= 2) 
     {
        if (fzcheck())
        {
          Serial.print("#define ");
          Serial.println("Sudden_Underflow");
        }

        t_nan = -a->kind;

        if (need_nancheck())
        {
          Serial.print("#define ");
          Serial.println("NANCHECK");
        }

        if (sizeof(double) == 2*sizeof(unsigned int))
        {
           get_nanbits(nanbits, a->kind);
           Serial.print("#define QNaN0 0x");
           Serial.println(nanbits[0],HEX);
           Serial.print("#define QNaN1 0x");
           Serial.println(nanbits[1],HEX);
        }
     }
  }
  
  while(1){}
}
