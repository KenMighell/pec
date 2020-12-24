// file://pec.c
// 
// PEC: Period Error Calculator 
//
// @COPYLEFT 2013 Kenneth J. Mighell 
// GNU General Public License, verion 3.0 (GPLv3)
//
// How to compile:
//
// gcc -std=c99 -o pec pec.c 
//
// Test: ./pec 0.5274 0.0104 90.0
// Result: 23 microdays (period errror estimate)
//

#include <stdio.h>
#include <assert.h>
#include <math.h>

double pec
// test: pec( 0.5724, 0.0104, 90.0 ) 
// result: 23 microdays (period errror estimate)
(
 double period,  // period [days]
 double sigma,   // one-sigma timing uncertainty for one flux value [days] 
 double length   // total length of time series [days] 
)
{
  double err, error, sig, count, periods, sum = 0.0, min_error = 1.0e30 ;
  long max_periods, i, n = 0;
  assert( (period > 0.0) && (sigma > 0.0) && (length > 0.0) );
  max_periods = (long)(length / period);
  sig = sqrt(2.0)*sigma; 
  for ( i = 1; i <= max_periods; i++ )
    {
      periods = max_periods - (i-1);
      count = i;
      n += count;
      err = (sig / periods);        // measurement error for one pair
      err *= (1.0 / sqrt(count));   // include all pairs
      sum += err * err;             // sum of the square of the errors
      error = sqrt(sum / count);    // total measurement error ... so far
      if ( error < min_error )
	min_error = error;
      if ( error > min_error )      // if getting worse, bail out now
      goto bye;
      printf (" %ld %ld %16.9e %16.9e %ld\n", 
	      (long)periods, (long)count, err, error, n );
    }
 bye: 
  return min_error;
}

int main(int argc, char *argv[])
{
  int status = 0;
  double periodD, sigmaD, max_daysD, error;

  if (argc < 4)
    {
      printf( "usage: pec periodD sigmaD max_daysD \n" );
      goto bye;
    }
  printf( "# %s %s %s %s\n", argv[0], argv[1], argv[2], argv[3] );

  if ( ( 1 != sscanf( argv[1], "%lg", &periodD) )
       || ( 1 != sscanf( argv[2], "%lg", &sigmaD ) )
       || ( 1 != sscanf( argv[3], "%lg", &max_daysD ) )
       )
    {
      printf( "*** ERROR *** Could not parse one of the input parameters!\n" );
      goto bye;
    }

  //periodD = 0.5274; // 0.5274 days (FN Lyr: a non-Blashko ab-type RR Lyrae) 
  //sigmaD = 0.0104;  // 0.0104 days (15 minutes)
  //max_daysD = 90.0; // 90 days is the length of a nominal Kepler quarter
  //min_error = 23 microdays

  printf("# %lg =periodD\n", periodD);
  printf("# %lg =sigmaD\n", sigmaD);
  printf("# %lg =max_daysD\n", max_daysD);

  error = pec( periodD, sigmaD, max_daysD );

  printf( "# %16.9f =error  command: %s %s %s %s | %12.6e\n",
	  error, argv[0], argv[1], argv[2], argv[3], error );

bye:
  return(status);
}
