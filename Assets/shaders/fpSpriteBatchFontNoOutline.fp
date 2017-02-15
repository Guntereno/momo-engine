precision mediump float;

uniform sampler2D uTexture;

varying vec2 vTexCoord;
varying vec4 vVertColor;
varying vec4 vChannel;

void main()
{
	vec4 pixel = texture2D( uTexture, vTexCoord );

	// Are we rendering a colored image, or 
	// a character from only one of the channels
	if(dot(vec4(1.0, 1.0, 1.0, 1.0), vChannel) > 0.0)
	{
		// Get the pixel value
		float val = dot(pixel, vChannel);
		pixel.rgb = vec3(1.0);
		pixel.a   = val;
	}

	gl_FragColor = pixel * vVertColor;
}

