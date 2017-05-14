/*
 * Copyright (C) 2016  Tom Trebisky  <tom@mmto.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. See README and COPYING for
 * more details.
 *
 * float.c for the Orange Pi PC and PC Plus
 *
 * Tom Trebisky  5/13/2017
 *
 */

static void
puts ( char *msg )
{
	serial_puts ( msg );
}

#ifdef notdef
/* We want to trigger some kind of fault.
 * Amazingly, ANY address on the Orange Pi is readable.
 * We do this to ensure that our exception handling code works.
 */
static void
fail ( void )
{
	puts ( "Preparing to fail ...\n" );

	/* This don't work !!
	long *p = (long *) 0xa0000000;
	*p = 0;
	 */

	// this is a handy thing gcc provides.
	// it yields an undefined instruction trap.
	//__builtin_trap ();

	// this yields an undefined instruction.
	// this is supposed to be an ARM
	// permanently undefined instruction encoding.
	//    0xf7fXaXXX
	asm volatile (".word 0xf7f0a000\n");

	// We can use this to test the fault routine
	// trap_ui ();

	/* We better not see this message */
	puts ( "All done failing.\n" );
}
#endif

static int
sqrt_i ( int arg )
{
        float farg = arg;
        float root;

        asm volatile ("vsqrt.f32 %0, %1" : "=w" (root) : "w" (farg) );

        return 10000 * root;
}

static int
sqrt_d ( int arg )
{
        double farg = arg;
        double root;

        asm volatile ("vsqrt.f64 %0, %1" : "=w" (root) : "w" (farg) );

        return 10000 * root;
}

void
arm_float ( void )
{
        int val;
        int num = 2;

        val = sqrt_i ( num );
        // printf ( "Square root of %d is %d\n", num, val );
	print_num ( "Square root is:", val );
        val = sqrt_d ( num );
        // printf ( "Square root of %d is %d\n", num, val );
	print_num ( "Square root is:", val );
}


/* This gets called by the test menu
 *   (or something of the sort)
 */
void
test_float ( void )
{
	int reg;

	serial_init ( 115200 );

	// fail ();

	fp_enable ();
	arm_float ();

	puts ( "Done\n" );
}

/* THE END */
