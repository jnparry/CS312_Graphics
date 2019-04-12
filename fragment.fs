// fragment.fs

uniform sampler2D u_Texture;
uniform float u_Threshold;
uniform int u_Ns;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform vec3 u_CamPos;

varying vec2 v_UV;

varying vec3 Normal;
varying vec3 FragPos;

void main()
{
    vec4 sample = texture2D(u_Texture, v_UV);
    if (sample.r > u_Threshold)
        discard;

    // ambient
    vec4  ambient  = vec4(u_Ka * u_LightColor, 1.0);

    // diffused
    vec3  norm     = normalize(Normal);
    vec3  lightDir = normalize(u_LightPos - FragPos);
    float diff     = max(dot(norm, lightDir), 0.0);
    vec4  diffuse  = vec4(diff * u_LightColor * u_Kd, 1.0);

    // specular
    vec3  camDir     = normalize(u_CamPos - FragPos);
    vec3  reflectDir = reflect(-lightDir, norm);
    float spec       = pow(max(dot(camDir, reflectDir), 0.0), 32.0);
    vec4  specular   = vec4(u_Ks * spec * u_LightColor, 1.0);

    // ambient, diffused, specular
    vec4  colorADS;
    colorADS = (ambient + diffuse + specular) * sample;

    // http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/toon-shader-version-ii/
    vec4  color;
    float intensity = dot(lightDir, norm);

    if (intensity > 0.95)
		color = vec4(1.0,1.0,1.0,1.0); 
    else if (intensity > 0.75)
        color = vec4(0.8, 0.8, 0.8, 1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.6,0.6,1.0); 
	else if (intensity > 0.25)
		color = vec4(0.4,0.4,0.4,1.0);
	else
		color = vec4(0.2,0.2,0.2,1.0); 

    if (dot(camDir, norm) < mix(0.4, 0.1, max(0.0, dot(norm, lightDir))))
        gl_FragColor = vec4(u_LightColor * vec3(0.0, 0.0, 0.0), 1.0);
    else
      gl_FragColor = color * sample;
    // gl_FragColor = colorADS;
}