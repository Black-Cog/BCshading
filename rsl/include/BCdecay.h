/******************************************************************************/
/*                                                                            */
/*    Copyright (c)Cedric Launay.                                             */
/*    All Rights Reserved.                                                    */
/*                                                                            */
/******************************************************************************/


#ifndef _BCdecay_h
#define _BCdecay_h



/******************************************************************************/
/***********************************  Color  **********************************/
/******************************************************************************/

float
decay(
	float lightDecay;
	float distance;
	)
{
	// constant
	if( lightDecay == 0 )
	{
	return 1;
	}

	// linear
	else if( lightDecay == 1 )
	{
		return 1 / distance ;
	}

	// quadratic
	else if( lightDecay == 2 )
	{
		return 1 / ( distance * distance) ;
	}

}


#endif