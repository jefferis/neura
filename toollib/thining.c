/* 
 * thin.c
 * 
 * Practical Algorithms for Image Analysis
 * 
 * Copyright (c) 1997, 1998, 1999 MLMSoftwareGroup, LLC
 */

/* THIN:        program performs thinning on image 
 *              usage: thin inimg outimg [-k K] [-n MAXITER] [-d DISPLAY] [-I] [-L]
 *              The <display> argument shows result of each iteration
 *              of thinning in "line-printer" style.
 *                              
 */
#include "thining.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define u_char unsigned char


typedef struct Image {
  unsigned char **img;
  long height, width, bps, spp;
} Image;


void print_sos_lic ()
{

};

#define ERRORMEM {printf("Error\n");return NULL;};


#define DFLTMAXK 3              /* dflt maximum thinning kernel size */
#define MAXMAXK 21              /* max of the maximum thinning kernel size */
#define MAXITER 50              /* maximum number of iterations */
#define MIN0RUN 5               /* minimum run of zeros to run-length code */
#define MAXDISPLAY 40           /* maximum length of line for display */

unsigned char OFF, ERASED, ON, PON;  /* values of pixels:
                                      * * - ERASED value is for non-anchor
                                      * * - ERASED  + 1 is for anchor
                                      * * - ERASED increments +2 each iteration */

unsigned char **image;          /* input/output image */
struct point {int x; int y;};
struct point imgSize;           /* image size */
long ySizeM1;                   /* y length minus 1 */
long **xRun;                    /* no., then x locns of 1/0 runs for each y */

long peel0 (unsigned char *, unsigned char *, long, long *, long *, long *);
long peel (unsigned char *, unsigned char *, long, long *, long *);
long ksize (long, long, long);
long sqron (long, long, long);
long getring (long, long, long, unsigned char *);
long thinring (unsigned char *, long, int *);
long chkconnect (unsigned char *, unsigned char *, long);
long anchor (unsigned char *, long);
long erasesqr (long, long, long, int, long *);
long display (long);
long usage (short);
long input (int, char **, long *, long *, long *, long *);

//Image Operation 
unsigned char **
ImageGetPtr (ip)
     Image *ip;
{
  return (ip->img);
}

long
ImageGetWidth (ip)
     Image *ip;
{
  return (ip->width);
}

long
ImageGetHeight (ip)
     Image *ip;
{
  return (ip->height);
}

long
ImageGetDepth (ip)
     Image *ip;
{
  return (ip->bps);
}


Image *
ImageCreate (ucharDataType  inputData)
{
    long height=inputData.sizeY;
    long  width=inputData.sizeX;
  /*register long         i; */
  register long x,y;              /* row increment of image */
  register unsigned char *p, **image;
  register Image *ip;
  register long rem, nwid;

  nwid = width;
    rem = nwid % 8;
    if (rem)
      nwid += 8 - rem;
      
  /*malloc the image structure first */
  if ((ip = (Image *) malloc (sizeof (Image))) == 0)
    ERRORMEM;

  /*Grayscale and B&W array */
  if ((image = (u_char **) calloc (height, sizeof (unsigned char *))) == 0)
      ERRORMEM;
  if ((p = (u_char *) calloc (height * nwid, sizeof (unsigned char))) == 0)
      ERRORMEM;
  for (y = 0; y < height; y++)
    image[y] = p + (y * nwid);
    
  ip->img = image;
  ip->height = height;
  ip->width = width;
  ip->bps = 1;
  
  for (y = 0; y <  height; y++)
      for (x = 0; x < width; x++)
      {
	image[y][x] = 0;
	if (GetBinaryDataAtPoint(inputData, x, y)) image[y][x] = 255;
      };
  return (ip);
}


