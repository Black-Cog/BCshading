/******************************************************************************/
/*                                                                            */
/*    Copyright (c)Cedric Launay.                                             */
/*    All Rights Reserved.                                                    */
/*                                                                            */
/******************************************************************************/

#include "include/BCutilities.h"
#include "include/BCillumination.h"
#include "include/BCpragma.h"


class BCglass(
	// init refract
	uniform color refractionColor               = 1   ;
	// uniform float refractionRoughness           = .08 ;
	uniform float refractionSamples             = 4   ;
	uniform float refractionIor                 = 1.5 ;
	// uniform float refractionAnisotropy          = 0   ;
	// uniform float refractionAnisotropyRoation   = 0   ;

	// init specular
	uniform color specularColor                 = 1   ;
	uniform string specularTexture              = ""  ;
	color specularGain                          = 1   ;
	uniform float specularRoughness             = .08 ;
	string specularRoughnessTexture             = ""  ;
	uniform float specularAnisotropy            = 0   ;
	uniform float specularAnisotropyRoation     = 0   ;
	uniform float specularSamples               = 4   ;
	uniform color specularDirectContribution    = 1   ;
	uniform color specularIndirectContribution  = 1   ;

	uniform float specularMix                   = 0   ;
	uniform color specularColor2                = 1   ;
	string specularTexture2                     = ""  ;
	uniform color specularGain2                 = 1   ;
	uniform float specularRoughness2            = .08 ;
	string specularRoughnessTexture2            = ""  ;
	uniform float specularAnisotropy2           = 0   ;
	uniform float specularAnisotropyRoation2    = 0   ;
	uniform float specularSamples2              = 4   ;
	uniform color specularDirectContribution2   = 1   ;
	uniform color specularIndirectContribution2 = 1   ;

	// init normalMap
	string normalMap                            = ""  ;
	uniform float bumpGain                      = 1   ;

	// init AOVs
	output varying color aov_refraction         = 0   ;
	output varying color aov_specularDirect     = 0   ;
	output varying color aov_specularIndirect   = 0   ;
	)
{
	// init input
	normal Nf;
	vector V;
	vector In;
	float refractionRoughnessValue;
	float specularRoughnessValue;
	float specularRoughnessValue2;
	color refractionValue;
	color specularValue;
	color specularValue2;
	color specularDirect1;
	color specularDirect2;
	color specularIndirect1;
	color specularIndirect2;
	// init components
	color refraction;
	color specularDirect;
	color specularIndirect;

	public void begin()
	{
		// defind normal and vector
		Nf = normalForward();
		In = normalize( I );
		V  = - In;

		// definde normal map
		if (normalMap != "") Nf = normalMap( Nf, normalMap, bumpGain );
		
		// defind color values
		refractionValue  = refractionColor;
		specularValue    = getColorValue( specularColor, specularTexture ) * specularGain;
		specularValue2   = getColorValue( specularColor2, specularTexture2 ) * specularGain2;

		// defind float values
		refractionRoughnessValue = specularRoughness / 5; 
		specularRoughnessValue   = specularRoughness;
		specularRoughnessValue2  = specularRoughness2;
	}


	public void displacement(output point P; output normal N)
	{
		P = P;
		N = calculatenormal( P );
	}


	public void surface(output color Ci, Oi)
	{
		// Computation refraction
		refraction = refractionComponent( Nf, V, refractionValue, refractionRoughnessValue, refractionIor, specularAnisotropy, specularAnisotropyRoation, refractionSamples );

		// Computation specularDirect and specularIndirect
		if( specularMix != 1 )
		{
			if( specularDirectContribution )
				specularDirect1  = specularDirectComponent( Nf, V, specularValue, specularRoughnessValue, specularAnisotropy, specularAnisotropyRoation ) * specularDirectContribution;
			if( specularIndirectContribution )
				specularIndirect1  = specularIndirectComponent( Nf, V, specularValue, specularRoughnessValue, specularAnisotropy, specularAnisotropyRoation, specularSamples ) * specularIndirectContribution;
		}
		if( specularMix != 0 )
		{
			if( specularDirectContribution2 )
				specularDirect2  = specularDirectComponent( Nf, V, specularValue2, specularRoughnessValue2, specularAnisotropy2, specularAnisotropyRoation2 ) * specularDirectContribution2;
			if( specularIndirectContribution2 )
				specularIndirect2  = specularIndirectComponent( Nf, V, specularValue2, specularRoughnessValue2, specularAnisotropy2, specularAnisotropyRoation2, specularSamples2 ) * specularIndirectContribution2;
		}

		specularDirect   = mix( specularDirect1, specularDirect2, specularMix );
		specularIndirect = mix( specularIndirect1, specularIndirect2, specularMix );

		// defind Oi / Ci
		Oi  = 1;
		Ci  = refraction;
		Ci += specularDirect + specularIndirect;


		aov_refraction = refraction;
		aov_specularDirect = specularDirect;
		aov_specularIndirect = specularIndirect;

	}
}

