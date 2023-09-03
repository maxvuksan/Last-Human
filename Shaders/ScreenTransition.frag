

uniform sampler2D u_texture;
uniform float u_transition_amount;
uniform vec4 u_colour;

void main()
{
	vec2 pos = gl_TexCoord[0].xy; 

	float t = smoothstep(0,1, u_transition_amount);

	gl_FragColor = mix(texture2D(u_texture, pos), u_colour, t);
}

