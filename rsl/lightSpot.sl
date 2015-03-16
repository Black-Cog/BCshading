
#include "include/BCpragma.h"
#include "include/BCdecay.h"
#include "include/BCutilities.h"

light lightSpot(
	float lightExposure      =  0;
	color lightColor         =  1;
	float lightConeAngle     = 60;
	float lightPenumbraAngle = 10; 
	float lightDecay         =  2;

	float lightShadowEnable  =  1;
	color lightShadowTint    =  0;
	float lightSize          = .2;
	float lightSamples       =  4;
	float lightBias          = -1;

	string  __category = "";
	)
{
	float intensity   = exposureToIntensity( lightExposure );
	float realSamples = lightSize!=0 ? exposureToIntensity( lightSamples ) : 1;

	uniform point pos = point "shader" 0;
	uniform vector lightDir = vector "shader" ( 0,0,1 ); 

	uniform float illumAngle  = radians( lightConeAngle / 2 + lightPenumbraAngle ); 
	uniform float insideAngle = radians( lightConeAngle / 2 ); 

	illuminate( pos, lightDir, illumAngle ) 
	{
		float angle = acos( normalize( L ).lightDir ); 
		float attenuation = decay( lightDecay, length(L) );
		attenuation *= 1 - smoothstep( insideAngle, illumAngle, angle );

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