int executeThining(ucharDataType inputData, ucharDataType outputData)
{
  register long x, y,           /* image coordinates */
    i, k;                       /* sidelength of current thinning kernel */
  Image *imgIO;                 /* structure for I/O images */
  long maxK,                    /* max. sidelength of thinning kernel */
    change[MAXMAXK],            /* no. erasures for each mask size */
    nMaxIter,                   /* maximum number of iterations */
    nIter,                      /* no. iterations */
    nChange,                    /* no. thinning operations on iteration */
    displayFlag,                /* display results after each iter if =1 */
    invertFlag,                 /* invert input image before processing */
    nONs,                       /* total ONs in original image */
    nErased;                    /* cumulative no. ERASED in image */
  unsigned char *ring;          /* ring of pixels on perimeter of kxk sqr */
  unsigned char *side;          /* array of 8-connected side pixels */
  int info; //Information Output

  imgIO = ImageCreate (inputData);
  image = imgIO->img;
  imgSize.x = ImageGetWidth (imgIO);
  imgSize.y = ImageGetHeight (imgIO);

  ySizeM1 = imgSize.y - 1;

  //Controll Parameter
  maxK = DFLTMAXK;
  nMaxIter = MAXITER;
  
  info=0;
  
  displayFlag=0;
/* invert image */
  invertFlag=0;
  if (invertFlag) {
    for (y = 0; y < imgSize.y; y++)
      for (x = 0; x < imgSize.x; x++)
      image[y][x] = 255 - image[y][x];
  }

  if ((xRun = (long **) calloc (imgSize.y, sizeof (long))) == NULL) {
    printf ("CALLOC: xRun -- not enough memory -- sorry\n");
    return (-1);
  }

  if ((ring = (unsigned char *) malloc (4 * (maxK - 1))) == NULL) {
    printf ("not enough memory -- sorry\n");
    return (-2);
  }
  if ((side = (unsigned char *) malloc (maxK)) == NULL) {
    printf ("not enough memory -- sorry\n");
    return (-3);
  }

/* initialize ON/OFF/ERASED values */
  OFF = 0;
  ERASED = 1;
  ON = 255;
  PON = ON - 1;

/* zero image borders */
  for (y = 0; y < imgSize.y; y++)
    image[y][0] = image[y][imgSize.x - 1] = OFF;
  for (x = 0; x < imgSize.x; x++)
    image[0][x] = image[ySizeM1][x] = OFF;

  for (k = 0; k < MAXMAXK; k++)
    change[k] = 0;

/* on first iteration, perform thinning and accumulate information
 * on x-runs */
if (info)  printf ("Start peeling!\n");
  nChange = peel0 (ring, side, maxK, change, &nONs, &nErased);

  if (info) printf ("iteration  1:\t");
  for (i = 3; i <= maxK; i++) {
    //printf (" %d) %3d;  ", i, change[i]);
    change[i] = 0;
  }
  if (info) printf ("\n");

  if (displayFlag == 1)
    display (nChange);
  ERASED += 2;

/* iteratively convolve through image until thinned */

  for (nIter = 1, nChange = 1; (nChange > 0) && nIter <= nMaxIter; nIter++) {
    nChange = peel (ring, side, maxK, change, &nErased);
    if (info) printf ("iteration %2d:\t", nIter + 1);
    for (i = 3; i <= maxK; i++) {
      //printf (" %d) %3d;  ", i, change[i]);
      change[i] = 0;
    }
    if (info) printf ("\n");
    /*if (displayFlag == 1)
      display (nChange);*/
    ERASED += 2;
  }
  printf ("%d thinned out of %d original ONs (%d%%)\n",
          nONs - nErased, nONs, ((nONs - nErased) * 100) / nONs);
  if (nIter >= nMaxIter && nChange != 0)
    printf ("Nuts -- maximum iterations reached\n");

  for (y = 1; y < ySizeM1; y++)
    for (x = 1; x < imgSize.x - 1; x++)
      image[y][x] = (image[y][x] < PON) ? OFF : ON;

/* un-invert image */
  if (invertFlag) {
    for (y = 0; y < imgSize.y; y++)
      for (x = 0; x < imgSize.x; x++)
      image[y][x] = 255 - image[y][x];
  }

  //ImageOut (argv[2], imgIO);
  //Create Output Data
  
   for (y = 0; y < imgSize.y; y++)
      for (x = 0; x < imgSize.x; x++)
	  SetBinaryDataAtPoint(outputData, x, y, image[y][x]);
  return (0);
}



