/******************************************************************************/
/*                                                                            */
/*    Copyright (c)Cedric Launay.                                             */
/*    All Rights Reserved.                                                    */
/*                                                                            */
/******************************************************************************/

#include "include/BCutilities.h"

#ifndef _BCillumination_h
#define _BCillumination_h

/////////////////////////////////////////////
////////////////// Diffuse //////////////////
/////////////////////////////////////////////

color
diffuseDirectComponent(
	varying normal Nf;
	varying color diffuseValue;
	)
{
	if( diffuseValue != 0 )
	{
		return diffuseValue * diffuse(Nf);
	}
	return color( 0 );
}

color
diffuseIndirectComponent(
	varying normal N;
	varying color diffuseValue;
	)
{
	extern point P;
	shader gi_shader = getshader( "maya_gi_shader" );
	if( gi_shader != null )
	{
		varying color gi = gi_shader->ComputeGI( P, N, diffuseValue );
		return gi * diffuseValue;
	}
	return color( 0 );
}


color
diffuseLoop(
	normal Nn;
	vector V;
	point Pos;
	string environnementMap;
	varying color diffuseValue;
	color environnementTint;
	color environnementShadowTint;
	float environnementRadius;
	float environnementSample;
	float areaSample;
	float indirect;
	)
{
	if( diffuseValue == 0 )
	{
		return 0;
	}
	else
	{
		color directDiffuse, indirectDiffuse, environnement, environnementUnShadow, area, areaUnshadow, result;

		directDiffuse = diffuse(Nn);

		illuminance( "", Pos, Nn, PI )
		{
			if( indirect == 0 )
			{
				trace( 	Pos, Nn,
						"distribution", "cosine",
						"samplecone", PI/2,
						"maxdist", environnementRadius,
						"samples", environnementSample,
						"environmentmap", environnementMap,
						"environmenttint", environnementTint,
						"samplearealights", areaSample,
						"environmentspace", "world",
						"subset", "",
						"environmentcontribution", environnement,
						"unshadowedenvironment", environnementUnShadow,
						"arealightcontribution", area,
						"unshadowedarealight", areaUnshadow
						);

				if( environnementShadowTint == 1 )
				{
					result = environnementUnShadow;
				}
				else
				{
					color environnementShadow = ( environnementUnShadow - environnement ) / environnementUnShadow;
					result = environnementUnShadow * ( 1 - environnementShadow * ( color(1) - environnementShadowTint ) );
				}
			}
			else
			{
				indirectDiffuse = trace( 	Pos, Nn,
											"distribution", "cosine",
											"samplecone", PI/2,
											"maxdist", environnementRadius,
											"samples", environnementSample,
											"environmentmap", environnementMap,
											"environmenttint", environnementTint,
											"samplearealights", areaSample,
											"environmentspace", "world",
											"subset", ""
											);
				result = indirectDiffuse;
			}
		}

		return diffuseValue * ( directDiffuse + result );
	}
}

