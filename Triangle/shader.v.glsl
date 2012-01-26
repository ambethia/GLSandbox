#version 150

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

uniform float time;
uniform float duration;

smooth out vec4 theColor;

void main()
{
  float scale = 3.14159f * 2.0f / duration;
  float elapsed = mod(time, duration);

  vec2 offset = vec2(
    cos(elapsed * scale) * 0.5f,
    sin(elapsed * scale) * 0.5f
  );

  gl_Position = position + vec4(offset, 0.0, 0.0);
  theColor = color;
}
