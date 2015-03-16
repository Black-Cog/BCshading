/******************************************************************************/
/*                                                                            */
/*    Copyright (c)Cedric Launay.                                             */
/*    All Rights Reserved.                                                    */
/*                                                                            */
/******************************************************************************/


#ifndef _BCutilities_h
#define _BCutilities_h

#define EPSILON  1e-6
#define INFINITE 1e10

/******************************************************************************/
/**********************************  Normal  **********************************/
/******************************************************************************/

normal
normalForward()
{
	extern normal N;
	extern vector I;

	normal Nn = normalize( N );

	return faceforward( Nn, I );
}

normal
normalMap(
	normal Nf;
	string normalMap;
	float bumpGain;
	)
{
	color map = texture( normalMap );
	normal newN = 0;

	// bump case
	if ( map[0] == map[1] && map[0] == map[2] )
	{
		extern point P;
		extern normal N;
		extern normal Ng;

		float offset = clamp(map[0] * bumpGain, 0, 1);

		normal Nn = normalize( N );
		normal Ngn = normalize(Ng);
		normal Noffset = Nn - Ngn;
		
		float scale = 0.25;
		
		point Pp =
			transform("world", P) +
			normalize(ntransform("world", Ngn)) * offset * scale;
		Nn = -ntransform("world", "current", calculatenormal(Pp));
		Nn = normalize(Nn);
		newN = normalize(Nn + Noffset);
	}
	// normal map case
	else
	{
		color normalizeNormalMap = ( map + 0.5 );
		newN = Nf * normal(normalizeNormalMap);
	}
	
	// output
	return normalize( newN );
}

/******************************************************************************/
/***********************************  Float  **********************************/
/******************************************************************************/

float
falloff(
	float min;
	float max;
	)
{
	extern normal N;
	extern vector I;

	normal Nf = normalForward();
	vector V  = - normalize( I );

	float falloff = ( Nf.V ) * 1;

	return mix( min, max, falloff );
}


float
getFloatValue(
	float colorValue;
	string textureValue;
	)
{
	float outValue = colorValue;
	if( textureValue != "" )
		outValue = float texture( textureValue );

	return outValue;
}

float
exposureToIntensity(
	float exposure;
	)
{
	float intensity = pow( 2, exposure );

	return intensity;
}

/******************************************************************************/
/***********************************  Color  **********************************/
/******************************************************************************/

color
falloff(
	color min;
	color max;
	)
{
	extern normal N;
	extern vector I;

	normal Nf = normalForward();
	vector V  = - normalize( I );

	float falloff = ( Nf.V ) * 1;

	return mix( min, max, falloff );
}

color
getColorValue(
	color colorValue;
	string textureValue;
	)
{
	color outValue = colorValue;
	if( textureValue != "" )
		outValue = color texture( textureValue );

	return outValue;
}

/******************************************************************************/
/**********************************  Vector  **********************************/
/******************************************************************************/

vector anisotrpyRotation(float rotation;)
{
	float normalRotation = rotation / 90 - 1;
	vector anisotropyDirection = ( normalRotation, 1 - abs( normalRotation ), 0 );

	return anisotropyDirection;
}



#endif