/* PEEL0:       function performs first thinning iteration where information
 *            on x-runs of ONs is accumulated
 *                    usage: nChange = peel0 (ring, side, maxK, change, 
 *                                                      &nOns, &nErased)
 */

long
peel0 (ring, side, maxK, change, nONs, nErased)
     unsigned char *ring;       /* ring of pixels on perimeter of kxk sqr */
     unsigned char *side;       /* array of 8-connected side pixels */
     long maxK,                 /* max sidelength of thinning kernel */
      *change,                  /* no. erasures for each mask size */
      *nONs,                    /* total original ON pixels */
      *nErased;                 /* cumulative no. ERASED in image */
{
  register long x, y,           /* image coordinates */
    iXRun,                      /* index of runs in x */
    xSizeM1,                    /* imgSize.x minus 1 */
    k;                          /* sidelength of current thinning kernel */
  long onRun,                   /* flag = 1 for run of 1s; 0 for 0s */
    permOn,                     /* permanent ON flag set to 1 if so */
    kM1,                        /* k minus 1 */
    nChange;                    /* no. thinning operations on iteration */

  *nONs = *nErased = nChange = 0;
  xSizeM1 = imgSize.x - 1;
  for (y = 1; y < ySizeM1; y++) {
    xRun[y] = (long *) calloc (imgSize.x + 1, sizeof (long));
    xRun[y][0] = -MIN0RUN;
    for (x = 1, iXRun = 1, onRun = 0; x < xSizeM1; x++) {
      permOn = 0;
      if (image[y][x] < ERASED) {
        if (onRun == 1) {
          onRun = 0;
          xRun[y][iXRun++] = x - 1;
        }
        k = 0;
      }
      else {
        if (onRun == 0) {
          onRun = 1;
          if ((x - xRun[y][iXRun - 1]) < MIN0RUN)
            --iXRun;
          else
            xRun[y][iXRun++] = x;
        }
        (*nONs)++;
        k = ksize (x, y, maxK);
      }
      kM1 = (k > 3) ? k - 1 : 3;
      while (k >= kM1) {
        if (sqron (x, y, k) == 0)
          break;
        if (getring (x, y, k, ring) == 1)
          break;
        if (thinring (ring, k, (int *) &permOn) == 1) {
          if (chkconnect (ring, side, k) == 1) {
            nChange++;
            (change[k])++;
            erasesqr (x, y, k, anchor (ring, k), nErased);
            break;
          }
        }
        --k;
      }
      if (permOn == 1)
        image[y][x] = PON;
    }
    --iXRun;
    if (iXRun % 2 != 0)
      xRun[y][++iXRun] = x;
    xRun[y][0] = iXRun;
    xRun[y] = (long *) realloc (xRun[y], (sizeof (long)) * (iXRun + 1));
  }
  return (nChange);
}


/* PEEL:        function performs an iteration of thinning
 *                usage: nChange = peel (ring, side, maxK, change, &nErased)
 */

long
peel (ring, side, maxK, change, nErased)
     unsigned char *ring;       /* ring of pixels on perimeter of kxk sqr */
     unsigned char *side;       /* array of 8-connected side pixels */
     long maxK,                 /* max sidelength of thinning kernel */
      *change,                  /* no. erasures for each mask size */
      *nErased;                 /* cumulative no. ERASED in image */

