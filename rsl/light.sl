light light(
	float exposure = 0;
	color lightcolor = 1 ;
	)
{
	illuminate( point "shader" 0 )
	{
		Cl = lightcolor;
	}
}
