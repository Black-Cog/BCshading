
#include "include/BCpragma.h"
#include "include/BCdecay.h"
#include "include/BCutilities.h"

light lightEnv(
	float lightExposure     =  0;
	color lightColor        =  1;	
	string lightEnvironment = "";

	float lightShadowEnable =  1;
	color lightShadowTint   =  0;
	float lightRadius       = 10;
	float lightSamples      =  4;
	float lightBias         = -1;

	string  __category = "";
	)
{
	float intensity   = exposureToIntensity( lightExposure );
	float realSamples = exposureToIntensity( lightSamples );

	normal wN = normalize( ntransform("current", Ns) );
	// normal wN = normalize( faceforward(Ns, I) );

	color envcolor = .18;
	illuminate( Ps + wN )
	{

		if( lightEnvironment == "" )
		{
			if( lightShadowEnable )
			{
				color shadowResult = 1 - occlusion( Ps, wN, realSamples,
													"coneangle", PI/2,
													"distribution", "cosine",
													"hitsides", "front",
													"hitmode", "primitive",
													"bias", lightBias,
													"maxdist", lightRadius,
													"label", "occlusion"
													);
			envcolor *= mix( lightShadowTint, 1, shadowResult );
			}
		}
		else
		{
			// wN = normal "world" ( wN );
			indirectdiffuse( 	Ps, wN, realSamples,
								"coneangle", PI/2,
								"distribution", "cosine",
								"hitsides", "front",
								"hitmode", "primitive",
								"bias", lightBias,
								"maxdist", lightRadius,
								"label", "environnement",
								"environmentmap", lightEnvironment,
								"environmentcolor", envcolor
								);
		}

		Cl = envcolor * lightColor * intensity;

	}

}
