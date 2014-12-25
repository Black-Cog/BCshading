/******************************************************************************/
/*                                                                            */
/*    Copyright (c)Cedric Launay.                                             */
/*    All Rights Reserved.                                                    */
/*                                                                            */
/******************************************************************************/


#ifndef _BCpragma_h
#define _BCpragma_h

#define MAX_SAMPLES 12

void pragma()
{
	// naming and field option
	#pragma annotation diffuseColor                   "label=Diffuse Color          : ; "
	#pragma annotation diffuseTexture                 "label=Diffuse Texture        : ; "
	#pragma annotation diffuseGain                    "label=Diffuse Gain           : ; "
	#pragma annotation diffuseDirectContribution      "label=Direct                 : ; "
	#pragma annotation diffuseIndirectContribution    "label=Indirect               : ; "

	#pragma annotation refractionColor                "label=Refraction Color       : ; "
	#pragma annotation refractionRoughness            "label=Roughness              : ;gadgettype=floatslider;min=0;max=1;         "
	#pragma annotation refractionSamples              "label=Samples                : ;gadgettype=intslider;min=1;max=MAX_SAMPLES; "
	#pragma annotation refractionIor                  "label=Ior                    : ;gadgettype=floatslider;min=0;max=4;         "
	#pragma annotation refractionAnisotropy           "label=Anisotropy             : ;gadgettype=floatslider;min=0;max=1;         "
	#pragma annotation refractionAnisotropyRoation    "label=Anisotropy Roation     : ;gadgettype=floatslider;min=0;max=180;       "


	#pragma annotation specularColor                  "label=Specular Color         : ; "
	#pragma annotation specularTexture                "label=Specular Texture       : ; "
	#pragma annotation specularGain                   "label=Specular Gain          : ; "
	#pragma annotation specularRoughness              "label=Roughness              : ;gadgettype=floatslider;min=0;max=1;         "
	#pragma annotation specularRoughnessTexture       "label=Roughness Texture      : ; "
	#pragma annotation specularAnisotropy             "label=Anisotropy             : ;gadgettype=floatslider;min=0;max=1;         "
	#pragma annotation specularAnisotropyRoation      "label=Anisotropy Roation     : ;gadgettype=floatslider;min=0;max=180;       "
	#pragma annotation specularSamples                "label=Samples                : ;gadgettype=intslider;min=1;max=MAX_SAMPLES; "
	#pragma annotation specularDirectContribution     "label=Direct                 : ; "
	#pragma annotation specularIndirectContribution   "label=Indirect               : ; "

	#pragma annotation specularMix                    "label=Specular Mix           : ;gadgettype=floatslider;min=0;max=1;         "
	#pragma annotation specularColor2                 "label=Specular Color         : ; "
	#pragma annotation specularTexture2               "label=Specular Texture       : ; "
	#pragma annotation specularGain2                  "label=Specular Gain          : ; "
	#pragma annotation specularRoughness2             "label=Roughness              : ;gadgettype=floatslider;min=0;max=1;         "
	#pragma annotation specularRoughnessTexture2      "label=Roughness Texture      : ; "
	#pragma annotation specularAnisotropy2            "label=Anisotropy             : ;gadgettype=floatslider;min=0;max=1;         "
	#pragma annotation specularAnisotropyRoation2     "label=Anisotropy Roation     : ;gadgettype=floatslider;min=0;max=180;       "
	#pragma annotation specularSamples2               "label=Samples                : ;gadgettype=intslider;min=1;max=MAX_SAMPLES; "
	#pragma annotation specularDirectContribution2    "label=Direct                 : ; "
	#pragma annotation specularIndirectContribution2  "label=Indirect               : ; "


	#pragma annotation transparency                   "label=Transparency           : ;gadgettype=floatslider;min=0;max=1;         "


	#pragma annotation normalMap                      "label=Normal Map             : ; "
	#pragma annotation bumpGain                       "label=Bump Gain              : ;gadgettype=floatslider;min=0;max=5;        "



	// grouping
	#pragma annotation "grouping" "Diffuse/diffuseColor;"
	#pragma annotation "grouping" "Diffuse/diffuseTexture;"
	#pragma annotation "grouping" "Diffuse/diffuseGain;"
	#pragma annotation "grouping" "Diffuse/Contribution/diffuseDirectContribution;"
	#pragma annotation "grouping" "Diffuse/Contribution/diffuseIndirectContribution;"


	#pragma annotation "grouping" "Refraction/refractionColor;"
	#pragma annotation "grouping" "Refraction/refractionRoughness;"
	#pragma annotation "grouping" "Refraction/refractionSamples;"
	#pragma annotation "grouping" "Refraction/refractionIor;"
	#pragma annotation "grouping" "Refraction/refractionAnisotropy;"
	#pragma annotation "grouping" "Refraction/refractionAnisotropyRoation;"


	#pragma annotation "grouping" "Specular/specularColor;"
	#pragma annotation "grouping" "Specular/specularTexture;"
	#pragma annotation "grouping" "Specular/specularGain;"
	#pragma annotation "grouping" "Specular/specularRoughness;"
	#pragma annotation "grouping" "Specular/specularRoughnessTexture;"
	#pragma annotation "grouping" "Specular/specularAnisotropy;"
	#pragma annotation "grouping" "Specular/specularAnisotropyRoation;"
	#pragma annotation "grouping" "Specular/specularSamples;"
	#pragma annotation "grouping" "Specular/Contribution/specularDirectContribution;"
	#pragma annotation "grouping" "Specular/Contribution/specularIndirectContribution;"

	#pragma annotation "grouping" "Specular/DoubleSpec/specularMix;"
	#pragma annotation "grouping" "Specular/DoubleSpec/specularColor2;"
	#pragma annotation "grouping" "Specular/DoubleSpec/specularTexture2;"
	#pragma annotation "grouping" "Specular/DoubleSpec/specularGain2;"
	#pragma annotation "grouping" "Specular/DoubleSpec/specularRoughness2;"
	#pragma annotation "grouping" "Specular/DoubleSpec/specularRoughnessTexture2;"
	#pragma annotation "grouping" "Specular/DoubleSpec/specularAnisotropy2;"
	#pragma annotation "grouping" "Specular/DoubleSpec/specularAnisotropyRoation2;"
	#pragma annotation "grouping" "Specular/DoubleSpec/specularSamples2;"
	#pragma annotation "grouping" "Specular/DoubleSpec/Contribution/specularDirectContribution2;"
	#pragma annotation "grouping" "Specular/DoubleSpec/Contribution/specularIndirectContribution2;"


	#pragma annotation "grouping" "Opacity/transparency;"


	#pragma annotation "grouping" "Bump/normalMap;"
	#pragma annotation "grouping" "Bump/bumpGain;"


	#pragma annotation "grouping" "AOVs/aov_diffuseDirect;"
	#pragma annotation "grouping" "AOVs/aov_diffuseIndirect;"
	#pragma annotation "grouping" "AOVs/aov_specularDirect;"
	#pragma annotation "grouping" "AOVs/aov_specularIndirect;"
	#pragma annotation "grouping" "AOVs/aov_refraction;"
}



#endif