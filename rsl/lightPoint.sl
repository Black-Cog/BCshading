
#include "include/BCpragma.h"
#include "include/BCdecay.h"
#include "include/BCutilities.h"

light lightPoint(
	float lightExposure = 0;
	color lightColor    = 1;
	float lightDecay    = 2;

	float lightShadowEnable =  1;
	color lightShadowTint   =  0;
	float lightSize         = .2;
	float lightSamples      =  4;
	float lightBias         = -1;

	string  __category = "";
	)
{
	float intensity   = exposureToIntensity( lightExposure );
	float realSamples = lightSize!=0 ? exposureToIntensity( lightSamples ) : 1;
	point pos = point "shader" 0;

	illuminate( pos )
	{
		float attenuation = decay( lightDecay, length(L) );

		Cl = lightColor * intensity * attenuation;

		if( lightShadowEnable )
		{
			color shadowResult = transmission(	Ps, pos,
												"samples",    realSamples,
												"samplebase", 0,
												"samplecone", lightSize,
												"bias",       lightBias
												);

			Cl *= mix( lightShadowTint, 1, shadowResult);
		}

	}

}
