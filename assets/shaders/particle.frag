#ifdef GL_ES
  precision highp float;
#endif

uniform sampler2D in_Texture;

varying vec2 ex_Uv;
varying vec3 ex_Colour;
void main()
{
  //gl_FragColor = texture2D(in_Texture, ex_Uv);

  //if(gl_FragColor.x + gl_FragColor.y + gl_FragColor.z < 1.25)
  //{
  //  gl_FragColor.r = 0.0;
  //  gl_FragColor.g = 0.0;
  //  gl_FragColor.b = 0.0;
  //  gl_FragColor.a = 1.0;
  //}
  //else
  //{
  //  gl_FragColor.r = 1.0;
  //  gl_FragColor.g = 1.0;
  //  gl_FragColor.b = 1.0;
  //  gl_FragColor.a = 0.5;
  //}
  gl_FragColor.r = ex_Colour.x;
  gl_FragColor.g = ex_Colour.y;
  gl_FragColor.b = ex_Colour.z;
  gl_FragColor.a = 0.5;
  

  //gl_FragColor = vec4(ex_Uv, 1.0, 1.0);
  //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
