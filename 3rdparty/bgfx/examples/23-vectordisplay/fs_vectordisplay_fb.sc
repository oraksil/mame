$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texColor, 0);

uniform vec4 u_params;
#define u_blur_scale    u_params.xy
#define u_compose_mult  u_params.z
#define u_compose_alpha u_params.w

void main() {
  vec4 texColor = texture2D(s_texColor, v_texcoord0.xy);
  gl_FragColor = v_color0 * texColor * vec4(1.0, 1.0, 1.0, u_compose_alpha);
}
