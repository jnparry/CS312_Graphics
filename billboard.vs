uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform vec3 u_CamPos;

attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec3 a_Normal;

varying vec2 v_UV;
varying vec3 Normal;
varying vec3 FragPos;

void main()
{
    mat4 u_MV = u_View * u_Model;
    u_MV[0][0] = 40.0;
    u_MV[0][1] = 0.0;
    u_MV[0][2] = 0.0;

    u_MV[1][0] = 0.0;
    u_MV[1][1] = 40.0;
    u_MV[1][2] = 0.0;

    u_MV[2][0] = 0.0;
    u_MV[2][1] = 0.0;
    u_MV[2][2] = 40.0;

    v_UV = a_UV;
    gl_Position = u_Proj * u_MV * vec4(a_Position, 1.0);
    FragPos     = vec3(u_MV * vec4(a_Position, 1.0));
    Normal      = vec3(u_Model * vec4(a_Normal, 0.0));
}