color
specularLoop(
	normal Nn;
	vector V;
	point Pos;
	string environnementMap;
	varying color specularValue;
	color environnementTint;
	color environnementShadowTint;
	float environnementRadius;
	float environnementSample;
	float areaSample;
	float roughness;
	float anisotropy;
	float anisotropyRoation;
	float indirect;
	)
{
	if( specularValue == 0 )
	{
		return 0;
	}
	else
	{
		color directSpecular, indirectSpecular, environnement, environnementUnShadow, area, areaUnshadow, result;

		float anisotropyValue = 1 / ( 1 - 0.75 * abs( anisotropy ) );
		vector anisotropyUdir = anisotrpyRotation( anisotropyRoation );
		color diffuseHit = diffuse(Nn);

		illuminance( "", Pos, Nn, PI )
		{
			directSpecular = bsdf(  L, Nn,
									"distribution", "ashikhmin-shirley",
									"samplecone", PI/2,
									"wo", V,
									"roughness", roughness * anisotropyValue,
									"roughnessv", roughness / anisotropyValue,
									"udir", anisotropyUdir
									);

			// TODO check if it's right !
			directSpecular *= diffuseHit;


			if( indirect == 0 )
			{
				trace( 	Pos, Nn,
						"distribution", "ashikhmin-shirley",
						"samplecone", PI/2,
						"wo", V,
						"roughness", roughness * anisotropyValue,
						"roughnessv", roughness / anisotropyValue,
						"udir", anisotropyUdir,
						"maxdist", environnementRadius,
						"samples", environnementSample,
						"environmentmap", environnementMap,
						"environmenttint", environnementTint,
						"samplearealights", areaSample,
						"environmentspace", "world",
						"subset", "",
						"environmentcontribution", environnement,
						"unshadowedenvironment", environnementUnShadow,
						"arealightcontribution", area,
						"unshadowedarealight", areaUnshadow
						);

				if( environnementShadowTint == 1 )
				{
					result = environnementUnShadow;
				}
				else
				{
					color environnementShadow = ( environnementUnShadow - environnement ) / environnementUnShadow;
					result = environnementUnShadow * ( 1 - environnementShadow * ( color(1) - environnementShadowTint ) );
				}
			}
			else
			{
				indirectSpecular = trace( 	Pos, Nn,
											"distribution", "ashikhmin-shirley",
											"samplecone", PI/2,
											"wo", V,
											"roughness", roughness * anisotropyValue,
											"roughnessv", roughness / anisotropyValue,
											"udir", anisotropyUdir,
											"maxdist", environnementRadius,
											"samples", environnementSample,
											"environmentmap", environnementMap,
											"environmenttint", environnementTint,
											"samplearealights", areaSample,
											"environmentspace", "world",
											"subset", ""
											);
				result = indirectSpecular;
			}
		}

		return specularValue * ( directSpecular + result );
	}
}




/////////////////////////////////////////////
////////////////// Specular /////////////////
/////////////////////////////////////////////

color
specularDirectComponent(
	varying normal Nf;
	varying vector V;
	varying color specularValue;
	varying float roughness;
	uniform float anisotropy;
	uniform float anisotropyRoation;
	)
{

	if( specularValue != 0 )
	{
		extern point P;

		color bsdf    = 0;
		color outSpecularDirect;
		float minBsdf = 0;
		float maxBsdf = 100;

		float anisotropyValue = 1 / ( 1 - 0.75 * abs( anisotropy ) );
		vector anisotropyUdir = anisotrpyRotation( anisotropyRoation );

		color diffuseHit = diffuse(Nf);

		illuminance( "-__3dfm_gi_light", P, Nf, PI )
		{
			// TODO check if it's right !
			if( diffuseHit != 0 )
			{
				bsdf += bsdf(
							L, Nf,
							"distribution", "ashikhmin-shirley",
							"wo", V,
							"roughness", roughness * anisotropyValue,
							"roughnessv", roughness / anisotropyValue,
							"udir", anisotropyUdir );
			}
		}

		outSpecularDirect  = clamp( bsdf, minBsdf, maxBsdf ) * specularValue;
		// TODO best setting for the specular falloff check for diffuse add > conservation of power law
		// outSpecularDirect *= falloff( 1, 0.5 );

		return outSpecularDirect;
	}
	return color( 0 );

}

