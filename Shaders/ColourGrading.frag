

uniform sampler2D u_texture;
uniform float u_time;
uniform float u_strength;
uniform float u_grey_scale_amount;

void main()
{
	vec2 pos = gl_TexCoord[0].xy; 
	vec4 colour_grade = vec4(0.35 + pos.y/3.0 + pos.x/1.5, pos.y/3.0 + pos.x/3.0, 0.1, 1.0) * u_strength/30;

	float r = texture2D(u_texture, pos).r;
	float g = texture2D(u_texture, pos).g;
	float b = texture2D(u_texture, pos).b;

	float total = (r+g+b)/3.0;

	float smooth_gsa = smoothstep(0,1, u_grey_scale_amount);
	vec4 grey_scale = vec4(total,total,total,1.0);

	vec4 col = texture2D(u_texture, pos);
	gl_FragColor = mix(col, grey_scale, smooth_gsa) + colour_grade; 
}


// notes:

// uniforms can be the following types
// float, vec2, vec3, vec4, mat2, mat3, mat4, sampler2D and samplerCube

//some default functions are
//sin(), cos(), tan(), asin(), acos(), atan(), pow(), exp(), log(), sqrt(), 
//abs(), sign(), floor(), ceil(), fract(), mod(), min(), max() and clamp().
//smoothstep(edge0, edge1, x), step(limit, x), mix(colour1, colour2, t)