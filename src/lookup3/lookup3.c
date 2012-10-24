/*
-------------------------------------------------------------------------------
lookup3.c, by Bob Jenkins, May 2006, Public Domain.
-------------------------------------------------------------------------------
*/

#define SELF_TEST 1

#include "lookup3.h"

#ifdef SELF_TEST

/* used for timings */
void driver1()
{
  uint8_t buf[256];
  uint32_t i;
  uint32_t h=0;
  time_t a,z;

  time(&a);
  for (i=0; i<256; ++i) buf[i] = 'x';
  for (i=0; i<1; ++i)
  {
    h = hashlittle(&buf[0],1,h);
  }
  time(&z);
  if (z-a > 0) printf("time %d %.8x\n", z-a, h);
}

/* check that every input bit changes every output bit half the time */
#define HASHSTATE 1
#define HASHLEN   1
#define MAXPAIR 60
#define MAXLEN  70
void driver2()
{
  uint8_t qa[MAXLEN+1], qb[MAXLEN+2], *a = &qa[0], *b = &qb[1];
  uint32_t c[HASHSTATE], d[HASHSTATE], i=0, j=0, k, l, m=0, z;
  uint32_t e[HASHSTATE],f[HASHSTATE],g[HASHSTATE],h[HASHSTATE];
  uint32_t x[HASHSTATE],y[HASHSTATE];
  uint32_t hlen;

  printf("No more than %d trials should ever be needed \n",MAXPAIR/2);
  for (hlen=0; hlen < MAXLEN; ++hlen)
  {
    z=0;
    for (i=0; i<hlen; ++i)  /*----------------------- for each input byte, */
    {
      for (j=0; j<8; ++j)   /*------------------------ for each input bit, */
      {
        for (m=1; m<8; ++m) /*------------ for serveral possible initvals, */
        {
          for (l=0; l<HASHSTATE; ++l)
            e[l]=f[l]=g[l]=h[l]=x[l]=y[l]=~((uint32_t)0);

          /*---- check that every output bit is affected by that input bit */
          for (k=0; k<MAXPAIR; k+=2)
          {
            uint32_t finished=1;
            /* keys have one bit different */
            for (l=0; l<hlen+1; ++l) {a[l] = b[l] = (uint8_t)0;}
            /* have a and b be two keys differing in only one bit */
            a[i] ^= (k<<j);
            a[i] ^= (k>>(8-j));
             c[0] = hashlittle(a, hlen, m);
            b[i] ^= ((k+1)<<j);
            b[i] ^= ((k+1)>>(8-j));
             d[0] = hashlittle(b, hlen, m);
            /* check every bit is 1, 0, set, and not set at least once */
            for (l=0; l<HASHSTATE; ++l)
            {
              e[l] &= (c[l]^d[l]);
              f[l] &= ~(c[l]^d[l]);
              g[l] &= c[l];
              h[l] &= ~c[l];
              x[l] &= d[l];
              y[l] &= ~d[l];
              if (e[l]|f[l]|g[l]|h[l]|x[l]|y[l]) finished=0;
            }
            if (finished) break;
          }
          if (k>z) z=k;
          if (k==MAXPAIR)
          {
             printf("Some bit didn't change: ");
             printf("%.8x %.8x %.8x %.8x %.8x %.8x  ",
                    e[0],f[0],g[0],h[0],x[0],y[0]);
             printf("i %d j %d m %d len %d\n", i, j, m, hlen);
          }
          if (z==MAXPAIR) goto done;
        }
      }
    }
   done:
    if (z < MAXPAIR)
    {
      printf("Mix success  %2d bytes  %2d initvals  ",i,m);
      printf("required  %d  trials\n", z/2);
    }
  }
  printf("\n");
}

