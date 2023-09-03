


uniform vec2 u_light_position;
uniform sampler2D u_texture;
uniform float u_drop_off;
uniform float u_feather;
uniform vec4 u_colour;
uniform float u_intensity;

void main(){

	//calculate distance between light points
	vec2 pos = gl_TexCoord[0].xy;

	vec2 source_pos = u_light_position * (vec2(300.0,200.0)/300.0);
	vec2 scaled_pos = pos * (vec2(300.0,200.0)/300.0); //scaling it to fit be appropriate dimensions in relation to the screen

	//vec4 light_colour = u_colour * smoothstep(0.0, 1.0/u_intensity, (1.0 - smoothstep(0.0, 1.0, distance(scaled_pos, source_pos) * 20.0/u_spread)));
	
	float distance_from_source = clamp(0.0, u_feather, (distance(scaled_pos, source_pos) - u_drop_off));

	float light_amount = (1.0 - (distance_from_source/u_feather)) * u_intensity;

	gl_FragColor = texture2D(u_texture, pos) + (light_amount * u_colour);

}