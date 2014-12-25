
#ifndef __advancedMaterial_h
#define __advancedMaterial_h

#include "global_illumination.h"
#include "shading_utils.h"
#include "utils.h"

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

/*
	This function computes the eta the surface would have
	/at normal incidence/ for the given intensity 'x'.

	Simplifyes the fresnel for such a scenario (for example
	the equation 22 from the GGX) gives a very simple:

      intensity = [(eta-1)*(eta-1)] / [(eta+1)*(eta+1)]; 

	Inverting this equation gives the equally simple:

      eta = (T+1) / (1-T) for T = sqrt(intensity)


	Wolfram: 
		
		solve T =((eta-1)*(eta-1)) / ((eta+1)*(eta+1)) for eta
*/
float eta_from_intensity( float x )
{
	if( x >= 1 )
	{
		/* perfect mirror */
		return 500;
	}

	float T = sqrt( x );
	return (T+1) / (1-T);
}

/****************************************************/
/******************** THE SHADER ********************/
/****************************************************/

void _3DelightMaterial(
		/* Diffuse parameters */
		color i_diffuse_color;
		float i_diffuse_roughness;
		color i_incandescence;
		uniform float i_incandescence_intensity;

		/* Subsurface scattering parameters */
		float i_sss_on;
		string i_subsurface_type;
		float i_sss_samples;
		string i_sss_group;
		color i_sss_scattering;
		float i_sss_scattering_scale;
		color i_sss_transmittance;
		float i_sss_transmittance_scale;
		float i_sss_ior;
		float i_sss_scale;

		/* Specular parameters */
		/* Reflection parameters */
		color i_reflect_color;
		float i_reflect_roughness;
		float i_reflect_reflectivity;
		float i_reflect_anisotropy;
		point i_reflect_anisotropy_space;
		color i_reflect_anisotropy_direction;
		float i_reflect_samples;
		float i_reflect_has_reflection;
		float i_reflect_has_specular;
		float i_reflect_has_environment;

		/* Refraction parameters */
		color i_refract_color;
		float i_refract_roughness;
		color i_refract_fog_color;
		float i_refract_fog_strength;
		float i_refract_ior;
		float i_refract_samples;
		float i_refract_has_reflection;
		float i_refract_has_environment;

		/* Transparency parameters */
		color i_transparency;

		float i_coating_on;
		color i_coating_color;
		float i_coating_roughness;
		float i_coating_reflectivity;
		float i_coating_samples;
		float i_coating_has_reflection;
		float i_coating_has_specular;
		float i_coating_has_environment;
		color i_coating_transmittance;
		float i_coating_thickness;

		float i_bump_layer;
		normal i_normal;
		normal i_bump_normal;

		output color o_outColor;
		output color o_outTransparency;)
{
	extern point P;
	extern vector I;

	/* From Maya header */
	string __raytype = "subsurface";
	float __is_shadow_ray = 1;
	float __is_irradiance_ray = 1;
	float __reflects = 1;
	float __refracts = 1;
	vector stangent = (0,0,0);


	uniform float __is_subsurface_ray =
		(__raytype == "subsurface") ? 1:0;

	/* Useful variables */
	uniform float ray_depth;
	rayinfo( "depth", ray_depth );

	normal Nn_coating = i_normal;
	normal Nn_base = i_normal;

	/* Bump */
	if( i_bump_layer != 2)
	{
		Nn_coating = i_bump_normal;
	}

	if( i_bump_layer != 1)
	{
		Nn_base = i_bump_normal;
	}

	Nn_coating = normalize(Nn_coating);
	Nn_base = normalize(Nn_base);

	normal Nf_coating = ShadingNormal(Nn_coating);
	normal Nf_base = ShadingNormal(Nn_base);

	/* If there is no refraction or sss,
	 * every side should look like front side */
	if( i_refract_color == 0 && __is_subsurface_ray == 0 )
	{
		Nn_coating = Nf_coating;
		Nn_base = Nf_base;
	}

	vector In = normalize(I);
	vector V = -In;

	float is_backside_coating = Nn_coating.In < 0 ? 0:1;
	float is_backside_base = Nn_base.In < 0 ? 0:1;

	float __tmp_buf;
	float i_reflect_model = 0;
	float i_refract_model = 0;
	float i_coating_model = 0;	

	color opacity = __is_subsurface_ray ? color(1) : 1 - i_transparency;

	/* Shader components */
	color diffuse_color = color(1);
	color diffuse = 0;
	color indirect_diffuse = 0;
	color sub_surface = 0;
	color incandescence = 0;

	/* Turn off incandescence for multi-light output */
	if( 0 != islightcategory( "__incandescence" ) )
	{
		incandescence = i_incandescence * i_incandescence_intensity;
	}

	color coating_color = 0;
	color coating = 0;
	color coating_reflect_object = 0;
	color coating_reflect_environment = 0;
	color coating_specular = 0;

	color reflect_color = i_reflect_color * opacity;
	color reflect = 0;
	color reflect_object = 0;
	color reflect_environment = 0;
	color reflect_specular = 0;

	color refract_color =
		__is_subsurface_ray == 1 ? color(0) : i_refract_color * opacity;
	color refract = 0;
	color refract_object = 0;
	color refract_environment = 0;

	/* Coating layer */
	float coating_ior = 1;
	color coating_absorbed = 1;

	/* The absorption coefficient specifies which amount of light
	 * is _absorbed_ for which wavelength, i.e. which colours vanish.
	 * This means that we have to use the inverse transmittance */
	color coating_absorbtion = 1 - i_coating_transmittance;

	/* Reflection layer */
	float reflect_samples = i_reflect_samples;
	if(ray_depth > 0)
	{
		reflect_samples = 1;
	}

	float reflect_ior = eta_from_intensity( i_reflect_reflectivity );

	/* Layer above current layes is not air.
	 * So we have to modify current OR. */
	reflect_ior = reflect_ior/coating_ior;

	reflect_color *= 1 - CIEluminance(coating_color);

	float reflect_fresnel;
	fresnel(In, Nn_base, 1/reflect_ior, reflect_fresnel, __tmp_buf);
	reflect_color *= reflect_fresnel;

	if( is_backside_base == 1 )
	{
		/* Don't reflect on back side. Reflection from back side is already
		 * part of refraction. */
		reflect_color = 0;
	}


	illuminance( "-__3dfm_gi_light", P, Nn_base, PI/2 )
	{
		if( Cl != 0 )
		{
			float nonspecular = 0;
			lightsource( "__nonspecular", nonspecular );
			float nondiffuse = 0;
			lightsource( "__nondiffuse", nondiffuse );
			string category = "";
			lightsource( "__category", category );

			vector Ln = normalize(L);


			/* Diffuse */
			if( diffuse_color != 0 && nondiffuse < 1 )
			{
				if( i_sss_on != 1 || __is_subsurface_ray == 1 )
				{
					/* Lighting using Oren-Nayar diffuse model */
					diffuse +=
						Cl * PI *
						(1 - nondiffuse) *
						bsdf(
							Ln, Nn_base,
							"wo", V,
							"bsdf", "oren-nayar",
							"roughness", i_diffuse_roughness );
				}
			}
		}
	}

	if( diffuse_color != 0 && ( i_sss_on != 1 || __is_subsurface_ray == 1 ) )
	{
		float arealight_samples = 16;
		option( "user:_3dfm_arealight_samples", arealight_samples );

		color clight=0;
		trace(
			P, Nn_base,
			"wo", V,
			"bsdf", "oren-nayar",
			"type", "transmission",
			"hitsides", "reversed",
			"samplearealights", 1,
			"samples", arealight_samples,
			"arealightcontribution", clight,
			"roughness", i_diffuse_roughness );
		diffuse += clight;
	}

	color gi_visibility = 1;
	color gi_env_diffuse = 0;
	color gi_color_bleeding = 0;

	if( diffuse_color != 0 )
	{
		if( i_sss_on != 1 || __is_subsurface_ray == 1 )
		{
			color indirect_diffuse_weight =
				coating_absorbed*diffuse_color;
		}
		else
		{
			color absorption = i_sss_transmittance;

			/* Absorption should be in [0,1] range, otherwise inverting will
			 * make a negative value. */
			if( max( max( absorption[0], absorption[1] ), absorption[2] ) > 1 )
			{
				absorption = normalize_color( absorption );
			}

			/*
				clamp scale values to 1e-4.
			*/
			float sss_scale =
				i_sss_scale > 0 ? i_sss_scale : 1e-4;
			float sss_transmittance_scale =
				i_sss_transmittance_scale > 0 ? i_sss_transmittance_scale : 1e-4;
			float sss_scattering_scale =
				i_sss_scattering_scale > 0 ? i_sss_scattering_scale : 1e-4;

			absorption = normalize_color(1-absorption);
			absorption += EPSILON;
			absorption *= sss_transmittance_scale;

			color scattering = normalize_color( i_sss_scattering );
			scattering *= sss_scattering_scale;

			if( i_subsurface_type != "" )
			{
				sub_surface =
						subsurface(
							P, normalize(i_normal), /* do not pass bump normal */
							"type", i_subsurface_type,
							"samples", i_sss_samples,
							"model", "grosjean",
							"scattering", scattering,
							"absorption", absorption,
							"ior", i_sss_ior,
							"scale", sss_scale,
							"irradiance", diffuse);
			}
			else
			{
				sub_surface =
						subsurface(
							P, normalize(i_normal), /* do not pass bump normal */
							"model", "grosjean",
							"scattering", scattering,
							"absorption", absorption,
							"ior", i_sss_ior,
							"scale", sss_scale,
							"irradiance", diffuse);
			}

			// sub_surface /= diffuse_color * coating_absorbed;

			diffuse = 0;
		}
	}

	o_outColor =
		// coating * coating_color +
		// coating_specular * coating_color +
		// reflect * reflect_color  +
		// reflect_specular * reflect_color +
		// refract * refract_color  +
		diffuse +
		// indirect_diffuse * diffuse_color  +
		// incandescence  +
		sub_surface ;

	o_outTransparency = 1;


}

