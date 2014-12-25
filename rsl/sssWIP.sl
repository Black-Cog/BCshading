/*
	A simple shader demonstrating the use of subsurface scattering	
*/

#include "include/BCutilities.h"
#include "include/BCillumination.h"
#include "include/BCpragma.h"

surface sssWIP( float Ks = .7, Kd = .6, Ka = .1, roughness = .04 )
{
	normal Nn = normalize(N);
    normal Nf = faceforward(Nn, I);
    vector V = normalize(-I);

	uniform string raytype = "unknown";
	
	rayinfo( "type", raytype );

	if( raytype == "subsurface" )
	{
    	Ci = Ka*ambient() + Kd*diffuse(Nn);
	}
	else
	{
		color diffuse = diffuse(Nn);
		
		// normalization a 1 entre scatteringDepth et absorptionDepth
		// float absorptionDepth = 0.25;
		// float scatteringDepth = 1 / absorptionDepth;
		float scatteringDepth = 0.1;
		
		color scattering = ( 1, 1, 1 ) * 1 / scatteringDepth;
		color absorption   = ( 1, 1, 1 );



		absorption = ( (1-absorption) + 0.000001 ) * 1 / scatteringDepth;
		Ci = subsurface(P, N,
					"type","raytrace",
					"samples", 32,
					"model", "grosjean",
					"scattering", scattering,
					"absorption", absorption,
					"ior", 1.2,
					"scale", 0.1,
					"irradiance", diffuse ) * 2;
	}
}

