/******************************************************************************/
/*                                                                            */
/*    Copyright (c)Cedric Launay.                                             */
/*    All Rights Reserved.                                                    */
/*                                                                            */
/******************************************************************************/

#include "include/BCutilities.h"
#include "include/BCillumination.h"
#include "include/BCpragma.h"

class BCmonolithic(
	// init diffuse
	uniform color diffuseColor                  = .18 ;
	string diffuseTexture                       = ""  ;
	uniform color diffuseGain                   = 1   ;
	// uniform color diffuseDirectContribution     = 1   ;
	// uniform color diffuseIndirectContribution   = 1   ;

	// init sss
	uniform float sssMix                        = 0   ;
	uniform color scatteringColor               = (.5, 1, 1);
	uniform color absorptionColor               = 1   ;
	uniform color sssGain                       = 1   ;
	uniform float scatteringDepth               = 0.1 ;
	uniform float sssSamples                    = 4 ;
	uniform float sssIor                        = 1.2 ;

	// init specular
	uniform color specularColor                 = 1   ;
	uniform string specularTexture              = ""  ;
	color specularGain                          = 1   ;
	uniform float specularRoughness             = .08 ;
	// string specularRoughnessTexture             = ""  ;
	uniform float specularAnisotropy            = 0   ;
	uniform float specularAnisotropyRoation     = 0   ;
	// uniform float specularSamples               = 4   ;
	// uniform color specularDirectContribution    = 1   ;
	// uniform color specularIndirectContribution  = 1   ;

	uniform float specularMix                   = 0   ;
	uniform color specularColor2                = 1   ;
	string specularTexture2                     = ""  ;
	uniform color specularGain2                 = 1   ;
	uniform float specularRoughness2            = .08 ;
	// string specularRoughnessTexture2            = ""  ;
	uniform float specularAnisotropy2           = 0   ;
	uniform float specularAnisotropyRoation2    = 0   ;
	// uniform float specularSamples2              = 4   ;
	// uniform color specularDirectContribution2   = 1   ;
	// uniform color specularIndirectContribution2 = 1   ;

	// init transparency
	uniform float transparency                  = 1   ;

	// init normalMap
	string normalMap                            = ""  ;
	uniform float bumpGain                      = 1   ;

	// init AOVs
	// output varying color aov_diffuseDirect      = 0   ;
	// output varying color aov_diffuseIndirect    = 0   ;
	// output varying color aov_specularDirect     = 0   ;
	// output varying color aov_specularIndirect   = 0   ;
	)
{
	// init input
	normal Nf;
	vector V;
	float specularRoughnessValue;
	float specularRoughnessValue2;
	color diffuseValue;
	color specularValue;
	color specularValue2;
	// color specularDirect1;
	// color specularDirect2;
	// color specularIndirect1;
	// color specularIndirect2;
	// init components
	color diffuseComponent;
	color sssComponent;
	color specularComponent;
	color specular1Component;
	color specular2Component;
	// color diffuseDirect;
	// color diffuseIndirect;
	// color specularDirect;
	// color specularIndirect;

	public void begin()
	{
		// defind normal
		Nf = normalForward();
		V  = - normalize( I );

		// definde normal map
		if (normalMap != "") Nf = normalMap( Nf, normalMap, bumpGain );

		// defind color values
		diffuseValue     = getColorValue( diffuseColor, diffuseTexture ) * diffuseGain;
		specularValue    = getColorValue( specularColor, specularTexture ) * specularGain;
		specularValue2   = getColorValue( specularColor2, specularTexture2 ) * specularGain2;

		// defind float values
		// TODO implement roughness mapping
		// float roughnessValue   = getFloatValue( roughness, roughnessTexture );
		// float roughnessValue2  = getFloatValue( roughness2, roughnessTexture2 );
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
		/*
		// Computation diffuseDirect and diffuseIndirect
		if( diffuseDirectContribution )
			diffuseDirect = diffuseDirectComponent( Nf, diffuseValue ) * diffuseDirectContribution;
		if( diffuseIndirectContribution )
			diffuseIndirect  = diffuseIndirectComponent( Nf, diffuseValue ) * diffuseIndirectContribution;

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
		*/


		// define aera options
		uniform float areaSample = 16;

		// define environnement options
		uniform string environnementMap = "";
		option( "user:environnementMap", environnementMap );
		uniform color environnementTint = 1;
		option( "user:environnementTint", environnementTint );
		uniform color environnementShadowTint = 0;
		option( "user:environnementShadowTint", environnementShadowTint );
		uniform float environnementRadius = 100;
		option( "user:environnementRadius", environnementRadius );
		uniform float environnementSample = 128;
		option( "user:environnementSample", environnementSample );

		// define indirect options
		uniform float diffuseIndirectEnable = 0;
		option( "user:diffuseIndirectEnable", diffuseIndirectEnable );
		uniform float specularIndirectEnable = 0;
		option( "user:specularIndirectEnable", specularIndirectEnable );


		// Computation diffuseComponent and sss
		if( sssMix != 1)
		{
			diffuseComponent = diffuseLoop( Nf, V, P, environnementMap,  diffuseValue, environnementTint, environnementShadowTint, environnementRadius, environnementSample, areaSample, diffuseIndirectEnable );
		}
		if( sssMix != 0)
		{
			// TODO : find a way for indirect lighting in sss
			sssComponent = subsurfaceComponent( P, normalize(N), V, sssSamples, sssIor, scatteringColor, absorptionColor, scatteringDepth, 0.1 ) * sssGain;
		}
		diffuseComponent =  mix( diffuseComponent, sssComponent, sssMix );

		// Computation specularComponent
		if( specularMix != 1 )
		{
			specular1Component = specularLoop( Nf, V, P, environnementMap,  specularValue, environnementTint, environnementShadowTint, environnementRadius, environnementSample, areaSample, specularRoughnessValue, specularAnisotropy, specularAnisotropyRoation, specularIndirectEnable );
		}
		if( specularMix != 0 )
		{
			specular2Component = specularLoop( Nf, V, P, environnementMap,  specularValue2, environnementTint, environnementShadowTint, environnementRadius, environnementSample, areaSample, specularRoughnessValue2, specularAnisotropy2, specularAnisotropyRoation2, specularIndirectEnable );
		}
		specularComponent = mix( specular1Component, specular2Component, specularMix );


		// defind Oi / Ci
		Oi  = transparency;
		Ci  = diffuseComponent;
		Ci += specularComponent;
		Ci *= Oi;

		// TODO : Find good way to output AOVs
		// defind AOVs
		// aov_diffuseDirect = diffuseDirect;
		// aov_diffuseIndirect = diffuseIndirect;
		// aov_specularDirect = specularDirect;
		// aov_specularIndirect = specularIndirect;

	}
}