{
  register long x, y,           /* image coordinates */
    xEnd, iXRun,                /* index of runs in x */
    k;                          /* sidelength of current thinning kernel */
  long noOnsInRow,              /* flag = 1 if no ONs in y-row (0 if ONs) */
    permOn,                     /* permanent ON flag set to 1 if so */
    kM1,                        /* k minus 1 */
    nChange;                    /* no. thinning operations on iteration */

  nChange = 0;
  for (y = 1; y < ySizeM1; y++) {
    for (iXRun = 1, noOnsInRow = 1; iXRun <= xRun[y][0]; iXRun += 2) {
      xEnd = xRun[y][iXRun + 1];
      for (x = xRun[y][iXRun]; x <= xEnd; x++) {
        permOn = 0;
        if (image[y][x] < ERASED)
          k = 0;
        else
          k = ksize (x, y, maxK);
        kM1 = (k > 3) ? k - 1 : 3;
        while (k >= kM1) {
          if (sqron (x, y, k) == 0)
            break;
          noOnsInRow = 0;
          if (getring (x, y, k, ring) == 1)
            break;
          if (thinring (ring, k, (int *) &permOn) == 1) {
            if (chkconnect (ring, side, k) == 1) {
              nChange++;
              (change[k])++;
              erasesqr (x, y, k, anchor (ring, k), nErased);
              break;
            }
          }
          --k;
        }
        if (permOn == 1)
          image[y][x] = PON;
      }
    }
    if (noOnsInRow == 1)
      xRun[y][0] = -xRun[y][0];
  }
  return (nChange);
}



/* KSIZE:       function determines k, where kxk is largest square
 *            around (x,y) which contains all ON or ERASED
 *                      usage: k = ksize (x, y, maxK)
 */

long
ksize (x, y, maxK)
     long x, y,                 /* image coordinates */
       maxK;                    /* maximum k value */
{
  register long xMask, yMask,   /* x,y mask coordinates */
    xEnd, yEnd,                 /* end coord.s of square */
    k;                          /* mask size */
  long upHalf, downHalf,        /* half of mask below and above center */
    xStart,                     /* x- start and end of square */
    yStart;                     /* y- start and end of square */

  for (k = 4; k <= maxK; k++) {
    if (k % 2 == 1)
      downHalf = upHalf = (k - 3) / 2;
    else {
      upHalf = (k - 2) / 2;
      downHalf = (k - 4) / 2;
    }
    xStart = x - downHalf;
    xEnd = x + upHalf;
    yStart = y - downHalf;
    yEnd = y + upHalf;
    for (yMask = yStart; yMask <= yEnd; yMask++)
      for (xMask = xStart; xMask <= xEnd; xMask++)
        if (image[yMask][xMask] < ERASED)
          return (k - 1);
  }
  return (maxK);
}




/* SQRON:       function tests pixels in kxk square are already erased or 
 *            for 3x3 if they can never be erased
 *                      usage: flag = sqron (x, y, k)
 *              flag = 0 if cannot erase any pixels in square
 *                   = 1 if at least one pixel is still ON
 */

long
sqron (x, y, k)
     register long x, y;        /* image coordinages */
     long k;                    /* square sidelength of ring */
{
  register long xEnd, yEnd,     /* upper bounds of center erase area */
    erasedP1;                   /* ERASED + 1 */
  long upHalf, downHalf,        /* half of mask below and above center */
    yStart, xStart;             /* bounds of center erase area */

/* check for 3x3 */
  if (k == 3) {
    if (image[y][x] == ON)
      return (1);
    else
      return (0);
  }

/* check center square */
  if (k % 2 == 1)
    downHalf = upHalf = (k - 3) / 2;
  else {
    upHalf = (k - 2) / 2;
    downHalf = (k - 4) / 2;
  }
  xStart = x - downHalf;
  xEnd = x + upHalf;
  yStart = y - downHalf;
  yEnd = y + upHalf;
  erasedP1 = ERASED + 1;
  for (y = yStart; y <= yEnd; y++)
    for (x = xStart; x <= xEnd; x++)
      if (image[y][x] >= erasedP1)
        return (1);
  return (0);
}



/* GETRING:     function gets ring of pixels on perimeter of k-size square
 *                usage: allOnes = getring (x, y, k, ring)
 *              If ring includes only 1-value pixels, function returns 1,
 *              otherwise 0.
 */

