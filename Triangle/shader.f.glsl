#version 150

smooth in vec4 theColor;

uniform float fragDuration;
uniform float time;

out vec4 outColor;

void main()
{
  float elapsed = mod(time, fragDuration);
  float lerp = elapsed / fragDuration;
  
  outColor = mix(vec4(1, 1, 1, 1), theColor, lerp);
}
