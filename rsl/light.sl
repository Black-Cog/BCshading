light light(
	color lightcolor = 1 ;
	)
{
	illuminate( point "shader" 0 )
	{
		Cl = lightcolor;
	}
}