long
getring (x, y, k, ring)
     register long x, y;        /* image coordinages */
     long k;                    /* square sidelength of ring */
     unsigned char *ring;       /* ring of pixels on perimeter of kxk sqr */
{
  register xEnd, yEnd,          /* x,y ends of square */
    allOnes,                    /* =1 if all ones in ring; 0 otherwise */
    i;
  long upHalf, downHalf,        /* half of mask below and above center */
    xStart,                     /* x- start and end of square */
    yStart;                     /* y- start and end of square */

  if (k % 2 == 1)
    downHalf = upHalf = (k - 1) / 2;
  else {
    upHalf = k / 2;
    downHalf = (k - 2) / 2;
  }
  xStart = x - downHalf;
  xEnd = x + upHalf;
  yStart = y - downHalf;
  yEnd = y + upHalf;

  allOnes = 1;
  i = 0;
  for (x = xStart, y = yStart; x <= xEnd; x++) {
    if (image[y][x] < ERASED)
      allOnes = 0;
    ring[i++] = image[y][x];
  }
  for (y = yStart + 1, x = xEnd; y <= yEnd; y++) {
    if (image[y][x] < ERASED)
      allOnes = 0;
    ring[i++] = image[y][x];
  }
  for (x = xEnd - 1, y = yEnd; x >= xStart; --x) {
    if (image[y][x] < ERASED)
      allOnes = 0;
    ring[i++] = image[y][x];
  }
  for (y = yEnd - 1, x = xStart; y > yStart; --y) {
    if (image[y][x] < ERASED)
      allOnes = 0;
    ring[i++] = image[y][x];
  }

/* if this square is already at border, cannot go to larger square */
  if (xStart <= 0 || yStart <= 0
      || xEnd >= (imgSize.x - 1) || yEnd >= (ySizeM1))
    return (0);

  return (allOnes);
}



/* THINRING:    function makes decision to thin or not based on CNUM
 *            and FNUM in perimeter ring
 *                      usage: flag = thinring (ring, k, &permOn)
 *              Flag = 1 if thinning conditions met, 0 otherwise.
 */

long
thinring (ring, k, permOn)
     unsigned char *ring;       /* ring of pixels on perimeter of kxk sqr */
     register long k;           /* square sidelength of ring */
     int *permOn;               /* permanent ON flag set to 1 if so */
{
  register nRing,               /* no. pixels in ring */
    cNum,                       /* connectivity number */
    n, i;
  long fNum,                    /* no. 1s on ring */
    m;
  long nOff,                    /* current run of OFF in ring */
    phi0,                       /* maximum run of OFF in ring */
    nFirstRun;                  /* no  OFF from ring[0] */
  long lower, upper;            /* adjacent ring elements for cNum calc */

  nRing = 4 * k - 4;

/* calculate FNUM */
  for (i = 0, fNum = 0; i < nRing; i++)
    if (ring[i] >= ERASED)
      fNum++;

/* calculate 4-connected run of 0s */
  nOff = (ring[0] < ERASED) ? 1 : 0;
  nFirstRun = (nOff == 1) ? 0 : -1;
  for (i = 1, phi0 = 0; i < nRing; i++) {
    if (ring[i] < ERASED)
      nOff++;
    else {
      if (nOff > 0) {
        phi0 = (nOff > phi0) ? nOff : phi0;
        if (nFirstRun == 0)
          nFirstRun = nOff;
        nOff = 0;
      }
    }
  }
  if (nOff > 0) {
    if (nFirstRun > 0)
      nOff += nFirstRun;
    phi0 = (nOff > phi0) ? nOff : phi0;
  }

/* CNUM */
/* CNUM skipping corners */
  for (i = 2, cNum = 0; i < nRing; i++) {
    lower = (long) ring[i - 1];
    if ((i % (k - 1)) == 0)
      i++;                      /* skip the corner pixels */
    upper = (long) ring[i];
    if (upper >= ERASED && lower < ERASED)
      cNum++;
  }
  if (ring[1] >= ERASED && ring[nRing - 1] < ERASED)
    cNum++;
/* CNUM at corners */
  for (n = 1; n < 4; n++) {
    m = n * (k - 1);
    if (ring[m] >= ERASED)
      if (ring[m - 1] < ERASED && ring[m + 1] < ERASED)
        cNum++;
  }
  if (ring[0] >= ERASED && ring[1] < ERASED && ring[nRing - 1] < ERASED)
    cNum++;

/* to thin or not to thin */
  if (cNum == 1)
    if (phi0 > (k - 2) && fNum > (k - 2))
      return (1);

/* for 3x3, set flag for perm. ON pixel if connection, end, or cross pt */
/* note that 2nd conditional for cross added 17-Mar-89 to make equivalent
 * to THINWX; this leaves non-8-conn. jct if cross-jct with one branch
 * just one pixel, but it's consistent */
  if (k == 3)
    if (cNum > 1 || fNum <= 1 || (cNum == 0 && fNum == 4))
      *permOn = 1;

  return (0);
}



