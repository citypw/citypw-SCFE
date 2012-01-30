/********************************************************************
 * File: instrument.c
 *
 * Instrumentation source -- link this with your application, and
 *  then execute to build trace data file (trace.txt).
 *
 * Author: M. Tim Jones <mtj@mtjones.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>

/* Function prototypes with attributes */
void main_constructor( void )
	__attribute__ ((no_instrument_function, constructor));

void main_destructor( void )
	__attribute__ ((no_instrument_function, destructor));

void __cyg_profile_func_enter( void *, void * ) 
	__attribute__ ((no_instrument_function));

void __cyg_profile_func_exit( void *, void * )
	__attribute__ ((no_instrument_function));


static FILE *fp;


void main_constructor( void )
{
  fp = fopen( "trace.txt", "w" );
  if (fp == NULL) exit(-1);
}


void main_deconstructor( void )
{
  fclose( fp );
}

/**
 * Shawn:there's only 2 lines of code that has been added about which make sure
 * the program write the infos into trace.txt when errors occur
 */
void __cyg_profile_func_enter( void *this, void *callsite )
{
  fprintf(fp, "E%p\n", (int *)this);
  fflush(fp);
}


void __cyg_profile_func_exit( void *this, void *callsite )
{
  fprintf(fp, "X%p\n", (int *)this);
  fflush(fp);
}

