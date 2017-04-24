//Ali Khalid
#version 150


in vec4 vPosition;
in vec4 vNormal;

out vec4 color;

uniform vec3 LAmbient,LDiffuse,LSpecular;
uniform vec3 MAmbient, MDiffuse, MSpecular;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;

void main()
{
	vec3 pos = (ModelView*vPosition).xyz;
	vec3 L = normalize(LightPosition.xyz - pos);
	vec3 E = normalize( -pos );
	vec3 H = normalize( L + E );

	vec3 N = normalize( ModelView*vNormal).xyz;

	vec3 ambient = LAmbient*MAmbient;
	float Kd = max(dot(L, N), 0.0);
	vec3 diffuse = Kd*LDiffuse*MDiffuse;
	float Ks = pow(max(dot(N, H), 0.0), Shininess);
	vec3 specular = Ks*LSpecular*MSpecular;
	if(dot(L,N)<0.0){
		specular = vec3(0.0, 0.0, 0.0);
	}
	gl_Position = Projection * ModelView * vPosition;
	color = vec4(ambient + diffuse + specular, 1.0);
}\0//used to stop the init shader to read garbage