/* CHKCONNECT:  function checks connectivity to see if current erasures
 *            combined with past erasures will destroy connectivity
 *                      usage: flag = chkconnect (ring, side, k)
 *              Function returns flag = 0 if connectivity destroyed,
 *              flag = 1 if connectivity retained.
 */

/* if corner or its nbr is ON, then that is value of side, otherwise OFF */
#define CORNER(ISIDE,ICORNER,ICORNERSIDE,ICORNEROTHER) \
		if (ring[ICORNER] >= PON || ring[ICORNEROTHER] >= ERASED) \
    		  side[ISIDE] = ON; \
		else side[ISIDE] = OFF

/* for a NW corner, check if corner is anchor point, and if so set to ON */
#define CORNERNW(ISIDE,ICORNER,ICORNERSIDE,ICORNEROTHER) \
		if (ring[ICORNER] >= ERASED + 1 \
		    || ring[ICORNEROTHER] >= ERASED) \
    		  side[ISIDE] = ON; \
		else side[ISIDE] = OFF

long
chkconnect (ring, side, k)
     unsigned char *ring;       /* ring of pixels on perimeter of kxk sqr */
     unsigned char *side;       /* array of 8-connected side pixels */
     long k;                    /* square sidelength of ring */
{
  register long i, nSide,       /* no. pixels along 8-connected side */
    nSideM1,                    /* nSide minus 1 */
    iSide,                      /* side array index */
    nRing;                      /* no. pixels in ring */
  long anON,                    /* an ON run along side */
    ONandERASE,                 /* run of ON and n ERASEs */
    N1;                         /* no. ones in nbrhood */

  nRing = 4 * k - 4;
  nSide = k;
  nSideM1 = nSide - 1;

/* calculate N1 */
  for (i = 0, N1 = 0; i < nRing; i++)
    if (ring[i] >= PON)
      N1++;
  if (N1 == 0)
    return (0);

/* check connectivity of west side */
  i = 3 * (k - 1);
  CORNER (0, i, i + 1, i - 1);
  for (i = i + 1, iSide = 1; i < nRing; i++, iSide++)
    side[iSide] = ring[i];
  CORNERNW (nSideM1, 0, nRing - 1, 1);

  for (i = 0, anON = 0, ONandERASE = 0; i < nSide; i++) {
    if (side[i] >= PON) {
      if (ONandERASE == 1)
        return (0);
      else
        anON = 1;
    }
    else if ((side[i] == ERASED || side[i] == ERASED + 1)
             && anON == 1)
      ONandERASE = 1;
    else if (side[i] < ERASED)
      anON = ONandERASE = 0;    /* off */
  }

/* check connectivity of north side */
  i = k - 1;
  CORNER (0, i, i - 1, i + 1);
  for (i = i - 1, iSide = 1; i >= 0; --i, iSide++)
    side[iSide] = ring[i];
  CORNERNW (nSideM1, 0, 1, nRing - 1);

  for (i = 0, anON = 0, ONandERASE = 0; i < nSide; i++) {
    if (side[i] >= PON) {
      if (ONandERASE == 1)
        return (0);
      else
        anON = 1;
    }
    else if ((side[i] == ERASED || side[i] == ERASED + 1)
             && anON == 1)
      ONandERASE = 1;
    else if (side[i] < ERASED)
      anON = ONandERASE = 0;    /* off */
  }

/* check connectivity of east side */
  i = k - 1;
  CORNER (0, i, i + 1, i - 1);
  for (i = i + 1, iSide = 1; iSide < nSideM1; i++, iSide++)
    side[iSide] = ring[i];
  CORNER (nSideM1, 2 * (k - 1), 2 * (k - 1) - 1, 2 * (k - 1) + 1);

  for (i = 0, anON = 0, ONandERASE = 0; i < nSide; i++) {
    if (side[i] >= PON) {
      if (ONandERASE == 1)
        return (0);
      else
        anON = 1;
    }
    else if ((side[i] == ERASED || side[i] == ERASED + 1)
             && anON == 1)
      ONandERASE = 1;
    else if (side[i] < ERASED)
      anON = ONandERASE = 0;    /* off */
  }
  return (1);
}



