uniform sampler2D colorMap; 
uniform sampler2D origMap; 
varying vec2 vTexCoord; 
uniform float sampleDist; 
uniform float strength; 
uniform float multiplier; 

float luminance(vec3 c)
{
	return dot( c, vec3(0.3, 0.59, 0.11) );
};

float highlights(vec3 c, float HighlightThreshold)
{
	return smoothstep(HighlightThreshold, 1.0, luminance(c.rgb));
};


void main() 
{ 
   vec2 samples[12];  
   samples[0]  = vTexCoord + sampleDist * vec2(-0.326212, -0.405805); 
   samples[1]  = vTexCoord + sampleDist * vec2(-0.840144, -0.073580); 
   samples[2]  = vTexCoord + sampleDist * vec2(-0.695914,  0.457137); 
   samples[3]  = vTexCoord + sampleDist * vec2(-0.203345,  0.620716); 
   samples[4]  = vTexCoord + sampleDist * vec2(0.962340, -0.194983); 
   samples[5]  = vTexCoord + sampleDist * vec2(0.473434, -0.480026); 
   samples[6]  = vTexCoord + sampleDist * vec2(0.519456,  0.767022); 
   samples[7]  = vTexCoord + sampleDist * vec2(0.185461, -0.893124); 
   samples[8]  = vTexCoord + sampleDist * vec2(0.507431,  0.064425); 
   samples[9]  = vTexCoord + sampleDist * vec2(0.896420,  0.412458); 
   samples[10]  = vTexCoord + sampleDist * vec2(-0.321940, -0.932615); 
   samples[11]  = vTexCoord + sampleDist * vec2(-0.791559, -0.597705); 
   vec4 sample = texture2D(colorMap, vTexCoord); 
   vec4 orig = texture2D(origMap, vTexCoord); 
   vec4 avg = sample; 
   vec4 col; 
   for (int i = 0; i < 12; i++) 
   { 
      vec2 tC=min(vec2(0.999,0.999),max(vec2(0.001,0.001),samples[i])); 
      avg +=texture2D(colorMap,tC); 
   } 
   avg /= 12.0; 
   avg.a = highlights(avg.rgb,strength);
//   gl_FragColor = sample+avg*avg.a; 
gl_FragColor.xyz=orig+avg*0.1+avg*avg.a;
gl_FragColor.w=1;
};
