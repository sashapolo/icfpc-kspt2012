// PARALLAX vertex shader
 
varying vec3 LightDirection; 
varying vec3 ViewDirection;
uniform vec3 cameraPos;
uniform mat4 mWorld;


void main()
{
   gl_Position = ftransform();
   vec3 Position = vec3(gl_ModelViewMatrix * gl_Vertex);
   vec3 PositionW = vec3(mWorld * gl_Vertex);
   
   gl_TexCoord[0] = gl_MultiTexCoord0;
   
	vec3 Normal = gl_NormalMatrix * gl_Normal;
	vec3 Tangent = gl_NormalMatrix * gl_MultiTexCoord1.xyz;
	vec3 Binormal = gl_NormalMatrix * gl_MultiTexCoord2.xyz;

   
   vec3 fvViewDirection = -Position;
   vec3 fvLightDirection = vec3(gl_LightSource[0].position.xyz -Position);
   

   ViewDirection.x  = dot( Tangent, fvViewDirection );
   ViewDirection.y  = dot( Binormal,fvViewDirection );
   ViewDirection.z  = dot( Normal,  fvViewDirection );
   
   LightDirection.x  = dot( Tangent, fvLightDirection );
   LightDirection.y  = dot( Binormal,fvLightDirection );
   LightDirection.z  = dot( Normal,  fvLightDirection  );   

   //mat3 mWorldToTangent = mat3( Tangent, Binormal, Normal );
//	ViewDirection  = mul( mWorldToTangent, fvViewDirection  );
//	LightDirection = mul( mWorldToTangent,fvLightDirection );
//ViewDirection=gl_MultiTexCoord2.xyz;
}