/* ANCHOR:      function returns 1 if core is exposed on NW side
 *                    usage: anchor (ring, k)
 */

long
anchor (ring, k)
     unsigned char *ring;       /* ring of pixels on perimeter of kxk sqr */
     register long k;           /* square sidelength of ring */
{
  register long nRing,          /* no. pixels in ring */
    i;

  nRing = 4 * k - 4;
  for (i = 0; i < k; i++)
    if (ring[i] >= ERASED)
      return (0);
  for (i = 3 * (k - 1) + 1; i < nRing; i++)
    if (ring[i] >= ERASED)
      return (0);
  return (1);
}



/* ERASESQR:    function erases square contained within square perimeter
 *                    usage: erasesqr (x, y, k, anchor, &nErased)
 *              If the core is an anchor, then the pixels are erased
 *              to ERASED + 1, otherwise they are erased to ERASED.
 *              For kxk > 3x3 erasure, PON pixels (permanent ON for 3x3)
 *              are erased; and if an anchor point (ERASED + 1) can be
 *              erased to a non-anchor point by a larger k (I don't know
 *              if this can actually happen... I'm pretty sure it cannot
 *              and have found on a fingerprint it does not, but have not
 *              thought it out yet.) then it is erased to ERASED.
 */
   /* want to erase if ERASED +1, not just ON */

long
erasesqr (x, y, k, anchor, nErased)
     register long x, y;        /* image coordinages */
     long k;                    /* square sidelength of ring */
     register int anchor;       /* 1 if core is NW pt of NW-SE diag; else 0 */
     long *nErased;             /* no. of erased */
{
  register long xEnd, yEnd;     /* upper bounds of center erase area */
  long upHalf, downHalf,        /* half of mask below and above center */
    yStart,                     /* bounds of center erase area */
    xStart;

/* erase for 3x3 */
  if (k == 3) {
    if (image[y][x] == ON) {
      (*nErased)++;
      image[y][x] = (anchor == 0) ? ERASED : ERASED + 1;
    }
  }
/* erase for kxk > 3x3 */
  else {
    if (k % 2 == 1)
      downHalf = upHalf = (k - 3) / 2;
    else {
      upHalf = (k - 2) / 2;
      downHalf = (k - 4) / 2;
    }
    xStart = x - downHalf;
    xEnd = x + upHalf;
    yStart = y - downHalf;
    yEnd = y + upHalf;
    for (y = yStart; y <= yEnd; y++) {
      for (x = xStart; x <= xEnd; x++) {
        if (image[y][x] >= ERASED + 1) {
          if (image[y][x] >= PON)
            (*nErased)++;
          image[y][x] = (anchor == 0) ? ERASED : ERASED + 1;
        }
      }
    }
  }
  return (0);
}



