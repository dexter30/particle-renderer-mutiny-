uniform mat4 in_Projection;
uniform mat4 in_View;
uniform mat4 in_Model;
uniform mat4 in_NormalMatrix;

attribute vec3 in_Position;
attribute vec3 in_Normal;

varying vec3 ex_LightPos;
varying vec3 ex_V;
varying vec3 ex_N;

void main()
{
  ex_N = normalize(in_NormalMatrix * vec4(in_Normal, 1)).xyz;
  ex_V = vec3(in_View * in_Model * vec4(in_Position, 1));
  ex_LightPos = vec4(in_View * vec4(10, 0, 10, 1)).xyz;
  gl_Position = in_Projection * in_View * in_Model * vec4(in_Position, 1);
}
