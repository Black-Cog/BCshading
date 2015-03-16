
#include "include/BCpragma.h"
#include "include/BCdecay.h"
#include "include/BCutilities.h"

light lightDirectionnal(
	float lightExposure = 0;
	color lightColor    = 1;
	float lightDecay    = 2;

	float lightShadowEnable =  1;
	color lightShadowTint   =  0;
	float lightAngle        =  2;
	float lightSamples      =  4;
	float lightBias         = -1;

	string  __category = "";
	)
{
	float intensity   = exposureToIntensity( lightExposure );
	float realSamples = lightAngle!=0 ? exposureToIntensity( lightSamples ) : 1;

	solar( vector "shader" (0,0,1), 0.0 )
	{
		Cl = lightColor * intensity;

		if( lightShadowEnable )
		{
			point to = Ps + ( vector "shader" (0,0,-1) ) * INFINITE;
			color shadowResult = transmission( 	Ps, to,
												"samples", realSamples,
												"samplecone", radians( lightAngle ),
												"bias", lightBias
												);

			Cl *= mix( lightShadowTint, 1, shadowResult);
		}
	}

}
