uniform sampler2DRect tex0;
uniform float amount;

void main()
{
  vec4 color = texture2DRect(tex0, gl_TexCoord[0].xy);
  float a = floor(color.r * 2.0);
  color.rgb = vec3(0.1);
  color.a = a;
  gl_FragColor = color;
}