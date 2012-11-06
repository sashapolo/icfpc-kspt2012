// PARALLAX pixel shader
 
varying vec3 LightDirection; 
varying vec3 ViewDirection;

uniform sampler2D colorMap;
uniform sampler2D bumpHeightMap;
uniform sampler2D specularMap;
uniform sampler2D glowMap;

vec2 ParallaxOcclusionMap(vec2 baseTC, float lod, in vec3 viewDirNrm, int numSteps, float displacement)
{
	float step =  1.0 / numSteps;
	float bumpScale = displacement;
	
	vec2 delta = vec2(viewDirNrm.x, viewDirNrm.y) * bumpScale / (-viewDirNrm.z * numSteps); // / max(-viewDirNrm.z * numSteps, 0.1)
		
	vec4 NB0 = texture2D(bumpHeightMap, baseTC);
			
	float height = 1 - step;
	vec4 offset = vec4(baseTC + delta, 0, lod);
	vec4 NB1 = texture2D(bumpHeightMap, offset);

	for (int i=0; i<numSteps; i++)
	{
		//[flatten]
		if (NB1.w >= height)
			break;
			
		NB0 = NB1;

		height -= step;
		offset.xy += delta;

		NB1 = texture2DLod(bumpHeightMap, offset,lod);
	}
	
	vec4 offsetBest = offset;
	float error = 1.0;

	float t1 = height;
	float t0 = t1 + step;
	
	float delta1 = t1 - NB1.w;
	float delta0 = t0 - NB0.w;

	vec4 intersect = vec4(delta * numSteps, delta * numSteps + baseTC);

	for (int i=0; i<10; i++)
	{
		//[flatten]
		if (abs(error) <= 0.01)
			break;
	
		float denom = delta1 - delta0;
		float t = (t0 * delta1 - t1 * delta0) / denom;
		offsetBest.xy = -t * intersect.xy + intersect.zw;
		
		vec4 NB = texture2DLod(bumpHeightMap, offsetBest,lod);

		error = t - NB.w;
		if (error < 0)
		{
			delta1 = error;
			t1 = t;
		}
		else
		{
			delta0 = error;
			t0 = t;
		}
	}
	
	return offsetBest.xy;
}

void main()
{

   vec2 tex=ParallaxOcclusionMap(gl_TexCoord[0],0,normalize(ViewDirection),15,0.04);

   vec3 N = normalize( ( texture2D( bumpHeightMap, tex).xyz * 2.0 ) - 1.0 );

   vec3 L = normalize(LightDirection);
   vec3 E = normalize(ViewDirection);
   vec3 R = reflect(-L, N);

   vec4 Color =  texture2D( colorMap, tex); 
   vec4 vSpecular= texture2D( specularMap, tex); 
   vec4 vGlow=texture2D( glowMap, tex); 
   
   vec4 Ambient  =  Color * .1;
   vec4 Diffuse    =  Color * max(dot(N,L),0.0);
   vec4 Specular =  Color *  pow(max(dot(R,E),0.0),20.0);
 
   
   gl_FragColor = Ambient + Diffuse + Specular + vGlow;
   //gl_FragColor.w=1.0f-fZ;
   //gl_FragColor=vec4(ViewDirection.x,ViewDirection.y,ViewDirection.z,1);
}