/****************************************************/
/**************** 3Delight Material *****************/
/****************************************************/

void maya_3DelightMaterial(
		/* Coating parameters */
		float i_coating_on;
		color i_coating_color;
		color i_coating_transmittance;
		float i_coating_thickness;
		float i_coating_roughness;
		float i_coating_samples;
		float i_coating_ior;
		float i_coating_has_reflection;
		float i_coating_has_specular;
		float i_coating_has_environment;

		/* Diffuse parameters */
		color i_diffuse_color;
		float i_diffuse_roughness;
		color i_incandescence;
		uniform float i_incandescence_intensity;

		/* Reflection parameters */
		color i_reflect_color;
		float i_reflect_roughness;
		float i_reflect_samples;
		float i_reflect_ior;
		float i_reflect_anisotropy;
		color i_reflect_anisotropy_direction;
		point i_reflect_anisotropy_space;
		float i_reflect_has_reflection;
		float i_reflect_has_specular;
		float i_reflect_has_environment;

		/* Subsurface scattering parameters */
		float i_sss_on;
		float i_sss_algorithm;
		float i_sss_samples;
		color i_sss_scattering;
		float i_sss_scattering_scale;
		color i_sss_transmittance;
		float i_sss_transmittance_scale;
		float i_sss_ior;
		float i_sss_scale;
		string i_sss_group;

		color i_refract_color;
		float i_refract_roughness;
		float i_refract_samples;
		float i_refract_ior;

		color i_transparency;

		float i_bump_layer;
		normal i_normalCamera;

		output color o_outColor;
		output color o_outTransparency;)
{
	extern normal N;

	_3DelightMaterial(
		/* Diffuse parameters */
		i_diffuse_color,
		i_diffuse_roughness,
		i_incandescence,
		i_incandescence_intensity,

		/* Subsurface scattering parameters */
		i_sss_on,
		i_sss_algorithm == 0 ? "raytrace" : "pointcloud",
		i_sss_samples,
		i_sss_group,
		i_sss_scattering,
		i_sss_scattering_scale,
		i_sss_transmittance,
		i_sss_transmittance_scale,
		i_sss_ior,
		i_sss_scale,

		/* Specular parameters */
		/* Reflection parameters */
		i_reflect_color,
		i_reflect_roughness,
		i_reflect_ior,
		i_reflect_anisotropy,
		i_reflect_anisotropy_space,
		i_reflect_anisotropy_direction,
		i_reflect_samples,
		i_reflect_has_reflection,
		i_reflect_has_specular,
		i_reflect_has_environment,

		/* Refraction parameters */
		i_refract_color,
		i_refract_roughness,
		1, 1,
		i_refract_ior,
		i_refract_samples,
		1,
		1,

		i_transparency,

		i_coating_on,
		i_coating_color,
		i_coating_roughness,
		i_coating_ior,
		i_coating_samples,
		i_coating_has_reflection,
		i_coating_has_specular,
		i_coating_has_environment,
		i_coating_transmittance,
		i_coating_thickness,

		i_bump_layer,
		N,
		i_normalCamera,

		o_outColor,
		o_outTransparency);
}

#endif