/* Check for reading beyond the end of the buffer and alignment problems */
void driver3()
{
  uint8_t buf[MAXLEN+20], *b;
  uint32_t len;
  uint8_t q[] = "This is the time for all good men to come to the aid of their country...";
  uint32_t h;
  uint8_t qq[] = "xThis is the time for all good men to come to the aid of their country...";
  uint32_t i;
  uint8_t qqq[] = "xxThis is the time for all good men to come to the aid of their country...";
  uint32_t j;
  uint8_t qqqq[] = "xxxThis is the time for all good men to come to the aid of their country...";
  uint32_t ref,x,y;
  uint8_t *p;

  printf("Endianness.  These lines should all be the same (for values filled in):\n");
  printf("%.8x                            %.8x                            %.8x\n",
         hashword((const uint32_t *)q, (sizeof(q)-1)/4, 13),
         hashword((const uint32_t *)q, (sizeof(q)-5)/4, 13),
         hashword((const uint32_t *)q, (sizeof(q)-9)/4, 13));
  p = q;
  printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n",
         hashlittle(p, sizeof(q)-1, 13), hashlittle(p, sizeof(q)-2, 13),
         hashlittle(p, sizeof(q)-3, 13), hashlittle(p, sizeof(q)-4, 13),
         hashlittle(p, sizeof(q)-5, 13), hashlittle(p, sizeof(q)-6, 13),
         hashlittle(p, sizeof(q)-7, 13), hashlittle(p, sizeof(q)-8, 13),
         hashlittle(p, sizeof(q)-9, 13), hashlittle(p, sizeof(q)-10, 13),
         hashlittle(p, sizeof(q)-11, 13), hashlittle(p, sizeof(q)-12, 13));
  p = &qq[1];
  printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n",
         hashlittle(p, sizeof(q)-1, 13), hashlittle(p, sizeof(q)-2, 13),
         hashlittle(p, sizeof(q)-3, 13), hashlittle(p, sizeof(q)-4, 13),
         hashlittle(p, sizeof(q)-5, 13), hashlittle(p, sizeof(q)-6, 13),
         hashlittle(p, sizeof(q)-7, 13), hashlittle(p, sizeof(q)-8, 13),
         hashlittle(p, sizeof(q)-9, 13), hashlittle(p, sizeof(q)-10, 13),
         hashlittle(p, sizeof(q)-11, 13), hashlittle(p, sizeof(q)-12, 13));
  p = &qqq[2];
  printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n",
         hashlittle(p, sizeof(q)-1, 13), hashlittle(p, sizeof(q)-2, 13),
         hashlittle(p, sizeof(q)-3, 13), hashlittle(p, sizeof(q)-4, 13),
         hashlittle(p, sizeof(q)-5, 13), hashlittle(p, sizeof(q)-6, 13),
         hashlittle(p, sizeof(q)-7, 13), hashlittle(p, sizeof(q)-8, 13),
         hashlittle(p, sizeof(q)-9, 13), hashlittle(p, sizeof(q)-10, 13),
         hashlittle(p, sizeof(q)-11, 13), hashlittle(p, sizeof(q)-12, 13));
  p = &qqqq[3];
  printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n",
         hashlittle(p, sizeof(q)-1, 13), hashlittle(p, sizeof(q)-2, 13),
         hashlittle(p, sizeof(q)-3, 13), hashlittle(p, sizeof(q)-4, 13),
         hashlittle(p, sizeof(q)-5, 13), hashlittle(p, sizeof(q)-6, 13),
         hashlittle(p, sizeof(q)-7, 13), hashlittle(p, sizeof(q)-8, 13),
         hashlittle(p, sizeof(q)-9, 13), hashlittle(p, sizeof(q)-10, 13),
         hashlittle(p, sizeof(q)-11, 13), hashlittle(p, sizeof(q)-12, 13));
  printf("\n");

  /* check that hashlittle2 and hashlittle produce the same results */
  i=47; j=0;
  hashlittle2(q, sizeof(q), &i, &j);
  if (hashlittle(q, sizeof(q), 47) != i)
    printf("hashlittle2 and hashlittle mismatch\n");

  /* check that hashword2 and hashword produce the same results */
  len = 0xdeadbeef;
  i=47, j=0;
  hashword2(&len, 1, &i, &j);
  if (hashword(&len, 1, 47) != i)
    printf("hashword2 and hashword mismatch %x %x\n",
           i, hashword(&len, 1, 47));

  /* check hashlittle doesn't read before or after the ends of the string */
  for (h=0, b=buf+1; h<8; ++h, ++b)
  {
    for (i=0; i<MAXLEN; ++i)
    {
      len = i;
      for (j=0; j<i; ++j) *(b+j)=0;

      /* these should all be equal */
      ref = hashlittle(b, len, (uint32_t)1);
      *(b+i)=(uint8_t)~0;
      *(b-1)=(uint8_t)~0;
      x = hashlittle(b, len, (uint32_t)1);
      y = hashlittle(b, len, (uint32_t)1);
      if ((ref != x) || (ref != y))
      {
        printf("alignment error: %.8x %.8x %.8x %d %d\n",ref,x,y,
               h, i);
      }
    }
  }
}

/* check for problems with nulls */
 void driver4()
{
  uint8_t buf[1];
  uint32_t h,i,state[HASHSTATE];


  buf[0] = ~0;
  for (i=0; i<HASHSTATE; ++i) state[i] = 1;
  printf("These should all be different\n");
  for (i=0, h=0; i<8; ++i)
  {
    h = hashlittle(buf, 0, h);
    printf("%2ld  0-byte strings, hash is  %.8x\n", i, h);
  }
}

void driver5()
{
  uint32_t b,c;
  b=0, c=0, hashlittle2("", 0, &c, &b);
  printf("hash is %.8lx %.8lx\n", c, b);   /* deadbeef deadbeef */
  b=0xdeadbeef, c=0, hashlittle2("", 0, &c, &b);
  printf("hash is %.8lx %.8lx\n", c, b);   /* bd5b7dde deadbeef */
  b=0xdeadbeef, c=0xdeadbeef, hashlittle2("", 0, &c, &b);
  printf("hash is %.8lx %.8lx\n", c, b);   /* 9c093ccd bd5b7dde */
  b=0, c=0, hashlittle2("Four score and seven years ago", 30, &c, &b);
  printf("hash is %.8lx %.8lx\n", c, b);   /* 17770551 ce7226e6 */
  b=1, c=0, hashlittle2("Four score and seven years ago", 30, &c, &b);
  printf("hash is %.8lx %.8lx\n", c, b);   /* e3607cae bd371de4 */
  b=0, c=1, hashlittle2("Four score and seven years ago", 30, &c, &b);
  printf("hash is %.8lx %.8lx\n", c, b);   /* cd628161 6cbea4b3 */
  c = hashlittle("Four score and seven years ago", 30, 0);
  printf("hash is %.8lx\n", c);   /* 17770551 */
  c = hashlittle("Four score and seven years ago", 30, 1);
  printf("hash is %.8lx\n", c);   /* cd628161 */
}


int main()
{
  driver1();   /* test that the key is hashed: used for timings */
  driver2();   /* test that whole key is hashed thoroughly */
  driver3();   /* test that nothing but the key is hashed */
  driver4();   /* test hashing multiple buffers (all buffers are null) */
  driver5();   /* test the hash against known vectors */
  return 1;
}

#endif  /* SELF_TEST */
