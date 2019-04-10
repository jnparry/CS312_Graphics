// fragment.fs
// #version 330

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

    // if (dot(camDir, norm) < 0.3)
    //     gl_FragColor = vec4(u_LightColor * vec3(0.0, 0.0, 0.0), 1.0);
    // // if (dot(camDir, norm) < mix(0.4, 0.1, max(0.0, dot(norm, lightDir))))
    //     // gl_FragColor = vec4(u_LightColor * vec3(0.0, 0.0, 0.0), 1.0);
    // else
        gl_FragColor = color * sample;

    // outline code
    // vec3 H = normalize(lightDir + camDir);
    // float outlineDiff = u_Kd * diff;
    // float outlineSpec = 0;

    // if (dot(lightDir, norm) > 0.0)
    //     outlineSpec = u_Ks * pow(max(dot(H, norm), 0.0)), u_Ns);

    // float edgeDetection = (dot(camDir, norm) > 0.3) ? 1: 0;
    // float light = edgeDetection * (outlineDiff + outlineSpec);
    // vec3 edgeColor = vec3(light, light, light);
    // gl_FragColor = vec4(edgeColor, 1.0);
}

    // outlining
    // if (dot(camDir, norm) < mix(0.4, 0.1, max(0.0, dot(norm, lightDir))))
    //     gl_FragColor = vec4(u_LightColor * vec3(0.0, 0.0, 0.0), 1.0);


    // VERTEX SHADER STUFF
    // extractedModel[0][0] = modelView[0][0];
    // extractedModel[0][1] = modelView[0][1];
    // extractedModel[0][2] = modelView[0][2];

    // extractedModel[1][0] = modelView[0][0];
    // extractedModel[1][1] = modelView[0][1];
    // extractedModel[1][2] = modelView[0][2];

    // extractedModel[2][0] = modelView[0][0];
    // extractedModel[2][1] = modelView[0][1];
    // extractedModel[2][2] = modelView[0][2];






// uniform sampler2D u_Texture;
// uniform float u_Threshold;
// 
// varying vec2 v_UV;

// void main()
// {
// vec4 sample = texture2D(u_Texture, v_UV);	
// if(sample.r > u_Threshold)
// {
// 	discard;
// }

// 	gl_FragColor = sample;

// }
// -----------------------------------------

// Your vertex.vs file should look like:
// ---------------------------------------------
// uniform mat4 u_Matrix;
// attribute vec3 a_Position;
// attribute vec2 a_UV;

// varying vec2 v_UV;

// void main()
// {
// 	v_UV = a_UV;
// 	gl_Position = u_Matrix * vec4(a_Position, 1.0);
// }