color
specularIndirectComponent(
	varying normal Nf;
	varying vector V;
	varying color specularValue;
	uniform float roughness;
	uniform float anisotropy;
	uniform float anisotropyRoation;
	uniform float samples;
	)
{

	if( specularValue != 0 )
	{
		extern point P;
		extern vector I;

		vector In = normalize(I);

		color environmentcontribution = 0;
		float distance = 1;
		float minSpecularIndirect = 0;
		float maxSpecularIndirect = 100;	
		color outSpecularIndirect = 0;

		if( roughness == 0 )
		{
			outSpecularIndirect = trace(
									P, Nf, distance,
									"samples", 1,
									"distribution", "blinn",
									"wo", V,
									"roughness", roughness,
									"weight", 1,
									"environmentmap", "",
									"environmentspace", "",
									"environmentcontribution", environmentcontribution,
									"subset", "");
		}
		else
		{
			float samplesNumber   = exposureToIntensity( samples );

			if( anisotropy == 0 )
			{
				outSpecularIndirect = trace(
									P, Nf, distance,
									"samples", samplesNumber,
									"distribution", "blinn",
									"wo", V,
									"roughness", roughness,
									"weight", 1,
									"environmentmap", "",
									"environmentspace", "",
									"environmentcontribution", environmentcontribution,
									"subset", "");
			}
			else
			{
				float anisotropyValue  = 1/(1-0.75*abs(anisotropy));
				vector anisotropyUdir = anisotrpyRotation( anisotropyRoation );

				outSpecularIndirect = trace(
									P, Nf, distance,
									"samples", samplesNumber,
									"distribution", "ashikhmin-shirley",
									"wo", V,
									"roughness", roughness * anisotropyValue,
									"roughnessv", roughness / anisotropyValue,
									"udir", anisotropyUdir,
									"weight", 1,
									"environmentmap", "",
									"environmentspace", "",
									"environmentcontribution", environmentcontribution,
									"subset", "");
			}
		}

		outSpecularIndirect = clamp( outSpecularIndirect, minSpecularIndirect, maxSpecularIndirect ) * specularValue;

		return outSpecularIndirect;
	}
	return color( 0 );
}

/////////////////////////////////////////////
///////////////// Refraction ////////////////
/////////////////////////////////////////////

color refractionComponent(
		varying normal Nf;
		varying vector V;
		varying color refractionValue;
		uniform float roughness;
		uniform float ior;
		uniform float anisotropy;
		uniform float anisotropyRoation;
		uniform float samples;
		)
{

	if( refractionValue != 0 )
	{
		extern point P;
		extern vector I;

		vector In = normalize(I);

		float samplesNumber = exposureToIntensity( samples );
		vector trace_dir = refract( In, Nf, 1 / (ior*10) );
		float distance   = 1;
		color environmentcontribution = 0;
		color outRefraction = 0;

		if( roughness == 0 )
		{
			outRefraction =	trace(
								P, trace_dir, distance,
								"samples", 1,
								"weight", 1,
								"environmentmap", "",
								"environmentspace", "",
								"environmentcontribution", environmentcontribution,
								"subset", "");
		}
		else
		{
			vector Vrefraction = reflect(-trace_dir, -Nf);
			normal Nfinvert = -Nf;

			if( anisotropy == 0 )
			{
				outRefraction =	trace(
									P, Nfinvert, distance,
									"samples", samplesNumber,
									"distribution", "blinn",
									"wo", Vrefraction,
									"roughness", roughness,
									"weight", 1,
									"environmentmap", "",
									"environmentspace", "",
									"environmentcontribution", environmentcontribution,
									"subset", "");
			}
			else
			{
				float anisotropyValue  = 1/(1-0.75*abs(anisotropy));
				vector anisotropyUdir = anisotrpyRotation( anisotropyRoation );

				outRefraction =	trace(
									P, Nfinvert, distance,
									"samples", samplesNumber,
									"distribution", "ashikhmin-shirley",
									"wo", Vrefraction,
									"roughness", roughness * anisotropyValue,
									"roughnessv", roughness / anisotropyValue,
									"udir", anisotropyUdir,
									"weight", 1,
									"environmentmap", "",
									"environmentspace", "",
									"environmentcontribution", environmentcontribution,
									"subset", "");
			}
		}

		outRefraction *= refractionValue;

		return outRefraction;
	}
	return color( 0 );
}

/////////////////////////////////////////////
//////////////// Subsurface /////////////////
/////////////////////////////////////////////

color normalize_color(color c)
{
	float multiplier =
		max(max(c[0], c[1]), c[2]);

	if( multiplier>0 )
	{
		return c/multiplier;
	}

	return 0;
}