/* DISPLAY:     function displays results after each iteration of thinning
 *                    usage: display (nChange)
 */

long
display (nChange)
     long nChange;              /* no. thinning operations on iteration */
{
  long y, x;                    /* image coordinates */
  char c;

  for (y = 0; y < imgSize.y; y++) {
    for (x = 0; x < imgSize.x; x++) {
      if (image[y][x] == ON)
        printf ("* ");
      else if (image[y][x] == PON)
        printf ("O ");
      else if (image[y][x] == ERASED)
        printf ("e ");
      else if (image[y][x] == ERASED + 1)
        printf ("a ");
      else
        printf ("  ");
    }
    printf ("\n");
  }
  if (nChange > 0) {
    printf ("Enter <CR> to continue next iteration.\n");
    scanf ("%c", &c);
  }
  return (0);
}



/* USAGE:       function gives instructions on usage of program
 *                    usage: usage (flag)
 *              When flag is 1, the long message is given, 0 gives short.
 */

long
usage (flag)
     short flag;                /* flag =1 for long message; =0 for short message */
{

/* print short usage message or long */
  printf ("USAGE: thin inimg outimg [-k K] [-n MAXITER] [-d DISPLAY] [-I] [-L]\n");
  if (flag == 0)
    return (-1);

  printf ("\nthin performs iterative thinning of binary objects\n");
  printf ("in input image to produce skeleton image with values\n");
  printf ("OFF (0) anf ON (255)\n\n");
  printf ("ARGUMENTS:\n");
  printf ("    inimg: input image filename (TIF)\n");
  printf ("   outimg: output image filename (TIF)\n\n");
  printf ("OPTIONS:\n");
  printf ("     -k K: window size for kxk mask (k >= 3, default = %d)\n", DFLTMAXK);
  printf (" -n NITER: maximum number of iterations (default max = %d)\n", MAXITER);
  printf ("       -d: to display results of each iteration (< 40x40 image)\n");
  printf ("       -I: invert input image before processing\n");
  printf ("       -L: print Software License for this module\n");

  return (-1);
}


/* INPUT:       function reads input parameters
 *                  usage: input (argc, argv, &maxK, &nMaxIter, &displayFlag)
 */

#define USAGE_EXIT(VALUE) {usage (VALUE); return (-1);}

long
input (argc, argv, maxK, nMaxIter, displayFlag, invertFlag)
     int argc;
     char *argv[];
     long *maxK,                /* max. sidelength of thinning kernel */
      *nMaxIter,                /* max. no. iterations */
      *displayFlag,             /* display results after each iter if =1 */
      *invertFlag;              /* invert input image before processing */

{
  long n;
  static char *kString = "-k";
  static char *nString = "-n";
  static char *dString = "-d";
  static char *IString = "-I";
  static char *dashString = "-";

  if (argc == 1)
    USAGE_EXIT (1);
  if (argc == 2)
    USAGE_EXIT (0);

  *maxK = DFLTMAXK;
  *nMaxIter = MAXITER;
  *displayFlag = 0;
  *invertFlag = 0;

  for (n = 3; n < argc; n++) {
    if (strcmp (argv[n], kString) == 0) {
      if (++n == argc || argv[n][0] == '-')
        USAGE_EXIT (0);
      *maxK = atol (argv[n]);
    }
    else if (strcmp (argv[n], nString) == 0) {
      if (++n == argc || argv[n][0] == '-')
        USAGE_EXIT (0);
      *nMaxIter = atol (argv[n]);
    }
    else if (strcmp (argv[n], dString) == 0)
      *displayFlag = 1;
    else if (strcmp (argv[n], IString) == 0)
      *invertFlag = 1;
    else if (strcmp (argv[n], "-L") == 0) {
      print_sos_lic ();
      exit (0);
    }
    else
      USAGE_EXIT (0);
  }
  if (*maxK < 3)
    USAGE_EXIT (1);
  return (0);
}
