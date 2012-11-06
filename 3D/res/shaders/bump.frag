// BUMP pixel shader
 
varying vec3 LightDirection; 
varying vec3 ViewDirection;

uniform sampler2D colorMap;
uniform sampler2D bumpMap;
uniform sampler2D specularMap;
uniform sampler2D glowMap;

void main()
{

   
   vec3 N = normalize( ( texture2D( bumpMap, gl_TexCoord[0]).xyz * 2.0 ) - 1.0 );
   vec3 L = normalize(LightDirection);
        vec3 E = normalize(ViewDirection);
   vec3 R = reflect(-L, N);

   vec4 Color =  texture2D( colorMap, gl_TexCoord[0].st); 
   vec4 vSpecular= texture2D( specularMap, gl_TexCoord[0].st); 
   vec4 vGlow=texture2D( glowMap, gl_TexCoord[0].st); 
   
   vec4 Ambient  =  Color * .1;
   vec4 Diffuse    =  Color * max(dot(N,L),0.0);
   vec4 Specular =  vSpecular *  pow(max(dot(R,E),0.0),20.0);
 
   
   gl_FragColor = Ambient + Diffuse + Specular + vGlow;
}