color
subsurfaceComponent(
	varying normal Nf;
	)
{
	// if( diffuseValue != 0 )
	// {
	// 	return diffuseValue * diffuse(Nf);
	// }
	// return color( 0 );

// 
	extern point P;
	extern normal N;
	// color absorption = ( 0.75, 0.5, 0.5 );
	// float sss_scale;
	// float sss_transmittance_scale;
	// float sss_scattering_scale;
	// color i_sss_scattering = ( 1, 1, 1 );
	// float i_sss_ior = 1.2;

	// float i_sss_scale = 0.1;
	// float i_sss_transmittance_scale = 0.1;
	// float i_sss_scattering_scale = 1;
	// string i_subsurface_type = "raytraced";

	// float i_sss_samples = 16;

	// /* Absorption should be in [0,1] range, otherwise inverting will
	//  * make a negative value. */
	// if( max( max( absorption[0], absorption[1] ), absorption[2] ) > 1 )
	// {
	// 	absorption = normalize_color( absorption );
	// }

	// /*
	// 	clamp scale values to 1e-4.
	// */
	// sss_scale = i_sss_scale > 0 ? i_sss_scale : 1e-4;
	// sss_transmittance_scale = i_sss_transmittance_scale > 0 ? i_sss_transmittance_scale : 1e-4;
	// sss_scattering_scale = i_sss_scattering_scale > 0 ? i_sss_scattering_scale : 1e-4;

	// absorption = normalize_color(1-absorption);
	// absorption += EPSILON;
	// absorption *= sss_transmittance_scale;

	// color scattering = normalize_color( i_sss_scattering );
	// scattering *= sss_scattering_scale;

	// color sub_surface = 0 ;

	// if( i_subsurface_type != "" )
	// {
	// 	sub_surface = subsurface(
	// 				P, normalize(Nf), /* do not pass bump normal */
	// 				"type", i_subsurface_type,
	// 				"samples", i_sss_samples,
	// 				"model", "grosjean",
	// 				"scattering", scattering,
	// 				"absorption", absorption,
	// 				"ior", i_sss_ior,
	// 				"scale", sss_scale,
	// 				"irradiance", diffuse(Nf));
	// }
	// else
	// {
	// 	sub_surface = subsurface(
	// 				P, normalize(Nf), /* do not pass bump normal */
	// 				"model", "grosjean",
	// 				"scattering", scattering,
	// 				"absorption", absorption,
	// 				"ior", i_sss_ior,
	// 				"scale", sss_scale,
	// 				"irradiance", diffuse(Nf));
	// }

	// sub_surface /= diffuse_color * coating_absorbed;

	color absorption   = ( 0.6, 0.5, 0.5 );
	color transmission = ( 0.5, 0.5, 0.5 );
	color sub_surface;

	// sub_surface = subsurface( 
	// 				P, N,
	// 				"type", "raytrace",
	// 				"samples", 4,
	// 				"model", "grosjean",
	// 				"scattering", transmission,
	// 				"absorption", absorption,
	// 				"ior", 1.2,
	// 				"scale", 0.1,
	// 				"irradiance", 1
	// 				);

	// color diffuse = diffuse(Nf);
	color diffuse = 0;
	extern vector I;
	vector V  = - normalize( I );
	float i_diffuse_roughness = 0.5;

	float arealight_samples = 16;
		// option( "user:_3dfm_arealight_samples", arealight_samples );

	color clight=0;
	trace(
		P, Nf,
		"wo", V,
		"bsdf", "oren-nayar",
		"type", "transmission",
		"hitsides", "reversed",
		"samplearealights", 1,
		"samples", arealight_samples,
		"arealightcontribution", clight,
		"roughness", i_diffuse_roughness );
	diffuse += clight;










	sub_surface = subsurface( 
					P, N,
					"type", "raytrace",
					"samples", 4,
					"model", "grosjean",
					"scattering", transmission,
					"absorption", absorption,
					"ior", 1.2,
					"scale", 0.1,
					"irradiance", diffuse
					);

	return diffuse ;
// 

}

#endif